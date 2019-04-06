

/*
 * POSIX Real Time Example
 * using a single pthread as RT thread
 */

#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <iostream>
#include <unistd.h>
#include <math.h>

#include <time.h>
#include <sys/stat.h>
#include "rt_log.h"
#include <libgen.h>

#include "ecat_master.h"
#include "ecat_slave.h"
#include "ecat_task.h"



// EtherCAT
static ec_master_t *master = NULL;
static ec_master_state_t master_state = {};

static ec_domain_t *domain1 = NULL;
static ec_domain_state_t domain1_state = {};

static uint8_t *domain1_pd = NULL;

slaves_t slaves;


uint32_t loop_counter = 0;
uint16_t min_time_us = 1000;
uint16_t max_time_us = 1000;
uint16_t last_time_us = 0;
static float x,y,z;

static inline void tsnorm(struct timespec *ts)
{
    while (ts->tv_nsec >= 1000000000)
    {
        ts->tv_nsec -= 1000000000;
        ts->tv_sec++;
    }
}

void rt_check_domain_state(void)
{
    ec_domain_state_t ds = {};

    ecrt_domain_state(domain1, &ds);

    if (ds.working_counter != domain1_state.working_counter)
    {
//        printf("Domain1: WC %u.\n", ds.working_counter);
        RT_PRINT("Domain1: WC" + std::to_string(ds.working_counter));
    }

    if (ds.wc_state != domain1_state.wc_state)
    {
//        printf("Domain1: State %u.\n", ds.wc_state);
        RT_PRINT("Domain1: State " + std::to_string(ds.wc_state));
    }

    domain1_state = ds;
}

/****************************************************************************/
void rt_check_master_state(void)
{
    ec_master_state_t ms;

    ecrt_master_state(master, &ms);

    if (ms.slaves_responding != master_state.slaves_responding)
    {
//        printf("%u slave(s).\n", ms.slaves_responding);
        RT_PRINT(std::to_string(ms.slaves_responding) + " slave(s).");
    }

    if (ms.al_states != master_state.al_states)
    {
//        printf("AL states: 0x%02X.\n", ms.al_states);
        RT_PRINT("AL states: " + std::to_string(ms.al_states));
    }

    if (ms.link_up != master_state.link_up)
    {
//        printf("Link is %s.\n", ms.link_up ? "up" : "down");
//        RT_PRINT("AL states: " + std::to_string(ms.al_states));
    }

    master_state = ms;
}


void *thread_func(void *data)
{
    struct timespec systime;
    struct timespec lasttime = {0, 0};
    struct timespec looptime;
    sleep(1);

    if (clock_gettime(CLOCK_REALTIME, &looptime) == -1)
    {
        std::cout<< "clock_gettime error !!!" <<std::endl;
        goto out_loop;
    }

    while (1)
    {
        looptime.tv_nsec += 1000000;
        tsnorm(&looptime);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &looptime, NULL);

        {
            // receive EtherCAT frames
            ecrt_master_receive(master);
            ecrt_domain_process(domain1);

            //
            rt_check_domain_state();
            rt_check_master_state();

    //        if(master_state.slaves_responding == static_cast<unsigned int>(1))
            if(master_state.al_states == 0x08)
            {
                #ifdef IMU_Pos_0
                slaves.imu_0.DataRead(domain1_pd);
                #endif

                #ifdef IMU_Pos_1
                slaves.imu_1.DataRead(domain1_pd);
                #endif

                #ifdef  WMIO_Pos_0
                static uint8_t leds=0x55;
                slaves.wmio_0.DataRead(domain1_pd);
                EC_WRITE_U8(domain1_pd+slaves.wmio_0.data.led0_7, leds++);
                #endif

                #ifdef MOTOR_Pos_0
                    ControlTask(domain1_pd, slaves);
                #endif
            }

            //time test
            {
//                for(uint32_t i=0; i<100; i++)
//                {
//                    x = tan(i);
//                    y = atan(i);
//                    z += x+y;
//                    y=z;
//                }

                loop_counter++;
                if (clock_gettime(CLOCK_REALTIME, &systime) == -1)
                {
                    std::cout<< "clock_gettime error !!!" <<std::endl;
                    goto out_loop;
                }

                {
                    static int i=0;
                    if(i++ < 2000) last_time_us=1000;
                    else last_time_us = 1000000*(systime.tv_sec - lasttime.tv_sec) + (systime.tv_nsec - lasttime.tv_nsec)/1000;
                    min_time_us = min_time_us > last_time_us?   last_time_us:min_time_us;
                    max_time_us = max_time_us < last_time_us?   last_time_us:max_time_us;
                    lasttime = systime;
                }
            }
            //distribute clock
            ecrt_master_application_time(master, 1000000000*((uint64_t)systime.tv_sec) + systime.tv_nsec);
//            ecrt_master_application_time(master, systime.tv_nsec);
            ecrt_master_sync_reference_clock(master);
            ecrt_master_sync_slave_clocks(master);

            // send process data
            ecrt_domain_queue(domain1);
            ecrt_master_send(master);
        }

    }
out_loop:
        return NULL;
}


int EcatMasterInit(void)
{
//            LOG(INFO) <<"Requesting master..."<<std::endl;
    master = ecrt_request_master(0);
    if (!master)
    {
//                LOG(ERROR) << "ecrt_request_master error" <<std::endl;
        return -1;
    }

//            LOG(INFO) <<"ecrt_master_create_domain..."<<std::endl;
    domain1 = ecrt_master_create_domain(master);
    if (!domain1)
    {
//                LOG(ERROR) << "ecrt_master_create_domain error" << std::endl;
        return -1;
    }

//            LOG(INFO) <<"Creating slave configurations..."<<std::endl;

    #ifdef IMU_Pos_0
        slaves.imu_0.Init(master,IMU_Pos_0);
    #endif

    #ifdef IMU_Pos_1
        slaves.imu_1.Init(master,IMU_Pos_1);
    #endif

    #ifdef MOTOR_Pos_0
        slaves.motor_0.Init(master, MOTOR_Pos_0);
    #endif

    #ifdef WMIO_Pos_0
        slaves.wmio_0.Init(master, WMIO_Pos_0);
    #endif



//            if (ecrt_domain_reg_pdo_entry_list(domain1, domain1_regs))

//            if (ecrt_domain_reg_pdo_entry_list(domain1, &EcatSlave::domain_regs[0]))
     if (ecrt_domain_reg_pdo_entry_list(domain1, EcatSlave::domain_regs.data()))
    {
//                LOG(ERROR) << "PDO entry registration failed!" << std::endl;
        return -1;
    }


    ecrt_slave_config_dc(slaves.motor_0.sc_motor, 0x0300, 1000000, 440000, 0, 0);

//            LOG(INFO) <<"Activating master..."<<std::endl;
    if (ecrt_master_activate(master))
    {
//                LOG(ERROR) << "ecrt_master_activate failed!" << std::endl;
        return -1;
    }

//            LOG(INFO) <<"get domain data pointer..."<<std::endl;
    if (!(domain1_pd = ecrt_domain_data(domain1)))
    {
//                LOG(ERROR) << "Failed to get domain data pointer" << std::endl;
        return -1;
    }


    ec_master_info_t *master_info;
    if(0 != ecrt_master(master, master_info))
    {
        std::cout<< "ecrt_master error!!!!!!!!!!!!!!!!!!!!!! " << std::endl;
        return -1;
    }
    else
    {
        std::cout<< "slave_count" << master_info->slave_count << std::endl;
    }
}

int main(int argc, char* argv[])
{
    chdir(dirname(argv[0])); //设置当前目录为应用程序所在的目录。
    struct sched_param param;
    pthread_attr_t attr;
    pthread_t thread;
    int ret;

    /* Lock memory */
    if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
            printf("mlockall failed: %m\n");
            exit(-2);
    }


    EcatMasterInit();

    /* Initialize pthread attributes (default values) */
    ret = pthread_attr_init(&attr);
    if (ret) {
            printf("init pthread attributes failed\n");
            goto out;
    }

    /* Set a specific stack size  */
    ret = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
    if (ret) {
        printf("pthread setstacksize failed\n");
        goto out;
    }

    /* Set scheduler policy and priority of pthread */
    ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    if (ret) {
            printf("pthread setschedpolicy failed\n");
            goto out;
    }
    param.sched_priority = 90;
    ret = pthread_attr_setschedparam(&attr, &param);
    if (ret) {
            printf("pthread setschedparam failed\n");
            goto out;
    }
    /* Use scheduling parameters of attr */
    ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if (ret) {
            printf("pthread setinheritsched failed\n");
            goto out;
    }



    /* Create a pthread with specified attributes */
    ret = pthread_create(&thread, &attr, thread_func, NULL);
    if (ret) {
            printf("create pthread failed\n");
            goto out;
    }
    ret = pthread_detach(thread);
    if (ret)    printf("join pthread failed: %m\n");


    //RT_PRINT
    {
        param.sched_priority = 80;
        ret = pthread_attr_setschedparam(&attr, &param);
        if (ret) {
                printf("pthread setschedparam failed\n");
                goto out;
        }
        /* Use scheduling parameters of attr */
        ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        if (ret) {
                printf("pthread setinheritsched failed\n");
                goto out;
        }

        /* Create a pthread with specified attributes */
        ret = pthread_create(&thread, &attr, RTLogThread, NULL);
        if (ret) {
                printf("create pthread failed\n");
                goto out;
        }
        ret = pthread_detach(thread);
        if (ret)    printf("join pthread failed: %m\n");

    }




    while (1)
    {
        sleep(1);
        std::cout<< "cycle times: " << std::dec << loop_counter \
                 << "    last_time_us: " << std::dec << last_time_us\
                 << "    min_time_us: " << std::dec << min_time_us \
                 << "    max_time_us: " << std::dec << max_time_us << std::endl;
        std::cout<< "z: " << z << std::endl;

        #ifdef IMU_Pos_0
        slaves.imu_0.DataPlay();
        #endif

        #ifdef IMU_Pos_1
        slaves.imu_1.DataPlay();
        #endif

        #ifdef WMIO_Pos_0
        slaves.wmio_0.DataPlay();
        #endif

        #ifdef MOTOR_Pos_0
        slaves.motor_0.Display();
        #endif

        RT_PRINT("I am RT print hahahahaha");

        std::cout<< std::endl;
    }

    std::cout <<"End of Program"<<std::endl;
    ecrt_release_master(master);


out:
    return ret;
}



//#include <sys/stat.h>
//#include <thread>
//#include <time.h>
//#include "ecat_task.h"
//#include "ecat_wm_io.h"
//#include "ecat_master.h"
//#include "glog/logging.h"
//#include "record.h"


//RT_TASK my_task;

//static int run = 1;

//using namespace std;
//using namespace tinyxml2;

///****************************************************************************/
//// EtherCAT
//static ec_master_t *master = NULL;
//static ec_master_state_t master_state = {};

//static ec_domain_t *domain1 = NULL;
//static ec_domain_state_t domain1_state = {};

//static uint8_t *domain1_pd = NULL;


///****************************************************************************
// * Signal handler
// ***************************************************************************/

//void signal_handler(int sig)
//{
//    run = 0;
//}


///****************************************************************************
// * Main function
// ***************************************************************************/
//int main(int argc, char *argv[])
//{
////    chdir(dirname(argv[0])); //设置当前目录为应用程序所在的目录。
////    LoadXML();

//    std::thread record(RecordThread);
//    record.detach();

//    /* Perform auto-init of rt_print buffers if the task doesn't do so */
//    rt_print_auto_init(1);

//    signal(SIGTERM, signal_handler);
//    signal(SIGINT, signal_handler);

//}

///**************************************************************************
