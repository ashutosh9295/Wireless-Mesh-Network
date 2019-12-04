#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "common.h"
#include "hal_nrf_hw.h"
#include "hal_nrf.h"
#include "hal_nrf_reg.h"

uint32_t deviceID(void);

void sendDataPacket(uint8_t *packet, uint8_t packetLength) 
{
  hal_nrf_write_tx_pload(packet, packetLength);
  CE_PULSE();  

}

uint16_t receiveDataPacket(uint8_t *packet) //change the type to uint16 or 32 and then return the length
{
   uint16_t payload_length = 0;
   uint16_t PLL = hal_nrf_read_rx_pload(packet);
   payload_length = (PLL & 0x00FF); // take the bottom 8 bit
   printf("%d -- payload\n",payload_length);
   return payload_length;
}

ParserReturnVal_t CmdSendPacket(int mode)
{
  char rc, *data;
  uint8_t length;
  uint8_t  id;
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  rc = fetch_cmd_args(&data);
  id = data[5]-'0';

  if(id > 4  || id <= 0)
  {
    printf("Must specify last char as 1 or 2 or 3 depending on NODE ID.\n");
    return CmdReturnBadParameter1;
  }
  if(rc) {
    printf("Must specify data value to the user\n");
    return CmdReturnBadParameter1;
  }
  

  length = strlen(data);
  
  sendDataPacket((uint8_t*)data, length);
  printf("Data sent is = %s & Length of data = %i\n\r", data, length);
  return CmdReturnOk;
}

ADD_CMD("sendPacket",CmdSendPacket,"Send packet parameters")

ParserReturnVal_t CmdReceivePacket(int mode)
{
  uint8_t rxData_P[32] = {0};
  uint16_t length = 0;
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

// create a for loop here
  length = receiveDataPacket(rxData_P);

  if(rxData_P[5] == 0x31){
    printf("NODE 1 is ALIVE\n");
  }
  else if(rxData_P[5] == 0x32){
    printf("NODE 2 is ALIVE\n");
  }
  else if(rxData_P[5] == 0x33){
    printf("NODE 3 is ALIVE\n");
  }
  printf("Data in hex:\n");
  for (uint8_t i = 0; i < length-1; i++){
      printf("%X  ", rxData_P[i]);
    }
  printf("\n");
  printf("Data Received - %s\n", rxData_P);
  return CmdReturnOk;
}

ADD_CMD("receivePacket",CmdReceivePacket,"Send packet parameters")
