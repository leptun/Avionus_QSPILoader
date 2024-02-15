/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    quadspi.c
  * @brief   This file provides code for the configuration
  *          of the QUADSPI instances.
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
/* Includes ------------------------------------------------------------------*/
#include "quadspi.h"

/* USER CODE BEGIN 0 */
#include <string.h>
static uint8_t QSPI_AutoPollingMemReady(uint32_t timeout_ms);
static uint8_t QSPI_AutoPollingMemReadyQPI(uint32_t timeout_ms);
static uint8_t QSPI_WriteEnable(void);
static uint8_t QSPI_WriteEnableQPI(void);
static uint8_t QSPI_CheckID(void);
static uint8_t QSPI_Configuration(void);
static uint8_t QSPI_ExitAX(void);
static uint8_t QSPI_EnterQPI(void);
static uint8_t QSPI_ExitQPI(void);
static uint8_t QSPI_ResetChip(void);
/* USER CODE END 0 */

QSPI_HandleTypeDef hqspi;

/* QUADSPI init function */
void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 3;
  hqspi.Init.FifoThreshold = 4;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 23;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_ENABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

void HAL_QSPI_MspInit(QSPI_HandleTypeDef* qspiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspInit 0 */

  /* USER CODE END QUADSPI_MspInit 0 */
    /* QUADSPI clock enable */
    __HAL_RCC_QSPI_CLK_ENABLE();

    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**QUADSPI GPIO Configuration
    PF6     ------> QUADSPI_BK1_IO3
    PF7     ------> QUADSPI_BK1_IO2
    PF8     ------> QUADSPI_BK1_IO0
    PF9     ------> QUADSPI_BK1_IO1
    PB2     ------> QUADSPI_CLK
    PE7     ------> QUADSPI_BK2_IO0
    PE8     ------> QUADSPI_BK2_IO1
    PE9     ------> QUADSPI_BK2_IO2
    PE10     ------> QUADSPI_BK2_IO3
    PC11     ------> QUADSPI_BK2_NCS
    PB6     ------> QUADSPI_BK1_NCS
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN QUADSPI_MspInit 1 */

  /* USER CODE END QUADSPI_MspInit 1 */
  }
}

void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* qspiHandle)
{

  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

  /* USER CODE END QUADSPI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_QSPI_CLK_DISABLE();

    /**QUADSPI GPIO Configuration
    PF6     ------> QUADSPI_BK1_IO3
    PF7     ------> QUADSPI_BK1_IO2
    PF8     ------> QUADSPI_BK1_IO0
    PF9     ------> QUADSPI_BK1_IO1
    PB2     ------> QUADSPI_CLK
    PE7     ------> QUADSPI_BK2_IO0
    PE8     ------> QUADSPI_BK2_IO1
    PE9     ------> QUADSPI_BK2_IO2
    PE10     ------> QUADSPI_BK2_IO3
    PC11     ------> QUADSPI_BK2_NCS
    PB6     ------> QUADSPI_BK1_NCS
    */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_11);

  /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

  /* USER CODE END QUADSPI_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


uint8_t
CSP_QUADSPI_Init(void) {
    //prepare QSPI peripheral for ST-Link Utility operations
    if (HAL_QSPI_DeInit(&hqspi) != HAL_OK) {
        return HAL_ERROR;
    }
    MX_QUADSPI_Init();
    if (QSPI_ResetChip() != HAL_OK) {
        return HAL_ERROR;
    }
    HAL_Delay(10);
    if (QSPI_CheckID() != HAL_OK) {
		return HAL_ERROR;
	}
    if (QSPI_Configuration() != HAL_OK) {
        return HAL_ERROR;
    }
    if (QSPI_EnterQPI() != HAL_OK) {
		return HAL_ERROR;
	}
    return HAL_OK;
}

uint8_t
CSP_QSPI_Erase_Chip(void) {
	if (QSPI_WriteEnableQPI() != HAL_OK) {
		return HAL_ERROR;
	}
	QSPI_CommandTypeDef sCommand = {
		.Instruction = CHIP_ERASE,
		.Address = 0,
		.AlternateBytes = 0,
		.AddressSize = QSPI_ADDRESS_24_BITS,
		.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
		.DummyCycles = 0,
		.InstructionMode = QSPI_INSTRUCTION_4_LINES,
		.AddressMode = QSPI_ADDRESS_NONE,
		.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
		.DataMode = QSPI_DATA_NONE,
		.NbData = 0,
		.DdrMode = QSPI_DDR_MODE_DISABLE,
		.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
		.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
	};
	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (QSPI_AutoPollingMemReadyQPI(50000) != HAL_OK) {
		return HAL_ERROR;
	}
    return HAL_OK;
}

uint8_t
CSP_QSPI_EraseSector(uint32_t EraseStartAddress, uint32_t EraseEndAddress) {
	EraseStartAddress &= 0x0FFFFFFF;
	EraseEndAddress &= 0x0FFFFFFF;

	QSPI_CommandTypeDef sCommand = {
		.Instruction = SECTOR_ERASE,
		.Address = 0,
		.AlternateBytes = 0,
		.AddressSize = QSPI_ADDRESS_24_BITS,
		.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
		.DummyCycles = 0,
		.InstructionMode = QSPI_INSTRUCTION_4_LINES,
		.AddressMode = QSPI_ADDRESS_4_LINES,
		.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
		.DataMode = QSPI_DATA_NONE,
		.NbData = 0,
		.DdrMode = QSPI_DDR_MODE_DISABLE,
		.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
		.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
	};
	EraseStartAddress &= ~(MEMORY_SECTOR_SIZE - 1); // align to (dual flash) sector boundary

    while (EraseEndAddress >= EraseStartAddress) {
        sCommand.Address = EraseStartAddress;
        if (QSPI_WriteEnableQPI() != HAL_OK) {
            return HAL_ERROR;
        }
        if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            return HAL_ERROR;
        }
        EraseStartAddress += MEMORY_SECTOR_SIZE;
        if (QSPI_AutoPollingMemReadyQPI(400) != HAL_OK) {
            return HAL_ERROR;
        }
    }

    return HAL_OK;
}

uint8_t
CSP_QSPI_WriteMemory(uint8_t* buffer, uint32_t address, uint32_t buffer_size) {
	address &= 0x0FFFFFFF;
	if (address & 0x01 || buffer_size & 0x01) {
		// address must be 2B aligned
		return HAL_ERROR;
	}
	QSPI_CommandTypeDef sCommand = {
		.Instruction = PAGE_PROGRAM,
		.Address = 0,
		.AlternateBytes = 0,
		.AddressSize = QSPI_ADDRESS_24_BITS,
		.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
		.DummyCycles = 0,
		.InstructionMode = QSPI_INSTRUCTION_4_LINES,
		.AddressMode = QSPI_ADDRESS_4_LINES,
		.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
		.DataMode = QSPI_DATA_4_LINES,
		.NbData = 0,
		.DdrMode = QSPI_DDR_MODE_DISABLE,
		.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
		.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
	};
	sCommand.Address = address;
	while (buffer_size > 0) {
		// if address is not aligned to page boundary, reduce number of data so we don't wrap in page
		sCommand.NbData = MEMORY_PAGE_SIZE - (sCommand.Address & (MEMORY_PAGE_SIZE - 1));
		if (sCommand.NbData > buffer_size) {
			// if the last page is incomplete, reduce the number of data
			sCommand.NbData = buffer_size;
		}
		if (QSPI_WriteEnableQPI() != HAL_OK) {
			return HAL_ERROR;
		}
        if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            return HAL_ERROR;
        }
        if (HAL_QSPI_Transmit(&hqspi, buffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            return HAL_ERROR;
        }
        if (QSPI_AutoPollingMemReadyQPI(5) != HAL_OK) {
            return HAL_ERROR;
        }
        sCommand.Address += sCommand.NbData;
        buffer += sCommand.NbData;
        buffer_size -= sCommand.NbData;
	}

    return HAL_OK;
}

uint8_t
CSP_QSPI_ReadMemory(uint8_t* buffer, uint32_t address, uint32_t buffer_size) {
	address &= 0x0FFFFFFF;
	if (address & 0x01 || buffer_size & 0x01) {
		// address must be 2B aligned
		return HAL_ERROR;
	}
	QSPI_CommandTypeDef sCommand = {
		.Instruction = FAST_READ_QUAD_IO_DTR,
		.Address = address,
		.AlternateBytes = 0,
		.AddressSize = QSPI_ADDRESS_24_BITS,
		.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
		.DummyCycles = 4 - 1, //mode bits included
		.InstructionMode = QSPI_INSTRUCTION_4_LINES,
		.AddressMode = QSPI_ADDRESS_4_LINES,
		.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES,
		.DataMode = QSPI_DATA_4_LINES,
		.NbData = buffer_size,
		.DdrMode = QSPI_DDR_MODE_ENABLE,
		.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
		.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
	};

//	QSPI_CommandTypeDef sCommand = {
//		.Instruction = FAST_READ_QUAD_IO,
//		.Address = address,
//		.AlternateBytes = 0,
//		.AddressSize = QSPI_ADDRESS_24_BITS,
//		.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
//		.DummyCycles = 8 - 2, //mode bits included
//		.InstructionMode = QSPI_INSTRUCTION_4_LINES,
//		.AddressMode = QSPI_ADDRESS_4_LINES,
//		.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES,
//		.DataMode = QSPI_DATA_4_LINES,
//		.NbData = buffer_size,
//		.DdrMode = QSPI_DDR_MODE_DISABLE,
//		.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
//		.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
//	};
	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (HAL_QSPI_Receive(&hqspi, buffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}

    return HAL_OK;
}

uint8_t
CSP_QSPI_EnableMemoryMappedMode(void) {
	QSPI_CommandTypeDef sCommand = {
		.Instruction = FAST_READ_QUAD_IO_DTR,
		.Address = 0,
		.AlternateBytes = 0xA0,
		.AddressSize = QSPI_ADDRESS_24_BITS,
		.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
		.DummyCycles = 4 - 1, //mode bits included
		.InstructionMode = QSPI_INSTRUCTION_4_LINES,
		.AddressMode = QSPI_ADDRESS_4_LINES,
		.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES,
		.DataMode = QSPI_DATA_4_LINES,
		.NbData = 0,
		.DdrMode = QSPI_DDR_MODE_ENABLE,
		.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
		.SIOOMode = QSPI_SIOO_INST_ONLY_FIRST_CMD
	};

//	QSPI_CommandTypeDef sCommand = {
//		.Instruction = FAST_READ_QUAD_IO,
//		.Address = 0,
//		.AlternateBytes = 0,
//		.AddressSize = QSPI_ADDRESS_24_BITS,
//		.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
//		.DummyCycles = 8 - 2, //mode bits included
//		.InstructionMode = QSPI_INSTRUCTION_4_LINES,
//		.AddressMode = QSPI_ADDRESS_4_LINES,
//		.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES,
//		.DataMode = QSPI_DATA_4_LINES,
//		.NbData = 0,
//		.DdrMode = QSPI_DDR_MODE_DISABLE,
//		.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
//		.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
//	};
    QSPI_MemoryMappedTypeDef sMemMappedCfg = {
    	.TimeOutPeriod = 0,
		.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE,
    };

    if (HAL_QSPI_MemoryMapped(&hqspi, &sCommand, &sMemMappedCfg) != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}

uint8_t
CSP_QSPI_DisableMemoryMappedMode(void) {
	if (HAL_QSPI_Abort(&hqspi) != HAL_OK) {
		return HAL_ERROR;
	}

	if (QSPI_ExitAX() != HAL_OK) {
		return HAL_ERROR;
	}

	return HAL_OK;
}

static uint8_t
QSPI_AutoPollingMemReady(uint32_t timeout_ms) {
	QSPI_CommandTypeDef sCommand = {
		.Instruction = READ_STATUS_REGISTER,
		.Address = 0,
		.AlternateBytes = 0,
		.AddressSize = QSPI_ADDRESS_24_BITS,
		.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
		.DummyCycles = 0,
		.InstructionMode = QSPI_INSTRUCTION_1_LINE,
		.AddressMode = QSPI_ADDRESS_NONE,
		.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
		.DataMode = QSPI_DATA_1_LINE,
		.NbData = 0,
		.DdrMode = QSPI_DDR_MODE_DISABLE,
		.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
		.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
	};
    QSPI_AutoPollingTypeDef sConfig = {
    	.Match = 0x0000,
		.Mask = 0x0101,
		.Interval = 0x10,
		.StatusBytesSize = 2,
		.MatchMode = QSPI_MATCH_MODE_AND,
		.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE
    };

    if (HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig, timeout_ms) != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}

static uint8_t
QSPI_AutoPollingMemReadyQPI(uint32_t timeout_ms) {
	QSPI_CommandTypeDef sCommand = {
		.Instruction = READ_STATUS_REGISTER,
		.Address = 0,
		.AlternateBytes = 0,
		.AddressSize = QSPI_ADDRESS_24_BITS,
		.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
		.DummyCycles = 0,
		.InstructionMode = QSPI_INSTRUCTION_4_LINES,
		.AddressMode = QSPI_ADDRESS_NONE,
		.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
		.DataMode = QSPI_DATA_4_LINES,
		.NbData = 0,
		.DdrMode = QSPI_DDR_MODE_DISABLE,
		.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
		.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
	};
    QSPI_AutoPollingTypeDef sConfig = {
    	.Match = 0x0000,
		.Mask = 0x0101,
		.Interval = 0x10,
		.StatusBytesSize = 2,
		.MatchMode = QSPI_MATCH_MODE_AND,
		.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE
    };

    if (HAL_QSPI_AutoPolling(&hqspi, &sCommand, &sConfig, timeout_ms) != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}

static uint8_t
QSPI_WriteEnable(void) {
	// write enable
	{
		QSPI_CommandTypeDef sCommand = {
			.Instruction = WRITE_ENABLE,
			.Address = 0,
			.AlternateBytes = 0,
			.AddressSize = QSPI_ADDRESS_24_BITS,
			.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
			.DummyCycles = 0,
			.InstructionMode = QSPI_INSTRUCTION_1_LINE,
			.AddressMode = QSPI_ADDRESS_NONE,
			.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
			.DataMode = QSPI_DATA_NONE,
			.NbData = 0,
			.DdrMode = QSPI_DDR_MODE_DISABLE,
			.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
			.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
		};
		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
			return HAL_ERROR;
		}
	}

    return HAL_OK;
}

static uint8_t
QSPI_WriteEnableQPI(void) {
	// write enable
	{
		QSPI_CommandTypeDef sCommand = {
			.Instruction = WRITE_ENABLE,
			.Address = 0,
			.AlternateBytes = 0,
			.AddressSize = QSPI_ADDRESS_24_BITS,
			.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
			.DummyCycles = 0,
			.InstructionMode = QSPI_INSTRUCTION_4_LINES,
			.AddressMode = QSPI_ADDRESS_NONE,
			.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
			.DataMode = QSPI_DATA_NONE,
			.NbData = 0,
			.DdrMode = QSPI_DDR_MODE_DISABLE,
			.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
			.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
		};
		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
			return HAL_ERROR;
		}
	}

    return HAL_OK;
}

/* Enable quad mode and set dummy cycles count */
static uint8_t
QSPI_CheckID(void) {
	uint8_t jedec_ids[6] = { 0 };
	static const uint8_t expected_jedec_ids[6] = { 0x9d, 0x9d, 0x60, 0x60, 0x17, 0x17 };
	QSPI_CommandTypeDef sCommand = {
		.Instruction = READ_JEDEC_ID,
		.Address = 0,
		.AlternateBytes = 0,
		.AddressSize = QSPI_ADDRESS_24_BITS,
		.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
		.DummyCycles = 0,
		.InstructionMode = QSPI_INSTRUCTION_1_LINE,
		.AddressMode = QSPI_ADDRESS_NONE,
		.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
		.DataMode = QSPI_DATA_1_LINE,
		.NbData = 6,
		.DdrMode = QSPI_DDR_MODE_DISABLE,
		.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
		.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
	};
	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (HAL_QSPI_Receive(&hqspi, jedec_ids, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (memcmp(jedec_ids, expected_jedec_ids, sizeof(jedec_ids))) {
		return HAL_ERROR;
	}
	return HAL_OK;
}

/* Enable quad mode and set dummy cycles count */
static uint8_t
QSPI_Configuration(void) {
	// Read status register
	{
		uint8_t status_registers[2] = { 0 };
		QSPI_CommandTypeDef sCommand = {
			.Instruction = READ_STATUS_REGISTER,
			.Address = 0,
			.AlternateBytes = 0,
			.AddressSize = QSPI_ADDRESS_24_BITS,
			.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
			.DummyCycles = 0,
			.InstructionMode = QSPI_INSTRUCTION_1_LINE,
			.AddressMode = QSPI_ADDRESS_NONE,
			.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
			.DataMode = QSPI_DATA_1_LINE,
			.NbData = 2,
			.DdrMode = QSPI_DDR_MODE_DISABLE,
			.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
			.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
		};
		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
			return HAL_ERROR;
		}
		if (HAL_QSPI_Receive(&hqspi, status_registers, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
			return HAL_ERROR;
		}

		if (!(status_registers[0] & status_registers[1] & 0x40)) {
			// QE not enabled for at least one flash, write it to 1 on both flashes
			// Enable QE bit in status register
			{
				if (QSPI_WriteEnable() != HAL_OK) {
					return HAL_ERROR;
				}
				uint8_t status_registers[2] = { 0x40, 0x40 };
				QSPI_CommandTypeDef sCommand = {
					.Instruction = WRITE_STATUS_REGISTER,
					.Address = 0,
					.AlternateBytes = 0,
					.AddressSize = QSPI_ADDRESS_24_BITS,
					.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
					.DummyCycles = 0,
					.InstructionMode = QSPI_INSTRUCTION_1_LINE,
					.AddressMode = QSPI_ADDRESS_NONE,
					.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
					.DataMode = QSPI_DATA_1_LINE,
					.NbData = 2,
					.DdrMode = QSPI_DDR_MODE_DISABLE,
					.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
					.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
				};
				if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
					return HAL_ERROR;
				}
				if (HAL_QSPI_Transmit(&hqspi, status_registers, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
					return HAL_ERROR;
				}
		        if (QSPI_AutoPollingMemReady(20) != HAL_OK) {
		            return HAL_ERROR;
		        }
			}
		}
	}

	// set read parameters
	{
		// dummy cycles=0x10 -> 4 cycles@FRQDTR@64MHz, 8 cycles@FRQIO@133Mhz
		// driver strength = 0x07 - default
		uint8_t read_parameters_registers[2] = { 0xF0, 0xF0 };

		QSPI_CommandTypeDef sCommand = {
			.Instruction = SET_READ_PARAMETERS,
			.Address = 0,
			.AlternateBytes = 0,
			.AddressSize = QSPI_ADDRESS_24_BITS,
			.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
			.DummyCycles = 0,
			.InstructionMode = QSPI_INSTRUCTION_1_LINE,
			.AddressMode = QSPI_ADDRESS_NONE,
			.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
			.DataMode = QSPI_DATA_1_LINE,
			.NbData = 2,
			.DdrMode = QSPI_DDR_MODE_DISABLE,
			.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
			.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
		};
		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
			return HAL_ERROR;
		}
		if (HAL_QSPI_Transmit(&hqspi, read_parameters_registers, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
			return HAL_ERROR;
		}
	}
    return HAL_OK;
}

static uint8_t
QSPI_ExitAX(void) {
	// Exit AX mode from FRQDTR - 4 cycles
	{
		QSPI_CommandTypeDef sCommand = {
			.Instruction = 0,
			.Address = 0,
			.AlternateBytes = 0,
			.AddressSize = QSPI_ADDRESS_24_BITS,
			.AlternateBytesSize = QSPI_ALTERNATE_BYTES_16_BITS,
			.DummyCycles = 0,
			.InstructionMode = QSPI_INSTRUCTION_NONE,
			.AddressMode = QSPI_ADDRESS_NONE,
			.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES,
			.DataMode = QSPI_DATA_NONE,
			.NbData = 0,
			.DdrMode = QSPI_DDR_MODE_DISABLE,
			.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
			.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
		};

		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
			return HAL_ERROR;
		}
	}

	// Exit AX mode from FRQIO and FRDDTR - 8 cycles
	{
		QSPI_CommandTypeDef sCommand = {
			.Instruction = 0,
			.Address = 0,
			.AlternateBytes = 0,
			.AddressSize = QSPI_ADDRESS_24_BITS,
			.AlternateBytesSize = QSPI_ALTERNATE_BYTES_32_BITS,
			.DummyCycles = 0,
			.InstructionMode = QSPI_INSTRUCTION_NONE,
			.AddressMode = QSPI_ADDRESS_NONE,
			.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES,
			.DataMode = QSPI_DATA_NONE,
			.NbData = 0,
			.DdrMode = QSPI_DDR_MODE_DISABLE,
			.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
			.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
		};

		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
			return HAL_ERROR;
		}
	}

	// Exit AX mode from FRDIO - 16 cycles
	{
		QSPI_CommandTypeDef sCommand = {
			.Instruction = 0,
			.Address = 0,
			.AlternateBytes = 0,
			.AddressSize = QSPI_ADDRESS_24_BITS,
			.AlternateBytesSize = QSPI_ALTERNATE_BYTES_32_BITS,
			.DummyCycles = 0,
			.InstructionMode = QSPI_INSTRUCTION_NONE,
			.AddressMode = QSPI_ADDRESS_NONE,
			.AlternateByteMode = QSPI_ALTERNATE_BYTES_2_LINES,
			.DataMode = QSPI_DATA_NONE,
			.NbData = 0,
			.DdrMode = QSPI_DDR_MODE_DISABLE,
			.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
			.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
		};

		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
			return HAL_ERROR;
		}
	}

	return HAL_OK;
}

static uint8_t
QSPI_EnterQPI(void) {
	// Enter QPI mode
	{
		QSPI_CommandTypeDef sCommand = {
			.Instruction = ENTER_QPI_MODE,
			.Address = 0,
			.AlternateBytes = 0,
			.AddressSize = QSPI_ADDRESS_24_BITS,
			.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
			.DummyCycles = 0,
			.InstructionMode = QSPI_INSTRUCTION_1_LINE,
			.AddressMode = QSPI_ADDRESS_NONE,
			.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
			.DataMode = QSPI_DATA_NONE,
			.NbData = 0,
			.DdrMode = QSPI_DDR_MODE_DISABLE,
			.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
			.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
		};

		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
			return HAL_ERROR;
		}
	}

	return HAL_OK;
}

static uint8_t
QSPI_ExitQPI(void) {
	// Exit QPI mode
	{
		QSPI_CommandTypeDef sCommand = {
			.Instruction = EXIT_QPI_MODE,
			.Address = 0,
			.AlternateBytes = 0,
			.AddressSize = QSPI_ADDRESS_24_BITS,
			.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
			.DummyCycles = 0,
			.InstructionMode = QSPI_INSTRUCTION_4_LINES,
			.AddressMode = QSPI_ADDRESS_NONE,
			.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
			.DataMode = QSPI_DATA_NONE,
			.NbData = 0,
			.DdrMode = QSPI_DDR_MODE_DISABLE,
			.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
			.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
		};

		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
			return HAL_ERROR;
		}
	}

	return HAL_OK;
}

static uint8_t
QSPI_ResetChip(void) {
	if (QSPI_ExitAX() != HAL_OK) {
		return HAL_ERROR;
	}

	if (QSPI_ExitQPI() != HAL_OK) {
		return HAL_ERROR;
	}

	// Enable reset
	{
		QSPI_CommandTypeDef sCommand = {
			.Instruction = SOFTWARE_RESET_ENABLE,
			.Address = 0,
			.AlternateBytes = 0,
			.AddressSize = QSPI_ADDRESS_24_BITS,
			.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
			.DummyCycles = 0,
			.InstructionMode = QSPI_INSTRUCTION_1_LINE,
			.AddressMode = QSPI_ADDRESS_NONE,
			.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
			.DataMode = QSPI_DATA_NONE,
			.NbData = 0,
			.DdrMode = QSPI_DDR_MODE_DISABLE,
			.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
			.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
		};

		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
			return HAL_ERROR;
		}
	}

	// Execute reset
	{
		QSPI_CommandTypeDef sCommand = {
			.Instruction = SOFTWARE_RESET,
			.Address = 0,
			.AlternateBytes = 0,
			.AddressSize = QSPI_ADDRESS_24_BITS,
			.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
			.DummyCycles = 0,
			.InstructionMode = QSPI_INSTRUCTION_1_LINE,
			.AddressMode = QSPI_ADDRESS_NONE,
			.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
			.DataMode = QSPI_DATA_NONE,
			.NbData = 0,
			.DdrMode = QSPI_DDR_MODE_DISABLE,
			.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY,
			.SIOOMode = QSPI_SIOO_INST_EVERY_CMD
		};

		if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
			!= HAL_OK) {
			return HAL_ERROR;
		}
	}

    return HAL_OK;
}

/* USER CODE END 1 */
