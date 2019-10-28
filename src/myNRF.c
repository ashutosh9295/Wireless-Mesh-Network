#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "common.h"
#include "hal_nrf_hw.h"
#include "hal_nrf.h"
#include "hal_nrf_reg.h"

void sendDataPacket(uint8_t *packet, uint8_t packetLength)
{
  CSN_HIGH();
  CSN_LOW();
  hal_nrf_write_tx_pload(packet, packetLength);
  CSN_HIGH();
}

void receiveDataPacket(uint8_t *packet)
{
    CSN_HIGH();
    CSN_LOW();
    hal_nrf_read_rx_pload(packet);
    CSN_HIGH();
}