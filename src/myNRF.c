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
  hal_nrf_write_tx_pload(packet, packetLength);
  CE_PULSE();
}

void receiveDataPacket(uint8_t *packet)
{
    hal_nrf_read_rx_pload(packet);
    CE_PULSE();
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
  
  return CmdReturnOk;
}

ADD_CMD("sendPacket",CmdSendPacket,"Send packet parameters")

ParserReturnVal_t CmdReceivePacket(int mode)
{
  char rc, *data;
  //uint8_t length;
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  rc = fetch_cmd_args(&data);
  if(rc) {
    printf("Must specify data value to the user\n");
    return CmdReturnBadParameter1;
  }

  uint8_t dataPacket = atoi(data);
  receiveDataPacket(&dataPacket);
  
  return CmdReturnOk;
}

ADD_CMD("receivePacket",CmdReceivePacket,"Send packet parameters")