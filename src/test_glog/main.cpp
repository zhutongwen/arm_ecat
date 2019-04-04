
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
#include <sys/mman.h>

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



int main(int argc, char* argv[])
{
    GlogInit(argv);

    if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
            printf("mlockall failed: %m\n");
            exit(-2);
    }

    while(1)
    {
        LOG(INFO) <<"glog start"<<std::endl;
        LOG(WARNING) <<"warning!!!!!!!!!"<<std::endl;
        LOG(ERROR) <<"error!!!!!!!!!!!!!"<<std::endl;
    }

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
//  printf("line = %d\n",__LINE__);//行号
//    std::cout << __FILE__ << std::endl;
//  return 0;
//}
