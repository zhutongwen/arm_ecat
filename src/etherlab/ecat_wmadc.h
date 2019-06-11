#ifndef WM_ADC_H
#define WM_ADC_H

#include "ecat_slave.h"

class EcatWmadc : public EcatSlave
{
public:

    typedef struct
    {
        unsigned int analog_data[4];
        unsigned int keys;
        unsigned int leds;
        unsigned int null[2];
    }offset_t;

    typedef struct
    {
        int16_t analog_data[4];
        uint8_t key0_1;
        uint8_t led0_7;
    }data_t;


    offset_t            offset;
    data_t              data;
    ec_slave_config_t   *sc;


    //TxPdo
    ec_pdo_entry_info_t wmlan9252_adc_txpdo_entries[6] = {
        {0x6000, 0x01, 8}, /* key0_2 */
        {0x6000, 0x02, 8}, //reserve
        {0x6020, 0x01, 16}, /* analog_Input */
        {0x6020, 0x02, 16},
        {0x6020, 0x03, 16},
        {0x6020, 0x04, 16},
    };
    ec_pdo_info_t wmlan9252_adc_txpdos[2] = {
        {0x1A02, 4, wmlan9252_adc_txpdo_entries + 2}, /* TxPdo Channel 2 */
        {0x1A00, 2, wmlan9252_adc_txpdo_entries + 0}, /* TxPdo Channel 1 */
    };
    //RxPdo
    ec_pdo_entry_info_t wmlan9252_adc_rxpdo_entries[2] = {
        {0x7000, 0x01, 8}, /* led0_8 */
        {0x7000, 0x02, 8}, //reserve
    };
    ec_pdo_info_t wmlan9252_adc_rxpdos[1] = {
        {0x1601, 2, wmlan9252_adc_rxpdo_entries + 0}, /* RxPdo Channel 1 */
    };
    ec_sync_info_t wmlan9252_adc_syncs[3] = {
        {2, EC_DIR_OUTPUT, 1, wmlan9252_adc_rxpdos, EC_WD_ENABLE},
        {3, EC_DIR_INPUT,  2, wmlan9252_adc_txpdos, EC_WD_ENABLE},
        {0xff}
    };

    int Init(   ec_master_t    *master_,
                uint16_t        alias_, /**< Slave alias. */
                uint16_t        position_);

    void DataRead(uint8_t *domain1_pd_);
    void DataWrite(uint8_t *domain1_pd_);
    void DataPlay(void);

private:
    #define WMADC             0xE0000002, 0x92521010

};



#endif
