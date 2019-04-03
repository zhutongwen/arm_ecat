
#include "glog/logging.h"
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

struct message
{
    long msg_type;
    char msg_text[BUFFER_SIZE];
};

void GlogInit(char **argv)
{
    char defpath[100] = "./glog";
    //创建log文件夹
     mkdir("./glog",0777);
     //log初始化
     google::InitGoogleLogging(argv[0]);

     FLAGS_colorlogtostderr = true;//设置输出到屏幕的日志显示相应颜色
     FLAGS_logbufsecs = 5;//缓冲日志输出，默认为30秒，0为立即输出
     FLAGS_max_log_size = 1; //最大日志大小为 1MB
     FLAGS_stop_logging_if_full_disk = true;//当磁盘被写满时，停止日志输出
     google::SetStderrLogging(google::GLOG_INFO); //设置级别 高于 google::INFO 的日志同时输出到屏幕GLOG_INFO
     //GLOG_WARNING
     //日志名称和输出地址
     char Info[50] = {0};
     char Warn[50] = {0};
     char Error[50] = {0};
     char Fatal[50] = {0};

     strcpy(Info, defpath);
     strcpy(Warn, defpath);
     strcpy(Error, defpath);
     strcpy(Fatal, defpath);

     strcat(Info, "/Info_");
     strcat(Warn, "/Warning_");
     strcat(Error, "/Error_");
     strcat(Fatal, "/Fatal_");
     google::SetLogDestination(google::GLOG_INFO,Info);//设置特定严重级别的日志的输出目录和前缀。第一个参数为日志级别，第二个参数表示输出目录及日志文件名前缀
     google::SetLogDestination(google::GLOG_WARNING,Warn);
     google::SetLogDestination(google::GLOG_ERROR,Error);
     google::SetLogDestination(google::GLOG_FATAL,Fatal);

//     while(1)
     {
         LOG(INFO) <<"glog start"<<std::endl;
         LOG(WARNING) <<"warning!!!!!!!!!"<<std::endl;
         LOG(ERROR) <<"error!!!!!!!!!!!!!"<<std::endl;
     }

}
void MsgsndThread(void)
{
    {
//        int qid;
//        key_t key;
//        struct message msg;
//        /*根据不同的路径和关键字产生标准的 key*/
//        if ((key = ftok(".", 'a')) == -1)
//        {
//            perror("ftok");
//            exit(1);
//        }
//        /*创建消息队列*/
//        if ((qid = msgget(key, IPC_CREAT|0666)) == -1)
//        {
//            perror("msgget");
//            exit(1);
//        }
//        printf("Open queue %d\n",qid);
//        while(1)
//        {
//            printf("Enter some message to the queue:");
//            if ((fgets(msg.msg_text, BUFFER_SIZE, stdin)) == NULL)
//            {
//                puts("no message");
//                exit(1);
//            }
//            msg.msg_type = getpid();
//            /*添加消息到消息队列*/
//            if ((msgsnd(qid, &msg, strlen(msg.msg_text), 0)) < 0)
//            {
//                perror("message posted");
//                exit(1);
//            }
//            if (strncmp(msg.msg_text, "quit", 4) == 0)
//            {
//                break;
//            }
//        }
//        exit(0);
    }
}

void MsgrcvThread(void)
{
    {
//        int qid;
//        key_t key;
//        struct message msg;

        /*根据不同的路径和关键字产生标准的 key*/
//        if ((key = ftok(".", 'a')) == -1)
//        {
//            perror("ftok");
//            exit(1);
//        }
//        /*创建消息队列*/
//        if ((qid = msgget(key, IPC_CREAT|0666)) == -1)
//        {
//            perror("msgget");
//            exit(1);
//        }
//        printf("Open queue %d\n", qid);
//        do
//        {
//            /*读取消息队列*/
//            memset(msg.msg_text, 0, BUFFER_SIZE);
//            if (msgrcv(qid, (void*)&msg, BUFFER_SIZE, 0, 0) < 0)
//            {
//                perror("msgrcv");
//                exit(1);
//            }
////            printf("The message from process %d : %s", msg.msg_type, msg.msg_text);
//            std::cout << "The message from process" << msg.msg_type << msg.msg_text << std::endl;
//        } while(strncmp(msg.msg_text, "quit", 4));
//        /*从系统内核中移走消息队列 */
//        if ((msgctl(qid, IPC_RMID, NULL)) < 0)
//        {
//            perror("msgctl");
//            exit(1);
//        }
//        exit(0);
    }
}


int main(int argc, char* argv[])
{
//    GlogInit(argv);

//    std::fstream examplefile;
//    examplefile.open("example.txt");

//    if (examplefile.is_open())
//    {
//        LOG(INFO) << "record start!\n";
//        examplefile << "This is a line.\n";
//        examplefile << "This is another line.\n";
//        examplefile.close();
//    }
//    else
//    {
//        LOG(ERROR) << "record error!\n";
//    }


    std::thread msgsnd(MsgsndThread);
    msgsnd.detach();

//    std::thread msgrcv(MsgrcvThread);
//    msgrcv.detach();

    while(1)
    {
        std::cout <<"123456" << std::endl;
        sleep(1);

    }

    return 0;
}

//#include<stdio.h>
//#include <iostream>
//#include <fstream>

//int main()
//{
//  char file[100];
//  char func[100];
//  int line;
//  sprintf(file,__FILE__); //文件名
//  sprintf(func,__FUNCTION__);//函数名
//  printf("file=%s\n",__FILE__);
//  printf("func=%s\n",func);
//  printf("%05d\n",__LINE__);//行号
//    std::cout << __FILE__ << std::endl;
//    std::cout<< "clock_gettime error !!!" <<std::endl;
//  return 0;
//}
