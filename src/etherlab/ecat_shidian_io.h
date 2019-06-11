#ifndef ETHERCAT_ShiDianIO_H
#define ETHERCAT_ShiDianIO_H


#include "ecat_slave.h"


class EcatShiDianIO : public EcatSlave
{
public:

    typedef struct
    {
        unsigned int u16Dis;
        unsigned int u16Dos;
    }offset_t;

    typedef struct
    {
        uint16_t u16Dis;
        uint16_t u16Dos;
    }data_t;

    offset_t offset;
    data_t data;
    ec_slave_config_t   *sc;


    //TxPdo
    ec_pdo_entry_info_t txpdo_entries[1] = {
        {0x6000, 0x01, 16}, /* u16Dis */
    };
    ec_pdo_info_t txpdos[1] = {
        {0x1a00, 1, txpdo_entries + 0}, /* TxPdo Channel 1 */
    };
    //RxPdo
    ec_pdo_entry_info_t rxpdo_entries[1] = {
        {0x7000, 0x01, 16}, /* u16Dos */
    };
    ec_pdo_info_t rxpdos[1] = {
        {0x1600, 1, rxpdo_entries + 0}, /* RxPdo Channel 1 */
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

    EcatShiDianIO();
    EcatShiDianIO(uint16_t alias, uint16_t position);
    ~EcatShiDianIO();

private:

    #define SHIDIAN_IO             0x09090980, 0x00000004
//    volatile unsigned int test;
};



#endif
