#ifndef ECAT_MASTER_H
#define ECAT_MASTER_H

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


#include "ecat_imu.h"
#include "ecat_wmio.h"
#include "ecat_wmadc.h"
#include "ecat_motor.h"
#include "ecat_syckin_io.h"
#include "ecat_shidian_io.h"



#define PI  3.141592654


//#define IMU_Pos_1       0, 1


//#define IMU_Pos_1       0, 1


#define MOTOR_Pos_0       0, 0
#define IMU_Pos_0         0, 1
//#define IMU_Pos_1         0, 3
//#define IMU_Pos_2         0, 4
//#define IMU_Pos_3         0, 5
//#define IMU_Pos_4         0, 6
//#define SYCKIN_Pos_0      0, 0
//#define SHIDIAN_Pos_0     0, 0

//#define WMIO_Pos_0        0, 0
//#define WMADC_Pos_0       0, 1

typedef struct
{
#ifdef WMADC_Pos_0
    EcatWmadc wmadc_0;
#endif

#ifdef SHIDIAN_Pos_0
    EcatShiDianIO ShiDian_0;
#endif

#ifdef SYCKIN_Pos_0
    EcatSycKinIO SycKin_0;
#endif



#ifdef MOTOR_Pos_0
    EcatMotor motor_0;
#endif

#ifdef IMU_Pos_0
    EcatImu imu_0;
#endif

#ifdef IMU_Pos_1
    EcatImu imu_1;
#endif

#ifdef IMU_Pos_2
    EcatImu imu_2;
#endif

#ifdef IMU_Pos_3
    EcatImu imu_3;
#endif

#ifdef IMU_Pos_4
    EcatImu imu_4;
#endif

#ifdef WMIO_Pos_0
    EcatWmio wmio_0;
#endif


}slaves_t;

//extern slaves_t slaves;

void LoopTest(void);


class EcatMaster
{
public:
    ec_master_t *master = NULL;
    ec_master_state_t master_state = {};
    ec_domain_t *domain1 = NULL;
    ec_domain_state_t domain1_state = {};
    uint8_t *domain1_pd = NULL;

    slaves_t slaves;


    EcatMaster();
    ~EcatMaster();

    void rt_check_domain_state(void);
    void rt_check_master_state(void);
    void Init(void);
    void LoopTest(void);

private:


};
#endif
