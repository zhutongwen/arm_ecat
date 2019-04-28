#include "ecat_task.h"
#include <math.h>



void ControlTask(uint8_t *domain1_pd_, slaves_t &slaves_)
{
    /****************************************************************************/
    // motor
    #ifdef MOTOR_Pos_0
    {
//        std::cout << slaves_.motor_0.motor_state << std::endl;
        slaves_.motor_0.DataRead(domain1_pd_);

        if(slaves_.motor_0.motor_state == slaves_.motor_0.STATE_CSV)
        {
            static float f32angle = 0;
            int32_t s32velocity = 0;

            f32angle += 0.0002;
            if(f32angle >= 1) f32angle = 0;

            s32velocity = 5.0*65535.0*sin(f32angle*(2.0*PI));
            slaves_.motor_0.SetTargtVelocity(domain1_pd_, static_cast<int32_t>(s32velocity));
        }

        else if(slaves_.motor_0.motor_state == slaves_.motor_0.STATE_INIT)
        {
            slaves_.motor_0.Enable(domain1_pd_);
        }
        else if(slaves_.motor_0.motor_state == slaves_.motor_0.STATE_ENABLED)
        {
            slaves_.motor_0.Homing(domain1_pd_);
        }
        else if(slaves_.motor_0.motor_state == slaves_.motor_0.STATE_HOMED)
        {
            slaves_.motor_0.SetMode(domain1_pd_, slaves_.motor_0.MODE_CSV);//static_cast<mode_t>
        }
    }
    #endif
}
