/* USER CODE BEGIN Header */

/*
 * 		Programa principal  Phenix 2025
  *
    Fecha inicialización	:	02/05/2025
    Fecha actualización 	:	22/12/2025
    Realizado por	    	:	C.E. Colombo
    Compilador utilizado	:	ST - Eclipse IDE
    Proyecto	    		:	Phenix 2025
    Archivo		    		: 	Main.c
    Versión	   	    		:	4.50.00
    Objetivo	    		:	Actualización del software de la Tamañadora
                            Se comenzó a estudiar los elemntos a usar el 03/01/2022
                            Ver. 1.00.00	Instalación del Software de desarrollo
                            11/05/2022		Estudio micro a utilizar							==>
                                            Diseño del circuito y busqueda de materiales		==>
                                            Estudio de placa STM32407VGT6 y puesta en marcha 	==>
                                            Armado y puesta en marcha LCD 4 x 20				==>		12hs.		ok.
                                            Armado manejo de teclado y puesta en marcha			==>		15hs.		ok.
                                            Pruebas y retoques de lo escrito					==>		 8hs.		ok.
                                            Diseño del circuito electrónico						==>
                                            Puesta en marcha del RTC y pruebas					==>		 8hs.		ok.
                                            Armado y prueba Buzzer								==>		 2hs.		ok.
                                            Modificaciones del circuito electrónico				==>
                                            Medición de tensiones varias PC0-PC1-PC2-PC3-
                                                                         PC4-PC5-Vbat			==>		 12hs.		ok.
                                            Prueba de 3 entradas de sincronismo					==>		 20hs.		ok.
                                            Armado y prueba de salidas serie					==>		 20hs.		ok.


*****			Parto de la versión Versión 9.31A de Phenix (realizada para el MC68332)

                            Ver. 1.20.00	Se cambió por el micro STM32F407VGT6, la diferencia
                            01/06/2025		está en la cantidad de patas y Ports que dispone
                                            esto se hace por la disponibilidad de micro.
                                            Se cambian la distribución de patas para adaptar
                                            al mejor desarrollo del impreso (LCD_EN, LCD_RS,
                                            LCD_WR) el teclado se mantiene.
                                            Se maneja el nuevo conversor A/D

                            Ver. 1.30.00
                            01/07/2025		Se comienza la prueba y testeo del hardware nuevo:
                                            Puesta en marcha LCD 4 x 20							==>		3hs.		ok.
                                            puesta en marcha del teclado						==>		2hs.		ok.
                                            Puesta en marcha del RTC y pruebas					==>		2hs.		ok.
                                            Prueba Buzzer										==>		1hs.		ok.
                                            Medición de tensiones varias PC0-PC1-PC2-PC3-
                                                                         PC4-PC5-Vbat			==>		12hs.		ok.
                                            Prueba de 3 entradas de sincronismo					==>		10hs.		ok.
                                                Hay que unir las patas 1 y 3 del conector  RJ
                                                de salida a placa nueva.
                                                Se agregaron 3 resistencias de 100K en paralelo
                                                con los led D5, D6, D7
                                            Prueba de salidas serie	por DMA						==>		20hs.		ok.
                                                1.- Comandos CPU --> Placas de salida			==>		15hs.		ok.
                                                    -*OJO*- No coinciden los pines del conector
                                                    RJ de salida (patas 4 y 5)a las placas de
                                                    salida con el conector de entrada (patas 1 y 2
                                                    ó 7 y 8) de la placa de salidas.
                                                    a.- Transmisión modo trabajo				==>		8hs.		ok.
                                                        Se modifico en la placa de Salidas el
                                                        comando del modo Trabajo es 16 en
                                                        lugar de 15
                                                    b.- Recepción modo trabajo					==>
                                                2.- Comandos PC --> CPU	--> Placas de salida	==>		15hs.		ok.
                                                    a.- Transmisión modo test					==>		10hs.		ok.
                                                        Se corrigio la  cantidad de bytes a
                                                        transmitir
                                                    b.- Recepción modo test						==>		10hs.		ok.

                                                3.- Comandos PC --> CPU							==>
                                                    a.- Transmisión modo test					==>		10hs.		ok.
                                                    b.- Recepción modo test						==>		10hs.		ok.
                                                    c.- Transmisión modo trabajo				==>		10hs.		ok.
                                                    d.- Recepción modo trabajo					==>

                                                3.- Comandos CPU --> módulo ESP8266				==>
                                                    3.1.- Carga de datos						==>
                                                    3.2.- conexión red interna					==>
                                                    3.3.- Carga de comandos 					==>
                                                    3.4.- Prueba de campo						==>
                                            Prueba del A/D 8 canales							==>
                                                1.- Lectura de datos							==>
                                                2.- Sincronizar con los pulsos de sincronismo	==>

                            Ver. 2.00.00
                            24/08/2025		Se arreglaron algunos problemas de recepción de la placa
                                            de salidas, se limpia el buffer de recepción de la placa
                                            de salidas y se habilita el DMA_Sal al entrar al comando
                                            de pedir los datos a dichas placas.

                            Ver. 2.10.00
                            13/09/2025		Se agregaron las rutinas de la medidiciones del A/D
                                            de 8 canales para calibración de celdas (Pablo), un
                                            par de variables en la Var_Ram, en Definiciones.h se
                                            agregaron constante para la medición del conversor A/D.
                                            Se cambio en "Trabajo" la medición de Velocidad, la
                                            paso a una Función, tuve que modificarla, no devolvia
                                            el dato de la velocidad, agregue la variable Dato_Velo
                                            para solucionar el problema y cambié la variable del
                                            tiempo para no tener problemas con el uso del Time_Out_G.
                                            Pase las rutinas que tienen que ver con el A/D de las
                                            celdas de carga al archivo Conversor.c
                                            En interrupciones deje la versión que yo hice, la que
                                            estaba no hace bien el ajuste de cuentas.
                                            En el archivo Testeos.c agregue la opción 6 para poder
                                            calibrar los 8 canales del  A/D, se probó el hacer cero,
                                            indica error de cero, es lógico porque el valor de
                                            cuentas es alto.
                                            Modifique el número de versión.

                            Ver. 2.15.00    Con respecto al Conversor A/D
                            17/09/2025		Los tiempos  de acceso estan bien y la lógica de lectura
                                            también encontre que no estan conectadas a tierra 4 patitas
                                            de las referencias, REFAN, REFBN, REFCN, REFDN, es posible
                                            que esto vuelva inestable la medición, el otro tema a revisar
                                            es el de la tensión de referencia y como se corrige lo leido
                                            para aprovechar las 64.000 cuentas
                                            La conección de las 4 patas de referencia, solucionaron el
                                            tema de la inestabilidad en la lectura, ahora se realiza una
                                            buena lectura muy estable, queda por revisar el tema de la
                                            excursión de cuentas 0 --> 3V cuentas 0 --> 32700.
                                            El problema es otro error de hardware, la pata 1 HVSS debería
                                            ir a -5V NO a tierra GND. Por este motivo tenemos la mitad de
                                            la excursión. Para nuestro uso nos podemos manejar igual, pero
                                            desaprovechamos el 50% del  A/D.

                            Ver. 2.20.00	Se agregaron las rutinas de toma y verificacion de taras, las que son accesibles
                            9/10/2025		desde el teclado (funciones de Testeos) y desde el comando de la PC.
                                            Se detecto que el capacitor C158, colocado en la entrada de la referencia del A/D, esta al reves.
                                            Se debe invertir en la placa. Hay una inestabilidad del orden de hasta 5000 cuentas en la medicion,
                                            pero al realizar los promedios queda enmascarado. Se modifico la rutina de lectura del A/D
                                            (en la interrupcion) porque no se cumplian los tiempos requeridos en la hoja de datos,
                                            pero no hubo mejora respecto a la inestabilidad reportada.

                            Ver. 3.00.00	Se verificaron las rutinas de toma de taras desde el comando de la PC.
                            22/12/2025		Se verifico la calibración de las balanzas, se hicieron ajustes, ver los valores
                                            con signo tanto en modo estático como en dinámico.
                                            En modo SORT se modificaron las variables donde se guarda  ls datos de los límites
                                            y los datos de las salidas.
                                            HAY QUE REVISAR LA FORMA DE GUARDAR LOS DATOS EN LA FLASH Y LA CCM.
                                            HAY QUE VERIFICAR LA COMUNICACIÓN CON LA CPU.
                                            HAY QUE REVISAR LOS DATOS DE LAS MEDICIONES DEL A/D.

                            Ver. 4.00.00	Se trabajo sobre la comunicación entre la CPU y la PC, la recepción es por It-Idle,
                            26/01/2026		se recibe por el callback, se coloca un Flag y se re arma la recepción para el próximo
                                            paquete, se agregaron contadores para saber que cantidad de transmisiones/recepciones
                                            se hiciero y para conocer el origen de los errores (OverRun, etc),se realizo una
                                            rutina de recuperación de la comunicación (HAL_UART_ErrorCallback) y colocando un
                                            Jumper en la entrada 8 se pueden visualizar dichos datos.
                                            Se arreglaron las transmisiones de datos del número de placas A/D en la rutina de
                                            calibración y toma de taras, hubo que correr el puntero en 2 byte (el número de la
                                            CPU es igual que el de la placa A/D la diferencia esta en que CPU tiene 7 dígitos y
                                            placa A/D solo 5).
                                            Se arreglo la salida al display de los datos de bruto y peso en modo estático.
                                            Se modifico el Define.h con el número de versión.

                            Ver. 4.50.00	Se trabajo sobre la comunicación entre la CPU y la PC, la recepción es ahora por DMA
                            06/02/2026		se recibe por el callback, se coloca un Flag y se transfieren los datos a un auxiliar
                                                                                        para que sea procesado en el programa principal y no se bloqueen las comunicaciones.

   */

#define PosVariables 0 /*   	0 = Variables propias \
                               1 = Variables externas */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "Variables.h"
#include <stdint.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern void InitTec(void);
extern void Ini_LCD(void);
extern void Clr_LCD(void);
extern void Print_LCD(uint8_t col, uint8_t fila, int8_t *cadena);
extern void Modo_Testeos(void);
extern void Pongo_Hora(int8_t Hora, int8_t renglon);
extern void Inicializo_Rx(int8_t Usart); //* Inicializo todas las variables para la rutina de recepción
/*      Modulo Trabajo y Test Maquina         */
extern void Trabajo(void);            // Modo trabajo
extern void Test(void);               // Modo test
extern void Ini_Serie(int16_t Usart); // Inicializo las series
extern void BKPSRAM_Init(void);       // Inicializo la BackUp SRAM
extern void Init_Variables_Config(void);
extern void Mido_Vbat();
extern void Mido_33V();
extern void Mido_5VD();
extern void Mido_5VA();
extern void Mido_5NVA();

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_uart4_tx;
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart6_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Init_Variables(void);
void RTC_CalendarConfig(void);

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
    {

    /* USER CODE BEGIN 1 */
    uint8_t j;

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
    BKPSRAM_Init(); // enable PWR backup domain access (RTC,BKReg)
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_RTC_Init();
    MX_TIM6_Init();
    MX_UART4_Init();
    MX_USART6_UART_Init();
    MX_USART1_UART_Init();

    /* Initialize interrupts */
    MX_NVIC_Init();
    /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim6); // Habilita el timer 6

    Flg_Display = true; // Habilita el display
    Flag_Errores_Display = false;

    Ini_LCD(); // Inicializa el display LCD
    InitTec(); // Inicializo el teclado
    Clr_LCD(); // Borro el LCD
    Print_LCD(0, 0, (int8_t *)"    Phenix  2025    ");
    Print_LCD(0, 2, (int8_t *)Vers_CPU);
    Print_LCD(0, 1, (int8_t *)"Nodo     Inicio     ");
    Pongo_Hora(true, 3);
    // Verifico la memoria
    Print_LCD(10, 2, (int8_t *)"MEM.     ");
    HAL_GPIO_WritePin(CPU_OK_GPIO_Port, CPU_OK_Pin, SET); // Prendo el led Verde
    Init_Variables();                                     // Inicialización de variables
    Init_Variables_Config();                              // Carga de coeficientes y datos generales de la Flash-CCM-BackUp
    HAL_GPIO_WritePin(CPU_OK_GPIO_Port, CPU_OK_Pin, SET); // Prendo el led Verde

    HAL_GPIO_WritePin(GPIOC, Reset_Pin, GPIO_PIN_SET); // Manda un RESET al conversor A/D por si al darle alimentacion no arranca
    for (j = 0; j < 2; ++j)                            // Hace una demora para que actue el RESET del A/D
        {
        __asm__("nop");
        }
    HAL_GPIO_WritePin(GPIOC, Reset_Pin, GPIO_PIN_RESET);

    Print_LCD(5, 1, (int8_t *)Config.Nodo); // Pongo el Nº de nodo. Debo cargarlo después de inicializar variables

    HAL_GPIO_WritePin(Test_V_OK_GPIO_Port, Test_V_OK_Pin, SET); // Prendo el led Rojo
    Flag_Error = 0;
    Mido_Vbat();
    Mido_33V();
    Mido_5VD();
    switch (Flag_Error) // Si hay error lo muestro
        {
    case 0:
        Print_LCD(10, 2, (int8_t *)"Tens. OK  ");
        break;
    case 1:
        Print_LCD(10, 2, (int8_t *)"Vbat. Mal "); // Agregar parpadeo del led rojo si la medicion da mal
        break;
    case 2:
        Print_LCD(10, 2, (int8_t *)"V3,3V Mal ");
        break;
    case 3:
        Print_LCD(10, 2, (int8_t *)"V 5V  Mal ");
        break;
    case 4:
        Print_LCD(10, 2, (int8_t *)"VA 5V Mal ");
        break;
    case 5:
        Print_LCD(10, 2, (int8_t *)"VA-5V Mal ");
        break;
    default:
        Print_LCD(10, 2, (int8_t *)"Tens. Mal ");
        break;
        }
    HAL_GPIO_WritePin(Test_V_OK_GPIO_Port, Test_V_OK_Pin, RESET); // Apago el led Rojo

    Time_Out_G = 2000; // pongo el Time Out en 2 seg.
    while (Time_Out_G > 0)
        ; // Espero para ver

    Flag_Error = 0;
    HAL_GPIO_WritePin(AD0_OK_GPIO_Port, AD0_OK_Pin, SET); // Prendo el led Ambar
    Mido_5VA();
    Mido_5NVA();
    switch (Flag_Error) // Si hay error lo muestro
        {
    case 0:
        Print_LCD(10, 2, (int8_t *)" A/D   OK ");
        break;
    case 4:
        Print_LCD(10, 2, (int8_t *)"VA 5V Mal ");
        break;
    case 5:
        Print_LCD(10, 2, (int8_t *)"VA-5V Mal ");
        break;
    default:
        Print_LCD(10, 2, (int8_t *)"Tens. Mal ");
        break;
        }
    Time_Out_G = 1500; // pongo el Time Out en 2 seg.
    while (Time_Out_G > 0)
        ; // Espero para ver

    if (HAL_GPIO_ReadPin(VM12_GPIO_Port, VM12_Pin) == 0) // Verifico si tengo tensión de sensores
        Print_LCD(10, 2, (int8_t *)"V.Sen. OK ");        // Tengo tensión
    else
        Print_LCD(10, 2, (int8_t *)"V.Sen. 0V "); // No Hay tensión

    Time_Out_G = 1500; // pongo el Time Out en 2 seg.
    while (Time_Out_G > 0)
        ; // Espero para ver

    Print_LCD(10, 2, (int8_t *)"          ");
    Print_LCD(17, 1, (int8_t *)"FIN");
    Time_Out_G = 1500; // pongo el Time Out en 2 seg.
    while (Time_Out_G > 0)
        ; // Espero para ver

    /* Inicializo las series después de levantar los datos de la Flash */
    Ini_Serie(Comu_PC); // Inicializo la serie PC
    Ini_Serie(Comu_PSalidas);
    Ini_Serie(Comu_ESP);
    // Inicializo la recepción por DMA+IDLE con la PC
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuffer_PC_Rx, Buffer_Rx);
    if (huart1.hdmarx != NULL)
        {
        __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
        }
    Flag_Tara_Inicial = false; // pongo el flag para hacer la tara cuando entro 1º vez

    /**
     * 	Para ingresar al Modo TESTEOS, o sea trabajar con teclado y display DEBEN estar colocados ambos JUMPER en J29
     *        J29
     * 	*--*  7-8 - Pruebas
     * 	*--*  5-6 - Display
     * 	*  *  3-4 - SIN USO
     * 	*--*  1-2 - Errores de comunicación
     *
     */
    if (HAL_GPIO_ReadPin(C_S_Display_GPIO_Port, C_S_Display_Pin) != 0) // Miro el jumper de display
        Flg_Display = false;                                           // Desahabilito el display
    else                                                               // Si tengo display y esta el jumper, habilito el modo testeo
        {
        if (HAL_GPIO_ReadPin(Pruebas_GPIO_Port, Pruebas_Pin) == 0) // Verifico el jumper de testeo
            {
            HAL_GPIO_WritePin(Modo_Test_GPIO_Port, Modo_Test_Pin, SET);
            Modo_Testeos(); // Testeo manual de las tensiones, teclado, reloj, etc.
            HAL_GPIO_WritePin(Modo_Test_GPIO_Port, Modo_Test_Pin, RESET);
            }
        }

    Flag_Primer_ingreso = true; //  Aviso que vengo de la inicialización
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
        {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        while (1 == 1) /* NO salgo nunca */
            {
            Trabajo(); /* Voy a modo Trabajo */
            Test();    /* Voy a modo Test */
            }
        }
    /* USER CODE END 3 */
    }

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
    {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 10;
    RCC_OscInitStruct.PLL.PLLN = 210;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        {
        Error_Handler();
        }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
        {
        Error_Handler();
        }
    }

/**
 * @brief NVIC Configuration.
 * @retval None
 */
static void MX_NVIC_Init(void)
    {
    /* EXTI0_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    /* EXTI1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
    /* EXTI2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    /* EXTI15_10_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    /* TIM6_DAC_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    /* USART1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    /* DMA2_Stream2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
    /* DMA2_Stream7_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
    /* USART6_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(USART6_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
    /* DMA2_Stream6_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
    /* UART4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(UART4_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
    /* DMA1_Stream2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
    /* DMA1_Stream4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
    /* ADC_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(ADC_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
    }

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
void MX_ADC1_Init(void)
    {

    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */

    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
     */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
        {
        Error_Handler();
        }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_12;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
        Error_Handler();
        }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
    }

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
void MX_RTC_Init(void)
    {

    /* USER CODE BEGIN RTC_Init 0 */
    //	HAL_PWR_EnableBkUpAccess();				//enable PWR backup domain access (RTC,BKReg)
    // 1. Habilitar el reloj para el Power Control (PWR)
    // __HAL_RCC_PWR_CLK_ENABLE() - ya se llamó en SystemClock_Config()

    // 2. Habilitar el acceso al dominio de respaldo (Backup Domain)
    // Esto es crucial para poder acceder y modificar los registros del RTC.
    // HAL_PWR_EnableBkUpAccess() - ya se llamó despues de SystemClock_Config()
    // Si no usas BKPSRAM, descomentar la siguiente línea:
    // HAL_PWR_EnableBkUpAccess();

    // El reloj para el RTC (LSE/LSI) y el periférico RTC_CLK_ENABLE()
    // son configurados por la función MX_RTC_Init() generada por CubeIDE,
    // que es llamada desde main().
    // Aquí solo nos aseguramos de que el acceso al dominio de respaldo esté habilitado.

    /* USER CODE END RTC_Init 0 */

    // RTC_TimeTypeDef sTime = {0};
    // RTC_DateTypeDef sDate = {0};

    /* USER CODE BEGIN RTC_Init 1 */

    /* USER CODE END RTC_Init 1 */

    /** Initialize RTC Only
     */
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv = 255;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
        {
        Error_Handler();
        }

    /* USER CODE BEGIN Check_RTC_BKUP */

    /* USER CODE BEGIN RTC_Init 2 */
    /*##-2- Check if Data stored in BackUp register1: No Need to reconfigure RTC#*/
    /* Read the Back Up Register 1 Data */
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != NumeroMagico)
        {
        /* Configure RTC Calendar */
        RTC_CalendarConfig();
        }
    else
        {
        /* Check if the Power On Reset flag is set */
        if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
            {
            /* Turn on LED2: Power on reset occurred */
            }
        /* Check if Pin Reset flag is set */
        if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
            {
            /* Turn on LED1: External reset occurred */
            }
        /* Clear source Reset Flag */
        __HAL_RCC_CLEAR_RESET_FLAGS();
        }
    /* USER CODE END Check_RTC_BKUP */
    }

/**
 * @brief TIM6 Initialization Function
 * @param None
 * @retval None
 */
void MX_TIM6_Init(void)
    {

    /* USER CODE BEGIN TIM6_Init 0 */
    /* Frecuencia de CLK = 84MHz (APB1)
     * Tiempo de interrupción 1 mseg.
     * Preescaler 	= 839		(de 0 a 839 -->  84Mhz % 840  = 100Khz)
     * Periodo      = 100	    (de 0 a  99 --> 100Khz % 100  =   1KHz)
     * Tiempo entre interrupciones ~1 mseg.
     *
     */

    /* USER CODE END TIM6_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM6_Init 1 */

    /* USER CODE END TIM6_Init 1 */
    htim6.Instance = TIM6;
    htim6.Init.Prescaler = 840;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.Period = 99;
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
        {
        Error_Handler();
        }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
        {
        Error_Handler();
        }
    /* USER CODE BEGIN TIM6_Init 2 */

    /* USER CODE END TIM6_Init 2 */
    }

/**
 * @brief UART4 Initialization Function
 * @param None
 * @retval None
 */
void MX_UART4_Init(void)
    {

    /* USER CODE BEGIN UART4_Init 0 */

    /* USER CODE END UART4_Init 0 */

    /* USER CODE BEGIN UART4_Init 1 */
    /**
         UART4 = comunicacion con modulo ESP32
     */
    /* USER CODE END UART4_Init 1 */
    huart4.Instance = UART4;
    huart4.Init.BaudRate = 115200;
    huart4.Init.WordLength = UART_WORDLENGTH_8B;
    huart4.Init.StopBits = UART_STOPBITS_1;
    huart4.Init.Parity = UART_PARITY_NONE;
    huart4.Init.Mode = UART_MODE_TX_RX;
    huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart4.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart4) != HAL_OK)
        {
        Error_Handler();
        }
    /* USER CODE BEGIN UART4_Init 2 */

    /* USER CODE END UART4_Init 2 */
    }

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
void MX_USART1_UART_Init(void)
    {

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */
    /**
         USART1 = comunicacion con la PC
     */
    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 19200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
        {
        Error_Handler();
        }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */
    }

/**
 * @brief USART6 Initialization Function
 * @param None
 * @retval None
 */
void MX_USART6_UART_Init(void)
    {

    /* USER CODE BEGIN USART6_Init 0 */

    /* USER CODE END USART6_Init 0 */

    /* USER CODE BEGIN USART6_Init 1 */
    /**
         USART6 = comunicacion con las placas de salida
     */

    /* USER CODE END USART6_Init 1 */
    huart6.Instance = USART6;
    huart6.Init.BaudRate = 9600;
    huart6.Init.WordLength = UART_WORDLENGTH_8B;
    huart6.Init.StopBits = UART_STOPBITS_1;
    huart6.Init.Parity = UART_PARITY_NONE;
    huart6.Init.Mode = UART_MODE_TX_RX;
    huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart6.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart6) != HAL_OK)
        {
        Error_Handler();
        }
    /* USER CODE BEGIN USART6_Init 2 */

    /* USER CODE END USART6_Init 2 */
    }

/**
 * Enable DMA controller clock
 */
void MX_DMA_Init(void)
    {

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
    }

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
void MX_GPIO_Init(void)
    {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* USER CODE BEGIN MX_GPIO_Init_1 */

    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, CONVST_Pin | Reset_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, CS_Pin | RD_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, Fila_0_Pin | Fila_1_Pin | Fila_2_Pin | Fila_3_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, Dir_485_PSalida_Pin | Dir_485_PC_Pin | AD0_OK_Pin | Test_V_OK_Pin | Modo_Test_Pin | Modo_Trabajo_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, DD0_Pin | DD1_Pin | DD2_Pin | DD3_Pin | DD4_Pin | DD5_Pin | DD6_Pin | DD7_Pin | LCD_RS_Pin | LCD_WR_Pin | LCD_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, Buzzer_Pin | Error_Pin | CPU_OK_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : D2_Pin D3_Pin D4_Pin D5_Pin
                             D6_Pin D7_Pin D8_Pin D9_Pin
                             D10_Pin D11_Pin D12_Pin D13_Pin
                             D14_Pin D15_Pin D0_Pin D1_Pin */
    GPIO_InitStruct.Pin = D2_Pin | D3_Pin | D4_Pin | D5_Pin | D6_Pin | D7_Pin | D8_Pin | D9_Pin | D10_Pin | D11_Pin | D12_Pin | D13_Pin | D14_Pin | D15_Pin | D0_Pin | D1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : Input6_Pin VM12_Pin */
    GPIO_InitStruct.Pin = Input6_Pin | VM12_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PH0 PH1 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    /*Configure GPIO pin : CONVST_Pin */
    GPIO_InitStruct.Pin = CONVST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(CONVST_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : CS_Pin RD_Pin */
    GPIO_InitStruct.Pin = CS_Pin | RD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : Fila_0_Pin Fila_1_Pin Fila_2_Pin Fila_3_Pin */
    GPIO_InitStruct.Pin = Fila_0_Pin | Fila_1_Pin | Fila_2_Pin | Fila_3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : Columna_0_Pin Columna_1_Pin Columna_2_Pin Columna_3_Pin
                             Columna_4_Pin */
    GPIO_InitStruct.Pin = Columna_0_Pin | Columna_1_Pin | Columna_2_Pin | Columna_3_Pin | Columna_4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : Plato_1_Pin SinPlato_Pin SinFino_Pin */
    GPIO_InitStruct.Pin = Plato_1_Pin | SinPlato_Pin | SinFino_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : Pruebas_Pin C_S_Display_Pin Input7_Pin Input8_Pin */
    GPIO_InitStruct.Pin = Pruebas_Pin | C_S_Display_Pin | Input7_Pin | Input8_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : Dir_485_PSalida_Pin Dir_485_PC_Pin */
    GPIO_InitStruct.Pin = Dir_485_PSalida_Pin | Dir_485_PC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : DD0_Pin DD1_Pin DD2_Pin DD3_Pin
                             DD4_Pin DD5_Pin DD6_Pin DD7_Pin
                             LCD_RS_Pin LCD_WR_Pin LCD_EN_Pin */
    GPIO_InitStruct.Pin = DD0_Pin | DD1_Pin | DD2_Pin | DD3_Pin | DD4_Pin | DD5_Pin | DD6_Pin | DD7_Pin | LCD_RS_Pin | LCD_WR_Pin | LCD_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : PC9 */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : Buzzer_Pin Error_Pin CPU_OK_Pin */
    GPIO_InitStruct.Pin = Buzzer_Pin | Error_Pin | CPU_OK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : Busy_Int_Pin */
    GPIO_InitStruct.Pin = Busy_Int_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(Busy_Int_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : Reset_Pin */
    GPIO_InitStruct.Pin = Reset_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(Reset_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : Input1_Pin Input2_Pin Input3_Pin Input4_Pin
                             Input5_Pin */
    GPIO_InitStruct.Pin = Input1_Pin | Input2_Pin | Input3_Pin | Input4_Pin | Input5_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : AD0_OK_Pin Test_V_OK_Pin Modo_Test_Pin Modo_Trabajo_Pin */
    GPIO_InitStruct.Pin = AD0_OK_Pin | Test_V_OK_Pin | Modo_Test_Pin | Modo_Trabajo_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */

    /* USER CODE END MX_GPIO_Init_2 */
    }

/* USER CODE BEGIN 4 */
/*---------------------------------------------------------------------------*/
/* Rutina   	: RTC_CalendarConfig
*  Objetivo 	: Inicialización del reloj del sistema
*  Entrada  	:
*  Descripcion 	: Se inicializa   el reloj con los datos Originales si está mal
*  				  el Número Mágico
*
-----------------------------------------------------------------------------*/
void RTC_CalendarConfig(void)
    {
    /*##-1- Configure the Time #################################################*/
    /* Set Time: 10:20:00 */
    sTime.Hours = 0x16;
    sTime.Minutes = 0x0;
    sTime.Seconds = 0x0;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
        {
        Error_Handler();
        }

    /*##-2- Configure the Date #################################################*/
    /* Set Date: Saturday June 25th 2022 */
    sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
    sDate.Month = RTC_MONTH_DECEMBER;
    sDate.Date = 0x17;
    sDate.Year = 0x25;

    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
        {
        Error_Handler();
        }

    /*##-3- Writes a data in a RTC Backup data Register1 #######################*/
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, NumeroMagico);
    }

/*---------------------------------------------------------------------------*/
/* Rutina   	: Init_Variables
*  Objetivo 	: Inicialización de las variables de RAM del sistema
*  Entrada  	:
*  Descripcion 	:
*
-----------------------------------------------------------------------------*/
void Init_Variables(void)
    {
    int8_t x;

    Time_Clock = 0;      // Timer para poner la hora
    Time_Buzzer = 0;     // Timer del Buzzer
    Time_Maquina = 2000; // Timer para varificar si la máquina esta funcionando
    Time_Out_Velo = 0;

    Cont_AD = 0;               // contador para promedios de lectura del A/D
    Channel_AD = 0;            // contador del AD para leer el teclado o la velocidad.
    Velocidad_medida = 0;      // Velocidad de la maquina, Cantidad de frutos por seg.
    Conta_Velo = 0;            // Contador tiempo para calcular la velocidad en mseg.
    Conta_Sincro = 0;          // Contador de los pulsos de sincronismo
    Conta_Ejes = 0;            // Contador de platos o ejes
    Cant_Ejes_Med = 0;         // Cantidad de Ejes medidos
    Cant_Pulsos_Med = 0;       // Cantidad de Pulsos recibidos del encoder
    Flag_Osciloscopio = false; // Modo osciloscopio apagado al iniciar
    Flag_Tara_OK = false;      // Indicador de taras NO realizadas => no se puede pasar a modo trabajo
    Flag_Maq_Enable = false;

    for (x = 0; x < 8; x++)
        {
        PromAD[x] = 0;  // Valor leido del A/D promediado
        TotalAD[x] = 0; // Valor Totalizado del A/D para promediar
        }

    for (x = 0; x < 8; x++) // Inicializa el sector de variables de las Taras de los platillos
        {
        for (Cant_Ejes_Med = 0; Cant_Ejes_Med < 500; Cant_Ejes_Med++)
            {
            Tara[x][Cant_Ejes_Med] = Cero_CH_CCM[x]; // Inicializa el sector de variables de las Taras de los platillos
            }
        }

    State = false; // Flag para medir velocidad
    Flag_Pruebas = false;
    Flag_Maq_Fun = false;        // Si Flag_Maq_Fun = True  --> Maquina en funcionamiento
    Flag_Sinc_eje = false;       // Flag para comenzar a contar pulsos
    Flag_Secuencia = false;      // Flag para saber si la secuencia de pulsos es correcta
    Flag_SecuenEje = false;      // Flag para saber si la secuencia de pulsos es correcta
    Flag_SecuEje1 = false;       // Flag para saber si la secuencia de pulsos es correcta
    Flag_Enable_AD = false;      // Flag para comenzar a Medir con el A/D principal
    Flag_Trabajo = false;        /* si = True modo trabajo  */
    Flag_Primer_ingreso = false; /* si = True se ingreso por primera vez al programa  */
    Flag_Rec_PC = true;          /* Pongo el flag  que permite inicializar el DMA PC */
    Flag_Rec_PSal = true;        /* Pongo el flag  que permite inicializar el DMA Sal */
    Flag_Rec_ECP = true;         /* Pongo el flag  que permite inicializar el DMA ECM */

    /* Inicializo las variables de comunicación */
    Inicializo_Rx(Comu_PC);
    Inicializo_Rx(Comu_PSalidas);
    Inicializo_Rx(Comu_ESP);
    Error_Aux = 0;
    Dato_Error[0] = Null;
    Dato_Error[1] = Null;
    Dato_Error[2] = Null;
    Cant_Envios = 0;
    Cant_Envios_Sal = 0;
    Peso_min = Peso_Minimo;
    UART1_cnt_rx_event = 0;
    UART1_cnt_drop = 0;
    UART1_cnt_err = 0;
    UART1_cnt_tx_start_fail = 0;
    /* Detalle de errores UART1 */
    UART1_err_fe = 0;
    UART1_err_ne = 0;
    UART1_err_pe = 0;
    UART1_err_ore = 0;
    UART1_last_errorcode = 0;
    UART1_last_sr = 0;

    /* Detalle de errores UART1 */
    UART6_cnt_tx_start_fail = 0;
    UART6_cnt_tx_wd_recover = 0;

    /* ===================== PC TX (DMA) - Busy + 1 cola ===================== */
    PC_Tx_Busy = 0;
    PC_Tx_Pending = 0;
    PC_Tx_Pending_Len = 0;
    Comm_Lock = false;
    PSal_Tx_Busy = false;
    PSal_Tx_WD_ms = 0;
    }

/*----------------------------------------------------------------------------------------------------------------------------------*/

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
    {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    //  __disable_irq();
    HAL_GPIO_WritePin(CPU_OK_GPIO_Port, CPU_OK_Pin, RESET);       // Apago el led Verde
    HAL_GPIO_WritePin(Test_V_OK_GPIO_Port, Test_V_OK_Pin, RESET); // Apago el led Rojo
    HAL_GPIO_WritePin(AD0_OK_GPIO_Port, AD0_OK_Pin, RESET);       // Apago el led Ambar
    HAL_GPIO_WritePin(Modo_Test_GPIO_Port, Modo_Test_Pin, RESET);
    Clr_LCD(); /* Borro el LCD */
    Print_LCD(0, 0, (int8_t *)"    Phenix  2025    ");
    Print_LCD(0, 1, (int8_t *)"ERROR  EJECUCION    ");
    Flag_Error = true; // Activo el led de ERROR
    while (1)
        {
        }
    /* USER CODE END Error_Handler_Debug */
    }
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
    {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
    }
#endif /* USE_FULL_ASSERT */
