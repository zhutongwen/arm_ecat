

#include "ecat_wmio.h"

int EcatWmio::Init   (ec_master_t    *master_,
                    uint16_t        alias_, /**< Slave alias. */
                    uint16_t        position_)
{
    ec_slave_config_t *sc_imu;
    sc_imu = ecrt_master_slave_config(master_, alias_, position_, WMIO);
    if(!sc_imu)
    {
        fprintf(stderr, "Failed to get slave configuration.\n");
        return -1;
    }
    if (ecrt_slave_config_pdos(sc_imu, EC_END, wmlan9252_io_syncs))
    {
        fprintf(stderr, "Failed to configure PDOs.\n");
        return -1;
    }

    EcatSlave::domain_regs.pop_back(); //删除a向量的最后一个元素
    EcatSlave::domain_regs.push_back({alias_, position_, WMIO, 0x6000, 0x01, &offset.keys, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, WMIO, 0x6020, 0x01, &offset.analog_data, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, WMIO, 0x7000, 0x01, &offset.leds, NULL});
    EcatSlave::domain_regs.push_back({});

    return 0;
}

void EcatWmio::DataRead(uint8_t *domain1_pd_)
{
    data.key0_1 = EC_READ_U8(domain1_pd_ + offset.keys);
    data.analog_data = EC_READ_U16(domain1_pd_ + offset.analog_data);

}

void EcatWmio::DataPlay(void)
{
    std::cout << "keys: " << (uint16_t)data.key0_1 << std::endl;
    std::cout << "analog_data: " << data.analog_data << std::endl;
}
