#include "ecat_syckin_io.h"
#include "rt_log.h"

EcatSycKinIO::EcatSycKinIO()
{
    std::cout << "????????????????????????????????" << std::endl;
}

EcatSycKinIO::EcatSycKinIO(uint16_t alias, uint16_t position)
{


}

EcatSycKinIO::~EcatSycKinIO()
{

}

int EcatSycKinIO::Init   (ec_master_t    *master_,
                    uint16_t        alias_, /**< Slave alias. */
                    uint16_t        position_)
{
    ec_slave_config_t *sc;
    sc = ecrt_master_slave_config(master_, alias_, position_, SYCKIN_IO);
    if(NULL == sc)
    {
        std::cout << "Failed to get slave configuration!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        return -1;
    }
    if (0 != ecrt_slave_config_pdos(sc, EC_END, syncs))
    {
        std::cout << "Failed to configure PDOs!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        return -1;
    }

    EcatSlave::domain_regs.pop_back(); //删除a向量的最后一个元素
    EcatSlave::domain_regs.push_back({alias_, position_, SYCKIN_IO, 0x6000, 0x01, &offset.u16Dis, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, SYCKIN_IO, 0x6000, 0x02, &offset.u16DoStatus, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, SYCKIN_IO, 0x7000, 0x01, &offset.u16Dos, NULL});
    EcatSlave::domain_regs.push_back({alias_, position_, SYCKIN_IO, 0x7000, 0x02, &offset.u16DoEns, NULL});
    EcatSlave::domain_regs.push_back({});

    return 0;
}

void EcatSycKinIO::DataRead(uint8_t *domain1_pd_)
{
    // read process data
    data.u16Dis = EC_READ_U16(domain1_pd_ + offset.u16Dis);
    data.u16DoStatus = EC_READ_U16(domain1_pd_ + offset.u16DoStatus);
}
void EcatSycKinIO::DataWrite(uint8_t *domain1_pd_)
{
//    EC_WRITE_U16(domain1_pd_ + offset.u16Dos, 0xaa55);
}
void EcatSycKinIO::DataPlay(void)
{
    RT_PRINT("u16Dis: " + std::to_string(data.u16Dis));
}
