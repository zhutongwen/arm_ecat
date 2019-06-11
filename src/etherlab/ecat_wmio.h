#ifndef WM_IO_H
#define WM_IO_H

#include "ecat_slave.h"

class EcatWmio : public EcatSlave
{
public:

    typedef struct
    {
        unsigned int analog_data;
        unsigned int keys;
        unsigned int leds;
        unsigned int null;
    }offset_t;

    typedef struct
    {
        uint16_t analog_data;
        uint16_t key0_1;
        uint16_t led0_7;
    }data_t;


    offset_t    offset;
    data_t      data;
    ec_slave_config_t *sc_io;


    //TxPdo
    ec_pdo_entry_info_t wmlan9252_io_txpdo_entries[2] = {
        {0x6000, 0x01, 16}, /* key0_2 */
//        {0, 0, 8}, //reserve
        {0x6020, 0x01, 16}, /* analog_Input */
    };
    ec_pdo_info_t wmlan9252_io_txpdos[2] = {
        {0x1A02, 1, wmlan9252_io_txpdo_entries + 1}, /* TxPdo Channel 2 */
        {0x1A00, 1, wmlan9252_io_txpdo_entries + 0}, /* TxPdo Channel 1 */
    };
    //RxPdo
    ec_pdo_entry_info_t wmlan9252_io_rxpdo_entries[1] = {
        {0x7000, 0x01, 16}, /* led0_8 */
//        {0, 0, 8}, //reserve
    };
    ec_pdo_info_t wmlan9252_io_rxpdos[1] = {
        {0x1601, 1, wmlan9252_io_rxpdo_entries + 0}, /* RxPdo Channel 1 */
    };
    ec_sync_info_t wmlan9252_io_syncs[3] = {
        {2, EC_DIR_OUTPUT, 1, wmlan9252_io_rxpdos, EC_WD_ENABLE},
        {3, EC_DIR_INPUT,  2, wmlan9252_io_txpdos, EC_WD_ENABLE},
        {0xff}
    };

    int Init(   ec_master_t    *master_,
                uint16_t        alias_, /**< Slave alias. */
                uint16_t        position_);

    void DataRead(uint8_t *domain1_pd_);
    void DataWrite(uint8_t *domain1_pd_);
    void DataPlay(void);

private:
    #define WMIO             0xE0000003, 0x92521001

};



#endif
