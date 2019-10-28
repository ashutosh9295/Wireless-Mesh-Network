#include <stdio.h>
#include <stdint.h>
#include "common.h"
#include "main.h"
#include "hal_nrf_reg.h"
#include "hal_nrf.h"
#include "hal_nrf_hw.h"

void T_Payload_SPI(uint8_t *payload, uint8_t Load_len)
{
  uint8_t i=0;
  uint8_t buffer_pointer = WR_TX_PLOAD;
  CSN_LOW();
  CE_LOW();
  
  HAL_SPI_Transmit(&hspi3, &buffer_pointer, 2, 0);
  for (i=0; i<Load_len; i++){
    HAL_SPI_Transmit(&SpiHandle, (payload+i), 2, 0);
  }
  CSN_HIGH();
}


void T_Data_SPI(uint8_t buffer_pointer, uint8_t data_buffer)
{
  CSN_LOW();
  HAL_SPI_Transmit(&hspi3, &buffer_pointer, 2, 0);
  HAL_SPI_Transmit(&hspi3, &data_buffer, 2, 0);
  
  printf("data: %d", data_buffer);
  CSN_HIGH();
}

void R_Data_SPI(uint8_t buffer_pointer, uint8_t data_buffer)
{
  CSN_LOW();
  HAL_SPI_Receive(&hspi3, *buffer_pointer, 2, 0);
  HAL_SPI_Transmit(&hspi3, *data_buffer, 2, 0);
  
  CSN_HIGH();

  return data_buffer;
}

void R_Payload_SPI(uint8_t *payload, uint8_t Load_len)
{
  uint8_t buffer_pointer = 0;
  uint8_t data_buffer[payloadLen];
  uint8_t i= 0;

  buffer_pointer = RD_RX_PLOAD_W;
  payloadLen = receive_data_from_spi(buffer_pointer, data_buffer[0]);

  printf("\n\n\n\rPayload Length %d\n\n\n\n\r", Load_Len);

  buffer_pointer = RD_RX_PAYLOAD;

  //RESET_CE;
  SET_CSN;
  HAL_Delay(10);
  RESET_CSN;
  //uint8_t Rxd_Data = 0;

  HAL_SPI_Receive(&hspi3, data_buffer, 1, 0);
  HAL_SPI_Receive(&hspi3, data_buffer, 1, 0);

	for(i = 0; i<payloadLen; i++)
	{
		HAL_SPI_Receive(&hspi3, (payload+i), 1, 0);
		// HAL_Delay(1);
		printf("PL %d %d\n\r", *(payload+i), i);
	}
	SET_CSN;

	return(payloadLen);
}



