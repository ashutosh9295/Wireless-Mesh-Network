#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "common.h"
#include "hal_nrf_hw.h"
#include "hal_nrf.h"
#include "hal_nrf_reg.h"
#include "target_includes.h"

static const uint8_t address[HAL_NRF_AW_5BYTES] = {0x22,0x33, 0x44,0x55,0x01};
void radio_set_status (radio_status_t new_status);

static radio_status_t status;

void radio_init (const uint8_t *address, hal_nrf_operation_mode_t operational_mode)
{
  hal_nrf_close_pipe(HAL_NRF_ALL);               // First close all radio pipes
                                                 // Pipe 0 and 1 open by default
  hal_nrf_open_pipe(HAL_NRF_PIPE0, false);       // Open pipe0, without/autoack

  hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);       // Operates in 16bits CRC mode
  hal_nrf_set_auto_retr(0, 10);                  // Disables auto retransmit

  hal_nrf_set_address_width(HAL_NRF_AW_5BYTES);  // 5 bytes address width
  hal_nrf_set_address(HAL_NRF_TX, address);      // Set device's addresses
  hal_nrf_set_address(HAL_NRF_PIPE0, address);   // Sets recieving address on 
                                                 // pipe0  
  
  if(operational_mode == HAL_NRF_PTX)            // Mode depentant settings
  {
    hal_nrf_set_operation_mode(HAL_NRF_PTX);     // Enter TX mode
  }
  else
  {
    hal_nrf_set_operation_mode(HAL_NRF_PRX);     // Enter RX mode
    hal_nrf_set_rx_pload_width((uint8_t)HAL_NRF_PIPE0, 10);
                                                 // Pipe0 expect 
                                                 // PAYLOAD_LENGTH byte payload
                                                 // PAYLOAD_LENGTH in radio.h
  }

  hal_nrf_set_rf_channel(40);                    // Operating on static channel 
                                                 // Defined in radio.h. 
                                                 // Frequenzy = 
                                                 //        2400 + RF_CHANNEL
  hal_nrf_set_power_mode(HAL_NRF_PWR_UP);        // Power up device

//hal_nrf_set_datarate(HAL_NRF_1MBPS);           // Uncomment this line for 
                                                 // compatibility with nRF2401 
                                                 // and nRF24E1

  radio_set_status (RF_IDLE);                    // Radio now ready
}                                                

void radio_set_status (radio_status_t new_status)
{
  status = new_status;
}

ParserReturnVal_t CmdRadioInit(int mode)
{
  char opMode = '\0';
  int rc;
  uint16_t length;
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
    rc = fetch_uint16_arg(&length);
  if(rc) {
    printf("Must specify data value to the user\n");
    return CmdReturnBadParameter1;
  }

  if (length == 0){
      opMode = HAL_NRF_PTX;
  }
  else if (length == 1){
      opMode = HAL_NRF_PRX;
  }

  radio_init(address, opMode);
  printf("Radio Initialised\n");
  
  return CmdReturnOk;
}

ADD_CMD("radioInit",CmdRadioInit,"Initialise Radio, set 0 for transmission or 1 for receive")

