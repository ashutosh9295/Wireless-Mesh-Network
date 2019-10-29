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

ParserReturnVal_t CmdSendPacket(int mode)
{
  uint32_t len, length, rc, data;
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  len = fetch_uint32_arg(&length);
  if(len) {
    printf("Must specify length value to the user\n");
    return CmdReturnBadParameter1;
  }

  rc = fetch_uint32_arg(&data);
  if(rc) {
    printf("Must specify data value to the user\n");
    return CmdReturnBadParameter1;
  }

  sendDataPacket(*data, len);
  receiveDataPacket(*data);
  
  return CmdReturnOk;
}

ADD_CMD("sendPacket",CmdSendPacket,"Send packet parameters")
