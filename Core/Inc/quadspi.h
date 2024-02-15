/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    quadspi.h
  * @brief   This file contains all the function prototypes for
  *          the quadspi.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QUADSPI_H__
#define __QUADSPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern QSPI_HandleTypeDef hqspi;

/* USER CODE BEGIN Private defines */

uint8_t CSP_QUADSPI_Init(void);
uint8_t CSP_QSPI_EraseSector(uint32_t EraseStartAddress, uint32_t EraseEndAddress);
uint8_t CSP_QSPI_WriteMemory(uint8_t* buffer, uint32_t address, uint32_t buffer_size);
uint8_t CSP_QSPI_ReadMemory(uint8_t* buffer, uint32_t address, uint32_t buffer_size);
uint8_t CSP_QSPI_EnableMemoryMappedMode(void);
uint8_t CSP_QSPI_DisableMemoryMappedMode(void);
uint8_t CSP_QSPI_Erase_Chip (void);

/* USER CODE END Private defines */

void MX_QUADSPI_Init(void);

/* USER CODE BEGIN Prototypes */

/*dual IS25LP064A memory parameters*/
#define MEMORY_FLASH_SIZE               0x1000000ul // 2x 8MB = 16MB
#define MEMORY_BLOCK_SIZE               0x20000ul   // 2x 64KB = 128KB
#define MEMORY_SECTOR_SIZE              0x2000ul    // 2x 4KB = 8KB
#define MEMORY_PAGE_SIZE                0x200ul     // 2x 256B = 512B


/*dual IS25LP064A commands */
#define FAST_READ_QUAD_IO 0xEB //FRQIO
#define FAST_READ_QUAD_IO_DTR 0xED //FRQDTR
#define PAGE_PROGRAM 0x02 //PP
#define SECTOR_ERASE 0x20 //SER
#define CHIP_ERASE 0xC7 //CER
#define WRITE_ENABLE 0x06 //WREN
#define READ_STATUS_REGISTER 0x05 //RDSR
#define WRITE_STATUS_REGISTER 0x01 //WRSR
#define ENTER_QPI_MODE 0x35 //QPIEN
#define EXIT_QPI_MODE 0xF5 //QPIDI
#define SET_READ_PARAMETERS 0xC0 //SRP
#define READ_JEDEC_ID 0x9F //RDJDID
#define SOFTWARE_RESET_ENABLE 0x66 //RSTEN
#define SOFTWARE_RESET 0x99 //RST

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __QUADSPI_H__ */

