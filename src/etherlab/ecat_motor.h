#ifndef ECAT_MOTOR_H_
#define ECAT_MOTOR_H_

#include  <iostream>
#include  "ecrt.h"
#include  "ecat_slave.h"


class EcatMotor: public EcatSlave
{
public:
    //mode: 0x01 position mode
    //      0x03 velocity mode
    //      0x04 torque mode

    typedef enum
    {
        MODE_PROFILE_POSITION = 1,
        MODE_PROFILE_VELOCITY = 3,
        MODE_PROFILE_TORQUE = 4,
        MODE_HOMING = 6,
        MODE_INTERPOLATED_POSITION = 7,
        MODE_CSP = 8,
        MODE_CSV = 9,
        MODE_CST = 10,
    }mode_t;

    typedef struct
    {
        unsigned int target_pos;
        unsigned int target_vel;
        unsigned int target_tor;
        unsigned int max_tor;
        unsigned int control_word;
        unsigned int mode;
        unsigned int vel_offset;
        unsigned int tor_offset;

        unsigned int actual_pos;
        unsigned int actual_vel;
        unsigned int actual_cur;
        unsigned int actual_tor;
        unsigned int status_word;
        unsigned int error_code;
        unsigned int mode_display;        
    } offset_t;

    typedef struct
    {
        int32_t  target_pos;
        int32_t  target_vel;
        int16_t  target_tor;
        int16_t  max_tor;
        uint16_t control_word;
        mode_t   mode;
        int32_t  vel_offset;
        int16_t  tor_offset;

        int32_t  actual_pos;
        int32_t  actual_vel;
        int16_t  actual_cur;
        int16_t  actual_tor;
        uint16_t status_word;
        uint16_t error_code;
        int8_t   mode_display;        
    }data_t;

    offset_t            offset;
    data_t              data;
    ec_slave_config_t   *sc_motor;

    // motor pdo
    /*******************************************************************************/
    //RxPdo
    ec_pdo_entry_info_t motor_rxpdo_entries[8] =
    {
        {0x607a, 0x00, 32}, //pos_target_value  s32
        {0x60ff, 0x00, 32}, //vel_target_value  s32
        {0x6071, 0x00, 16}, //tor_target_value  s16
        {0x6072, 0x00, 16}, //tor_max_value     s16
        {0x6040, 0x00, 16}, //control_word      u16
        {0x6060, 0x00, 8},  //module            u8
        {0x60b1, 0x00, 32}, //velocity offset   s32
        {0x60b2, 0x00, 16}, //torque_offset     s16
    };
    ec_pdo_info_t motor_rxpdos[3] =
    {
        {0x1605, 6, motor_rxpdo_entries + 0},
        {0x1617, 1, motor_rxpdo_entries + 6},
        {0x1618, 1, motor_rxpdo_entries + 7},
    };
    // TxPdo
    ec_pdo_entry_info_t motor_txpdo_entries[7] =
    {
        {0x6064, 0x00, 32}, //pos_actual_value  s32
        {0x606c, 0x00, 32}, //vel_actual_value  s32
        {0x6078, 0x00, 16}, //cur_actual_value  s16
        {0x6077, 0x00, 16}, //tor_actual_value  s16
        {0x6041, 0x00, 16}, //status_word       u16
        {0x603f, 0x00, 16}, //status_word       u16
        {0x6061, 0x00, 8},  //mode_display      u8
    };
    ec_pdo_info_t motor_txpdos[7] =
    {
        {0x1a0e, 1, motor_txpdo_entries + 0}, //pos_actual_value  s32
        {0x1a11, 1, motor_txpdo_entries + 1}, //vel_actual_value  s32
        {0x1a1f, 1, motor_txpdo_entries + 2}, //cur_actual_value  s16
        {0x1a13, 1, motor_txpdo_entries + 3}, //tor_actual_value  s16
        {0x1a0a, 1, motor_txpdo_entries + 4}, //status_word       u16
        {0x1a20, 1, motor_txpdo_entries + 5}, //status_word       u16
        {0x1a0b, 1, motor_txpdo_entries + 6}, //module_display    u8

    };
    ec_sync_info_t syncs[3] = {
        {2, EC_DIR_OUTPUT, 3, motor_rxpdos, EC_WD_ENABLE},
        {3, EC_DIR_INPUT,  7, motor_txpdos, EC_WD_ENABLE},
        {0xff}
    };

    EcatMotor();
    ~EcatMotor();

    int DataRead(uint8_t *domain1_pd_);
    int Display(void);
    int Init(ec_master_t *master_,
             uint16_t alias_, /**< Slave alias. */
             uint16_t position_/**< Slave position. */);
    int Enable(uint8_t *domain1_pd_, void *para);
    int Homing(uint8_t *domain1_pd_, void *para);
    int SetMode(uint8_t *domain1_pd_, mode_t mode_);
    int SetTargtVelocity(uint8_t *domain1_pd_, int32_t velocity_);
    int SetTargtTorque(uint8_t *domain1_pd_, int16_t torque_);

    uint8_t GetMode();
    int32_t GetActualPosition();
    int32_t GetActualVelocity();
    int16_t GetActualCurrent();

    int SetTargtPosition(int32_t position);
    int SetTargtTorque(int16_t torque);

    typedef enum
    {
        INIT = 0,

        ENABLEING = 1,
        ENABLED = 2,

        HOMING = 3,
        HOMED = 4,

        MODE_SETING = 5,
        MODE_SETED = 6,

        RUN_CSP_MODE = 8,
        RUN_CSV_MODE = 9,
        RUN_CST_MODE = 10,

        DISABLED = 11,


        ENABLE_FALT = -1,
        HOME_FALT = -2,
        MODE_SET_ERROR = -3
    }MortrState;

    MortrState motor_state{MortrState::INIT};

    enum MotorError
    {
        SUCCESS = 0,/**<无错误 */
        EXECUTING = 1,/**<命令执行过程中 */
        EXE_FAULT = 2,/**<命令执行错误，该错误可以复位自动清除 */
        MODE_CHANGE = 3,/**<正在切换控制模式 */
        NOT_START = 4,/**<命令未执行 */

        CMD_ERROR = -1,/**<命令执行出错，此时所有电机应该去使能以停止 */
        HOME_ERROR = -2,/**<找HOME命令执行出错，此时所有电机应该HALT */
        ENABLE_ERROR = -3,/**<电机在运转过程中掉使能 */
        MODE_ERROR = -4,/**<电机在运转过程中控制模式出错 */
    };

private:

    uint32_t i32enable_counter = 0;
    uint32_t i32enable_error_counter = 0;
    uint32_t u32_home_counter = 0;

    //ELMO
//    #define ELMO            0x0000009a, 0x00030924
    //DRIVER
    #define ELMO            0x0000089c, 0x20173052


//    enum
//    {

//    }home_mode{35};
};

#endif
