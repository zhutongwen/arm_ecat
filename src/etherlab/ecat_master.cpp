

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
#include "ecat_master.h"
#include "ecat_slave.h"
#include <time.h>
#include <sys/stat.h>


// EtherCAT
static ec_master_t *master = NULL;
static ec_master_state_t master_state = {};

static ec_domain_t *domain1 = NULL;
static ec_domain_state_t domain1_state = {};

static uint8_t *domain1_pd = NULL;

slaves_t slaves;

//EcatImu imux(0,0);

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
        printf("Domain1: WC %u.\n", ds.working_counter);
    }

    if (ds.wc_state != domain1_state.wc_state)
    {
        printf("Domain1: State %u.\n", ds.wc_state);
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
        printf("%u slave(s).\n", ms.slaves_responding);
    }

    if (ms.al_states != master_state.al_states)
    {
        printf("AL states: 0x%02X.\n", ms.al_states);
    }

    if (ms.link_up != master_state.link_up)
    {
        printf("Link is %s.\n", ms.link_up ? "up" : "down");
    }

    master_state = ms;
}


void *thread_func(void *data)
{
    struct timespec systime;
    struct timespec lasttime = {0, 0};
    struct timespec looptime;
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

                slaves.wmio_0.DataRead(domain1_pd);
            }

            //time test
            {
                for(uint32_t i=0; i<300; i++)
                {
                    x = tan(i);
                    y = atan(i);
                    z += x+y;
                    y=z;
                }

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
//                    last_time_us = lasttime.tv_sec == 0? 1000:1000000*(systime.tv_sec - lasttime.tv_sec) + (systime.tv_nsec - lasttime.tv_nsec)/1000;
                    min_time_us = min_time_us > last_time_us?   last_time_us:min_time_us;
                    max_time_us = max_time_us < last_time_us?   last_time_us:max_time_us;
                    lasttime = systime;
                }
            }
            //distribute clock
//            ecrt_master_application_time(master, 1000000000*systime.tv_sec + systime.tv_nsec);
//            ecrt_master_sync_reference_clock(master);
//            ecrt_master_sync_slave_clocks(master);

            // send process data
            ecrt_domain_queue(domain1);
            ecrt_master_send(master);
        }

    }
out_loop:
        return NULL;
}

void ShowVec1(const std::vector<int>& valList)
{
    int count = valList.size();
    for (int i = 0; i < count;i++)
    {
        std::cout << valList[i] << std::endl;
    }
}

int main(int argc, char* argv[])
{
        struct sched_param param;
        pthread_attr_t attr;
        pthread_t thread;
        int ret;

        /* Lock memory */
        if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
                printf("mlockall failed: %m\n");
                exit(-2);
        }

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
        ret = pthread_create(&thread, &attr, thread_func, NULL);
        if (ret) {
                printf("create pthread failed\n");
                goto out;
        }

        //ecat
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

            #ifdef WMIO_Pos_2
                slaves.wmio_0.Init(master, WMIO_Pos_2);
            #endif

            #ifdef MOTOR_Pos_0
                slaves.motor_0.Init(master, MOTOR_Pos_0);
            #endif


            #ifdef WMLAN9252_IO_POS
                {
                    sc_lan9252_01 = ecrt_master_slave_config(master, WMLAN9252_IO_POS, WMLAN9252_IO);
                    if (!sc_lan9252_01)
                    {
                        LOG(ERROR) << "Failed to configure slave" << std::endl;
                        return -1;
                    }

                    if (ecrt_slave_config_pdos(sc_lan9252_01, EC_END, wmlan9252_io_syncs))
                    {
                        LOG(ERROR) << "Failed to configure PDOs" << std::endl;
                        return -1;
                    }
                }
            #endif

//            if (ecrt_domain_reg_pdo_entry_list(domain1, domain1_regs))

//            if (ecrt_domain_reg_pdo_entry_list(domain1, &EcatSlave::domain_regs[0]))
             if (ecrt_domain_reg_pdo_entry_list(domain1, EcatSlave::domain_regs.data()))
            {
//                LOG(ERROR) << "PDO entry registration failed!" << std::endl;
                return -1;
            }

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

        ret = pthread_detach(thread);
        if (ret)    printf("join pthread failed: %m\n");

        while (1)
        {
            sleep(1);
            std::cout<< "cycle times: " << loop_counter << "    last_time_us: " << last_time_us << "    min_time_us: " << min_time_us << "    max_time_us: " << max_time_us << std::endl;
            std::cout<< "z: " << z << std::endl << std::endl;

            #ifdef IMU_Pos_0
            slaves.imu_0.DataPlay();
            #endif

            #ifdef IMU_Pos_1
            slaves.imu_1.DataPlay();
            #endif
            slaves.wmio_0.DataPlay();

            std::cout<< "number of domain_regs: " << EcatSlave::domain_regs.size() << std::endl << std::endl << std::endl;


        }

//            LOG(INFO) <<"End of Program"<<std::endl;
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



///****************************************************************************/
//// process data

//slaves_t slaves;


//void GlogInit(char **argv)
//{
//    char defpath[100] = "./glog";
//    //创建log文件夹
//     mkdir("./glog",0777);
//     //log初始化
//     google::InitGoogleLogging(argv[0]);

//     FLAGS_colorlogtostderr = true;//设置输出到屏幕的日志显示相应颜色
//     FLAGS_logbufsecs = 5;//缓冲日志输出，默认为30秒，0为立即输出
//     FLAGS_max_log_size = 1; //最大日志大小为 1MB
//     FLAGS_stop_logging_if_full_disk = true;//当磁盘被写满时，停止日志输出
//     google::SetStderrLogging(google::GLOG_INFO); //设置级别 高于 google::INFO 的日志同时输出到屏幕GLOG_INFO
//     //GLOG_WARNING
//     //日志名称和输出地址
//     char Info[50] = {0};
//     char Warn[50] = {0};
//     char Error[50] = {0};
//     char Fatal[50] = {0};

//     strcpy(Info, defpath);
//     strcpy(Warn, defpath);
//     strcpy(Error, defpath);
//     strcpy(Fatal, defpath);

//     strcat(Info, "/Info_");
//     strcat(Warn, "/Warning_");
//     strcat(Error, "/Error_");
//     strcat(Fatal, "/Fatal_");
//     google::SetLogDestination(google::GLOG_INFO,Info);//设置特定严重级别的日志的输出目录和前缀。第一个参数为日志级别，第二个参数表示输出目录及日志文件名前缀
////     google::SetLogDestination(google::GLOG_WARNING,Warn);
////     google::SetLogDestination(google::GLOG_ERROR,Error);
////     google::SetLogDestination(google::GLOG_FATAL,Fatal);

////     while(1)
//     {
//         LOG(INFO) <<"glog start"<<std::endl;
////         LOG(WARNING) <<"warning!!!!!!!!!"<<std::endl;
////         LOG(ERROR) <<"error!!!!!!!!!!!!!"<<std::endl;
//     }

//}



//// process data
//const static ec_pdo_entry_reg_t domain1_regs[] =
//{

//#ifdef FORCE_Pos_0
//    {FORCE_Pos_0,  FORCE, 0x3101, 0x01, &slaves.force_0.offset.f32data, NULL},
//#endif

//#ifdef IMU_Pos_0
//    {IMU_Pos_0,  IMU, 0x6000, 0x01, &slaves.imu_0.offset.gx, NULL},
//    {IMU_Pos_0,  IMU, 0x6000, 0x02, &slaves.imu_0.offset.gy, NULL},
//    {IMU_Pos_0,  IMU, 0x6000, 0x03, &slaves.imu_0.offset.gz, NULL},
//    {IMU_Pos_0,  IMU, 0x6000, 0x04, &slaves.imu_0.offset.ax, NULL},
//    {IMU_Pos_0,  IMU, 0x6000, 0x05, &slaves.imu_0.offset.ay, NULL},
//    {IMU_Pos_0,  IMU, 0x6000, 0x06, &slaves.imu_0.offset.az, NULL},
//    {IMU_Pos_0,  IMU, 0x6000, 0x07, &slaves.imu_0.offset.counter, NULL},
//    {IMU_Pos_0,  IMU, 0x7011, 0x01, &slaves.imu_0.offset.led, NULL},
//#endif

//#ifdef IMU_Pos_1
//    {IMU_Pos_1,  IMU, 0x6000, 0x01, &slaves.imu_1.offset.gx, NULL},
//    {IMU_Pos_1,  IMU, 0x6000, 0x02, &slaves.imu_1.offset.gy, NULL},
//    {IMU_Pos_1,  IMU, 0x6000, 0x03, &slaves.imu_1.offset.gz, NULL},
//    {IMU_Pos_1,  IMU, 0x6000, 0x04, &slaves.imu_1.offset.ax, NULL},
//    {IMU_Pos_1,  IMU, 0x6000, 0x05, &slaves.imu_1.offset.ay, NULL},
//    {IMU_Pos_1,  IMU, 0x6000, 0x06, &slaves.imu_1.offset.az, NULL},
//    {IMU_Pos_1,  IMU, 0x6000, 0x07, &slaves.imu_1.offset.counter, NULL},
//    {IMU_Pos_1,  IMU, 0x7011, 0x01, &slaves.imu_1.offset.led, NULL},
//#endif

//#ifdef MOTOR_Pos_0
//    {MOTOR_Pos_0,  ELMO, 0x607a, 0x00, &slaves.motor_0.offset.target_pos, NULL},
//    {MOTOR_Pos_0,  ELMO, 0x60ff, 0x00, &slaves.motor_0.offset.target_vel, NULL},
//    {MOTOR_Pos_0,  ELMO, 0x6071, 0x00, &slaves.motor_0.offset.target_tor, NULL},
//    {MOTOR_Pos_0,  ELMO, 0x6072, 0x00, &slaves.motor_0.offset.max_tor, NULL},
//    {MOTOR_Pos_0,  ELMO, 0x6040, 0x00, &slaves.motor_0.offset.control_word, NULL},
//    {MOTOR_Pos_0,  ELMO, 0x6060, 0x00, &slaves.motor_0.offset.mode, NULL},
//    {MOTOR_Pos_0,  ELMO, 0x60b1, 0x00, &slaves.motor_0.offset.vel_offset, NULL},
//    {MOTOR_Pos_0,  ELMO, 0x60b2, 0x00, &slaves.motor_0.offset.tor_offset, NULL},

//    {MOTOR_Pos_0,  ELMO, 0x6064, 0x00, &slaves.motor_0.offset.actual_pos, NULL},
//    {MOTOR_Pos_0,  ELMO, 0x606c, 0x00, &slaves.motor_0.offset.actual_vel, NULL},
//    {MOTOR_Pos_0,  ELMO, 0x6078, 0x00, &slaves.motor_0.offset.actual_cur, NULL},
//    {MOTOR_Pos_0,  ELMO, 0x6077, 0x00, &slaves.motor_0.offset.actual_tor, NULL},
//    {MOTOR_Pos_0,  ELMO, 0x6041, 0x00, &slaves.motor_0.offset.status_word, NULL},
//    {MOTOR_Pos_0,  ELMO, 0x6061, 0x00, &slaves.motor_0.offset.mode_display, NULL},
//#endif

//#ifdef WMLAN9252_IO_POS
//    {WMLAN9252_IO_POS,  WMLAN9252_IO, 0x7000, 0x01, &off_lan9252_io.leds, NULL},
//    {WMLAN9252_IO_POS,  WMLAN9252_IO, 0x6000, 0x01, &off_lan9252_io.keys, NULL},
//    {WMLAN9252_IO_POS,  WMLAN9252_IO, 0x6020, 0x01, &off_lan9252_io.analog_data, NULL},
//#endif
//    {}
//};

///*****************************************************************************
// * Realtime task
// ****************************************************************************/
//void rt_check_domain_state(void)
//{
//    ec_domain_state_t ds = {};

//    ecrt_domain_state(domain1, &ds);

//    if (ds.working_counter != domain1_state.working_counter)
//    {
//        rt_printf("Domain1: WC %u.\n", ds.working_counter);
//    }

//    if (ds.wc_state != domain1_state.wc_state)
//    {
//        rt_printf("Domain1: State %u.\n", ds.wc_state);
//    }

//    domain1_state = ds;
//}

///****************************************************************************/
//void rt_check_master_state(void)
//{
//    ec_master_state_t ms;

//    ecrt_master_state(master, &ms);

//    if (ms.slaves_responding != master_state.slaves_responding)
//    {
//        rt_printf("%u slave(s).\n", ms.slaves_responding);
//    }

//    if (ms.al_states != master_state.al_states)
//    {
//        rt_printf("AL states: 0x%02X.\n", ms.al_states);
//    }

//    if (ms.link_up != master_state.link_up)
//    {
//        rt_printf("Link is %s.\n", ms.link_up ? "up" : "down");
//    }

//    master_state = ms;
//}

///****************************************************************************/
//void my_task_proc(void *arg)
//{
//    rt_task_set_periodic(NULL, TM_NOW, 1000000); // ns

//    while (run)
//    {
//        rt_task_wait_period(NULL);

//        // receive EtherCAT frames
//        ecrt_master_receive(master);
//        ecrt_domain_process(domain1);

//        //
//        rt_check_domain_state();
//        rt_check_master_state();

////        if(master_state.slaves_responding == static_cast<unsigned int>(1))
//        if(master_state.al_states == 0x08)
//        {
//            ControlTask(domain1_pd, slaves);
//        }

//        //distribute clock
//        ecrt_master_application_time(master, rt_timer_read());
//        ecrt_master_sync_reference_clock(master);
//        ecrt_master_sync_slave_clocks(master);

//        // send process data
//        ecrt_domain_queue(domain1);
//        ecrt_master_send(master);
//    }
//}

///****************************************************************************
// * Signal handler
// ***************************************************************************/

//void signal_handler(int sig)
//{
//    run = 0;
//}

//void TimeThread(void)
//{
//    time_t timep;
//    std::string tm;
//    while (1)
//    {
//        time(&timep);
//        tm = ctime(&timep);
//        for(int i = 0; i < tm.size(); i++)
//        {
//            if(tm.at(i) == ' ') tm.at(i)='_';
//            else if(tm.at(i) == ':') tm.at(i)='.';
//        }
//        std::cout<< tm;
//        std::cout<<"hell"<<std::endl;
//        usleep(1000);
//    }
//}

///****************************************************************************
// * Main function
// ***************************************************************************/
//int main(int argc, char *argv[])
//{
////    chdir(dirname(argv[0])); //设置当前目录为应用程序所在的目录。
////    LoadXML();

////    exit(0);

//    GlogInit(argv);

////    std::thread time(TimeThread);
////    time.detach();

//    std::thread record(RecordThread);
//    record.detach();


//    /* Perform auto-init of rt_print buffers if the task doesn't do so */
//    rt_print_auto_init(1);

//    signal(SIGTERM, signal_handler);
//    signal(SIGINT, signal_handler);

//    mlockall(MCL_CURRENT | MCL_FUTURE);

//    LOG(INFO) <<"Requesting master..."<<std::endl;
//    master = ecrt_request_master(0);
//    if (!master)
//    {
//        LOG(ERROR) << "ecrt_request_master error" <<std::endl;
//        return -1;
//    }

//    LOG(INFO) <<"ecrt_master_create_domain..."<<std::endl;
//    domain1 = ecrt_master_create_domain(master);
//    if (!domain1)
//    {
//        LOG(ERROR) << "ecrt_master_create_domain error" << std::endl;
//        return -1;
//    }

//    LOG(INFO) <<"Creating slave configurations..."<<std::endl;

//    #ifdef IMU_Pos_0
//        slaves.imu_0.Init(master,IMU_Pos_0);
//    #endif

//    #ifdef IMU_Pos_1
//        slaves.imu_0.Init(master,IMU_Pos_1);
//    #endif

//    #ifdef FORCE_Pos_0
//        slaves.force_0.Init(master, FORCE_Pos_0);
//    #endif

//    #ifdef MOTOR_Pos_0
//        slaves.motor_0.Init(master, MOTOR_Pos_0);
//    #endif


//    #ifdef WMLAN9252_IO_POS
//        {
//            sc_lan9252_01 = ecrt_master_slave_config(master, WMLAN9252_IO_POS, WMLAN9252_IO);
//            if (!sc_lan9252_01)
//            {
//                LOG(ERROR) << "Failed to configure slave" << std::endl;
//                return -1;
//            }

//            if (ecrt_slave_config_pdos(sc_lan9252_01, EC_END, wmlan9252_io_syncs))
//            {
//                LOG(ERROR) << "Failed to configure PDOs" << std::endl;
//                return -1;
//            }
//        }
//    #endif

//    if (ecrt_domain_reg_pdo_entry_list(domain1, domain1_regs))
//    {
//        LOG(ERROR) << "PDO entry registration failed!" << std::endl;
//        return -1;
//    }

//    LOG(INFO) <<"Activating master..."<<std::endl;
//    if (ecrt_master_activate(master))
//    {
//        LOG(ERROR) << "ecrt_master_activate failed!" << std::endl;
//        return -1;
//    }

//    LOG(INFO) <<"get domain data pointer..."<<std::endl;
//    if (!(domain1_pd = ecrt_domain_data(domain1)))
//    {
//        LOG(ERROR) << "Failed to get domain data pointer" << std::endl;
//        return -1;
//    }

//    LOG(INFO) <<"rt_task_create..."<<std::endl;
//    int ret = rt_task_create(&my_task, "my_task", 0, 99, T_FPU);
//    if (ret < 0)
//    {
//        LOG(ERROR) << "Failed to create task:"  << strerror(-ret) << std::endl;
//        return -1;
//    }

//    LOG(INFO) <<"Starting rt my_task..."<<std::endl;
//    ret = rt_task_start(&my_task, &my_task_proc, NULL);
//    if (ret < 0)
//    {
//        LOG(ERROR) << "Failed to start task:"  << strerror(-ret) << std::endl;
//        return -1;
//    }

//    while (run)
//    {
//        sched_yield();
//    }

//    LOG(INFO) <<"Deleting realtime task..."<<std::endl;
//    rt_task_delete(&my_task);

//    LOG(INFO) <<"End of Program"<<std::endl;
//    ecrt_release_master(master);

//    return 0;
//}

///**************************************************************************
