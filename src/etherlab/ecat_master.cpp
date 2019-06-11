#include "ecat_task.h"
#include "ecat_master.h"

void EcatMaster::rt_check_domain_state(void)
{
    ec_domain_state_t ds;

    ecrt_domain_state(domain1, &ds);

    if (ds.working_counter != domain1_state.working_counter)
    {
        RT_PRINT("Domain1: WC" + std::to_string(ds.working_counter));
    }

    if (ds.wc_state != domain1_state.wc_state)
    {
        RT_PRINT("Domain1: State " + std::to_string(ds.wc_state));
    }
    domain1_state = ds;
}

/****************************************************************************/
void EcatMaster::rt_check_master_state(void)
{
    ec_master_state_t ms;

    ecrt_master_state(master, &ms);

    if (ms.slaves_responding != master_state.slaves_responding)
    {
        RT_PRINT(std::to_string(ms.slaves_responding) + " slave(s).");
        master_state.slaves_responding = ms.slaves_responding;
    }

    if (ms.al_states != master_state.al_states)
    {
        RT_PRINT("ms AL states: " + std::to_string(ms.al_states));
        RT_PRINT("master_state AL states: " + std::to_string(master_state.al_states));
        master_state.al_states = ms.al_states;
    }

    if (ms.link_up != master_state.link_up)
    {
        RT_PRINT("AL states: " + std::to_string(ms.al_states));
        master_state.link_up = ms.link_up;
    }
}

void LoopTest(void)
{

}

EcatMaster::EcatMaster(void)
{

}

void EcatMaster::Init(void)
{
    RT_PRINT("Requesting master...");
    master = ecrt_request_master(0);
    if (!master)
    {
        RT_PRINT("ecrt_request_master error!!!");
    }

    RT_PRINT("ecrt_master_create_domain...");
    domain1 = ecrt_master_create_domain(master);
    if (!domain1)
    {
        RT_PRINT("ecrt_master_create_domain error!!!");
    }

    RT_PRINT("Creating slave configurations...");

    #ifdef SHIDIAN_Pos_0
        slaves.ShiDian_0.Init(master,SHIDIAN_Pos_0);
    #endif

    #ifdef SYCKIN_Pos_0
        slaves.SycKin_0.Init(master, SYCKIN_Pos_0);
    #endif

    #ifdef MOTOR_Pos_0
        slaves.motor_0.Init(master, MOTOR_Pos_0);
    #endif

    #ifdef IMU_Pos_0
        slaves.imu_0.Init(master, IMU_Pos_0);
    #endif

    #ifdef IMU_Pos_1
        slaves.imu_1.Init(master, IMU_Pos_1);
    #endif

    #ifdef IMU_Pos_2
        slaves.imu_2.Init(master, IMU_Pos_2);
    #endif

    #ifdef IMU_Pos_3
        slaves.imu_3.Init(master, IMU_Pos_3);
    #endif

    #ifdef IMU_Pos_4
        slaves.imu_4.Init(master, IMU_Pos_4);
    #endif

    #ifdef WMIO_Pos_0
        slaves.wmio_0.Init(master, WMIO_Pos_0);
    #endif

    #ifdef WMADC_Pos_0
        slaves.wmadc_0.Init(master, WMADC_Pos_0);
    #endif

     if (ecrt_domain_reg_pdo_entry_list(domain1, EcatSlave::domain_regs.data()))
    {
        RT_PRINT("PDO entry registration failed!!!");
    }

    #ifdef MOTOR_Pos_0
         ecrt_slave_config_dc(slaves.motor_0.sc_motor, 0x0300, 1000000, 440000, 0, 0);
    #endif

    #ifdef WMIO_Pos_0
        ecrt_slave_config_dc(slaves.wmio_0.sc_io, 0x0300, 1000000, 440000, 0, 0);
    #endif

    #ifdef WMADC_Pos_0
        ecrt_slave_config_dc(slaves.wmadc_0.sc, 0x0300, 1000000, 440000, 0, 0);
    #endif

    #ifdef SYCKIN_Pos_0
        ecrt_slave_config_dc(slaves.SycKin_0.sc, 0x0300, 500000, 220000, 0, 0);
    #endif
    #ifdef SHIDIAN_Pos_0
//        ecrt_slave_config_dc(slaves.ShiDian_0.sc, 0x0300, 500000, 220000, 0, 0);
    #endif


    RT_PRINT("Activating master...");
    if (ecrt_master_activate(master))
    {
        RT_PRINT("ecrt_master_activate failed!!!");
    }

    RT_PRINT("get domain data pointer...");
    if (!(domain1_pd = ecrt_domain_data(domain1)))
    {
        RT_PRINT("Failed to get domain data pointer" );
    }

    ec_master_info_t master_info;
    if(0 != ecrt_master(master, &master_info))
    {
        RT_PRINT("ecrt_master error!!!");
    }
    else
    {
        RT_PRINT("number of slave(s):" + std::to_string(master_info.slave_count));
    }



}

EcatMaster::~EcatMaster(void)
{

}
