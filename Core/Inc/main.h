/* USER CODE BEGIN Header */
/*
 * 	Fecha inicialización	:	28/06/2025
	Fecha actualización 	:	28/06/2025
	Realizado por	    	:	C.E. Colombo
	Compilador utilizado	:	ST - Eclipse IDE
	Proyecto	    		:	Phenix 2025 - 407VGT6
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_ADC1_Init(void);
void MX_RTC_Init(void);
void MX_TIM6_Init(void);
void MX_UART4_Init(void);
void MX_USART6_UART_Init(void);
void MX_USART1_UART_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define D2_Pin GPIO_PIN_2
#define D2_GPIO_Port GPIOE
#define D3_Pin GPIO_PIN_3
#define D3_GPIO_Port GPIOE
#define D4_Pin GPIO_PIN_4
#define D4_GPIO_Port GPIOE
#define D5_Pin GPIO_PIN_5
#define D5_GPIO_Port GPIOE
#define D6_Pin GPIO_PIN_6
#define D6_GPIO_Port GPIOE
#define Input6_Pin GPIO_PIN_13
#define Input6_GPIO_Port GPIOC
#define Xtal_0_Pin GPIO_PIN_14
#define Xtal_0_GPIO_Port GPIOC
#define Xtal_1_Pin GPIO_PIN_15
#define Xtal_1_GPIO_Port GPIOC
#define CONVST_Pin GPIO_PIN_0
#define CONVST_GPIO_Port GPIOC
#define CS_Pin GPIO_PIN_1
#define CS_GPIO_Port GPIOC
#define VM5AP_Pin GPIO_PIN_2
#define VM5AP_GPIO_Port GPIOC
#define VM5D_Pin GPIO_PIN_3
#define VM5D_GPIO_Port GPIOC
#define Tx_ESP32_Pin GPIO_PIN_0
#define Tx_ESP32_GPIO_Port GPIOA
#define Rx_ESP32_Pin GPIO_PIN_1
#define Rx_ESP32_GPIO_Port GPIOA
#define Fila_0_Pin GPIO_PIN_2
#define Fila_0_GPIO_Port GPIOA
#define Fila_1_Pin GPIO_PIN_3
#define Fila_1_GPIO_Port GPIOA
#define Fila_2_Pin GPIO_PIN_4
#define Fila_2_GPIO_Port GPIOA
#define Fila_3_Pin GPIO_PIN_5
#define Fila_3_GPIO_Port GPIOA
#define Columna_0_Pin GPIO_PIN_6
#define Columna_0_GPIO_Port GPIOA
#define Columna_1_Pin GPIO_PIN_7
#define Columna_1_GPIO_Port GPIOA
#define VM3_Pin GPIO_PIN_4
#define VM3_GPIO_Port GPIOC
#define VM5AN_Pin GPIO_PIN_5
#define VM5AN_GPIO_Port GPIOC
#define Plato_1_Pin GPIO_PIN_0
#define Plato_1_GPIO_Port GPIOB
#define Plato_1_EXTI_IRQn EXTI0_IRQn
#define SinPlato_Pin GPIO_PIN_1
#define SinPlato_GPIO_Port GPIOB
#define SinPlato_EXTI_IRQn EXTI1_IRQn
#define SinFino_Pin GPIO_PIN_2
#define SinFino_GPIO_Port GPIOB
#define SinFino_EXTI_IRQn EXTI2_IRQn
#define D7_Pin GPIO_PIN_7
#define D7_GPIO_Port GPIOE
#define D8_Pin GPIO_PIN_8
#define D8_GPIO_Port GPIOE
#define D9_Pin GPIO_PIN_9
#define D9_GPIO_Port GPIOE
#define D10_Pin GPIO_PIN_10
#define D10_GPIO_Port GPIOE
#define D11_Pin GPIO_PIN_11
#define D11_GPIO_Port GPIOE
#define D12_Pin GPIO_PIN_12
#define D12_GPIO_Port GPIOE
#define D13_Pin GPIO_PIN_13
#define D13_GPIO_Port GPIOE
#define D14_Pin GPIO_PIN_14
#define D14_GPIO_Port GPIOE
#define D15_Pin GPIO_PIN_15
#define D15_GPIO_Port GPIOE
#define Pruebas_Pin GPIO_PIN_10
#define Pruebas_GPIO_Port GPIOB
#define C_S_Display_Pin GPIO_PIN_11
#define C_S_Display_GPIO_Port GPIOB
#define Input7_Pin GPIO_PIN_12
#define Input7_GPIO_Port GPIOB
#define Input8_Pin GPIO_PIN_13
#define Input8_GPIO_Port GPIOB
#define Dir_485_PSalida_Pin GPIO_PIN_14
#define Dir_485_PSalida_GPIO_Port GPIOB
#define Dir_485_PC_Pin GPIO_PIN_15
#define Dir_485_PC_GPIO_Port GPIOB
#define DD0_Pin GPIO_PIN_8
#define DD0_GPIO_Port GPIOD
#define DD1_Pin GPIO_PIN_9
#define DD1_GPIO_Port GPIOD
#define DD2_Pin GPIO_PIN_10
#define DD2_GPIO_Port GPIOD
#define DD3_Pin GPIO_PIN_11
#define DD3_GPIO_Port GPIOD
#define DD4_Pin GPIO_PIN_12
#define DD4_GPIO_Port GPIOD
#define DD5_Pin GPIO_PIN_13
#define DD5_GPIO_Port GPIOD
#define DD6_Pin GPIO_PIN_14
#define DD6_GPIO_Port GPIOD
#define DD7_Pin GPIO_PIN_15
#define DD7_GPIO_Port GPIOD
#define Tx_Salida_Pin GPIO_PIN_6
#define Tx_Salida_GPIO_Port GPIOC
#define Rx_Salida_Pin GPIO_PIN_7
#define Rx_Salida_GPIO_Port GPIOC
#define VM12_Pin GPIO_PIN_8
#define VM12_GPIO_Port GPIOC
#define Columna_2_Pin GPIO_PIN_8
#define Columna_2_GPIO_Port GPIOA
#define Columna_3_Pin GPIO_PIN_9
#define Columna_3_GPIO_Port GPIOA
#define Columna_4_Pin GPIO_PIN_10
#define Columna_4_GPIO_Port GPIOA
#define Buzzer_Pin GPIO_PIN_11
#define Buzzer_GPIO_Port GPIOA
#define Error_Pin GPIO_PIN_12
#define Error_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define CLK_Pin GPIO_PIN_14
#define CLK_GPIO_Port GPIOA
#define CPU_OK_Pin GPIO_PIN_15
#define CPU_OK_GPIO_Port GPIOA
#define Busy_Int_Pin GPIO_PIN_10
#define Busy_Int_GPIO_Port GPIOC
#define Busy_Int_EXTI_IRQn EXTI15_10_IRQn
#define RD_Pin GPIO_PIN_11
#define RD_GPIO_Port GPIOC
#define Reset_Pin GPIO_PIN_12
#define Reset_GPIO_Port GPIOC
#define Input1_Pin GPIO_PIN_0
#define Input1_GPIO_Port GPIOD
#define Input2_Pin GPIO_PIN_1
#define Input2_GPIO_Port GPIOD
#define Input3_Pin GPIO_PIN_2
#define Input3_GPIO_Port GPIOD
#define Input4_Pin GPIO_PIN_3
#define Input4_GPIO_Port GPIOD
#define Input5_Pin GPIO_PIN_4
#define Input5_GPIO_Port GPIOD
#define LCD_RS_Pin GPIO_PIN_5
#define LCD_RS_GPIO_Port GPIOD
#define LCD_WR_Pin GPIO_PIN_6
#define LCD_WR_GPIO_Port GPIOD
#define LCD_EN_Pin GPIO_PIN_7
#define LCD_EN_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define AD0_OK_Pin GPIO_PIN_4
#define AD0_OK_GPIO_Port GPIOB
#define Test_V_OK_Pin GPIO_PIN_5
#define Test_V_OK_GPIO_Port GPIOB
#define Tx_PC_Pin GPIO_PIN_6
#define Tx_PC_GPIO_Port GPIOB
#define Rx_PC_Pin GPIO_PIN_7
#define Rx_PC_GPIO_Port GPIOB
#define Modo_Test_Pin GPIO_PIN_8
#define Modo_Test_GPIO_Port GPIOB
#define Modo_Trabajo_Pin GPIO_PIN_9
#define Modo_Trabajo_GPIO_Port GPIOB
#define D0_Pin GPIO_PIN_0
#define D0_GPIO_Port GPIOE
#define D1_Pin GPIO_PIN_1
#define D1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
