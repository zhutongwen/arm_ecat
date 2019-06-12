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
#include "ecat_task.h"

//uint: us
#define LOOP_PERIOD 1000

uint32_t loop_counter = 0;
uint16_t min_time_us = LOOP_PERIOD;
uint16_t max_time_us = LOOP_PERIOD;
uint16_t last_time_us = 0;

EcatMaster  EMaster;

static inline void tsnorm(struct timespec *ts)
{
    while (ts->tv_nsec >= 1000000000)
    {
        ts->tv_nsec -= 1000000000;
        ts->tv_sec++;
    }
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
        looptime.tv_nsec += 1000*LOOP_PERIOD;
        tsnorm(&looptime);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &looptime, NULL);

        //time test
        {
            loop_counter++;
            if (clock_gettime(CLOCK_REALTIME, &systime) == -1)
            {
                std::cout<< "clock_gettime error !!!" <<std::endl;
                goto out_loop;
            }

            {
                static uint32_t i=0;
                //if(i++ < 10000) last_time_us = min_time_us;
                if(EMaster.master_state.al_states != 0x08) last_time_us = LOOP_PERIOD;
                else last_time_us = 1000000*(systime.tv_sec - lasttime.tv_sec) + (systime.tv_nsec - lasttime.tv_nsec)/1000;
                min_time_us = min_time_us > last_time_us?   last_time_us:min_time_us;
                max_time_us = max_time_us < last_time_us?   last_time_us:max_time_us;
                lasttime = systime;
            }
        }


        // receive EtherCAT frames
        ecrt_master_receive(EMaster.master);
        ecrt_domain_process(EMaster.domain1);

        //
        EMaster.rt_check_domain_state();
        EMaster.rt_check_master_state();

        static int is_link = 0;
        if(EMaster.master_state.al_states == 0x08) is_link = 1;


        if(is_link)
        {
            ControlTask(EMaster.domain1_pd, EMaster.slaves);

            if(EMaster.master_state.al_states != 0x08)
            {
                std::cout << "error" << std::endl;
                break;
            }
        }

        //distribute clock
        #if (1)
        {
            ecrt_master_application_time(EMaster.master, 1000000000*((uint64_t)systime.tv_sec) + systime.tv_nsec);
            //ecrt_master_application_time(master, systime.tv_nsec);
            ecrt_master_sync_reference_clock(EMaster.master);
            ecrt_master_sync_slave_clocks(EMaster.master);
        }
        #endif

        // send process data
        ecrt_domain_queue(EMaster.domain1);
        ecrt_master_send(EMaster.master);
    }

out_loop:
        return NULL;
}

int main(int argc, char* argv[])
{
    chdir(dirname(argv[0])); //设置当前目录为应用程序所在的目录。
    struct sched_param param;
    pthread_attr_t attr;
    pthread_t thread;
    int ret;

    pthread_t thread_print;
    if (pthread_create(&thread_print, NULL, RTPrintThread, NULL))
    {
        printf("create pthread failed\n");
        goto out;
    }
    if (pthread_detach(thread_print))
    {
        printf("join pthread failed: %m\n");
        goto out;
    }

    EMaster.Init();

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
    ret = pthread_attr_setstacksize(&attr, 10 * PTHREAD_STACK_MIN);
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

    //ethercat loop
    {
        param.sched_priority = 99;
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
    }

    while (1)
    {
        sleep(1);
        RT_PRINT("cycle times: " + std::to_string(loop_counter));
//        RT_PRINT("last_time_us: " + std::to_string(last_time_us));
        RT_PRINT("min_time_us: " + std::to_string(min_time_us));
        RT_PRINT("max_time_us: " + std::to_string(max_time_us));

//        EMaster.slaves.imu_0.DataPlay();
        std::cout<< std::endl;

    }

    std::cout <<"End of Program"<<std::endl;
    ecrt_release_master(EMaster.master);
out:
    return ret;
}
