#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
  char rc, *data;
  uint8_t length;
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  rc = fetch_cmd_args(&data);
  if(rc) {
    printf("Must specify data value to the user\n");
    return CmdReturnBadParameter1;
  }

  length = strlen(data);
  uint8_t dataPacket = atoi(data);
  sendDataPacket(&dataPacket, length);
  receiveDataPacket(&dataPacket);
  
  return CmdReturnOk;
}

ADD_CMD("sendPacket",CmdSendPacket,"Send packet parameters")