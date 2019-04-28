#include "rt_log.h"
#include <ios>
#include <iostream>
#include <fstream>

int qid;
std::string space("  ");




void *RTLogThread(void *data)
{
    struct message msg;
    static std::fstream log_file;//("log_file.txt");
//    if(log_file.is_open())     log_file << "msg.msg_text.c_str()" << std::endl;
//    std::streambuf *x = log_file.rdbuf();
//    std::cout.rdbuf(log_file.rdbuf());
    if ((qid = msgget(1, IPC_CREAT|0666)) == -1)
    {
        perror("msgget");
        exit(1);
    }

    log_file.close();
    log_file.open("/tmp/log_file.txt", std::ios::out | std::ios::trunc);
    if(log_file.is_open())     log_file << "msg.msg_text.c_str()" << std::endl;

    do
    {
        if (msgrcv(qid, (void*)&msg, BUFFER_SIZE, 0, 0) < 0)
        {
            perror("msgrcv");
            exit(1);
        }


        if(log_file.is_open())     log_file << msg.msg_text.c_str() << std::endl;
        std::cout << msg.msg_text.c_str() << std::endl;
//        std::cout.rdbuf(x);

        msg.msg_text.clear();
    } while(strncmp(msg.msg_text.data(), "quit", 4));
    /*从系统内核中移走消息队列 */
    if ((msgctl(qid, IPC_RMID, NULL)) < 0)
    {
        perror("msgctl");
        exit(1);
    }
    exit(0);
}
