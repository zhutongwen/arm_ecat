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
#include <ios>
#include <iostream>
#include <fstream>

#include <vector>

typedef struct
{
    int32_t  target_pos;
    int32_t  target_vel;
//    int16_t  target_tor;
//    int16_t  max_tor;
//    uint16_t control_word;
//    int8_t   mode;
//    int32_t  vel_offset;
//    int16_t  tor_offset;

    int32_t  actual_pos;
    int32_t  actual_vel;
    int16_t  actual_cur;
//    int16_t  actual_tor;
    uint16_t status_word;
    uint16_t error_code;
    int16_t   mode_display;
}motor_data_t;

typedef struct
{
    float gx;
    float gy;
    float gz;
    float ax;
    float ay;
    float az;
    uint32_t counter;
    uint16_t led;
}imu_data_t;

typedef struct
{
    uint32_t        counter;
    motor_data_t    motor[6];
    imu_data_t      imu;
}log_data_t;

//typedef struct
//{
//    char p;
//}print_data_t;



extern std::string space;
extern uint32_t printf_write_counter;
extern uint32_t printf_read_cunter;

#define LOG_BUFF        10000
#define PRINT_BUFF      1000
//#define RT_PRINT(x)     {vPrintdata.push_back(basename((char*)__FILE__) + space + std::to_string(__LINE__) + ": " + x + '\n');}
#define RT_PRINT(x)     {print_data[printf_write_counter%PRINT_BUFF] = (basename((char*)__FILE__) + space + std::to_string(__LINE__) + ": " + x + '\n'); printf_write_counter++;}

//extern std::vector<std::string> vLogdata;
extern std::vector<std::string> vPrintdata;

extern uint32_t log_write_counter;
extern uint32_t log_read_cunter;

extern log_data_t log_data[LOG_BUFF];
extern std::string print_data[PRINT_BUFF];

void *RTPrintThread(void *data);

//////////////////////////////////////////////////////////////



#endif
