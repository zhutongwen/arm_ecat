#include "ecat_motor.h"
#include "rt_log.h"
EcatMotor::EcatMotor()
{


}

EcatMotor::~EcatMotor()
{

}

int EcatMotor::Display(void)
{
//    std::cout << "actual_pos:" << std::dec << data.actual_pos << std::endl;
//    std::cout << "actual_vel:" << std::dec << data.actual_vel << std::endl;
//    std::cout << "actual_cur:" << std::dec << data.actual_cur << std::endl;
//    std::cout << "actual_tor:" << std::dec << data.actual_tor << std::endl;
//    std::cout << "status_word: 0x" << std::hex << data.status_word << std::endl;
//    std::cout << "mode_display: 0x" << std::hex << (uint16_t)data.mode_display << std::endl;


    RT_PRINT("actual_pos: " + std::to_string(data.actual_pos));
    RT_PRINT("actual_vel: " + std::to_string(data.actual_vel));
    RT_PRINT("actual_cur: " + std::to_string(data.actual_cur));
    RT_PRINT("actual_tor: " + std::to_string(data.actual_tor));
    RT_PRINT("status_word: " + std::to_string(data.status_word));
    RT_PRINT("mode_display: " + std::to_string(data.mode_display));

    return 0;
}

int EcatMotor::Init(ec_master_t *master_,
                    uint16_t alias_, /**< Slave alias. */
                    uint16_t position_ /**< Slave position. */
                    )
{
    sc_motor = ecrt_master_slave_config(master_, alias_, position_, ELMO);
    if(!sc_motor)
    {
        fprintf(stderr, "Failed to get slave configuration.\n");
        return -1;
    }
    if (ecrt_slave_config_pdos(sc_motor, EC_END, syncs))
    {
        fprintf(stderr, "Failed to configure PDOs.\n");
        return -1;
    }

//

    //homing config
    {
        //home mode
        if(ecrt_slave_config_sdo8(sc_motor,0x6098,0x00,static_cast<int8_t>(35)) < 0)
        {
            std::cout << "config homing mode error" << std::endl;
            return -1;
        }
        //home acc
        if(ecrt_slave_config_sdo32(sc_motor,0x6099,0x01,static_cast<int32_t>(200000)) < 0)
        {
            std::cout << "config homing mode error" << std::endl;
            return -1;
        }
        //home high velocity
        if(ecrt_slave_config_sdo32(sc_motor,0x6099,0x02,static_cast<int32_t>(10000)) < 0)
        {
            std::cout << "config homing acc error" << std::endl;
            return -1;
        }
        //home low velocity
        if(ecrt_slave_config_sdo32(sc_motor,0x609a,0x00,static_cast<int32_t>(100000)) < 0)
        {
            std::cout << "config low velocity error" << std::endl;
            return -1;
        }
        //home offset
        if(ecrt_slave_config_sdo32(sc_motor,0x607c,0x00,static_cast<int32_t>(0)) < 0)
        {
            std::cout << "config homing offset error" << std::endl;
            return -1;
        }
    }
    #if 0
    //position mode config
    {
        //profile velocity
        if(ecrt_slave_config_sdo32(sc_motor_01,0x6081,0x00,10000) < 0)
        {
            std::cout << "config homing mode error" << std::endl;
            return -1;
        }
        //end velocity
        if(ecrt_slave_config_sdo32(sc_motor_01,0x6082,0x00,10000) < 0)
        {
            std::cout << "config homing mode error" << std::endl;
            return -1;
        }
        //profile acceleration
        if(ecrt_slave_config_sdo32(sc_motor_01,0x6083,0x00,10000) < 0)
        {
            std::cout << "config homing mode error" << std::endl;
            return -1;
        }
        //profile deceleration
        if(ecrt_slave_config_sdo32(sc_motor_01,0x6084,0x00,10000) < 0)
        {
            std::cout << "config homing mode error" << std::endl;
            return -1;
        }
    }
    #endif


    EcatSlave::domain_regs.pop_back(); //删除a向量的最后一个元素
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x607a, 0x00, &offset.target_pos, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x60ff, 0x00, &offset.target_vel, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x6071, 0x00, &offset.target_tor, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x6072, 0x00, &offset.max_tor, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x6040, 0x00, &offset.control_word, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x6060, 0x00, &offset.mode, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x60b1, 0x00, &offset.vel_offset, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x60b2, 0x00, &offset.tor_offset, NULL});

    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x6064, 0x00, &offset.actual_pos, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x606c, 0x00, &offset.actual_vel, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x6078, 0x00, &offset.actual_cur, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x6077, 0x00, &offset.actual_tor, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x6041, 0x00, &offset.status_word, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x6061, 0x00, &offset.mode_display, NULL});
    EcatSlave::domain_regs.push_back({});

//    ecrt_slave_config_dc(sc_motor, 0x0300, 1000000, 440000, 0, 0);

    return 0;
}

int EcatMotor::Enable(uint8_t *domain1_pd_)
{
    // read process data
    data.actual_pos = EC_READ_S32(domain1_pd_ + offset.actual_pos);
    data.actual_vel = EC_READ_S32(domain1_pd_ + offset.actual_vel);
    data.actual_cur = EC_READ_S16(domain1_pd_ + offset.actual_cur);
    data.actual_tor = EC_READ_S16(domain1_pd_ + offset.actual_tor);
    data.status_word = EC_READ_U16(domain1_pd_ + offset.status_word);
    data.mode_display = EC_READ_U8(domain1_pd_ + offset.mode_display);

    int motor_status = data.status_word & 0x006f;

    if(data.mode_display != 0x06)
    {
//        std::cout << "--------mode_display--------" << std::endl;
        EC_WRITE_S8(domain1_pd_ + offset.mode, static_cast<int8_t>(6));//homing  mode
    }
    else
    {
        //motor enable
        if(motor_status == 0x0040 || motor_status == 0x0600)// switch on disable
        {
            EC_WRITE_U16(domain1_pd_ + offset.control_word, 0x0006); //shut down
//            std::cout << "--------shut down--------" << std::endl;
        }
        else if (motor_status == 0x0021) //read to switch on
        {
            EC_WRITE_U16(domain1_pd_ + offset.control_word, 0x0007); //switch on
//            std::cout << "-------switch on---------" << std::endl;
        }
        else if (motor_status == 0x0023) //switch on
        {
            EC_WRITE_U16(domain1_pd_ + offset.control_word, 0x000f); //Enable Operation
//            std::cout << "-------Enable Operation---------" << std::endl;
        }
        else if(motor_status == 0x0027)//operation enable
        {
            //successfull, but still need to wait for 10 more cycles to make it stable
            static int enable_period_ = 0;
            if(++enable_period_ > 20)
            {
                enable_period_ = 0;
                motor_state = STATE_ENABLED;
                std::cout << "motor has been enabled" << std::endl;
                std::cout << "--------operation enable--------" << std::endl;
            }

        }
        else //falt
        {
//            std::cout << "---------falt restet-------" << std::endl;

            EC_WRITE_U16(domain1_pd_ + offset.control_word, 0x0080); //falt restet
            static int counter = 0;
            if(++counter > 500)
            {
                counter = 0;
                motor_state = STATE_FALT;
            }
        }
    }
    return 0;
}

int EcatMotor::Homing(uint8_t *domain1_pd_)
{
    data.status_word = EC_READ_U16(domain1_pd_ + offset.status_word);
    data.mode_display = EC_READ_U8(domain1_pd_ + offset.mode_display);

    int motor_status = data.status_word & 0x006f;

    //motor not enabled
    if(motor_status != 0x0027)
    {        
        std::cout << data.status_word << std::endl;
        std::cout << "homing error, please enable motor first!" << std::endl;
        return -1;
    }

    else//motor has enabled
    {
        //set homeing mode
        if(data.mode_display != 0x06)
        {
//            std::cout << "--------mode_display--------" << std::endl;
//            motor_status = MOTOR_INIT;
            EC_WRITE_S8(domain1_pd_ + offset.mode, static_cast<int8_t>(6));//homing  mode
        }

        else
        {
            if((data.status_word & 0x3400) == 0x0400)//homing procedure is interrupted or not started
            {
//                std::cout << "to start homing procedure" << std::endl;
                EC_WRITE_U16(domain1_pd_ + offset.control_word, static_cast<uint16_t>(0x1f));
//                motor_state =MOTOR_HOMEING;

            }
            else if((data.status_word & 0x3400) == 0x0000) //homing ing
            {
//                std::cout << "homing procedure is running................."<< std::endl;
//                motor_state =MOTOR_HOMEING;
            }
            else if((data.status_word & 0x3400) == 0x1400) //homing ing
            {
                std::cout << "homing successfully"<< std::endl;
                motor_state =STATE_HOMED;
            }
        }
    }
}

int EcatMotor::DataRead(uint8_t *domain1_pd_)
{
    data.actual_pos = EC_READ_S32(domain1_pd_ + offset.actual_pos);
    data.actual_vel = EC_READ_S32(domain1_pd_ + offset.actual_vel);
    data.actual_cur = EC_READ_S16(domain1_pd_ + offset.actual_cur);
    data.actual_tor = EC_READ_S16(domain1_pd_ + offset.actual_tor);
    data.status_word = EC_READ_U16(domain1_pd_ + offset.status_word);
    data.mode_display = EC_READ_U8(domain1_pd_ + offset.mode_display);
}

int EcatMotor::SetMode(uint8_t *domain1_pd_, mode_t mode_)
{
    int motor_status = data.status_word & 0x006f;
    if(motor_status != 0x0027)
    {
        std::cout << "homing error, please enable motor first!" << std::endl;
        return -1;
    }
    else
    {
        switch (mode_)
        {
            case MODE_CSP:
                //targetposition should be equal to actualposition
                data.actual_pos = EC_READ_S32(domain1_pd_ + offset.actual_pos);
                EC_WRITE_S32(domain1_pd_ + offset.target_pos, static_cast<int32_t>(data.actual_pos));
                motor_state = STATE_CSP;
                break;
            case MODE_CSV:
                //velocity loop to set velocity of 0
                EC_WRITE_S32(domain1_pd_ + offset.target_vel, static_cast<int32_t>(0));
                motor_state = STATE_CSV;
                break;
            case MODE_CST:
                EC_WRITE_S16(domain1_pd_ + offset.target_tor, static_cast<int32_t>(0));
                motor_state = STATE_CST;
                break;
            default:
                return -1;
                break;
        }
    }

    if(data.mode_display == mode_)
    {
        std::cout << "mode set successful";
        return 0;
    }
    else
    {
        EC_WRITE_S8(domain1_pd_ + offset.mode, static_cast<int8_t>(mode_));
    }
    return 0;
}

int EcatMotor::SetTargtVelocity(uint8_t *domain1_pd_, int32_t velocity_)
{
    if(motor_state != STATE_CSV)
    {
        return -1;
    }

    EC_WRITE_S32(domain1_pd_ + offset.target_vel, velocity_);
}
