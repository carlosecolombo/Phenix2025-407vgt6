/**
   	Fecha inicialización	:	28/06/2025
	Fecha actualización 	:	12/07/2025
	Realizado por	    	:	C.E. Colombo
	Compilador utilizado	:	ST - Eclipse IDE
	Proyecto	    		:	Phenix 2025 - 407VGT6
	Archivo		    		: 	Estructuras_Peri.h
	Versión	   	    		:	1.10.00
	Objetivo	    		:	Estructuras de los Periféricos
*/

    /*------------------------------------------------------------------------
		 Estrucuras de los perifericos ST32F407 - Phenix 2025
	------------------------------------------------------------------------*/
/* Definiciones generales de las distintas variables Globales */
#if PosVariables != 0		                /* Estrucuras externas */
    extern ADC_HandleTypeDef hadc1;

    extern RTC_HandleTypeDef hrtc;
    extern RTC_TimeTypeDef sTime;
    extern RTC_DateTypeDef sDate;

    extern TIM_HandleTypeDef htim6;
    extern TIM_HandleTypeDef htim10;

    extern UART_HandleTypeDef huart4;
    extern UART_HandleTypeDef huart1;
    extern UART_HandleTypeDef huart6;

    extern DMA_HandleTypeDef hdma_uart4_tx;
    extern DMA_HandleTypeDef hdma_usart1_tx;
    extern DMA_HandleTypeDef hdma_usart6_tx;

    extern GPIO_InitTypeDef GPIO_InitStruct;
    extern ADC_ChannelConfTypeDef sConfig;
    extern TIM_MasterConfigTypeDef sMasterConfig;
    extern TIM_ClockConfigTypeDef sClockSourceConfig;

#else			/* Estrucuras propias */
    ADC_HandleTypeDef hadc1;

    RTC_HandleTypeDef hrtc;
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    RTC_AlarmTypeDef sAlarm = {0};

    TIM_HandleTypeDef htim6;
    TIM_HandleTypeDef htim10;

    UART_HandleTypeDef huart4;
    UART_HandleTypeDef huart1;
    UART_HandleTypeDef huart6;

    DMA_HandleTypeDef hdma_uart4_tx;
    DMA_HandleTypeDef hdma_usart1_tx;
    DMA_HandleTypeDef hdma_usart6_tx;

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    ADC_ChannelConfTypeDef sConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};

#endif


