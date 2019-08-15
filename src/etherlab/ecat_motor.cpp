#include "ecat_motor.h"
#include "rt_log.h"
EcatMotor::EcatMotor()
{


}

EcatMotor::~EcatMotor()
{

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

    //homing config
    {
        //home mode
//        if(ecrt_slave_config_sdo8(sc_motor, 0x6098, 0x00, static_cast<std::int8_t>(17)) < 0)
        if(ecrt_slave_config_sdo8(sc_motor,0x6098,0x00,static_cast<int8_t>(35)) < 0)
        {
            std::cout << "config homing mode error" << std::endl;
            return -1;
        }
        //home acc
        if(ecrt_slave_config_sdo32(sc_motor, 0x609a, 0x00, static_cast<std::uint32_t>(200000)) < 0)
        {
            std::cout << "config homing mode error" << std::endl;
            return -1;
        }
        //home high velocity
        if(ecrt_slave_config_sdo32(sc_motor, 0x6099, 0x01, static_cast<std::uint32_t>(200000)) < 0)
        {
            std::cout << "config homing acc error" << std::endl;
            return -1;
        }
        //home low velocity
        if(ecrt_slave_config_sdo32(sc_motor, 0x6099, 0x02, static_cast<std::uint32_t>(100000)) < 0)
        {
            std::cout << "config low velocity error" << std::endl;
            return -1;
        }
        //home offset
        if(ecrt_slave_config_sdo32(sc_motor, 0x607c, 0x00, static_cast<std::int32_t>(0)) < 0)
        {
            std::cout << "config homing offset error" << std::endl;
            return -1;
        }
    }

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
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x603f, 0x00, &offset.error_code, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, ELMO, 0x6061, 0x00, &offset.mode_display, NULL});

    EcatSlave::domain_regs.push_back({});

    ecrt_slave_config_dc(sc_motor, 0x0300, 1000000, 500000, 0, 0);

    return 0;
}

int EcatMotor::DataRead(uint8_t *domain1_pd_)
{
    data.actual_pos = EC_READ_S32(domain1_pd_ + offset.actual_pos);
    data.actual_vel = EC_READ_S32(domain1_pd_ + offset.actual_vel);
    data.actual_cur = EC_READ_S16(domain1_pd_ + offset.actual_cur);
    data.actual_tor = EC_READ_S16(domain1_pd_ + offset.actual_tor);
    data.status_word = EC_READ_U16(domain1_pd_ + offset.status_word);
    data.error_code = EC_READ_U16(domain1_pd_ + offset.error_code);
    data.mode_display = EC_READ_U8(domain1_pd_ + offset.mode_display);


//    vLogdata.push_back(std::to_string(data.actual_pos) + ' ');
//    vLogdata.push_back(std::to_string(data.actual_vel) + ' ');
//    vLogdata.push_back(std::to_string(data.actual_cur) + ' ');
//    vLogdata.push_back(std::to_string(data.actual_tor) + ' ');
//    vLogdata.push_back(std::to_string(data.status_word) + ' ');
//    vLogdata.push_back(std::to_string(data.error_code) + ' ');
//    vLogdata.push_back(std::to_string(data.mode_display) + ' ');
}

int EcatMotor::Display(void)
{
    RT_PRINT("actual_pos: " + std::to_string(data.actual_pos));
//    RT_PRINT("actual_vel: " + std::to_string(data.actual_vel));
//    RT_PRINT("actual_cur: " + std::to_string(data.actual_cur));
//    RT_PRINT("actual_tor: " + std::to_string(data.actual_tor));
    RT_PRINT("status_word: " + std::to_string(data.status_word));
    RT_PRINT("error_code: " + std::to_string(data.error_code));
    RT_PRINT("mode_display: " + std::to_string(data.mode_display));

    return 0;
}

int EcatMotor::Enable(uint8_t *domain1_pd_, void *para)
{
    if(motor_state == MortrState::ENABLED) return 0;
    int motor_status = data.status_word & 0x006f;

    if(motor_status != 0x0027)
    {
        EC_WRITE_S32(domain1_pd_ + offset.target_pos, static_cast<std::int32_t>( EC_READ_S32(domain1_pd_ + offset.actual_pos)));
        EC_WRITE_S32(domain1_pd_ + offset.target_vel, static_cast<std::int32_t>(0));
        EC_WRITE_S16(domain1_pd_ + offset.target_tor, static_cast<std::int16_t>(0));
    }

    if(data.mode_display != 0x0a)
    {
        EC_WRITE_U8(domain1_pd_ + offset.mode, static_cast<uint8_t>(0x0a));//homing  mode
    }
    else
    {
        //motor enable
        if(motor_status == 0x0040 || motor_status == 0x0060)// switch on disable
        {
            EC_WRITE_U16(domain1_pd_ + offset.control_word, static_cast<std::uint16_t>(0x0006)); //shut down
        }
        else if (motor_status == 0x0021) //read to switch on
        {
            EC_WRITE_U16(domain1_pd_ + offset.control_word, static_cast<std::uint16_t>(0x0007)); //switch on
        }
        else if (motor_status == 0x0023) //switch on
        {
            EC_WRITE_U16(domain1_pd_ + offset.control_word, static_cast<std::uint16_t>(0x000f)); //Enable Operation
        }
        else if(motor_status == 0x0027)//operation enable
        {
            //successfull, but still need to wait 10 more cycles to make it stable
            if(++i32enable_counter == 50)
            {
                i32enable_counter = 0;
                motor_state = MortrState::ENABLED;
                RT_PRINT("motor " + std::to_string((int)para) + " has been enabled");
            }
        }
        else //falt
        {
            EC_WRITE_U16(domain1_pd_ + offset.control_word, static_cast<std::uint16_t>(0x0080)); //falt restet
            if(++i32enable_error_counter > 1000)
            {
                i32enable_error_counter = 0;
                motor_state = MortrState::INIT;
                return -1;
            }
        }
    }
    return -1;
}

int EcatMotor::Homing(uint8_t *domain1_pd_, void *para)
{
    if(motor_state == MortrState::HOMED) return 0;

    //motor not enabled
    if(motor_state != MortrState::ENABLED)
    {
        RT_PRINT("homing error, please enable motor first!");
        return -1;
    }
    else//motor has enabled
    {
        //set homeing mode
        if(data.mode_display != 0x06)
        {
            EC_WRITE_S8(domain1_pd_ + offset.mode, static_cast<int8_t>(0x06));//homing  mode
            //RT_PRINT("ccccccccccccccccccccc");
        }
        else
        {
            if((data.status_word & 0x3400) == 0x0400)//homing procedure is interrupted or not started
            {                
                u32_home_counter++;
                if(u32_home_counter < 10) //std::cout << "to start homing procedure" << std::endl;
                {
                    EC_WRITE_U16(domain1_pd_ + offset.control_word, static_cast<uint16_t>(0x1f));
                }
                else if(u32_home_counter < 20)
                {
                    EC_WRITE_U16(domain1_pd_ + offset.control_word, static_cast<uint16_t>(0x0f));
                }
                else
                {
                    u32_home_counter = 0;
                }
                return MotorError::NOT_START;
            }
            else if((data.status_word & 0x3400) == 0x0000) //homing ing
            {
                u32_home_counter = 0;
                EC_WRITE_U16(domain1_pd_ + offset.control_word, static_cast<uint16_t>(0x1f));
                return MotorError::EXECUTING;
            }

            else if ((data.status_word & 0x3400) == 0x2000 || (data.status_word & 0x3400) == 0x2400)
            {
                //homing error occurred, velocity is not 0 , or homing error occurred, velocity is 0, should halt
                EC_WRITE_U16(domain1_pd_ + offset.control_word, static_cast<std::uint16_t>(0x0100));

                u32_home_counter = 0;
                return MotorError::HOME_ERROR;
            }

            else if((data.status_word & 0x3400) == 0x1400)
            {
                u32_home_counter = 0;
                motor_state = MortrState::HOMED;
                RT_PRINT("motor " + std::to_string((int)para) + " homing completed");
                return MotorError::SUCCESS;
            }
            else
            {
                u32_home_counter = 0;
                return MotorError::EXECUTING;
            }
        }
    }
    return 0;
}


int EcatMotor::SetMode(uint8_t *domain1_pd_, mode_t mode_)
{
    switch (mode_)
    {
        case MODE_CSP:
            if(motor_state == MortrState::RUN_CSP_MODE)  return MortrState::MODE_SETED;
        break;
        case MODE_CSV:
            if(motor_state == MortrState::RUN_CSV_MODE)  return MortrState::MODE_SETED;
        break;
        case MODE_CST:
            if(motor_state == MortrState::RUN_CST_MODE)  return MortrState::MODE_SETED;
        break;

        default:
            return MortrState::MODE_SET_ERROR;
        break;
    }

    //motor not enabled
//    if(motor_state != MortrState::HOMED)
//    {
//        RT_PRINT("homing error, please enable motor first!");
//        return MortrState::MODE_SET_ERROR;
//    }
//    else
    {
        switch (mode_)
        {
            case MODE_CSP:
                //targetposition should be equal to actualposition
                data.actual_pos = EC_READ_S32(domain1_pd_ + offset.actual_pos);
                EC_WRITE_S32(domain1_pd_ + offset.target_pos, static_cast<std::int32_t>(data.actual_pos));
                motor_state = MortrState::RUN_CSP_MODE;
                break;
            case MODE_CSV:
                //velocity loop to set velocity of 0
                EC_WRITE_S32(domain1_pd_ + offset.target_vel, static_cast<std::int32_t>(0));
                motor_state = MortrState::RUN_CSV_MODE;
                break;
            case MODE_CST:
                EC_WRITE_S16(domain1_pd_ + offset.target_tor, static_cast<std::int16_t>(0));
                motor_state = MortrState::RUN_CST_MODE;
                break;
            default:
                return MortrState::MODE_SET_ERROR;
                break;
        }
    }

    if(data.mode_display == mode_)
    {
        RT_PRINT("mode set successful");
        return MortrState::MODE_SETED;
    }
    else
    {
        EC_WRITE_S8(domain1_pd_ + offset.mode, static_cast<int8_t>(mode_));
        return MortrState::MODE_SETING;
    }
}

int EcatMotor::SetTargtVelocity(uint8_t *domain1_pd_, int32_t velocity_)
{
    if(motor_state != MortrState::RUN_CSV_MODE)
    {
        return -1;
    }

    EC_WRITE_S32(domain1_pd_ + offset.target_vel, velocity_);
}

int EcatMotor::SetTargtTorque(uint8_t *domain1_pd_, int16_t torque_)
{
    if(motor_state != MortrState::RUN_CST_MODE)
    {
        return -1;
    }

    EC_WRITE_S16(domain1_pd_ + offset.target_tor, torque_);
}
