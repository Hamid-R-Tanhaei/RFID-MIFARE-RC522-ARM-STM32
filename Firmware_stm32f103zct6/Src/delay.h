    #ifndef DWT_STM32_DELAY_H
    #define DWT_STM32_DELAY_H
    #ifdef __cplusplus
    extern "C" {
    #endif

    #include "stm32f1xx_hal.h"
    /**
     * @brief Initializes DWT_Cycle_Count for DWT_Delay_us function
     * @return Error DWT counter
     * 1: DWT counter Error
     * 0: DWT counter works
     */
    uint32_t DWT_Delay_Init(void);
    void	DWT_Delay_ms(volatile uint32_t miliseconds);

    /**
     * @brief This function provides a delay (in microseconds)
     * @param microseconds: delay in microseconds
     */
    __STATIC_INLINE void DWT_Delay_us(volatile uint32_t microseconds)
    {
    // uint32_t clk_cycle_start = DWT->CYCCNT;
    	DWT->CYCCNT = 0;
     /* Go to number of cycles for system */
     //microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
    microseconds *= (SystemCoreClock / 1000000);
//	DWT->CYCCNT = 0;

     //microseconds = microseconds - 8;
     /* Delay till end */
     //while (((DWT->CYCCNT - clk_cycle_start) + 8) < microseconds);
     while (DWT->CYCCNT < microseconds);
    }

    #ifdef __cplusplus
    }
    #endif
#endif
