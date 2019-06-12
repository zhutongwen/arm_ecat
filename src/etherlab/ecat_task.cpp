#include "ecat_task.h"
#include <math.h>


extern EcatMaster  EMaster;
void ControlTask(uint8_t *domain1_pd_, slaves_t &slaves_)
{
//    vLogdata.push_back(std::to_string(EMaster.u32GlobalCounter++) + ' ');
//    RT_log_file << EMaster.u32GlobalCounter++ << ' ';
    /****************************************************************************/
    // motor
    #ifdef MOTOR_Pos_0
    for(int i = 0; i < 6; i++)
    {
        slaves_.motor[i].DataRead(domain1_pd_);

        switch (slaves_.motor[i].motor_state)
        {
            case slaves_.motor[i].STATE_INIT:
                slaves_.motor[i].Enable(domain1_pd_);
            break;

            case slaves_.motor[i].STATE_ENABLED:
                slaves_.motor[i].Homing(domain1_pd_);
            break;

            case slaves_.motor[i].STATE_HOMED:
                slaves_.motor[i].SetMode(domain1_pd_, slaves_.motor[i].MODE_CSV);//static_cast<mode_t>
            break;

            case slaves_.motor[i].STATE_CSV:
            {
                static float f32angle = 0;

                f32angle += 0.0001;
                if(f32angle >= 1) f32angle = 0;

                slaves_.motor[i].data.target_vel = 1.0*65535.0*sin(f32angle*(2.0*PI));
//                slaves_.motor_0.data.target_vel = 20*65536*slaves_.imu_0.data.ax;

                slaves_.motor[i].SetTargtVelocity(domain1_pd_, static_cast<int32_t>(slaves_.motor[i].data.target_vel));
            }
            break;

//            case slaves_.motor_0.STATE_CST:
//            {
//                 slaves_.motor_0.SetTargtTorque(domain1_pd_, static_cast<int16_t>(100));
//            }
            break;
            default:
                break;
        }
    }
    #endif

    #ifdef SHIDIAN_Pos_0
    static uint32_t i=0;
    {
        static uint16_t counter = 0;

        counter = ~counter;

        slaves_.ShiDian_0.DataRead(domain1_pd_);
        EC_WRITE_U16(domain1_pd_ + slaves_.ShiDian_0.offset.u16Dos, counter);
    }
    #endif

    #ifdef SYCKIN_Pos_0
        static uint32_t i=0;
//        if(++i > 500)
        {
            static uint16_t counter = 0;
//            counter <<= 1;
//            if(!counter) counter = 1;
//            i = 0;

            counter = ~counter;

            slaves_.SycKin_0.DataRead(domain1_pd_);
            EC_WRITE_U16(domain1_pd_ + slaves_.SycKin_0.offset.u16Dos, counter);

//            std::cout<< "DIs: " << slaves_.SycKin_0.data.u16Dis << std::endl;
//            std::cout<< "DOs: " << counter << std::endl;
//            std::cout<< std::endl;
        }
    #endif

    #ifdef IMU_Pos_0
        slaves_.imu_0.DataRead(domain1_pd_);
    #endif

    #ifdef IMU_Pos_1
        slaves_.imu_1.DataRead(domain1_pd_);
    #endif

    #ifdef IMU_Pos_2
        slaves_.imu_2.DataRead(domain1_pd_);
    #endif

    #ifdef IMU_Pos_3
        slaves_.imu_3.DataRead(domain1_pd_);
    #endif

    #ifdef IMU_Pos_4
        slaves_.imu_4.DataRead(domain1_pd_);
    #endif

    #ifdef  WMIO_Pos_0
        slaves_.wmio_0.DataRead(domain1_pd_);
        EC_WRITE_U16(domain1_pd_ + slaves_.wmio_0.offset.leds, slaves_.wmio_0.data.led0_7++);
    #endif

    #ifdef  WMADC_Pos_0
        static uint32_t i=0;
        if(++i>200)
        {
            i=0;
            slaves_.wmadc_0.data.led0_7++;
        }
        slaves_.wmadc_0.DataRead(domain1_pd_);
        EC_WRITE_U8(domain1_pd_ + slaves_.wmadc_0.offset.leds, slaves_.wmadc_0.data.led0_7);
    #endif
}
