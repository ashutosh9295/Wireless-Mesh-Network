/*
 * Copyright (c) 2006 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */
/** @file
 *  C-file defining the hardware depenent interface of the nRF24l01.
 *
 *  @author Jon Helge Nistad
 *
 */

#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "common.h"

extern SPI_HandleTypeDef hspi3;

uint8_t hal_nrf_rw(uint8_t value)
{
  uint8_t RxData;
  HAL_SPI_TransmitReceive(&hspi3, &value, &RxData, 1, 1000);
  
  return RxData;
}
//hal_spi_master_read_write(value); // replace with the hal spi read write function


