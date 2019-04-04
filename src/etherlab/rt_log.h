#ifndef RT_LOG_H
#define RT_LOG_H


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
#include <sstream>
#include <iostream>
#include <string>
#include <libgen.h>

#define BUFFER_SIZE 512
#define RT_PRINT(x) {   struct message msg; \
                        msg.msg_type = getpid(); \
                        msg.msg_text = basename((char*)__FILE__) + space + std::to_string(__LINE__) + ": " + x; \
                        msgsnd(qid, &msg, msg.msg_text.length(), 0);}




struct message
{
    long msg_type;
    std::string msg_text;
};

extern int qid;
extern std::string space;

void *RTLogThread(void *data);

#endif
