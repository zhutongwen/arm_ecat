#include "ecat_imu.h"
#include "rt_log.h"

EcatImu::EcatImu()
{

}

EcatImu::EcatImu(uint16_t alias, uint16_t position)
{


}

EcatImu::~EcatImu()
{

}

int EcatImu::Init   (ec_master_t    *master_,
                    uint16_t        alias_, /**< Slave alias. */
                    uint16_t        position_)
{
    sc_imu = ecrt_master_slave_config(master_, alias_, position_, IMU);
    if(NULL == sc_imu)
    {
        std::cout << "Failed to get slave configuration!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        return -1;
    }
    if (0 != ecrt_slave_config_pdos(sc_imu, EC_END, syncs))
    {
        std::cout << "Failed to configure PDOs!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        return -1;
    }

    EcatSlave::domain_regs.pop_back(); //删除a向量的最后一个元素
    EcatSlave::domain_regs.push_back({alias_, position_, IMU, 0x6000, 0x01, &offset.gx, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, IMU, 0x6000, 0x02, &offset.gy, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, IMU, 0x6000, 0x03, &offset.gz, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, IMU, 0x6000, 0x04, &offset.ax, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, IMU, 0x6000, 0x05, &offset.ay, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, IMU, 0x6000, 0x06, &offset.az, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, IMU, 0x6000, 0x07, &offset.counter, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, IMU, 0x7011, 0x01, &offset.led, NULL});
    EcatSlave::domain_regs.push_back({});

    return 0;
}

#define PI 3.14159265
void EcatImu::DataRead(uint8_t *domain1_pd_)
{
    // read process data
    data.gx = EC_READ_FLOAT(domain1_pd_ + offset.gx)/360.0 * 2 * PI;
    data.gy = EC_READ_FLOAT(domain1_pd_ + offset.gy)/360.0 * 2 * PI;
    data.gz = EC_READ_FLOAT(domain1_pd_ + offset.gz)/360.0 * 2 * PI;
    data.ax = 9.8*EC_READ_FLOAT(domain1_pd_ + offset.ax);
    data.ay = 9.8*EC_READ_FLOAT(domain1_pd_ + offset.ay);
    data.az = 9.8*EC_READ_FLOAT(domain1_pd_ + offset.az);
    data.counter = EC_READ_U32(domain1_pd_ + offset.counter);



//    RT_log_file << data.gx << ' ';
//    RT_log_file << data.gy << ' ';
//    RT_log_file << data.gz << ' ';
//    RT_log_file << data.ax << ' ';
//    RT_log_file << data.ay << ' ';
//    RT_log_file << data.az << ' ';
//    RT_log_file << data.counter << ' ';
//    RT_log_file << std::endl;


}
void EcatImu::DataWrite(uint8_t *domain1_pd_)
{
    EC_WRITE_U16(domain1_pd_ + offset.led, 0xaa55);
}
void EcatImu::DataPlay(void)
{
//    std::cout << "gx:" << data.gx << std::endl;
//    std::cout << "gy:" << data.gy << std::endl;
//    std::cout << "gz:" << data.gz << std::endl;
//    std::cout << "ax:" << data.ax << std::endl;
//    std::cout << "ay:" << data.ay << std::endl;
//    std::cout << "az:" << data.az << std::endl;
//    std::cout << "counter:" << std::dec << data.counter << std::endl << std::endl;

    RT_PRINT("ax: " + std::to_string(data.ax));
    RT_PRINT("ay: " + std::to_string(data.ay));
    RT_PRINT("az: " + std::to_string(data.az));
    RT_PRINT("counter: " + std::to_string(data.counter));
}
