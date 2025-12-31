/* USER CODE BEGIN Header */
/*---------------------------------------------------------------------------*/

/*      A T E N C I O N    D E    L A S    I N T E R R U P C I O N E S       */

/*---------------------------------------------------------------------------*/
/*
	Fecha inicialización	:	21/05/2022
	Fecha actualización 	:	22/12/2025
	Realizado por	    	:	C.E. Colombo
	Compilador utilizado	:	ST - Eclipse IDE
	Proyecto	    		:	Phenix 2025
	Archivo		    		: 	stm32f4xx_it.c
	Versión	   	    		:	3.00.00
	Objetivo				:	Atención de las interrupciones.
  */

#define  PosVariables   1   	/* 0 = Variables propias
								   1 = Variables externas */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Variables.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/*  Prototipos de las functiones externas */
extern void   ScanTec(void);
/*extern void   config_ext_channel_ADC(int8_t channel); */
extern void   Init_Medicion();							// Inicializa la suma acumulativa de valores medidos y el contador de cantidad de mediciones
extern void   delay_us(uint32_t microseconds);			// Delay en microsegundos por software (NO usa interrupciones)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void SysTick_Handler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void DMA1_Stream4_IRQHandler(void);
void ADC_IRQHandler(void);
/* void TIM1_UP_TIM10_IRQHandler(void); */
void USART1_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void UART4_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);
void DMA2_Stream6_IRQHandler(void);
void DMA2_Stream7_IRQHandler(void);
void USART6_IRQHandler(void);

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim6;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart6_tx;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */
	/******* Interrupción del Plato 1 	PB0 **********/

  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(Plato_1_Pin);
  /* USER CODE BEGIN EXTI0_IRQn 1 */
  /**
   * Interrupcion de PLATO 1 => PB0 - EXTI0_IRQ
   */
	Cant_Ejes_Med = Conta_Ejes;						// Guardo la cantidad de ejes medidos
	Conta_Ejes = 0;									// Pngo a cero el contador de Ejes
	Conta_Sincro = 0;								// Pongo a cero el contador de los pulsos de sincronismo
	Flag_Maq_Enable = true;							// Enable la máquina
	Flag_SecuEje1 = true;							// Paso el eje 1
	if(Flag_Init_Tara == true)						// Si esta tomando las taras cuanbdo ingresa el plato 1
		{
		Flag_Enable_Taras = True;					// Poen el flag para empezar a tomar las taras de sde el plato 1
		}
	if(Time_Maquina == 0)							// Si pasaron 2 seg. supongo que la máquina esta parada
		{
		Flag_Maq_Fun = false;						// Máquina parada
		Velocidad_medida = 0;						// Velocidad de máquina
		Conta_Velo_Maq = 0;
//		Flag_Maq_Enable = false;					// Disable la máquina
//		Flag_SecuenEje = false;						// Borro el Flag secuencia de ejes
//		Flag_Secuencia = false;						// Borro el Flag secuencia de los pulsos de sincronismo
		}

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */
	/**
	 * Interrupcion del sincronismo de PLATO (Un pulso por eje) => PB1 - EXTI1_IRQ
	 */
  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(SinPlato_Pin);
  /* USER CODE BEGIN EXTI1_IRQn 1 */
	if(Time_Maquina == 0)							// Si pasaron 2 seg. supongo que la máquina esta parada
		{
		Flag_Maq_Fun = false;						// Máquina parada
		Velocidad_medida = 0;						// Velocidad de máquina
		Conta_Velo_Maq = 0;
		Cant_Ejes_Med = 0;
		Cant_Pulsos_Med = 0;
//		Flag_Maq_Enable = false;					// Disable la máquina
//		Flag_SecuEje1 = false;						// Borro el Flag secuencia del eje 1
//		Flag_SecuenEje = false;						// Borro el Flag secuencia de ejes
//		Flag_Secuencia = false;						// Borro el Flag secuencia de los pulsos de sincronismo
		}
	else
		{
		Flag_Maq_Fun = true;						// Máquina en marcha
		Flag_SecuenEje = true;						// Pongo el Flag secuencia de ejes

		if(Conta_Sincro >= Config.Cant_Pulsos_Sincro - 5)
			Cant_Pulsos_Med = Conta_Sincro;
		else
			State = False;							// Genero velocidad CERO para parar el modo SORT por error de pulsos
													// de sincronismo fino.

		Conta_Sincro = 0;							// Pongo a cero el contador de los pulsos de sincronismo
		if(State == False)							// --> Calculo de velocidad
			{
			Conta_Velo = 0;							// Con la primera interrupcion inicializa el contador
			State = True;
			}
		else
			{										// Con la segunda interrupcion calcula la velocidad de la maquina (frecuencia)
			Conta_Velo_Maq = Conta_Velo;			// La cuenta la hago en el programa principal
			State = False;
			}
		}

	Conta_Ejes++;									// Incremento la cantidad de ejes medidos
	if(Conta_Ejes > Config.n_platillos)				// La cantidad de platos va de 0 a n_platillos - 1
		{
		Cant_Ejes_Med = Conta_Ejes;					// Guardo la cantidad de ejes medidos
		Conta_Ejes = 0;								// Si se supero la cantidad de ejes y no se puso en "0" es porque no llego
		}											// el plato 1
	Time_Maquina = 2000;							// Repongo el timer con cada plato

	if(Conta_Ejes > 3)								// Si conto 3 ejes, inicializa el indicador de eje 1
		{
		Flag_SecuEje1 = false;						// Borro el Flag secuencia del eje 1
		}

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */
	/**
	 * Interrupcion del sincronismo de fino (100 pulsos por eje) => PB2 - EXTI2_IRQ
	 */
  /* USER CODE END EXTI2_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(SinFino_Pin);
  /* USER CODE BEGIN EXTI2_IRQn 1 */

  	if(Flag_Osciloscopio == true)
  		{
  		if(Conta_Sincro >= Cant_Pulsos )				// Realiza la cantidad de mediciones definida en Cant_Pulsos
  			{
  			Flag_Enable_AD = false;						// Cuando llega a la cantidad de mediciones solicitada aborta el modo oscilocopio
  			Flag_Osciloscopio = false;					// da por finalizado el modo osciloscopio
  			Flag_Oscilos_End = true;					// pone un indicador para el loop principal marcando que termino de medir el modo osciloscopio
  			}
  		else											// da comienzo a una nueva serie de mediciones para el siguiente pulso de sincro fino
  			{
  			if(Flag_Enable_AD == false)
				{
  				Init_Medicion();						// Inicializa la suma acumulativa de valores medidos y el contador de cantidad de mediciones
				}
  			}
  		}
  	else
  		{
		Conta_Sincro++;
		if(Conta_Sincro > Config.Cant_Pulsos_Sincro)
			Conta_Sincro = 0;							// Pongo a cero el contador de los pulsos de sincronismo

		Flag_Secuencia = true;							// Pongo el Flag secuencia de ejes

		if(Conta_Sincro >= Config.Enable_AD)			// Comienzo de la medición del A/D
			{
			if(Flag_Enable_AD == false)
				{
				Init_Medicion();						// Inicializa la suma acumulativa de valores medidos y el contador de cantidad de mediciones
				}
			}

		if(Conta_Sincro >= Config.Disable_AD)			// Finalizo la medición del A/D
			{
			Flag_Enable_AD = false;						// Se inicializa el flag indicador de un nuevo ciclo de medicion del peso
			}
  		}
  /* USER CODE END EXTI2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */

  /* USER CODE END DMA1_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_rx);
  /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */

  /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream4 global interrupt.
  */
void DMA1_Stream4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream4_IRQn 0 */

  /* USER CODE END DMA1_Stream4_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_tx);
  /* USER CODE BEGIN DMA1_Stream4_IRQn 1 */

  /* USER CODE END DMA1_Stream4_IRQn 1 */
}

/**
  * @brief This function handles ADC1, ADC2 and ADC3 global interrupts.
  */
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */

  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC_IRQn 1 */
	int8_t Aux_Chanel;									// Auxiliar para el valor real del canal a leer

	Aux_Chanel = Channel_AD - 10;						// Descuento 10 para poder cargar  el dato en el array de datos

	TotalAD[Aux_Chanel] += HAL_ADC_GetValue(&hadc1);	// Leo el A/D y hago la suma acumulada para promediar
	Cont_AD ++;											// Incremento el contador de promedios
	if(Cont_AD > Num_Pro_def)							// Si llego a la cantidad de promedios tengo que procesarlo
		{
		PromAD[Aux_Chanel] = TotalAD[Aux_Chanel] / Num_Pro_def;	// Hago el promedio de los valores medidos

		if(PromAD[Aux_Chanel] < 1)						// Si el dato es menor a 1 lo hago ¡0
			PromAD[Aux_Chanel] = 0;

		if(PromAD[Aux_Chanel] > 4095)					// Si el dato es mayor a 4095 lo pongo en 4095
			PromAD[Aux_Chanel] = 4095;

	    Flag_Medicion = true;							// Termine la medición
		}
	else
		HAL_ADC_Start_IT(&hadc1);						//  Inicia conversion con el A/D

  /* USER CODE END ADC_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
	//	HAL_GPIO_WritePin(Dir_485_PC_GPIO_Port, Dir_485_PC_Pin,RESET);		/* Pongo la salidas PA10 = 0 (485) */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */
	/****************  Interrupción del A/D entradas celdas ****************/
	/** Los tiempos  de acceso estan bien y la lógica de lectura también
	 * encontre que no estan conectadas a tierre 4 patitas de las referencias
	 * REFAN, REFBN, REFCN, REFDN, es posible que esto vuelva inestable la
	 * medición, el otro tema a revisar el la tensión de referncia y como se
	 * corrige lo leido para aprovechar las 64.000 cuentas
	 * Se soluciona la inestabilidad con la conexión a GND.
	 */
	int8_t i;
	int8_t j;
	uint16_t Peso = 0;

	// Ahora debe leer los 8 valores medidos desde el conversor, accediendo de a uno a la vez por la interfase paralelo
	for (j = 0; j < 10; ++j)
    	{__asm__("nop");}

	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin,RESET);				// Baja la señal CS del AD para comenzar a leer los canales

	for(i = 0; i < 8; i++)										// Lee los 8 canales en forma sucesiva
		{
		HAL_GPIO_WritePin(RD_GPIO_Port, RD_Pin,RESET);			// Baja la señal RD del AD para obtener el dato del canal

		for (j = 0; j < 2; ++j)
	    	{__asm__("nop");}

		Peso = GPIOE->IDR;										// Lee el canal seleccionado

		/**	 Cuando la tension de entrada del conversor A/D se aproxima a cero, de vez en cuando
		 	 alguna medicion da como resultado un valor mayor a 32767, porque esta entregando
			 un valor negativo en CA2. Entonces ese valor se debe descartar poniendolo en cero ya que
		 	 no se contempla manejar valores negativos
			 Esta correcion es necesaria porque en el valor en display al aproximarse al cero, el numero
			 que muestra comienza a subir hasta que al quedar completamente en negativo, pone "-----",
			 pero en ese pequeño intervalo puede llevar a confusion ya que viene bajando el valor
			 y de repente empieza a subir
		*/

		if (Peso >= 0x8000)
			Peso = 0;

		/** Esta sería la corrección del valor de cuentas si usáramos todo el rango del A/D
			  para esto deberiamos cambiar en el Hardware, la pata 1 (HVSS) debería  conectarse  a -5V
			  en  lugar de GND
		*/

		/*
        if((Peso & 0x8000) == false)							// si  el valor es negativo
			Peso = 0x8000 + Peso ;								// sumo el valor
		else
			{
			Peso = (~ Peso) + 1;								// complemento a dos
			Peso = 0x7FFF - Peso ;								// sumo el valor
			}
*/

/*		// Esto se agrega para relevar las mediciones que entrega el conversor en cada ciclo de medicion
  		// Luego BO es necesario en produccion y DEBE quedar comentado
		if(Cantidad_Mediciones <= 50)							// Estas dos instrucciones se agregaron para debbuger
			Canal_AD_check[i][Cantidad_Mediciones] = Peso;			// luego se pueden comentar en producción
*/
     		Suma_Canal_AD[i]=Suma_Canal_AD[i] + Peso; 				// Hace la suma acumulativa de los valores leidos de cada canal
		HAL_GPIO_WritePin(RD_GPIO_Port, RD_Pin,SET);			// Sube la señal RD del AD para pasar al siguiente canal
		for (j = 0; j < 2; ++j)
	    	{__asm__("nop");}
		}

	for (j = 0; j < 2; ++j)
    	{__asm__("nop");}

	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin,SET);				// Sube la señal CS del AD para finalizar la lectura de los canales

	for (j = 0; j < 2; ++j)
    		{__asm__("nop");}

	Cantidad_Mediciones++;										// Actualiza el contador de mediciones
	
	if(Flag_Enable_AD == true)
		{	/* Disparo nuevamente la conversión  */
		HAL_GPIO_WritePin(CONVST_GPIO_Port, CONVST_Pin,SET);	// Pone la entrada CONVST del AD en 1 y la baja a 0 para dar inicio al ciclo de conversion

		for (j = 0; j < 2; ++j)
	    	{__asm__("nop");}

		HAL_GPIO_WritePin(CONVST_GPIO_Port, CONVST_Pin,RESET);	//
		}
	else
		{
		Flag_Conv_FIN = true;
		}
  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(Busy_Int_Pin);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

//  Flag_Conv_FIN = true;									// Pone el flag indicando que finalizo una conversion

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */

  /* USER CODE END UART4_IRQn 0 */
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
	// El tiempo de ejecucuón de la rutina es aproximadamente entre 2,7useg. y 4useg.

	Time_Out_G --;									/* Decremento el Time out de uso general */
	if(Time_Out_G  < 1)
		Time_Out_G = 0;

	Time_Out_485 --;								/* Decremento el Timer habilitor RS-458 */
	if(Time_Out_485  < 1)
		Time_Out_485 = 0;

	Time_Out_disp --;								/* Decremento el Time out del display */
	if(Time_Out_disp  <= 1)
		Time_Out_disp = 0;

	Time_Out_Velo --;								/* Decremento el Time out del display */
	if(Time_Out_Velo  <= 1)
		Time_Out_Velo = 0;

	Time_Clock --;									/* Decremento el Timer para poner el reloj */
	if(Time_Clock  < 1)
		Time_Clock = 0;

	if(Time_Buzzer--  < 1)							/* Decremento el Timer para apagar el Buzzer */
		{
		Time_Buzzer = 0;
		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin,RESET);	// Apago el Buzzer
		}

	if(Time_Parpadeo --  < 1)						/* Decremento el Timer para apagar el parpadeo */
		{
		if(Flag_Error == true)
			{
			Time_Parpadeo = Time_Parpa;
			HAL_GPIO_TogglePin(Error_GPIO_Port, Error_Pin);		// Prendo/Apago el led de ERROR Ambar
			}
		else
			{
			Time_Parpadeo = 0;
			HAL_GPIO_WritePin(Error_GPIO_Port, Error_Pin,RESET);
			}

		if(Flag_Grabo == true)
			{
			Time_Parpadeo = Time_Parpa;
			HAL_GPIO_TogglePin(CPU_OK_GPIO_Port, CPU_OK_Pin);	// Prendo/Apago el led de CPU OK Verde
			}
		}

	Time_Maquina --;								/* Decremento el Timer para varificar si la máquina esta funcionando */
	if(Time_Maquina  < 1)
		{
		Time_Maquina = 0;
		Flag_Maq_Fun = false;						// Máquina parada
		}

	Time_Medicion --;								// Actualiza el timer para la medicion estatica con el A/D
	if(Time_Medicion  < 1)
		Time_Medicion = 0;

	Conta_Velo++;									// Contador de tiempo para calcular la velocidad

	if(Flg_Display == true)							// Si tengo display habilito el scaneo del teclado
		{
		ScanTec();									/* Escaneo el teclado */
		}
  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream6 global interrupt.
  */
void DMA2_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream6_IRQn 0 */

  /* USER CODE END DMA2_Stream6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart6_tx);
  /* USER CODE BEGIN DMA2_Stream6_IRQn 1 */

  /* USER CODE END DMA2_Stream6_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA2_Stream7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream7_IRQn 0 */

  /* USER CODE END DMA2_Stream7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
  /* USER CODE BEGIN DMA2_Stream7_IRQn 1 */

  /* USER CODE END DMA2_Stream7_IRQn 1 */
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */

  /* USER CODE END USART6_IRQn 0 */
  HAL_UART_IRQHandler(&huart6);
  /* USER CODE BEGIN USART6_IRQn 1 */

  /* USER CODE END USART6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
