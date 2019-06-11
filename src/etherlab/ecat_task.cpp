#include "ecat_task.h"
#include <math.h>



void ControlTask(uint8_t *domain1_pd_, slaves_t &slaves_)
{
    /****************************************************************************/
    // motor
    #ifdef MOTOR_Pos_0
    {
//        vLogdata.push_back(std::to_string(slaves_.motor_0.data.actual_pos) + ' ');
//        vLogdata.push_back(std::to_string(slaves_.motor_0.data.target_pos) + ' ');
        slaves_.motor_0.DataRead(domain1_pd_);
//        slaves_.motor_0.DataRTlog(vLogdata);

        switch (slaves_.motor_0.motor_state)
        {
            case slaves_.motor_0.STATE_INIT:
                slaves_.motor_0.Enable(domain1_pd_);
            break;

            case slaves_.motor_0.STATE_ENABLED:
                slaves_.motor_0.Homing(domain1_pd_);
            break;

            case slaves_.motor_0.STATE_HOMED:
                slaves_.motor_0.SetMode(domain1_pd_, slaves_.motor_0.MODE_CSV);//static_cast<mode_t>
            break;

            case slaves_.motor_0.STATE_CSV:
            {
                static float f32angle = 0;

                f32angle += 0.0002;
                if(f32angle >= 1) f32angle = 0;

                slaves_.motor_0.data.target_vel = 5.0*65535.0*sin(f32angle*(2.0*PI));
                slaves_.motor_0.SetTargtVelocity(domain1_pd_, static_cast<int32_t>(slaves_.motor_0.data.target_vel));
            }
            break;

            case slaves_.motor_0.STATE_CST:
            {
                 slaves_.motor_0.SetTargtTorque(domain1_pd_, static_cast<int16_t>(100));
            }
            break;
            default:
                break;
        }
        vLogdata.push_back(std::to_string(slaves_.motor_0.data.actual_pos) + ' ');
        vLogdata.push_back(std::to_string(slaves_.motor_0.data.target_pos) + ' ');
        vLogdata.push_back(std::to_string(slaves_.motor_0.data.actual_vel) + ' ');
        vLogdata.push_back(std::to_string(slaves_.motor_0.data.target_vel) + ' ');
        vLogdata.push_back(std::to_string(slaves_.motor_0.data.mode_display) + ' ');
        vLogdata.push_back(std::to_string(slaves_.motor_0.data.status_word) + ' ');
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

    vLogdata.push_back("\n");
}
