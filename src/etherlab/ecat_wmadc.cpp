

#include "ecat_wmadc.h"

int EcatWmadc::Init   (ec_master_t    *master_,
                    uint16_t        alias_, /**< Slave alias. */
                    uint16_t        position_)
{
    sc = ecrt_master_slave_config(master_, alias_, position_, WMADC);
    if(!sc)
    {
        fprintf(stderr, "Failed to get slave configuration.\n");
        return -1;
    }
    if (ecrt_slave_config_pdos(sc, EC_END, wmlan9252_adc_syncs))
    {
        fprintf(stderr, "Failed to configure PDOs.\n");
        return -1;
    }

//    ecrt_slave_config_dc(sc_imu, 0x0300, 1000000, 440000, 0, 0);

    EcatSlave::domain_regs.pop_back(); //删除a向量的最后一个元素

    EcatSlave::domain_regs.push_back({alias_, position_, WMADC, 0x6020, 0x01, &offset.analog_data[0], NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, WMADC, 0x6020, 0x02, &offset.analog_data[1], NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, WMADC, 0x6020, 0x03, &offset.analog_data[2], NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, WMADC, 0x6020, 0x04, &offset.analog_data[3], NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, WMADC, 0x6000, 0x01, &offset.keys, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, WMADC, 0x6000, 0x02, &offset.null[0], NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, WMADC, 0x7000, 0x01, &offset.leds, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, WMADC, 0x7000, 0x02, &offset.null[1], NULL});
    EcatSlave::domain_regs.push_back({});

    return 0;
}

void EcatWmadc::DataRead(uint8_t *domain1_pd_)
{
    data.key0_1 = EC_READ_U8(domain1_pd_ + offset.keys);
    data.analog_data[0] = EC_READ_S16(domain1_pd_ + offset.analog_data[0]);
    data.analog_data[1] = EC_READ_S16(domain1_pd_ + offset.analog_data[1]);
    data.analog_data[2] = EC_READ_S16(domain1_pd_ + offset.analog_data[2]);
    data.analog_data[3] = EC_READ_S16(domain1_pd_ + offset.analog_data[3]);
}

void EcatWmadc::DataPlay(void)
{
    std::cout << "DOs: " << (uint16_t)data.led0_7 << std::endl;
    std::cout << "DIs: " << (uint16_t)data.key0_1 << std::endl;
    std::cout << "AI[0]: " << std::dec << data.analog_data[0] << std::endl;
    std::cout << "AI[1]: " << std::dec << data.analog_data[1] << std::endl;
    std::cout << "AI[2]: " << std::dec << data.analog_data[2] << std::endl;
    std::cout << "AI[3]: " << std::dec << data.analog_data[3] << std::endl;
}
