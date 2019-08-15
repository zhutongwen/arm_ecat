#include "ecat_task.h"
#include <math.h>


extern EcatMaster  EMaster;
void ControlTask(uint8_t *domain1_pd_, slaves_t &slaves_)
{

//    vLogdata.push_back(std::to_string(EMaster.u32GlobalCounter++) + ' ');


    // motor
    #if 1
    {
        for(int i=0; i<NUM_OF_MOTOR; i++)
        {
            slaves_.motor[i].DataRead(domain1_pd_);
        }

        #if 1
        switch (EMaster.m_state)
        {
            case 0:
                for(int i=0; i<NUM_OF_MOTOR; i++)
                {
                    slaves_.motor[i].Enable(domain1_pd_, (void*)i);

                    EMaster.m_state = -1;
                    if(slaves_.motor[i].motor_state != slaves_.motor[0].MortrState::ENABLED)  EMaster.m_state = 0;
                }
            break;

            case 2:
                for(int i=0; i<NUM_OF_MOTOR; i++)
                {
                    slaves_.motor[i].Homing(domain1_pd_, (void*)i);

                    EMaster.m_state = -1;
                    if(slaves_.motor[i].motor_state != slaves_.motor[0].MortrState::HOMED)  EMaster.m_state = 2;
                }
            break;

            case 4:
                for(int i=0; i<NUM_OF_MOTOR; i++)
                {
                    slaves_.motor[i].SetMode(domain1_pd_, slaves_.motor[0].MODE_CSV);

                    EMaster.m_state = -1;
                    if(slaves_.motor[i].motor_state != slaves_.motor[0].MortrState::RUN_CSV_MODE)  EMaster.m_state = 4;
                }
            break;

            case 6:
            {
                static float f32angle = 0;

                f32angle += 0.0001;
                if(f32angle >= 1) f32angle = 0;

                slaves_.motor[0].data.target_vel = 5.0*65535.0*sin(f32angle*(2.0*PI));

                for(int i=0; i<NUM_OF_MOTOR; i++)
                {
                    slaves_.motor[i].SetTargtVelocity(domain1_pd_, static_cast<int32_t>(slaves_.motor[0].data.target_vel));
                }

            }
            break;

            default:
                break;
        }
        #endif
    }
    #endif

    // motor
    #ifdef MOTOR_Pos_0_
    {
        slaves_.motor_0.DataRead(domain1_pd_);

        vLogdata.push_back(std::to_string(slaves_.motor_0.data.actual_tor) + ' ');
        vLogdata.push_back(std::to_string(slaves_.motor_0.data.target_pos) + ' ');
        vLogdata.push_back(std::to_string(slaves_.motor_0.data.actual_vel) + ' ');
        vLogdata.push_back(std::to_string(slaves_.motor_0.data.target_vel) + ' ');
        vLogdata.push_back(std::to_string(slaves_.motor_0.data.actual_cur) + ' ');
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
        if(++i > 200)
        {
            i = 0;
            static uint16_t counter = 0;
            counter <<= 1;
            if(!counter) counter = 1;

            slaves_.SycKin_0.DataRead(domain1_pd_);
            EC_WRITE_U16(domain1_pd_ + slaves_.SycKin_0.offset.u16Dos, counter);
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


    EMaster.u32GlobalCounter++;
    log_data[log_write_counter % LOG_BUFF].counter = EMaster.u32GlobalCounter;

    for(int i=0; i<6; i++)
    {
        log_data[log_write_counter % LOG_BUFF].motor[i].target_pos = slaves_.motor[i].data.target_pos;
        log_data[log_write_counter % LOG_BUFF].motor[i].actual_pos = slaves_.motor[i].data.actual_pos;
        log_data[log_write_counter % LOG_BUFF].motor[i].target_vel = slaves_.motor[i].data.target_vel;
        log_data[log_write_counter % LOG_BUFF].motor[i].actual_vel = slaves_.motor[i].data.actual_vel;
        log_data[log_write_counter % LOG_BUFF].motor[i].actual_cur = slaves_.motor[i].data.actual_cur;
        log_data[log_write_counter % LOG_BUFF].motor[i].mode_display = slaves_.motor[i].data.mode_display;
        log_data[log_write_counter % LOG_BUFF].motor[i].status_word = slaves_.motor[i].data.status_word;
        log_data[log_write_counter % LOG_BUFF].motor[i].error_code = slaves_.motor[i].data.error_code;
    }

    log_data[log_write_counter % LOG_BUFF].imu.gx = slaves_.imu_0.data.gx;
    log_data[log_write_counter % LOG_BUFF].imu.gy = slaves_.imu_0.data.gy;
    log_data[log_write_counter % LOG_BUFF].imu.gz = slaves_.imu_0.data.gz;
    log_data[log_write_counter % LOG_BUFF].imu.ax = slaves_.imu_0.data.ax;
    log_data[log_write_counter % LOG_BUFF].imu.ay = slaves_.imu_0.data.ay;
    log_data[log_write_counter % LOG_BUFF].imu.az = slaves_.imu_0.data.az;
    log_data[log_write_counter % LOG_BUFF].imu.counter = slaves_.imu_0.data.counter;
    ++log_write_counter;
//    vLogdata.push_back("\n");
}
