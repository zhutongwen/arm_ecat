

//#include "vector.h"




//int main(int argc, char* argv[])
//{
////std::vector<int> valList = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

//    valList1.push_back(1);

//    ShowVec1(valList1);
//    ShowVec2(valList1);
//    ShowVec3(valList1);
//    return 0;
//}




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
#include <sys/stat.h>
#include <fstream>
#include <thread>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

#define BUFFER_SIZE 512


int qid;

struct message
{
    long msg_type;
//    char msg_text[BUFFER_SIZE];
    std::string msg_text;
};


void *MsgsndThread(void *data)
{
    struct message msg;
    while(1)
    {
        msg.msg_text= __FILE__;
        msg.msg_type = getpid();
        /*添加消息到消息队列*/
        if ((msgsnd(qid, &msg, msg.msg_text.length(), 0)) < 0)
        {
            perror("message posted");
            exit(1);
        }
        sleep(1);
    }
    exit(0);
}


void *MsgrcvThread(void *data)
{
    struct message msg;
    do
    {
        /*读取消息队列*/
//        memset(msg.msg_text.c_, 0, BUFFER_SIZE);
        msg.msg_text.clear();
        if (msgrcv(qid, (void*)&msg, BUFFER_SIZE, 0, 0) < 0)
        {
            perror("msgrcv");
            exit(1);
        }
        std::cout << "The message from process: " << msg.msg_type << msg.msg_text << std::endl;
    } while(strncmp(msg.msg_text.data(), "quit", 4));
    /*从系统内核中移走消息队列 */
    if ((msgctl(qid, IPC_RMID, NULL)) < 0)
    {
        perror("msgctl");
        exit(1);
    }
    exit(0);
}


int main(int argc, char* argv[])
{
        struct sched_param param;
        pthread_attr_t attr;
        pthread_t thread;
        int ret;


        /*创建消息队列*/
        if ((qid = msgget(1, IPC_CREAT|0666)) == -1)
        {
            perror("msgget");
            exit(1);
        }
        printf("Open queue %d\n", qid);

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
        param.sched_priority = 50;
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
        ret = pthread_create(&thread, &attr, MsgsndThread, NULL);
        if (ret) {
                printf("create pthread failed\n");
                goto out;
        }

        ret = pthread_detach(thread);
        if (ret)    printf("join pthread failed: %m\n");


        /* Create a pthread with specified attributes */
        ret = pthread_create(&thread, &attr, MsgrcvThread, NULL);
        if (ret) {
                printf("create pthread failed\n");
                goto out;
        }

        ret = pthread_detach(thread);
        if (ret)    printf("join pthread failed: %m\n");

        while (1)
        {
            sleep(1);
//            std::cout<< "cycle times: " << std::endl;

        }


out:
        return ret;
}
