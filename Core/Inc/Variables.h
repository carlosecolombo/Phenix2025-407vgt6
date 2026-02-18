/*          Declaraciones variables generales  Phenix 2022
    Fecha inicialización	:	28/06/2025
    Fecha actualización 	:	27/01/2026
    Realizado por	    	:	C.E. Colombo
    Compilador utilizado	:	ST - Eclipse IDE
    Proyecto	    		:	Phenix 2025 - 407VGT6
    Archivo		    		: 	Variables.h
    Versión	   	    		:	4.00.00
    Objetivo	    		:	Definiciones generales de las variables
*/

#include "Definiciones.h"
#include "Estructuras_Peri.h"
#include "Estructuras.h"
#include "Var_Ram.h"
#include "Var_Flash.h"

/*------------------------------------------------------------------------
               Variables de Phenix 2022
------------------------------------------------------------------------*/
/* Definiciones generales de las distintas variables Globales */
#if PosVariables != 0 /* Variables externas */
/*      Variables Públicas del Programa       */
extern volatile int16_t Time_Out_485;   /* Timer para retrasa la transmisión  485 */
extern volatile int32_t Time_Out_G;     /* Time out de uso general */
extern volatile int16_t Time_Out_disp;  /* Time out del display */
extern volatile int32_t Time_Clock;     /* Timer para poner la hora */
extern volatile int16_t Time_Buzzer;    /* Timer del Buzzer */
extern volatile int16_t Time_Maquina;   /* Timer de Máquina parada */
extern volatile int16_t Time_Out_Velo;  /* Time Out cálculo velocidad */
extern volatile int16_t Time_Parpadeo;  /* Timer para el parpadeo del los leds */
extern volatile int16_t Time_Medicion;  // Timer para medicion estatica
extern volatile int16_t Time_Out_Flash; // Timer para espera para grabar flash

extern int8_t Flag_Pruebas;
extern int8_t Flag_Datos_Display;
extern int8_t Flag_Errores_Display;
extern volatile int8_t Flag_Error;
extern volatile int16_t Time_Parpa;
extern volatile int8_t Flag_Grabo;
extern volatile int8_t Flag_Rec_PC;   /* flag  que permite inicializar el DMA PC */
extern volatile int8_t Flag_Rec_PSal; /* flag  que permite inicializar el DMA Sal */
extern volatile int8_t Flag_Rec_ECP;  /* flag  que permite inicializar el DMA ECM */
extern int8_t Flg_Write;              // para grabar la BackUp ram
extern int8_t Flg_WriteFl;            // para grabar la flash
extern int8_t Comm_Lock;              // Para bloquear la comunicación cuando se graba en flash
extern int8_t Flag_Tara_Inicial;      // Para realizar la tara cuando arranca la máquina la primera vez
extern int8_t Flag_Parpa;
extern int8_t Flag_Parpa1;
extern int8_t Flag_Errores_Display1;
extern int8_t Flag_Pantalla_Tra;

extern uint8_t Cont_Velo;    // Calculo de la velocidad de la maquina
extern uint8_t Dato_Velo[6]; // Valor de la velocidad en ASCII

/* Variables Públicas del RELOJ */
extern int8_t buf_date[11];
extern int8_t buf_time[9]; /* Datos de fecha y hora */
extern int8_t Day;

/*------------------------------------------------------------------------
                Variables de la comunicación
------------------------------------------------------------------------*/
extern uint8_t Aux_buf_tx[Buffer_Tx];       // Auxiliar del buffer de transmision
extern int8_t Aux_recepcion_R0[4];          // Auxiliar de recepción
extern int8_t Aux_recepcion_R1[4];          // Auxiliar de recepción
extern uint8_t RxBuffer_PC_Rx[Buffer_Rx];   // Recepción por DMA de la PC
extern uint8_t RxBuffer_Sal[Buffer_Rx_Sal]; // Recepción por interrupciones de la placa de Salida
extern uint8_t buf_rx_test[3][3];           // buffer de recepción de error o test Gral. - USART 0/1/2
extern int8_t Nodo_ok;                      // flag para ver si el numero de nodo esta OK
extern uint8_t NodoOrRe[3];                 // Número del nodo origen de los datos recibidos
extern int8_t Comando_recibido;             // Comando recibido a procesar
extern uint16_t Check_sum;                  // Suma de los bytes recibidos
extern uint8_t RxDato1;
extern uint8_t RxDato5;
extern uint8_t RxDato6;
extern int16_t Conta;
extern int16_t Long_string;
extern int16_t Long_string_Sal;
extern volatile int8_t Flag_Rx_DMA_ECP;
extern volatile int8_t Flag_Rx_It_PC;
extern volatile uint8_t PC_FrameShadow[Buffer_Rx];
extern volatile uint8_t PSal_Tx_Busy;
extern volatile uint16_t PSal_Tx_WD_ms;

/* >>> CAMBIO OBLIGATORIO: Size de HAL es uint16_t <<< */
extern volatile uint16_t RxDataLen_Pc;  // Largo del string recibido PC
extern volatile uint16_t RxDataLen_Sal; // Largo del string recibido Salida
extern volatile uint16_t RxDataLen_ECP; // Largo del string recibido ECP

extern uint8_t Veo_Peso[81]; // Buffer para enviar peso y tamaño a la PC en modo Test y Trabajo

/*------------------------------------------------------------------------
               D R I V E R   D E   T E C L A D O
------------------------------------------------------------------------*/
extern uint16_t SalScan;
extern int8_t Buffer_Teclado[MAXCOLA];
extern int16_t Posible_Tecla;
extern int8_t Posible_Columna;
extern int8_t Flg_Posible;
extern int8_t Flg_NoTecla;
extern uint16_t Cnt_posible;
extern uint16_t Cnt_NoTecla;
extern int16_t Ptr_PongoTec;
extern int16_t Ptr_ConsumoTec;
extern int8_t Cnt_Error; // Inicializa el contador de error

/*------------------------------------------------------------------------
    V A R I A B L E S    A / D    I N T E R N O   M I C R O   4 2 9
------------------------------------------------------------------------*/
extern volatile uint16_t Cont_AD;     // contador para promedios de lectura del A/D de las tensiones del 429
extern volatile uint16_t PromAD[9];   // Valor leido del A/D promediado de las tensiones del 429
extern volatile uint32_t TotalAD[9];  // Valor Totalizado del A/D para promediar de las tensiones del 429
extern volatile int8_t Flag_Medicion; // Flag parar saber si termine de leer el AD de las tensiones del 429
extern volatile int16_t Channel_AD;   // Canal del AD

/*------------------------------------------------------------------------
    V A R I A B L E S    G E N E R A L E S   D E   M A Q U I N A
------------------------------------------------------------------------*/
extern int8_t Flg_ErrorFlash;             // Si el Flag == True --> hay Error al escribir la Flash
extern volatile int8_t Flg_Display;       // Si el Flag == True --> hay display LCD y teclado
extern volatile int8_t Flag_Enable_AD;    // Flag para comenzar a Medir con el A/D principal
extern int8_t Flag_Trabajo;               /* si = True modo trabajo  */
extern int8_t Flag_Primer_ingreso;        /* si = True se ingreso por primera vez al programa  */
extern int8_t Flag_IndicesOk;             /* Flag para sort de cola de salidas */
extern int8_t Flag_IndicesOk_E;           /* Flag para sort de cola de etiquetadoras */
extern volatile int8_t Flag_Init_Tara;    // Flag para habilitar toma de taras
extern volatile int8_t Flag_Enable_Taras; // Flag para habilitar la toma de taras desde la PC
extern int8_t Flag_Signo_Neto[8];         // Flag para indicar el signo del peso neto
extern volatile int8_t Flag_Final_Taras;  // Flag para habilitar la toma de taras desde la PC
extern volatile int8_t Flag_Final1_Taras; // Flag para habilitar la toma de taras desde la PC

/*      Variables del modo Sort     */
extern int16_t Puntero_Dato;                        /* puntero al buffer de Trabajo */
extern int16_t N_Transferencia;                     /* Número de orden de transmisión */
extern int16_t Peso_min;                            /* Peso mínimo a comparar */
extern int16_t Val_Salida[MaxLineas][Tot_Valvulas]; /* Salida asignada según el tamano */
extern int16_t Valvula[Tot_Valvulas];               /* Valvulas activas */
extern int16_t Indice[MaxLineas][Salidas];
extern int8_t Flag_General[MaxLineas][Salidas];
extern int16_t Bandeja[MaxLineas][Tot_Valvulas]; /* Salida activa */
extern int16_t Bandeja_Aux;                      /* Salida Aux activa */
extern int16_t Bandeja_Act[Salidas];             /* Salida a activar */
extern int16_t Tot_Frut_Salidas[Salidas];        /* Total de frutas por salida */

/* para uso de la etiquetadora */
extern int16_t Puntero_Dato_E; /* puntero al buffer de Trabajo */
extern int8_t Flag_General_E[MaxLineas][MaxEtiqueta];
extern int16_t Tamano_E[MaxLineas][Max_Tamanos]; /* Salida asignada según el tamano */
extern int16_t Sal_Etiqueta[Tot_Sal_Etiq];       /* Salidas etiquetadoras */
extern int16_t Indice_E[MaxLineas][MaxEtiqueta];

/* para uso de PRUEBAS */
extern int16_t Error_Aux;        /* Errores de comunicación salidas */
extern int8_t Dato_Error[3];     /* Valor del Error de comunicación */
extern uint32_t Cant_Envios;     /* Cantidad de envíos de comunicación  PC */
extern uint32_t Cant_Envios_Sal; /* Cantidad de envíos de comunicación placa salidas*/

/* Detalle de errores UART1 */
extern volatile uint32_t UART1_err_fe;
extern volatile uint32_t UART1_err_ne;
extern volatile uint32_t UART1_err_pe;
extern volatile uint32_t UART1_err_ore;
extern volatile uint32_t UART1_last_errorcode;
extern volatile uint32_t UART1_last_sr;
extern volatile uint32_t UART1_cnt_rx_event;
extern volatile uint32_t UART1_cnt_drop;
extern volatile uint32_t UART1_cnt_err;
extern volatile uint32_t UART1_cnt_tx_start_fail;
extern volatile uint32_t UART1_cnt_tc_timeout;
extern volatile uint32_t UART1_cnt_rx_rearm_fail;
extern volatile uint32_t UART1_cnt_tx_wd_recover;
extern volatile uint32_t UART6_cnt_tx_start_fail;
extern volatile uint32_t UART6_cnt_tx_wd_recover;
extern volatile uint16_t PC_Tx_WD_ms;
extern volatile uint8_t PC_Tx_ForceRecover; /* Pedido de recovery desde watchdog */
extern volatile uint8_t PC_Tx_WD_Expired;
extern volatile uint8_t PSal_Tx_WD_Expired;

/* ===================== PC TX (DMA) - Busy + 1 cola ===================== */
extern volatile uint8_t PC_Tx_Busy;
extern volatile uint8_t PC_Tx_Pending;
extern volatile uint16_t PC_Tx_Pending_Len;

/*      DEFINICION DE LAS ESTRUCTURAS																        */
extern struct Comunicacion_PC Comunica_PC;     // Estructura Comunicación PC
extern struct Comunicacion_PSal Comunica_PSal; // Estructura Comunicación Placas de salida
extern struct Comunicacion_ECP Comunica_ECP;   // Estructura Comunicación ECP8266
extern struct reg_Trabajo M_Trabajo;
extern struct reg_Trabajo Aux_Trabajo; /* estructura auxiliar para transmitir */

/*------------------------------------------------------------------------
                DEFINICION DE LAS TABLAS
------------------------------------------------------------------------*/
extern const uint8_t Mapeo[20];

#else /* Variables propias */

/*      Variables Públicas del Programa       */
volatile int16_t Time_Out_485;
volatile int32_t Time_Out_G;     /* Time out de uso general */
volatile int16_t Time_Out_disp;  /* Time out del display */
volatile int32_t Time_Clock;     /* Timer para poner la hora */
volatile int16_t Time_Buzzer;    /* Timer del Buzzer */
volatile int16_t Time_Maquina;   /* Timer de Máquina parada */
volatile int16_t Time_Out_Velo;  /* Time Out cálculo velocidad  */
volatile int16_t Time_Parpadeo;  /* Timer para el parpadeo del los leds */
volatile int16_t Time_Medicion;  // Timer para medicion estatica
volatile int16_t Time_Out_Flash; // Timer para espera para grabar flash

int8_t Flag_Pruebas;
int8_t Flag_Datos_Display;
int8_t Flag_Errores_Display;
volatile int8_t Flag_Error;
volatile int16_t Time_Parpa;
volatile int8_t Flag_Grabo;
volatile int8_t Flag_Rec_PC;   /* flag  que permite inicializar el DMA PC */
volatile int8_t Flag_Rec_PSal; /* flag  que permite inicializar el DMA Sal */
volatile int8_t Flag_Rec_ECP;  /* flag  que permite inicializar el DMA ECM */
int8_t Flg_Write;              // para grabar la BackUp ram
int8_t Flg_WriteFl;            // para grabar la flash
int8_t Comm_Lock;              // Para bloquear la comunicación cuando se graba en flash
int8_t Flag_Tara_Inicial;      // Para realizar la tara cuando arranca la máquina la primera vez
int8_t Flag_Parpa;
int8_t Flag_Parpa1;
int8_t Flag_Errores_Display1;
int8_t Flag_Pantalla_Tra;

uint8_t Cont_Velo;    // Calculo de la velocidad de la maquina
uint8_t Dato_Velo[6]; // Valor de la velocidad en ASCII

/* Variables Públicas del RELOJ */
int8_t buf_date[11];
int8_t buf_time[9]; /* Datos de fecha y hora */
int8_t Day;

/*------------------------------------------------------------------------
                Variables de la comunicación
------------------------------------------------------------------------*/
uint8_t Aux_buf_tx[Buffer_Tx];       // Auxiliar del buffer de transmision
int8_t Aux_recepcion_R0[4];          // Auxiliar de recepción
int8_t Aux_recepcion_R1[4];          // Auxiliar de recepción
uint8_t RxBuffer_PC_Rx[Buffer_Rx];   // Recepción por DMA de la PC
uint8_t RxBuffer_Sal[Buffer_Rx_Sal]; // Recepción por interrupciones de la placa de Salida
uint8_t buf_rx_test[3][3];           // buffer de recepción de error o test Gral. - USART 0/1/2
int8_t Nodo_ok;                      // flag para ver si el numero de nodo esta OK
uint8_t NodoOrRe[3];                 // Número del nodo origen de los datos recibidos
int8_t Comando_recibido;             // Comando recibido a procesar
uint16_t Check_sum;                  // Suma de los bytes recibidos
uint8_t RxDato1;
uint8_t RxDato5;
uint8_t RxDato6;
int16_t Conta;
int16_t Long_string;
int16_t Long_string_Sal;
volatile int8_t Flag_Rx_DMA_ECP;
volatile int8_t Flag_Rx_It_PC;
volatile uint8_t PC_FrameShadow[Buffer_Rx];
volatile uint8_t PSal_Tx_Busy;
volatile uint16_t PSal_Tx_WD_ms;

/* >>> CAMBIO OBLIGATORIO: Size de HAL es uint16_t <<< */
volatile uint16_t RxDataLen_Pc;  // Largo del string recibido PC
volatile uint16_t RxDataLen_Sal; // Largo del string recibido Salida
volatile uint16_t RxDataLen_ECP; // Largo del string recibido ECP

uint8_t Veo_Peso[81]; // Buffer para enviar peso y tamaño a la PC en modo Test y Trabajo

/*------------------------------------------------------------------------
               D R I V E R   D E   T E C L A D O
------------------------------------------------------------------------*/
uint16_t SalScan;
int8_t Buffer_Teclado[MAXCOLA];
int16_t Posible_Tecla;
int8_t Posible_Columna;
int8_t Flg_Posible;
int8_t Flg_NoTecla;
uint16_t Cnt_posible;
uint16_t Cnt_NoTecla;
int16_t Ptr_PongoTec;
int16_t Ptr_ConsumoTec;
int8_t Cnt_Error; // Inicializa el contador de error

/*------------------------------------------------------------------------
    V A R I A B L E S    A / D    I N T E R N O   M I C R O   4 2 9
------------------------------------------------------------------------*/
volatile uint16_t Cont_AD;     // contador para promedios de lectura del A/D de las tensiones del 429
volatile uint16_t PromAD[9];   // Valor leido del A/D promediado de las tensiones del 429
volatile uint32_t TotalAD[9];  // Valor Totalizado del A/D para promediar de las tensiones del 429
volatile int8_t Flag_Medicion; // Flag parar saber si termine de leer el AD de las tensiones del 429
volatile int16_t Channel_AD;   // Canal del AD

/*------------------------------------------------------------------------
    V A R I A B L E S    G E N E R A L E S   D E   M A Q U I N A
------------------------------------------------------------------------*/
int8_t Flg_ErrorFlash;             // Si el Flag == True --> hay Error al escribir la Flash
volatile int8_t Flg_Display;       // Si el Flag == True --> hay display LCD y teclado
volatile int8_t Flag_Enable_AD;    // Flag para comenzar a Medir con el A/D principal
int8_t Flag_Trabajo;               /* si = True modo trabajo  */
int8_t Flag_Primer_ingreso;        /* si = True se ingreso por primera vez al programa  */
int8_t Flag_IndicesOk;             /* Flag para sort de cola de salidas */
int8_t Flag_IndicesOk_E;           /* Flag para sort de cola de etiquetadoras */
volatile int8_t Flag_Init_Tara;    // Flag para habilitar toma de taras
volatile int8_t Flag_Enable_Taras; // Flag para habilitar la toma de taras desde la PC
int8_t Flag_Signo_Neto[8];         // Flag para indicar el signo del peso neto
volatile int8_t Flag_Final_Taras;  // Flag para habilitar la toma de taras desde la PC
volatile int8_t Flag_Final1_Taras; // Flag para habilitar la toma de taras desde la PC

/*      Variables del modo Sort     */
int16_t Puntero_Dato;                        /* puntero al buffer de Trabajo */
int16_t N_Transferencia;                     /* Número de orden de transmisión */
int16_t Peso_min;                            /* Peso mínimo a comparar */
int16_t Val_Salida[MaxLineas][Tot_Valvulas]; /* Salida asignada según el tamano */
int16_t Valvula[Tot_Valvulas];               /* Valvulas activas */
int16_t Indice[MaxLineas][Salidas];
int8_t Flag_General[MaxLineas][Salidas];
int16_t Bandeja[MaxLineas][Tot_Valvulas]; /* Salida activa */
int16_t Bandeja_Aux;                      /* Salida Aux activa */
int16_t Bandeja_Act[Salidas];             /* Salida a activar */
int16_t Tot_Frut_Salidas[Salidas];        /* Total de frutas por salida */

/* para uso de la etiquetadora */
int16_t Puntero_Dato_E; /* puntero al buffer de Trabajo */
int8_t Flag_General_E[MaxLineas][MaxEtiqueta];
int16_t Tamano_E[MaxLineas][Max_Tamanos]; /* Salida asignada según el tamano */
int16_t Sal_Etiqueta[Tot_Sal_Etiq];       /* Salidas etiquetadoras */
int16_t Indice_E[MaxLineas][MaxEtiqueta];

/* para uso de PRUEBAS */
int16_t Error_Aux;        /* Errores de comunicación salidas */
int8_t Dato_Error[3];     /* Valor del Error de comunicación */
uint32_t Cant_Envios;     /* Cantidad de envíos de comunicación  PC */
uint32_t Cant_Envios_Sal; /* Cantidad de envíos de comunicación placa salidas*/

/* Detalle de errores UART1 / UART6 */
volatile uint32_t UART1_err_fe;
volatile uint32_t UART1_err_ne;
volatile uint32_t UART1_err_pe;
volatile uint32_t UART1_err_ore;
volatile uint32_t UART1_last_errorcode;
volatile uint32_t UART1_last_sr;
volatile uint32_t UART1_cnt_rx_event;
volatile uint32_t UART1_cnt_drop;
volatile uint32_t UART1_cnt_err;
volatile uint32_t UART1_cnt_tx_start_fail;
volatile uint32_t UART1_cnt_tc_timeout;
volatile uint32_t UART1_cnt_rx_rearm_fail;
volatile uint32_t UART1_cnt_tx_wd_recover;
volatile uint32_t UART6_cnt_tx_start_fail;
volatile uint32_t UART6_cnt_tx_wd_recover;
volatile uint16_t PC_Tx_WD_ms;
volatile uint8_t PC_Tx_ForceRecover; /* Pedido de recovery desde watchdog */
volatile uint8_t PC_Tx_WD_Expired;
volatile uint8_t PSal_Tx_WD_Expired;

/* ===================== PC TX (DMA) - Busy + 1 cola ===================== */
volatile uint8_t PC_Tx_Busy;
volatile uint8_t PC_Tx_Pending;
volatile uint16_t PC_Tx_Pending_Len;

/*      DEFINICION DE LAS ESTRUCTURAS	 */

struct Comunicacion_PC Comunica_PC;     // Estructura Comunicación PC
struct Comunicacion_PSal Comunica_PSal; // Estructura Comunicación Placas de salida
struct Comunicacion_ECP Comunica_ECP;   // Estructura Comunicación ECP8266
struct reg_Trabajo M_Trabajo;           /* 1218 bytes largo de la estructura */
struct reg_Trabajo Aux_Trabajo;         /* estructura auxiliar para transmitir */

/*------------------------------------------------------------------------
                DEFINICION DE LAS TABLAS
------------------------------------------------------------------------*/
const uint8_t Mapeo[20] = {15, 0, 12, 19, 7, 8, 9, 24, 4, 5,
                           6, 23, 1, 2, 3, 13, 25, 26, 61, 30};

#endif
