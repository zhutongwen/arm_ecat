#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include <iostream>
#include <time.h>

#define BUFFER_SIZE 512
struct message
{
    long msg_type;
    //    char msg_text[BUFFER_SIZE];
    std::string msg_text;
};
int main()
{
    int qid;
    key_t key;
    struct message msg;
    msg.msg_text += "qwertyuioppasdfghjklzxcvbnm";
    /*根据不同的路径和关键字产生标准的 key*/
    if ((key = ftok(".", 'a')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    /*创建消息队列*/
    if ((qid = msgget(key, IPC_CREAT|0666)) == -1)
    {
        perror("msgget");
        exit(1);
    }
    printf("Open queue %d\n",qid);

    struct timespec systime;
    msg.msg_type = getpid();

    while(1)
    {
        clock_gettime(CLOCK_REALTIME, &systime);
        std::cout << "time in: " << systime.tv_sec << "." << systime.tv_nsec << std::endl;



//        /*添加消息到消息队列*/
//        if ((msgsnd(qid, &msg, strlen(msg.msg_text), 0)) < 0)
//        {
//            perror("message posted");
//            exit(1);
//        }

        msgsnd(qid, &msg, msg.msg_text.length(), 0);
//        msgsnd(qid, &msg, msg.msg_text.length(), 0);
//        msgsnd(qid, &msg, msg.msg_text.length(), 0);
//        msgsnd(qid, &msg, msg.msg_text.length(), 0);
//        msgsnd(qid, &msg, msg.msg_text.length(), 0);
//        msgsnd(qid, &msg, msg.msg_text.length(), 0);
//        msgsnd(qid, &msg, msg.msg_text.length(), 0);
//        msgsnd(qid, &msg, msg.msg_text.length(), 0);
//        msgsnd(qid, &msg, msg.msg_text.length(), 0);
//        msgsnd(qid, &msg, msg.msg_text.length(), 0);


        clock_gettime(CLOCK_REALTIME, &systime);
        std::cout << "time out: " << systime.tv_sec << "." << systime.tv_nsec << std::endl << std::endl;

        usleep(1000000);

    }
    exit(0);
}
