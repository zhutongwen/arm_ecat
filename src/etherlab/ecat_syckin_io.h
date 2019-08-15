#ifndef ETHERCAT_SycKinIO_H
#define ETHERCAT_SycKinIO_H


#include "ecat_slave.h"


class EcatSycKinIO : public EcatSlave
{
public:

    typedef struct
    {
        unsigned int u16Dis;
        unsigned int u16DoStatus;
        unsigned int u16DOOpen;
        unsigned int u16DoEns;
        unsigned int u16Dos;
    }offset_t;

    typedef struct
    {
        uint16_t u16Dis;
        uint16_t u16DoStatus;
        uint16_t u16DOOpen;
        uint16_t u16DoEns;
        uint16_t u16Dos;
    }data_t;

    offset_t offset;
    data_t data;
    ec_slave_config_t   *sc;


    //TxPdo
    ec_pdo_entry_info_t txpdo_entries[7] = {
        {0x6000, 0x01, 16}, /* u16Dis */
        {0x6000, 0x02, 16}, /* u16DoStatus */
        {0x6000, 0x03, 16},
    };
    ec_pdo_info_t txpdos[1] = {
        {0x1a00, 3, txpdo_entries + 0}, /* TxPdo Channel 1 */
    };
    //RxPdo
    ec_pdo_entry_info_t rxpdo_entries[2] = {
        {0x7000, 0x01, 16}, /* u16Dos */
        {0x7000, 0x02, 16}, /* u16DoEns */
    };
    ec_pdo_info_t rxpdos[1] = {
        {0x1601, 2, rxpdo_entries + 0}, /* RxPdo Channel 1 */
    };
    ec_sync_info_t syncs[3] = {
        {2, EC_DIR_OUTPUT, 1, rxpdos, EC_WD_ENABLE},
        {3, EC_DIR_INPUT,  1, txpdos, EC_WD_ENABLE},
        {0xff}
    };

    int Init(   ec_master_t    *master_,
            uint16_t        alias_, /**< Slave alias. */
            uint16_t        position_);


    void DataRead(uint8_t *domain1_pd_);
    void DataWrite(uint8_t *domain1_pd_);
    void DataPlay(void);

    EcatSycKinIO();
    EcatSycKinIO(uint16_t alias, uint16_t position);
    ~EcatSycKinIO();

private:
    #define SYCKIN_IO             0x51409102, 0x20190614
//    volatile unsigned int test;
};



#endif
