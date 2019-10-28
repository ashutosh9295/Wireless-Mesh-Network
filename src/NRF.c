#include <stdio.h>
#include <stdint.h>
#include "common.h"
#include "main.h"
#include "hal_nrf_reg.h"

void configforDypd()

{
	uint8_t spiCmd = 0;
	uint8_t spiData = 0;
	//POWER DOWN MODE
	spiCmd  = WR_REG | CONFIG;
	spiData = 0x38;
	send_data_to_spi(spiCmd, spiData);

	// Activate
	spiCmd  = LOCK_UNLOCK;
	spiData = 0x73;
	send_data_to_spi(spiCmd, spiData);

	// feature
	spiCmd  =  WR_REG | FEATURE;
	spiData = 0x04; // 00000100
	send_data_to_spi(spiCmd, spiData);

	// DYPD
	spiCmd = WR_REG | DYNPD;
	spiData = 0x07; //00000111;
	send_data_to_spi(spiCmd, spiData);
}

void send_payload_to_spi(uint8_t * payload, uint8_t payloadLen)
{
	uint8_t i = 0;
	uint8_t spiCmd = WR_TX_PAYLOAD;
	//RESET_CE;
	CSN_HIGH();
	timerDelay();
	//SET_CE;
	CSN_LOW();

	HAL_SPI_Transmit(&hspi3, &spiCmd, 1, 0);

	for(i = 0; i < payloadLen; i++)
	{
		HAL_SPI_Transmit(&hspi3, (payload+i), 1, 0);
		printf("payload %d\n\r", payload[i]);
	}
	//HAL_SPI_Transmit(&SpiHandle, &spiData, 1, 0);
	CSN_HIGH();
}

void send_data_to_spi(uint8_t spiCmd,uint8_t spiData)
{
	//RESET_CE;
	CSN_HIGH();
	timerDelay();
	//SET_CE;
	CSN_LOW();

	HAL_SPI_Transmit(&hspi3, &spiCmd, 1, 0);
	HAL_SPI_Transmit(&hspi3, &spiData, 1, 0);
	printf("spiData %d\n\r", spiData);

	CSN_HIGH();
	//RESET_CE;
}

uint8_t receive_data_from_spi(uint8_t spiCmd, uint8_t spiData)
{
	//RESET_CE;
	CSN_HIGH();
	timerDelay();
	CSN_LOW();
	//uint8_t Rxd_Data = 0;

	HAL_SPI_Transmit(&hspi3, &spiCmd, 1, 0);
	HAL_SPI_Transmit(&hspi3, &spiData, 1, 0);
	HAL_SPI_Receive(&hspi3, &spiCmd, 1, 0);
	// HAL_Delay(1);
	printf("Status %d\n\r", spiData);

	HAL_SPI_Receive(&hspi3, &spiData, 1, 0);
	//HAL_Delay(1);
	printf("Status %d\n\r", spiData);

	HAL_SPI_Receive(&hspi3, &spiData, 1, 0);
	//HAL_Delay(1);
	printf("Status %d\n\r", spiData);

	CSN_HIGH();
	return spiData;
}

uint8_t receive_payload_from_spi(uint8_t * payload, uint8_t payloadLen)
{
	uint8_t spiCmd = 0;

	uint8_t spiData[payloadLen];
	uint8_t i= 0;

	spiCmd = RD_RX_PLOAD_W;
	payloadLen = receive_data_from_spi(spiCmd, spiData[0]);

	printf("\n\n\n\rPayload Length %d\n\n\n\n\r", payloadLen);

	spiCmd = RD_RX_PAYLOAD;

	//RESET_CE;
	SET_CSN;
	HAL_Delay(10);
	RESET_CSN;
	//uint8_t Rxd_Data = 0;

	HAL_SPI_Transmit(&hspi3, &spiCmd, 1, 0);
	HAL_SPI_Transmit(&hspi3, spiData, 1, 0);
	HAL_SPI_Receive(&hspi3, spiData, 1, 0);
	printf("SL %d %d\n\r", *(spiData), i);
	HAL_SPI_Receive(&hspi3, spiData, 1, 0);
	printf("SL %d %d\n\r", *(spiData), i);

	for(i = 0; i<payloadLen; i++)
	{
		HAL_SPI_Receive(&hspi3, (payload+i), 1, 0);
		// HAL_Delay(1);
		printf("PL %d %d\n\r", *(payload+i), i);
	}
	SET_CSN;

	return(payloadLen);
}

void config_nrf24l01(uint8_t Mode)
{
	uint8_t spiCmd = 0;
	uint8_t spiData = 0;
	//configuration of nRF24L01

	if(Mode == Tx)
	{
		//SET_CE;
		printf("Configuration for TX\n\r");
		spiCmd = WRITE_REG | CONFIG;
		spiData = 26; //'00011010'
		send_data_to_spi(spiCmd, spiData);
	}
	if(Mode == Rx)
	{
		//SET_CE;
		printf("Configuration for RX\n\r");
		spiCmd = WR_REG | _NRF24L01P_REG_CONFIG;
		spiData = 59; //'00111011'
		receive_data_from_spi(spiCmd, spiData);
	}
}

void configRxAddress(uint8_t * Adrs)
{
	uint8_t i = 0;
	uint8_t spiCmd = WR_REG | RX_ADDR_P1;
	//RESET_CE;
	SET_CSN;
	HAL_Delay(10);
	//SET_CE;
	RESET_CSN;

	HAL_SPI_Transmit(&hspi3, &spiCmd, 1, 0);

	for(i = 0; i < 5; i++)
	{
		HAL_SPI_Transmit(&hspi3, (Adrs+i), 1, 0);
		printf("payload %d\n\r", Adrs[i]);
	}
	SET_CSN;
}



void configTxAddress(uint8_t * Adrs)
{
	uint8_t i = 0;
	uint8_t spiCmd = WR_REG | TX_ADDR;
	//RESET_CE;
	SET_CSN;
	HAL_Delay(10);
	//SET_CE;
	RESET_CSN;

	HAL_SPI_Transmit(&hspi3, &spiCmd, 1, 0);

	for(i = 0; i < 5; i++)
	{
		HAL_SPI_Transmit(&hspi3, (Adrs+i), 1, 0);
		printf("payload %d\n\r", Adrs[i]);
	}
	//HAL_SPI_Transmit(&SpiHandle, &spiData, 1, 0);
	SET_CSN;
}

void readpipeAdress(uint8_t spiCmd)
{
	// uint8_t spiCmd = 0;
	uint8_t spiData = 0;
	uint8_t i= 0;
	uint8_t pipeAdr[5] = {0,0,0,0,0};

	SET_CSN;
	HAL_Delay(10);
	RESET_CSN;
	//uint8_t Rxd_Data = 0;

	HAL_SPI_Transmit(&hspi3, &spiCmd, 1, 0);
	HAL_SPI_Transmit(&hspi3, &spiData, 1, 0);
	HAL_SPI_Receive(&hspi3, &spiData, 1, 0);
	printf("SL %d %d\n\r", spiData, i);
	HAL_SPI_Receive(&hspi3, &spiData, 1, 0);
	printf("SL %d %d\n\r", spiData, i);

	for(i = 0; i<5; i++)
	{
		HAL_SPI_Receive(&hspi3, (pipeAdr+i), 1, 0);
		// HAL_Delay(1);
		printf("P2 Adrs %d %d\n\r", *(pipeAdr+i), i);
	}
	SET_CSN;
}


