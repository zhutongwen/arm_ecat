#include "rt_log.h"

int qid;
std::string space("  ");




void *RTLogThread(void *data)
{
    struct message msg;
    if ((qid = msgget(1, IPC_CREAT|0666)) == -1)
    {
        perror("msgget");
        exit(1);
    }
    do
    {
        if (msgrcv(qid, (void*)&msg, BUFFER_SIZE, 0, 0) < 0)
        {
            perror("msgrcv");
            exit(1);
        }
        std::cout << msg.msg_text << std::endl;
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
