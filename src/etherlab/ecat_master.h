#ifndef ECAT_MASTER_H
#define ECAT_MASTER_H




#include "ecat_imu.h"
#include "ecat_wmio.h"


#define PI  3.141592654

//#define IMU_Pos_0       0, 2
//#define IMU_Pos_1       0, 1
#define WMIO_Pos_2      0, 0

//#define IMU_Pos_1       0, 1
//#define MOTOR_Pos_0     0, 0

typedef struct
{

#ifdef IMU_Pos_0
    EcatImu imu_0;
#endif

#ifdef IMU_Pos_1
    EcatImu imu_1;
#endif

#ifdef WMIO_Pos_2
    EcatWmio wmio_0;
#endif

#ifdef MOTOR_Pos_0
    EcatMotor motor_0;
#endif

}slaves_t;
#endif
