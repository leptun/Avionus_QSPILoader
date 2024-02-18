#include "quadspi.h"
#include "main.h"
#include "gpio.h"
#include "tim.h"

#define LOADER_OK   0x1
#define LOADER_FAIL 0x0

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
	LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_FOUR_TIMES); // Make all timers run at HCLK
	MX_TIM2_Init();
	LL_TIM_SetPrescaler(TIM2, __LL_TIM_CALC_PSC(SystemCoreClock, 8000ul));
	LL_TIM_GenerateEvent_UPDATE(TIM2);
	LL_TIM_EnableCounter(TIM2);

	return HAL_OK;
}
uint32_t HAL_GetTick(void) {
	return LL_TIM_GetCounter(TIM2) / 8;
}
void HAL_SuspendTick(void) { }
void HAL_ResumeTick(void) { }

/**
 * @brief  System initialization.
 * @param  None
 * @retval  LOADER_OK = 1   : Operation succeeded
 * @retval  LOADER_FAIL = 0 : Operation failed
 */
int __attribute__((used))
Init(void) {
	extern uint32_t _sbss[];
	extern uint32_t _ebss[];
	for (uint32_t *ptr = _sbss; ptr < _ebss; ptr++) {
		*ptr = 0;
	}
	uint32_t res;

    SystemInit();

    /* ADAPTATION TO THE DEVICE
     *
     * change VTOR setting for H7 device
     * SCB->VTOR = 0x24000000 | 0x200;
     *
     * change VTOR setting for other devices
     * SCB->VTOR = 0x20000000 | 0x200;
     *
     * */

    extern uint32_t g_pfnVectors[];
    SCB->VTOR = (uint32_t)g_pfnVectors;

    HAL_Init();

    // reset clocks
    SET_BIT(RCC->CR, RCC_CR_HSION); /* Set HSION bit to the reset value */
    while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == RESET) { } /* Wait till HSI is ready */
    CLEAR_REG(RCC->CFGR); /* Reset CFGR register */
    while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET) { } /* Wait till clock switch is ready */
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON); /* Clear PLLON bit */
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET) { } /* Wait till PLL is disabled */

    // configure clocks
    extern void SystemClock_Config(void);
    SystemClock_Config();

    MX_GPIO_Init();

    __HAL_RCC_QSPI_FORCE_RESET();  //completely reset peripheral
    __HAL_RCC_QSPI_RELEASE_RESET();

    if ((res = CSP_QUADSPI_Init())) {
        return res + 100;
    }

    if ((res = CSP_QSPI_EnableMemoryMappedMode())) {
        return res + 200;
    }

    return LOADER_OK;
}

/**
 * @brief   Program memory.
 * @param   Address: page address
 * @param   Size   : size of data
 * @param   buffer : pointer to data buffer
 * @retval  LOADER_OK = 1       : Operation succeeded
 * @retval  LOADER_FAIL = 0 : Operation failed
 */
int __attribute__((used))
Write(uint32_t Address, uint32_t Size, uint8_t* buffer) {
    if (CSP_QSPI_DisableMemoryMappedMode() != HAL_OK) {
        return LOADER_FAIL;
    }

    if (CSP_QSPI_WriteMemory((uint8_t*) buffer, (Address & (0x0fffffff)), Size) != HAL_OK) {
        return LOADER_FAIL;
    }

    if (CSP_QSPI_EnableMemoryMappedMode() != HAL_OK) {
        return LOADER_FAIL;
    }

    return LOADER_OK;
}

/**
 * @brief   Sector erase.
 * @param   EraseStartAddress :  erase start address
 * @param   EraseEndAddress   :  erase end address
 * @retval  LOADER_OK = 1       : Operation succeeded
 * @retval  LOADER_FAIL = 0 : Operation failed
 */
int __attribute__((used))
SectorErase(uint32_t EraseStartAddress, uint32_t EraseEndAddress) {
    if (CSP_QSPI_DisableMemoryMappedMode() != HAL_OK) {
        return LOADER_FAIL;
    }

    if (CSP_QSPI_EraseSector(EraseStartAddress, EraseEndAddress) != HAL_OK) {
        return LOADER_FAIL;
    }

    if (CSP_QSPI_EnableMemoryMappedMode() != HAL_OK) {
        return LOADER_FAIL;
    }

    return LOADER_OK;
}

/**
 * Description :
 * Mass erase of external flash area
 * Optional command - delete in case usage of mass erase is not planed
 * Inputs    :
 *      none
 * outputs   :
 *     none
 * Note: Optional for all types of device
 */
int __attribute__((used))
MassErase(void) {
    if (CSP_QSPI_DisableMemoryMappedMode() != HAL_OK) {
        return LOADER_FAIL;
    }

    if (CSP_QSPI_Erase_Chip() != HAL_OK) {
        return LOADER_FAIL;
    }

    if (CSP_QSPI_EnableMemoryMappedMode() != HAL_OK) {
        return LOADER_FAIL;
    }

    return LOADER_OK;
}

/**
 * Description :
 * Calculates checksum value of the memory zone
 * Inputs    :
 *      StartAddress  : Flash start address
 *      Size          : Size (in WORD)
 *      InitVal       : Initial CRC value
 * outputs   :
 *     R0             : Checksum value
 * Note: Optional for all types of device
 */
uint32_t __attribute__((used))
CheckSum(uint32_t StartAddress, uint32_t Size, uint32_t InitVal) {
    uint8_t missalignementAddress = StartAddress % 4;
    uint8_t missalignementSize = Size;
    int cnt;
    uint32_t Val;

    StartAddress -= StartAddress % 4;
    Size += (Size % 4 == 0) ? 0 : 4 - (Size % 4);

    for (cnt = 0; cnt < Size; cnt += 4) {
        Val = *(uint32_t*) StartAddress;
        if (missalignementAddress) {
            switch (missalignementAddress) {
                case 1:
                    InitVal += (uint8_t) (Val >> 8 & 0xff);
                    InitVal += (uint8_t) (Val >> 16 & 0xff);
                    InitVal += (uint8_t) (Val >> 24 & 0xff);
                    missalignementAddress -= 1;
                    break;
                case 2:
                    InitVal += (uint8_t) (Val >> 16 & 0xff);
                    InitVal += (uint8_t) (Val >> 24 & 0xff);
                    missalignementAddress -= 2;
                    break;
                case 3:
                    InitVal += (uint8_t) (Val >> 24 & 0xff);
                    missalignementAddress -= 3;
                    break;
            }
        } else if ((Size - missalignementSize) % 4 && (Size - cnt) <= 4) {
            switch (Size - missalignementSize) {
                case 1:
                    InitVal += (uint8_t) Val;
                    InitVal += (uint8_t) (Val >> 8 & 0xff);
                    InitVal += (uint8_t) (Val >> 16 & 0xff);
                    missalignementSize -= 1;
                    break;
                case 2:
                    InitVal += (uint8_t) Val;
                    InitVal += (uint8_t) (Val >> 8 & 0xff);
                    missalignementSize -= 2;
                    break;
                case 3:
                    InitVal += (uint8_t) Val;
                    missalignementSize -= 3;
                    break;
            }
        } else {
            InitVal += (uint8_t) Val;
            InitVal += (uint8_t) (Val >> 8 & 0xff);
            InitVal += (uint8_t) (Val >> 16 & 0xff);
            InitVal += (uint8_t) (Val >> 24 & 0xff);
        }
        StartAddress += 4;
    }

    return (InitVal);
}

/**
 * Description :
 * Verify flash memory with RAM buffer and calculates checksum value of
 * the programmed memory
 * Inputs    :
 *      FlashAddr     : Flash address
 *      RAMBufferAddr : RAM buffer address
 *      Size          : Size (in WORD)
 *      InitVal       : Initial CRC value
 * outputs   :
 *     R0             : Operation failed (address of failure)
 *     R1             : Checksum value
 * Note: Optional for all types of device
 */
uint64_t __attribute__((used))
Verify(uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size, uint32_t missalignement) {
    uint32_t VerifiedData = 0, InitVal = 0;
    uint64_t checksum;
    Size *= 4;

    checksum = CheckSum((uint32_t) MemoryAddr + (missalignement & 0xf),
                        Size - ((missalignement >> 16) & 0xF), InitVal);
    while (Size > VerifiedData) {
        if (*(uint8_t*) MemoryAddr++
            != *((uint8_t*) RAMBufferAddr + VerifiedData)) {
            return ((checksum << 32) + (MemoryAddr + VerifiedData));
        }
        VerifiedData++;
    }

    return (checksum << 32);
}
