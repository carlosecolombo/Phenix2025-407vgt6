/*---------------------------------------------------------------------------
          D R I V E R S         C O M U N I C A C I O N E S
---------------------------------------------------------------------------*/
/*	Fecha inicialización	:	27/05/2022
	Fecha actualización 	:	22/12/2025
	Realizado por			:	C.E. Colombo
	Compilador utilizado	:	ST - Eclipse IDE
	Proyecto	    		:	Phenix 2025
	Versión					:	3.00
	Objetivo				:	Rutinas de manejo de las comunicaciones
*/

#define  PosVariables   1    	/*  0 = Definicion de Variables
                              	    1 = Variables externas */
#include "main.h"
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Variables.h"

//##########################################################################################################################
// Invocacion de funciones externas

extern uint8_t  Teclado(void);																// Devolver el codigo de una tecla valida si fue presionada alguna
extern int8_t * str_time (int8_t * Ptr_time);												// Carga de la Hora
extern int8_t * str_date (int8_t * Ptr_date);												// Carga de la Fecha
extern int16_t 	mi_atoi(int8_t * s, int8_t Largo);
extern void 	Error_Handler(void);
extern void   	Print_LCD(uint8_t col,uint8_t fila,int8_t *cadena);
extern void     Write_Ram_BK(void);															// Guardo los datos de la ram en la back SRAM
extern void   	Clr_LCD(void);


//##########################################################################################################################
// Declaracion de funciones de este modulo

void Ini_Serie(int16_t Usart);														/* Inicializo las series */
void Inicializo_Tx(int8_t Usart);
void Inicializo_Rx(int8_t Usart);													/* Inicializo todas las variables para la rutina de recepción */
void Proc_Rx_PC(uint16_t LenRx);
void Tx_char(uint8_t tx_ch, int8_t Usart);  										/* Transmito un caracter */
void Armo_Cabecera(int8_t Usart);
void TX_Ack(int8_t Usart);
void TX_Can(int8_t Usart);
void TX_Nak(int8_t Usart);
void Ver_Cpu(void);
void Transmito_Ley1(void);
void Transmito_string(int16_t Usart, int8_t Flag_Check_Sun);
void Clr_Buffer(uint8_t *Buffer,uint8_t longitud);
void StartReceivingUART_DMA_PC(void);
void StartReceivingUART_DMA_Sal(void);
void Error_Handler_UART(void);
void Transfiero_Salidas(void);  													/* Mando datos a las placas de salida */
void Asig_Salidas(void);        													/* Asigno las salidas */
void Asig_Etiqueta(void);       													/* Asigno los tamanios a las etiquetadoras */
void Proc_rx_sal(uint8_t LenRx);
void Restart_UART_Reception(void);

/* ===================== PC TX (DMA) - Busy + 1 cola ===================== */
static volatile uint8_t  PC_Tx_Busy = 0;
static volatile uint8_t  PC_Tx_Pending = 0;
static volatile uint16_t PC_Tx_Pending_Len = 0;

static uint8_t PC_TX_Send_DMA(uint8_t *buf, uint16_t len);

/*---------------------------------------------------------------------------*/
/* Rutina   	: Ini_Serie
*  Objetivo 	: Inicialización de las tres series del sistema
*  Entrada  	: UART a utilizar
*  Descripcion 	:
*
-----------------------------------------------------------------------------*/
void Ini_Serie(int16_t Usart)
	{
	uint32_t Paridad;
	uint32_t BaudRate;

	// Configure USART in UART mode.
 	switch(Comu_Conf[Usart].paridad)
		{
		case 0:
			Paridad = UART_PARITY_NONE;
         	break;
		case 1:
			Paridad = UART_PARITY_EVEN;
			break;
		default:
			Paridad = UART_PARITY_ODD;
		break;
		}

	// Definicion de la velocidad de comunicacion
		switch(Comu_Conf[Usart].velocidad)
			{
	  		case 4:
				BaudRate = 115200;
				break;
	  		case 3:
				BaudRate = 57600;
				break;
		 	case 2:
				BaudRate = 38400;
		    	break;
			case 1:
				BaudRate = 19200;
		    	break;
			default:
				BaudRate = 9600;
				break;
			}

	switch(Usart)
		{
		case 0:							// PC
			  huart1.Instance = USART1;
			  huart1.Init.BaudRate = BaudRate;
			  huart1.Init.WordLength = Comu_Conf[1].bits_data;
			  huart1.Init.StopBits = UART_STOPBITS_1;
			  huart1.Init.Parity = Paridad;
			  huart1.Init.Mode = UART_MODE_TX_RX;
			  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
			  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
			  if (HAL_UART_Init(&huart1) != HAL_OK)
			  {
			    Error_Handler();
			  }
			break;
		case 1:							// Placas de Salida
			  huart6.Instance = USART6;
			  huart6.Init.BaudRate = BaudRate;
			  huart6.Init.WordLength = Comu_Conf[2].bits_data;
			  huart6.Init.StopBits = UART_STOPBITS_1;
			  huart6.Init.Parity = Paridad;
			  huart6.Init.Mode = UART_MODE_TX_RX;
			  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
			  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
			  if (HAL_UART_Init(&huart6) != HAL_OK)
			  {
			    Error_Handler();
			  }
			 			break;
		case 2:							// ESP8266
			  huart4.Instance = UART4;
			  huart4.Init.BaudRate = BaudRate;
			  huart4.Init.WordLength = Comu_Conf[1].bits_data;
			  huart4.Init.StopBits = UART_STOPBITS_1;
			  huart4.Init.Parity = Paridad;
			  huart4.Init.Mode = UART_MODE_TX_RX;
			  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
			  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
			  if (HAL_UART_Init(&huart4) != HAL_OK)
			  {
			    Error_Handler();
			  }
			break;
		}
	}

/*---------------------------------------------------------------------------*/
/* Rutina   	: Inicializo_Tx
*  Objetivo 	: Inicializar todas las variables de la rutina de Transmisión
*  Entrada  	: UART a utilizar
*  Descripcion 	:
*
-----------------------------------------------------------------------------*/
void Inicializo_Tx(int8_t Usart)
	{
    switch(Usart)
		{
    	case Comu_PC:
    		Clr_Buffer((uint8_t *) &Comunica_PC.buf_tx_PC, Buffer_Tx); 	/* BORRAR EL BUFFER DE TRANSMISION */
    		Comunica_PC.flg_tx = False;           		/* True = transmision OK */
    		Comunica_PC.cant_Tx = 0;              		/* Cant. Bytes transmitidos */
    		Comunica_PC.tot_Tx = 0;               		/* Total Bytes a transmitir */
    		Comunica_PC.errores_txrx = 0;
    		break;

    	case Comu_PSalidas:
    		Clr_Buffer((uint8_t *) &Comunica_PSal.buf_tx_Sal, Buffer_Tx_Sal); /* BORRAR EL BUFFER DE TRANSMISION */
    		Comunica_PSal.flg_tx = False;           	/* True = transmision OK */
    		Comunica_PSal.cant_Tx = 0;              	/* Cant. Bytes transmitidos */
    		Comunica_PSal.tot_Tx = 0;               	/* Total Bytes a transmitir */
    		Comunica_PSal.errores_txrx = 0;
    		break;
    	case Comu_ESP:
    		Clr_Buffer((uint8_t *) &Comunica_ECP.buf_tx_ECP, Buffer_Tx); /* BORRAR EL BUFFER DE TRANSMISION */
    		Comunica_ECP.flg_tx = False;           		/* True = transmision OK */
    		Comunica_ECP.cant_Tx = 0;					/* pongo a cero el contador de caracteres de transmicion */
    		Comunica_ECP.tot_Tx = 0;               		/* Total Bytes a transmitir */
    		Comunica_ECP.errores_txrx = 0;				/* borro los posibles errores */
    		break;
		}
	}
/*---------------------------------------------------------------------------*/
/* Rutina   	: Inicializo_Rx
*  Objetivo 	: Inicializar todas las variables de la rutina de recepción de la PC
*  Entrada  	: UART a utilizar
*  Descripcion 	:
*
-----------------------------------------------------------------------------*/
void Inicializo_Rx(int8_t Usart)
	{
    switch(Usart)
		{
    	case Comu_PC:
    		Clr_Buffer((uint8_t *) &Comunica_PC.buf_rx_PC, Buffer_Rx); 		/* BORRAR EL BUFFER DE RECEPCION */
    		Comunica_PC.flg_rx = False;           	/* True = recepcion OK 	*/
    		Comunica_PC.cant_Rx = 0;              	/* Cant. Bytes recibido	*/
    		Comunica_PC.tot_Rx = 0;               	/* Total Bytes a recibir */
    		Comunica_PC.errores_txrx = 0;
    		break;
    	case Comu_PSalidas:
    		Clr_Buffer((uint8_t *)RxBuffer_Sal, Buffer_Rx_Sal);				 //  Borro el buffer de recepción
    		Clr_Buffer((uint8_t *) &Comunica_PSal.buf_rx_Sal, Buffer_Rx_Sal); /* BORRAR EL BUFFER DE RECEPCION */
    		Comunica_PSal.flg_rx = False;           /* True = recepcion OK   */
    		Comunica_PSal.cant_Rx = 0;              /* Cant. Bytes recibidos */
    		Comunica_PSal.tot_Rx = 0;               /* Total Bytes a recibir */
    		Comunica_PSal.errores_txrx = 0;
    	 	break;
    	case Comu_ESP:
    		Clr_Buffer((uint8_t *) &Comunica_ECP.buf_rx_ECP, Buffer_Rx); 		/* BORRAR EL BUFFER DE RECEPCION */
    		Comunica_ECP.flg_rx = False;           	/* True = recepcion OK   */
    		Comunica_ECP.cant_Rx = 0;				/* pongo a cero el contador de caracteres de recepcion */
    		Comunica_ECP.tot_Rx = 0;               	/* Total Bytes a recibir */
    		Comunica_ECP.errores_txrx = 0;			/* borro los posibles errores */
    		break;
		}
	}

/*---------------------------------------------------------------------------*/
/* Rutina   	: Proc_Rx_PC
*  Objetivo 	: Analiza e incrementa los caracteres recibidos
*  Entrada  	:
*  Descripcion 	:
*  		Formato de recepción nuevo.
*			[stx][N# Destino][N# Origen][Long. string][comando][n datos][ck]
*			  0     1  2        3  4		 5  6        7 8    9.....n  n+1
*			La longitud del string se cuenta desde el comando (bit 7) hasta el ck (bit n+1)
*			Los datos están en ASCII
*				Comu_Pc.flg_rx = True  -->  La recepción OK, debe llegar en false.
*				Comu_Pc.busy = True    -->  Se pone cuando se está recibiendo un string
*				Long_string			   -->  indica el largo del string a recibir, se debe
*											inicializar en 1.
*				Comu_Pc.errores_txrx   -->	N# de error de Tx o Rx.
*                							= 0   	Sin errores
*											= 1	Error de largo del string
*											= 5	Error de recepción del string (mal ck)
*			Se recibe siempre el string completo aunque no sea para el nodo en curso, se analiza el
*			resultado en modo Test y modo Trabajo.
*
*/
void Proc_Rx_PC(uint16_t  LenRx)
    {
	int8_t 	dato = 0;										// para barrer la recepción
	uint8_t	Caracter_rx;

	Inicializo_Rx(Comu_PC);
	while(dato < LenRx)
		{

		*(Comunica_PC.buf_rx_PC + dato) = RxBuffer_PC_DMA[dato]; // lo guardo, en la posición que corresponda
		Caracter_rx = RxBuffer_PC_DMA[dato];
		if(dato == 0)
			{
			switch(Caracter_rx)
				{
				case STX:                   				// Recibí un STX */
					Nodo_ok = True;							// borro el flag del numero de nodo
					Check_sum = Caracter_rx;				// comienzo a calcular el Check sum
					break;
				default:
					break;
				}
			}
		else
			{
			switch(dato)									// comparo + 1 porque ya incremente
				{
				case 1:										// levanto el número de nodo Destino
					Aux_recepcion_R0[0] = Caracter_rx;
					break;
				case 2:
					Aux_recepcion_R0[1] = Caracter_rx;
					Aux_recepcion_R0[2] = Null;
					if(strcmp((char *)Aux_recepcion_R0, (char *)Config.Nodo) != 0) // si no es para este nodo
						Nodo_ok = False;					// error no es el n# de nodo
					break;
				case 3:										// evanto el número de nodo Origen
					NodoOrRe[0] = Caracter_rx;
					break;
				case 4:
					NodoOrRe[1] = Caracter_rx;
					NodoOrRe[2] = Null;
					break;
				case 5:										// Levanto la longitud del string
					Aux_recepcion_R0[0] = Caracter_rx;
					break;
				case 6:
					Aux_recepcion_R0[1]=Caracter_rx;
					Aux_recepcion_R0[2]=Null;
					Long_string = mi_atoi(Aux_recepcion_R0, 2);
					break;
				case 7:										// Levanto el comando a realizar
					Aux_recepcion_R0[0] = Caracter_rx;
					break;
				case 8:
					Aux_recepcion_R0[1] = Caracter_rx;
					Aux_recepcion_R0[2] = Null;
					if(Nodo_ok == True)
						Comando_recibido= mi_atoi(Aux_recepcion_R0, 2);

					else
						{
						Comando_recibido = No_nodo;			// Comando --> No es para este nodo
						Comunica_PC.flg_rx = true;			// flag de recepción ==> OK
						NodoOrRe[0]='9';					// pongo un nodo que no existe
						NodoOrRe[1]='9';
						}
					break;
				default:
					break;
				}               							// cierro el switch Caracter_rx */
					/* Sigo recibiendo hasta el largo previsto */
			if((dato - 6) >= Long_string)
				{
					/* verifico si el Ceck sum es correcto	*/
				if((Check_sum & 0x00FF) != Caracter_rx )
					{
					Comunica_PC.errores_txrx = 5; 			// error de checksum  --  Recepción
					}
				else
					{
					Comunica_PC.flg_rx = True;				// flag de recepción ==> terminada OK
					Comunica_PC.cant_Rx = 0;
					}
				}
			else											// Sumo todos los bytes menos el último
				Check_sum = Check_sum + Caracter_rx;		// calculo el Check sum

			}   /* cierro el else */
		dato++;
		}
	}
/*--------------------------------------------------------------------------------------------------------------*/
/* Rutina   	: Tx_char
*  Objetivo 	: Transmito un caracter por demanda
*  Entrada  	: Caracter a transmitir y UART a utilizar
*  Descripcion :Esta rutina recibe el caracter a transmitir y espera a que sea
* 				transmitido
*				Retorna Comu_Gral.errores_txrx = xx
*				donde 	xx = 0 --> sin errores
*						xx = 1 --> error time out Tx
*				Incrementa el Comu_Gral.cant_Tx si la transmisión fue OK.
*
*
*/
void Tx_char(uint8_t tx_ch, int8_t Usart)
    {

    switch(Usart)
		{
    	case Comu_PC:
    		if(HAL_UART_Transmit(&huart1, &tx_ch, 1, 10) != HAL_OK)
    			Comunica_PC.errores_txrx = 1; 				/* error de Time out de transmisión */

    		Comunica_PC.cant_Tx++;							/* incremento el contador de bytes transmitidos */
    		break;
    	case Comu_PSalidas:
    		if(HAL_UART_Transmit(&huart6, &tx_ch, 1, 10) != HAL_OK)
    			Comunica_PSal.errores_txrx = 1; 			/* error de Time out de transmisión */

    		Comunica_PSal.cant_Tx++;						/* incremento el contador de bytes transmitidos */
    		break;
    	case Comu_ESP:
    		if(HAL_UART_Transmit(&huart4, &tx_ch, 1, 10) != HAL_OK)
    			Comunica_ECP.errores_txrx = 1; 		/* error de Time out de transmisión */

    	    Comunica_ECP.cant_Tx++;							/* incremento el contador de bytes transmitidos */
    		break;

		}
    }
/*---------------------------------------------------------------------------*/
/* Rutina   	: Armo_Cabecera
*  Objetivo 	: Armo la cabecera para transmitir
*  Entrada  	: UART a utilizar
*  Descripcion 	:
*/
void Armo_Cabecera(int8_t Usart)
    {
    switch(Usart)
		{
    	case Comu_PC:
    		Inicializo_Tx(Comu_PC);
    		*(Comunica_PC.buf_tx_PC) = STX;							/* Cargo el chr de comienzo de Tx (STX) */
    		*(Comunica_PC.buf_tx_PC + 1) = NodoOrRe[0];				/* Nodo destino */
    		*(Comunica_PC.buf_tx_PC + 2) = NodoOrRe[1];				/* Nodo destino */
    		*(Comunica_PC.buf_tx_PC + 3) = Config.Nodo[0];			/* Nodo origen */
    		*(Comunica_PC.buf_tx_PC + 4) = Config.Nodo[1];			/* Nodo origen */
    		break;
    	case Comu_PSalidas:
    		Inicializo_Tx(Comu_PSalidas);
    		*(Comunica_PSal.buf_tx_Sal) = STX;						/* Cargo el chr de comienzo de Tx (STX) */
    		*(Comunica_PSal.buf_tx_Sal + 1) = NodoOrRe[0];			/* Nodo destino */
    		*(Comunica_PSal.buf_tx_Sal + 2) = NodoOrRe[1];			/* Nodo destino */
    		*(Comunica_PSal.buf_tx_Sal + 3) = Config.Nodo[0];		/* Nodo origen */
    		*(Comunica_PSal.buf_tx_Sal + 4) = Config.Nodo[1];		/* Nodo origen */
    		break;
    	case Comu_ESP:
    		Inicializo_Tx(Comu_ESP);
    		*(Comunica_ECP.buf_tx_ECP) = STX;						/* Cargo el chr de comienzo de Tx (STX) */
    		*(Comunica_ECP.buf_tx_ECP + 1) = NodoOrRe[0];			/* Nodo destino */
    		*(Comunica_ECP.buf_tx_ECP + 2) = NodoOrRe[1];			/* Nodo destino */
    		*(Comunica_ECP.buf_tx_ECP + 3) = Config.Nodo[0];		/* Nodo origen */
    		*(Comunica_ECP.buf_tx_ECP + 4) = Config.Nodo[1];		/* Nodo origen */
    		break;
		}
      }
/*---------------------------------------------------------------------------*/
/* Rutina   	: TX_Ack
*  Objetivo 	: Transmito ACK
*  Entrada  	: UART a utilizar
*  Descripcion 	:
*/
void TX_Ack(int8_t Usart)
	{
	Armo_Cabecera(Usart);
    switch(Usart)
		{
    	case Comu_PC:
    		*(Comunica_PC.buf_tx_PC + 5) = '0';				/* Longitud a transmitir o recibir */
    		*(Comunica_PC.buf_tx_PC + 6) = '3';
    		*(Comunica_PC.buf_tx_PC + 7) = '0';				// Comando a transmitir en ASCII = ACK (0x06)
    		*(Comunica_PC.buf_tx_PC + 8) = ACK;
    		*(Comunica_PC.buf_tx_PC + 9) = Null;			/* pongo el terminador */
    		break;
    	case Comu_PSalidas:
    		*(Comunica_PSal.buf_tx_Sal + 5) = '0';			/* Longitud a transmitir o recibir */
    		*(Comunica_PSal.buf_tx_Sal + 6) = '3';
    		*(Comunica_PSal.buf_tx_Sal + 7) = '0';			// Comando a transmitir en ASCII = ACK (0x06)
    		*(Comunica_PSal.buf_tx_Sal + 8) = ACK;
    		*(Comunica_PSal.buf_tx_Sal + 9) = Null;			/* pongo el terminador */
    		break;
    	case Comu_ESP:
    		*(Comunica_ECP.buf_tx_ECP + 5) = '0';			/* Longitud a transmitir o recibir */
    		*(Comunica_ECP.buf_tx_ECP + 6) = '3';
    		*(Comunica_ECP.buf_tx_ECP + 7) = '0';			// Comando a transmitir en ASCII = ACK (0x06)
    		*(Comunica_ECP.buf_tx_ECP + 8) = ACK;
    		*(Comunica_ECP.buf_tx_ECP + 9) = Null;			/* pongo el terminador */
    		break;
		}
    Transmito_string(Usart,true);							/* transmito y calculo en check sum */
	}
/*---------------------------------------------------------------------------*/
/* Rutina   	: TX_Can
*  Objetivo 	: Transmito Can
*  Entrada  	: UART a utilizar
*  Descripcion 	:
*/
void TX_Can(int8_t Usart)
	{
	Armo_Cabecera(Usart);
    switch(Usart)
		{
    	case Comu_PC:
    		*(Comunica_PC.buf_tx_PC + 5) = '0';				/* Longitud a transmitir o recibir */
    		*(Comunica_PC.buf_tx_PC + 6) = '3';
    		*(Comunica_PC.buf_tx_PC + 7) = '0';				// Comando a transmitir en ASCII = CAN (0x18)
    		*(Comunica_PC.buf_tx_PC + 8) = CAN;
    		*(Comunica_PC.buf_tx_PC + 9) = Null;			/* pongo el terminador */
    		break;
    	case Comu_PSalidas:
    		*(Comunica_PSal.buf_tx_Sal + 5) = '0';			/* Longitud a transmitir o recibir */
    		*(Comunica_PSal.buf_tx_Sal + 6) = '3';
    		*(Comunica_PSal.buf_tx_Sal + 7) = '0';			// Comando a transmitir en ASCII = CAN (0x18)
    		*(Comunica_PSal.buf_tx_Sal + 8) = CAN;
    		*(Comunica_PSal.buf_tx_Sal + 9) = Null;			/* pongo el terminador */
    		break;
    	case Comu_ESP:
    		*(Comunica_ECP.buf_tx_ECP + 5) = '0';			/* Longitud a transmitir o recibir */
    		*(Comunica_ECP.buf_tx_ECP + 6) = '3';
    		*(Comunica_ECP.buf_tx_ECP + 7) = '0';			// Comando a transmitir en ASCII = CAN (0x18)
    		*(Comunica_ECP.buf_tx_ECP + 8) = CAN;
    		*(Comunica_ECP.buf_tx_ECP + 9) = Null;			/* pongo el terminador */
    		break;
		}
    Transmito_string(Usart,true);							/* transmito y calculo en check sum */
	}
/*---------------------------------------------------------------------------*/
/* Rutina   	: TX_Nak
*  Objetivo 	: Transmito NAK
*  Entrada  	: UART a utilizar
*  Descripcion 	:
*/
void TX_Nak(int8_t Usart)
	{
	Armo_Cabecera(Usart);
    switch(Usart)
		{
    	case Comu_PC:
    		*(Comunica_PC.buf_tx_PC + 5) = '0';				/* Longitud a transmitir o recibir */
    		*(Comunica_PC.buf_tx_PC + 6) = '3';
    		*(Comunica_PC.buf_tx_PC + 7) = '0';				// Comando a transmitir en ASCII = NACK (0x15)
    		*(Comunica_PC.buf_tx_PC + 8) = NAK;
    		*(Comunica_PC.buf_tx_PC + 9) = Null;			/* pongo el terminador */
    		break;
    	case Comu_PSalidas:
    		*(Comunica_PSal.buf_tx_Sal + 5) = '0';			/* Longitud a transmitir o recibir */
    		*(Comunica_PSal.buf_tx_Sal + 6) = '3';
    		*(Comunica_PSal.buf_tx_Sal + 7) = '0';			// Comando a transmitir en ASCII = NACK (0x15)
    		*(Comunica_PSal.buf_tx_Sal + 8) = NAK;
    		*(Comunica_PSal.buf_tx_Sal + 9) = Null;			/* pongo el terminador */
    		break;
    	case Comu_ESP:
    		*(Comunica_ECP.buf_tx_ECP + 5) = '0';			/* Longitud a transmitir o recibir */
    		*(Comunica_ECP.buf_tx_ECP + 6) = '3';
    		*(Comunica_ECP.buf_tx_ECP + 7) = '0';			// Comando a transmitir en ASCII = NACK (0x15)
    		*(Comunica_ECP.buf_tx_ECP + 8) = NAK;
    		*(Comunica_ECP.buf_tx_ECP + 9) = Null;			/* pongo el terminador */
    		break;
		}
    Transmito_string(Usart,true);							/* transmito y calculo en check sum */
	}

/*---------------------------------------------------------------------------*/
/* Rutina   	: Ver_Cpu
*  Objetivo 	: Realizar tarea de enviar la versión de la CPU
*  Entrada  	:
*  Descripcion 	:
*/
void Ver_Cpu(void)
	{
	Armo_Cabecera(Comu_PC);
	*(Comunica_PC.buf_tx_PC + 5) = '1';						/* Longitud a transmitir o recibir */
	*(Comunica_PC.buf_tx_PC + 6) = '4';
	strcpy((char *)(Comunica_PC.buf_rx_PC + 7), (const char *)Vers_CPU);
    Transmito_string(0,False);								/* transmito y calculo en check sum */
	}
/*---------------------------------------------------------------------------*/
/* Rutina   	: Transmito_string
*  Objetivo 	: Realizar tarea de transmitir
*  Entrada  	: UART a utilizar y Si hago el CheckSum
*  Descripcion 	: Se transmite a la serie que corresponda un string completo
*/
void Transmito_string(int16_t Usart, int8_t Flag_Check_Sun)
	{
	uint8_t j;

#define  DMA   	1   			/*   	0 = transmite a demanda, byte a byte
										1 = transmite por DMA */
     /* Transmito los datos y calculo el Check sum, lo guardo al final */
    Check_sum = 0;
    j=0;
    switch(Usart)
		{
    	case 0:							// Transmito a la PC  por Pooling o DMA
        	HAL_GPIO_WritePin(Dir_485_PC_GPIO_Port, Dir_485_PC_Pin,SET);	/* Pongo la salidas PA10 = 1 (485) */
        	Time_Out_485 = 0;												/* pongo el Time Out en 1 mseg. */
    		while(Time_Out_485 != 0);										/* Espero la 485 */
		 #if DMA == 0
    		uint8_t x = 0;
    		while(*(Comunica_PC.buf_tx_PC + j) != Null)
    			{
    			Tx_char((uint8_t)*(Comunica_PC.buf_tx_PC + j),Usart);
    			Check_sum = Check_sum + *(Comunica_PC.buf_tx_PC + j);   	/* calculo el Check sum */
    			j++;
    			}
    		if(Flag_Check_Sun == true)										// Si es la PC agrego el checksum
    			{
    			Tx_char((uint8_t) Check_sum,Usart);							/* Transmito el Check sum */
    			*(Comunica_PC.buf_tx_PC + j) = Check_sum;					/* Cargo el Check sum en la última posición */
    			x = 1;
    			}
			*(Comunica_PC.buf_tx_PC + j + x) = Null;						/* pongo el terminador */
    		HAL_GPIO_WritePin(Dir_485_PC_GPIO_Port, Dir_485_PC_Pin,RESET);	/* Pongo la salidas PA10 = 0 (485) */
 		#else
    		if(Flag_Check_Sun == true)										// Si es la PC agrego el checksum
    			{
    			while(*(Comunica_PC.buf_tx_PC + j) != Null)
    				{
    				Check_sum = Check_sum + *(Comunica_PC.buf_tx_PC + j);   /* calculo el Check sum */
    				j++;
    				}
    			*(Comunica_PC.buf_tx_PC + j) = Check_sum;					/* Cargo el Check sum en la última posición */
    			*(Comunica_PC.buf_tx_PC + j + 1) = Null;					/* pongo el terminador */
    			Comunica_PC.tot_Tx = j + 1;
    			}

/*   		HAL_UART_Transmit_DMA(&huart1, (uint8_t *)Comunica_PC.buf_tx_PC, Comunica_PC.tot_Tx); */
            PC_TX_Send_DMA((uint8_t *)Comunica_PC.buf_tx_PC, (uint16_t)Comunica_PC.tot_Tx);
 		#endif
   		    break;
    	case 1:							// Transmito a la Placa de salidas por Pooling
    		while(*(Comunica_PSal.buf_tx_Sal + j) != Null)
    			{
    			Tx_char((uint8_t)*(Comunica_PSal.buf_tx_Sal + j),Usart);
    			Check_sum = Check_sum + *Comunica_PSal.buf_tx_Sal + j;   	// calculo el Check sum
    			j++;
    			}
    		if(Flag_Check_Sun == true)										// Si es la PC agrego el checksum
    			{
    			Tx_char((uint8_t) Check_sum,Usart);							// Transmito el Check sum
    			*(Comunica_PSal.buf_tx_Sal + j) = Check_sum;				// Cargo el Check sum en la última posición
    			*(Comunica_PSal.buf_tx_Sal + j + 1) = Null;					// pongo el terminador
    			}
    		break;
    	case 2:							// Transmito al módulo ECP por Pooling
    		while(*(Comunica_ECP.buf_tx_ECP + j) != Null)
    			{
    			Tx_char((uint8_t)*(Comunica_ECP.buf_tx_ECP + j),Usart);
    			Check_sum = Check_sum + *Comunica_ECP.buf_tx_ECP + j;   	// calculo el Check sum
    			j++;
    			}
    		if(Flag_Check_Sun == true)										// Si es la PC agrego el checksum
    			{
    			Tx_char((uint8_t) Check_sum,Usart);							// Transmito el Check sum
    			*(Comunica_ECP.buf_tx_ECP + j) = Check_sum;					// Cargo el Check sum en la última posición
    			*(Comunica_ECP.buf_tx_ECP + j + 1) = Null;					// pongo el terminador
    			}
    		break;
		}
   	}

/****************************************************************/
/* Rutina   	: Clr_Buffer
*  Objetivo 	: poner a cero un buffer determinado
*  Entrada  	: Buffer, longitud
*  Descripcion 	:
*
****************************************************************/
void Clr_Buffer(uint8_t *Buffer,uint8_t longitud)
	{
	uint8_t i;

    for(i = 0; i < longitud; i++)
	    *(Buffer + i) = Null;
	}

/*---------------------------------------------------------------------------*/
/* Rutina   	: Error_Handler_UART
*  Objetivo 	: Manejar los errores  de recepción del DMA
*  Entrada  	:
*  Descripcion 	:
*
-----------------------------------------------------------------------------*/
void Error_Handler_UART(void)
	{
//	_HAL_UART_CLEAR_FLAG(&huart1,HART_FLAG_RXNE);
	}

/**
 * @brief Reinicia la transferencia DMA de UART con detección IDLE.
 * Debe llamarse siempre después de un error, IDLE, o un procesamiento.

 **** NO SE USA NUNCA ?????? ****

 */
void Restart_UART_Reception(void)
{
	UART_HandleTypeDef *huart = &huart1;

    // 1. Limpia cualquier error pendiente que haya detenido la UART.
    // Esto es crucial antes de intentar reiniciar cualquier cosa.
    // La macro limpia los flags ORE, NE, FE y PE en el registro ESR/ICR del F4.
    __HAL_UART_CLEAR_OREFLAG(huart);
    __HAL_UART_CLEAR_NEFLAG(huart);
    __HAL_UART_CLEAR_FEFLAG(huart);
    __HAL_UART_CLEAR_PEFLAG(huart);

    // 2. Limpia la bandera IDLE, necesaria si la recepción terminó por IDLE.
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    // 3. Detiene cualquier transferencia DMA que pudiera estar en curso (por si acaso)
    //HAL_UART_DMAStop(huart);

    // 4. Reinicia la transferencia DMA con detección IDLE
    if (HAL_OK != HAL_UARTEx_ReceiveToIdle_DMA(huart, (uint8_t *)RxBuffer_PC_DMA, Buffer_Rx))
    {
        // Si incluso el reinicio falla (ej. error de configuración de DMA),
        // entramos al manejador de errores crítico.
        Error_Handler_UART();
    }
}
/*---------------------------------------------------------------------------*/
/* Rutina   	: HAL_UARTEx_RxEventCallback
*  Objetivo 	: Recibe la transmisión realizada por la CPU de largo variable
*  Entrada  	:
*  Descripcion 	: Recibe por interrupciones y avisa cuando el paquete se termino de
*  				  recibir o se rebaso el buffer de recepción.
*
-----------------------------------------------------------------------------*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
	{
    if (huart->Instance == huart1.Instance)
        {
        Flag_Rx_DMA_PC = true;     // evento: hay frame recibido
        RxDataLen_PC   = Size;     // largo real recibido (uint16_t)
        }   
/*
	if (huart->Instance == huart1.Instance)
		{
		 Flag_Rx_DMA_PC = true;						// Recibi un string
		 RxDataLen_PC = Size;						// Largo del string recibido
		 Flag_Rec_PC = true;
		}
*/
	else if (huart->Instance == huart6.Instance)	// Recepción de las Placas de salida
		{
		 RxDataLen_Sal = Size;						// Largo del string recibido
		 Proc_rx_sal(RxDataLen_Sal);
	     // Re-arma la recepción (IMPORTANTÍSIMO)
		 HAL_UARTEx_ReceiveToIdle_IT(&huart6, RxBuffer_Sal, Buffer_Rx_Sal);
		}

	else if (huart->Instance == huart4.Instance)
		{
		 Flag_Rx_DMA_ECP = true;					// Recibi un string
		 RxDataLen_ECP = Size;						// Largo del string recibido
		 Flag_Rec_ECP = true;
		 }

	}
/*---------------------------------------------------------------------------*/
/* Rutina   	: StartReceivingUART_DMA_PC      Placa PC  --> CPU
*  Objetivo 	: Inicia la recepción de un paquete UART usando DMA con detección de línea IDLE.
*  Entrada  	:
*  Descripcion 	: Inicia la recepción de un paquete UART usando DMA con detección de línea IDLE.
* 				  Esto prepara el UART para recibir el siguiente paquete de longitud variable.
				  Inicia la recepción DMA en modo "ReceiveToIdle".
				  El 'RxDataLen' aquí es el tamaño TOTAL del buffer DMA.
				  La recepción continuará hasta que se detecte una línea IDLE o el buffer se llene.
 *
-----------------------------------------------------------------------------*/
void StartReceivingUART_DMA_PC(void)
	{
    Flag_Rx_DMA_PC = false;
    RxDataLen_PC   = 0;

    if (HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuffer_PC_DMA, Buffer_Rx) != HAL_OK)
        {
        Clr_LCD();
        Print_LCD(0,0,(int8_t *) "    Phenix  2025    ");
        Print_LCD(0,1,(int8_t *) "   ERROR  DMA  PC   ");
        Error_Handler();
        }

    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);

/*	Flag_Rx_DMA_PC = false;
	Clr_Buffer((uint8_t *)RxBuffer_PC_DMA, Buffer_Rx);		//  Borro el buffer de recepción
	if(Flag_Rec_PC == true)
		{
		Flag_Rec_PC = false;
		if (HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuffer_PC_DMA, Buffer_Rx) != HAL_OK)
			{
			Clr_LCD();													 Borro el LCD 
			Print_LCD(0,0,(int8_t *) "    Phenix  2025    ");
			Print_LCD(0,1,(int8_t *) "   ERROR  DMA  PC   ");
			Error_Handler(); 							// Manejo de errores si la inicialización de la recepción falla
			}
		}
    // Esto asegura que solo te interrumpa cuando el paquete termina o el buffer se llena.
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
 */   
	}

static uint8_t PC_TX_Send_DMA(uint8_t *buf, uint16_t len)
    {
    if (len > Buffer_Tx) 
        len = Buffer_Tx;

    if (!PC_Tx_Busy)
        {
        PC_Tx_Busy = 1;
        HAL_GPIO_WritePin(Dir_485_PC_GPIO_Port, Dir_485_PC_Pin, SET); // habilita TX 485
        return (HAL_UART_Transmit_DMA(&huart1, buf, len) == HAL_OK) ? 1u : 0u;
        }

    /* Si está ocupado: guardo 1 mensaje pendiente */
    for (uint16_t i = 0; i < len; i++) 
        Aux_buf_tx[i] = buf[i];
    
    PC_Tx_Pending_Len = len;
    PC_Tx_Pending = 1;
    return 0;
    }
/*---------------------------------------------------------------------------*/
/* Rutina   : Proc_rx_sal
*  Objetivo : Analiza e incrementa los caracteres recibidos
*  Entrada  :
*  Descripcion : Formato de recepción:
*			[stx][Long. string][n datos][   ck   ]
*			  0       1  2      3.....n  n+1, n+2
*			La longitud del string se cuenta desde el comando (bit 5) hasta el ck (bit n+2)
*			Los datos están en ASCII
*				Comu_Sal.flg_rx = True  -->  La recepción OK, debe llegar en false.
*				Long_string 		    -->  indica el largo del string a recibir, se debe
*											 inicializar en 1.
*				Comu_Sal.errores_txrx   -->	N# de error de Tx o Rx.
*                						= 0   	Sin errores
*										= 5		Error de recepción del string (mal ck)
*/
void Proc_rx_sal(uint8_t LenRx)
    {

	int8_t 	dato = 0;											// para barrer la recepción
	uint8_t Caracter_rx;

	while(dato <= LenRx)
		{
		*(Comunica_PSal.buf_rx_Sal + dato) = RxBuffer_Sal[dato];
		Caracter_rx = RxBuffer_Sal[dato];					/* lo guardo */
		if(dato == 0)
		{
		switch(Caracter_rx)
			{
            case STX:															/* Recibí un STX */
            case SI:															/* Recibí un SI */
	            break;
            case ACK:															/* Recibí un Ack */
            case NAK:															/* Recibí un Nack */
	        case CAN:															/* Recibí un Can */
			default:
				Comunica_PSal.flg_rx = True;									/* flag de recepción ==> OK */
		        break;
			}
		}
	else
		{
			switch(dato)										/* Comparo + 1 porque ya incremente */
			{
			case 1:											/* Levanto la longitud del string */
				Aux_recepcion_R1[0] = Caracter_rx;
				break;
			case 2:
				Aux_recepcion_R1[1] = Caracter_rx;
				Aux_recepcion_R1[2] = Null;
				Long_string_Sal = (mi_atoi(Aux_recepcion_R1, 3))*2;
				break;
			default:															/* Sigo recibiendo hasta el largo previsto */
					if((dato - 3) == Long_string_Sal)
					Comunica_PSal.flg_rx = True; 								/* flag de recepción ==> terminada */

			}						/* cierro el switch Caracter_rx */
		}							/* cierro el else */
		dato++;
		}
	}

/*---------------------------------------------------------------------------*/
/* Rutina   	: Transfiero_Salidas
*  Objetivo 	: Transferir los datos de las válvulas que debo prender
*  Entrada  	: Comu_Gral[Comu_PSalidas].buf_tx
*  Descripcion 	: Se realiza la transmisión por DMA para no acupar tiempo de la CPU
*
-----------------------------------------------------------------------------*/
void Transfiero_Salidas(void)
    {
	int8_t 	aux;

	if(M_Trabajo.Velocidad != 0) 							/* si la máquina está parada, no hago nada */
        {
		HAL_GPIO_WritePin(Dir_485_PSalida_GPIO_Port, Dir_485_PSalida_Pin,SET);	/* Pongo la salidas PA11 = 1 (485) */
		Time_Out_485 = 0;									/* pongo el Time Out en 0 mseg. */
		Cant_Envios ++;
		Dato_Error[0] = Null;
		Dato_Error[1] = Null;
		Dato_Error[2] = Null;
		while(Time_Out_485 != 0);							/* Espero la 485 */

		aux = HAL_UART_Transmit_DMA(&huart6, (uint8_t *)Comunica_PSal.buf_tx_Sal, Comunica_PSal.tot_Tx);
		if(aux != 0)
			{
			Dato_Error[0] = (((aux >> 4) & 0x0F) + 0x30);
			Dato_Error[1] = ((aux  & 0x0F) + 0x30);
			Error_Aux++;
			}
        }
    }

/*---------------------------------------------------------------------------*/
/* Rutina   	: HAL_UART_TxCpltCallback
*  Objetivo 	: Poner a cero el pin de dirección de la RS-485
*  Entrada  	:
*  Descripcion 	: Se realiza la transmisión por DMA, antes de activar la transmisión se
*  				  pone a "1" el pin de  dirección de  la RS-485
*
-----------------------------------------------------------------------------*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)		/* Transmisión Placa Salidas y ECP */
	{
	if (huart->Instance == huart1.Instance)
        {
        HAL_GPIO_WritePin(Dir_485_PC_GPIO_Port, Dir_485_PC_Pin, RESET); // fin TX 485

        PC_Tx_Busy = 0;

        if (PC_Tx_Pending)
            {
            PC_Tx_Pending = 0;
            /* Vuelvo a habilitar TX para el siguiente mensaje */
            PC_Tx_Busy = 1;
            HAL_GPIO_WritePin(Dir_485_PC_GPIO_Port, Dir_485_PC_Pin, SET);
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *)Aux_buf_tx, PC_Tx_Pending_Len);
            }
        }
/*    
    if (huart->Instance == huart1.Instance)					 Recepción de la PC 
		{
	    HAL_GPIO_WritePin(Dir_485_PC_GPIO_Port, Dir_485_PC_Pin,RESET);	 Pongo la salidas PA10 = 0 (485) 
		}
*/
	else if(huart->Instance == huart6.Instance)				/* Recepción de las Placas de salida */
		{
	    HAL_GPIO_WritePin(Dir_485_PSalida_GPIO_Port, Dir_485_PSalida_Pin,RESET);/* Pongo la salidas PA11 = 0 (485) */
		}
	else if(huart->Instance == huart4.Instance)				/* Recepción del módulo ECP8266 */
		{
		}
    }

//*---------------------------------------------------------------------------*/
/* Rutina   	:   Asig_Salidas
*  Objetivo 	:   Actualizo estructura de salidas E_Salidas
*  Entrada  	:
*  Descripcion 	: El string recibido llena la matriz de la estructura E_Salidas,
				  el formato recepción en modo "SIN BANDEJA" es:

                "Config.modo_bandeja = 0"

  [stx][nodo dest.][nodo origen][long][comando][N#linea][Tamaño][Cant.Sal.Asignadas][sal 0] [%  0]......[sal 3] [% 3] [ck]
*   0     	1  2         			3  4       	5 6    	7 8      			9       	10 11       		12             		13 14 15 16       25 26 27 28 29

        El largo del string puede cambiar en función de la cantidad de salidas
        asignadas. Minimo 17 a 29 caracteres + Check Sum
        [stx]          		 	==> 1   byte.	byte. 0
        [nodo dest.]    		==> 2   bytes.	bytes.1-2
        [nodo origen]   		==> 2   bytes.	bytes.3-4
        [long]          		==> 2   bytes.  bytes.5-6	   	de 11 a 23
        [comando]       		==> 2   bytes.  bytes.7-8	   	86, 0x56
        [N#linea]       		==> 1   byte.   byte. 9		   	de 1 a 8    Ojo le resto 1 para ir en la matriz de 0 a 7
        [Tamaño]        		==> 2   bytes.  bytes.10-11   	de 01 a 20  Tamaño 0 no se le asigna salida matriz de 0 a 20
        [Cant.Sal.Asig]	 		==> 1   byte.   byte. 12	   	de 1 a 4
        [sal 0]         		==> 2   bytes.  bytes.13-14	   	de 01 a 33
        [% 0]           		==> 2   bytes.  bytes.15-16		de 01 a 10  porcentaje de a 10%
        [sal 1]         		==> 2   bytes.      			de 01 a 33
        [% 1]          			==> 2   bytes.      			de 01 a 10  porcentaje de a 10%
        [sal 2]         		==> 2   bytes.      			de 01 a 33
        [% 2]           		==> 2   bytes.      			de 01 a 10  porcentaje de a 10%
        [sal 3]         		==> 2   bytes.      			de 01 a 33
        [% 3]           		==> 2   bytes.      			de 01 a 10  porcentaje de a 10%
          ck					==>	1	bytes.

	"Config.modo_bandeja = 1" El porcentaje pasa a ser de 4 dígitos ya que es la cantidad de frutos que caen por salida
							antes de hacer el cambio de salida y transferir los datos y se agrega un flag para saber
							si se borran o no la cantidad de frutos que se habian contabilizado hasta el momento de
							la transmisión de cambio de salidas

  [stx][nodo dest.][nodo origen][long][comando][N#linea][Tamaño][Cant.Sal.Asignadas][Borrar .Sal.Asig][sal 0] [   %  0        ]....[sal 3] [   % 3          ] [ck]
*   0     		1 2         		3 4       	5 6    	7 8      			9       	10 11       		12             					13 		   	14 15  16 17 18 19     32 33  34 35 36 37  38

        El largo del string puede cambiar en función de la cantidad de salidas
        asignadas. Mínimo 19 a 37 caracteres + Check Sum.
        [stx]           		==> 1   byte. 0
        [nodo dest.]    		==> 2   bytes.1-2
        [nodo origen]   		==> 2   bytes.3-4
        [long]          		==> 2   bytes.5-6   			de 11 a 23
        [comando]       		==> 2   bytes.7-8   			86, 0x56
        [N#linea]       		==> 1   byte. 9     			de 1 a 8    Ojo le resto 1 para ir en la matriz de 0 a 7
        [Tamaño]        		==> 2   bytes.10-11 			de 01 a 22  Tamaño 0 no se le asigna salida matriz de 0 a 21
        [Cant.Sal.Asig] 		==> 1   byte. 12    			de 1 a 4
        [Borrar .Sal.Asig] 		==> 1   byte. 13				"0" = No borra; "1" = Borra
        [sal 0]         		==> 2   bytes.14-15     		de 01 a 33
        [% 0]           		==> 4   bytes.16-17-18-19   	de 01 a 9999  Cantidad de frutos
        [sal 1]         		==> 2   bytes.20-21     		de 01 a 33
        [% 1]           		==> 4   bytes.22-23-24-25   	de 01 a 9999  Cantidad de frutos
        [sal 2]         		==> 2   bytes.26-27     		de 01 a 33
        [% 2]           		==> 4   bytes.28-29-30-31   	de 01 a 9999  Cantidad de frutos
        [sal 3]         		==> 2   bytes.32-33     		de 01 a 33
        [% 3]           		==> 4   bytes.34-35-36-37   	de 01 a 9999  Cantidad de frutos
        [Ck]           			==> 1   byte. 38

    Los datos se guardan en la estructura reg_Salida ==> E_Salidas, son 20
    tamaños por cada línea E_Salidas[lineas 8][tamaños 20]
    Todos los datos vienen en ASCII por lo que las comparaciones se deben realizar en ASCII ó filtrar los múmeros
    de los caracteres recibidos restándoles "0x30" o " & 0x000F"

-----------------------------------------------------------------------------*/
void  Asig_Salidas(void)
    {
	int8_t aux[5];
	int8_t x;    											/* N# de línea, cantidad de frutos  */
	int8_t y;    											/* Tamaño       */
	int8_t Cant; 											/* Cantidad de salidas asignadas */
	int8_t j;												/* Lazo de carga de las salidas y porcentajes */
	int8_t Posicion;										/* Posicion según el largo del string (Bandeja => True/False) */
	int8_t Aux;												/* Auxiliar para cargar el string según el modo */
	int8_t Borrar;											/* Borrar el total de frutos */

    x = (*(Comunica_PC.buf_rx_PC + 9) & 0x000F) - 1; 		/* cargo el N# de línea, lo filtro y le resto  1 */
    aux[0] = *(Comunica_PC.buf_rx_PC + 10);
    aux[1] = *(Comunica_PC.buf_rx_PC + 11);
    aux[2] = Null;
    y = mi_atoi(aux, 3);									/* Valor del tamaño */

    Cant = (*(Comunica_PC.buf_rx_PC + 12) & 0x000F); 		/* cargo la Cantidad de salidas y lo filtro dejo el número en hexsa */
    E_Salidas_CCM[x][y].Cant_Boxes = Cant;
    E_Salidas_CCM[x][0].Cant_Boxes = 1;						/* inicializo el tamaño 0 */

    if(Config.modo_bandeja == 1)							/* Con Bandeja, 1 */
    	{
	    Borrar = (*(Comunica_PC.buf_rx_PC + 13) & 0x000F); 	/* cargo el flag de borrar totalizador de frutas, en ASCII "0", "1" */
    	Posicion = 6;
    	Aux = 14;
	    Print_LCD(11,3,(int8_t *)"Bandeja");
    	}
    else
    	{
	    Borrar = 1; 										/* cargo el flag de borrar totalizador de frutas, en ASCII "0" */
    	Posicion = 4;
    	Aux = 13;
	    Print_LCD(11,3,(int8_t *)"Normal ");
    	}

    for(j = 0; j < 4; j++)
        {
        if(j < Cant)
            {
            aux[0] = *(Comunica_PC.buf_rx_PC + Aux + (Posicion * j));
            aux[1] = *(Comunica_PC.buf_rx_PC + Aux + 1 + (Posicion * j));
            aux[2] = Null;
            E_Salidas_CCM[x][y].Sal[j] = mi_atoi(aux, 3);	/* Número de salida */

            aux[0] = *(Comunica_PC.buf_rx_PC + Aux + 2 + (Posicion * j));
            aux[1] = *(Comunica_PC.buf_rx_PC + Aux + 3 + (Posicion * j));
            aux[2] = Null;
            if(Config.modo_bandeja == 1)					/* Con Bandeja */
            	{
	            aux[2] = *(Comunica_PC.buf_rx_PC + Aux + 4 + (Posicion * j));	/* se agregan los dos datos nuevos del total de frutas */
    	        aux[3] = *(Comunica_PC.buf_rx_PC + Aux + 5 + (Posicion * j)); 	/* de 0 a 9999 */
        	    aux[4] = Null;
        	    }
            E_Salidas_CCM[x][y].Tot_Frutos_Box[j] = mi_atoi(aux, 5); /* Cantidad de frutos */
            }
        else
            {
        	E_Salidas_CCM[x][y].Sal[j] = 0;
        	E_Salidas_BK[x][y].Tot_Frutos_Box[j] = 0;
            }
        E_Salidas_CCM[x][0].Sal[j] = 0;						/* inicializo el tamaño 0 */
        E_Salidas_CCM[x][0].Tot_Frutos_Box[j] = 0;			/* inicializo el tamaño 0 */
        }


     /* borro los totalizadores de salidas y actualizo punteros */
    if(Borrar == 1)
   		{
    	Print_LCD(11,3,(int8_t *)"Borro   ");
    	E_Salidas_CCM[x][y].Total_frutos = 0;				/* Pongo a cero la cantidad de frutos recibidos */
    	E_Salidas_CCM[x][y].Puntero=0;						/* Pongo a cero el puntero a la salida activa */
    	E_Salidas_CCM[x][y].Salida_Actual = E_Salidas_BK[x][y].Sal[0];	/* actualizo salida actual */
	    for(x = 0; x < Config.n_salidas; x ++)
   	 		Tot_Frut_Salidas[x] = 0;

		}
   	else
		{
    	Print_LCD(11,3,(int8_t *)"No Borro");
   		}

    E_Salidas_CCM[x][0].Salida_Actual = 0;					/* inicializo el tamaño 0 */
    E_Salidas_CCM[x][0].Total_frutos = 0;					/* inicializo el tamaño 0 */
    E_Salidas_CCM[x][0].Puntero=0;							/* inicializo el tamaño 0 */
     if((x + 1) == Config.n_lineas )                     	// cuando llego a la última línea guardo los datos en la RAM BACKUP
         Flg_Write = true;					            	// Pongo el flag para grabar en la flash y Guardo los datos de la ram CCM en la Ram BackUp		 
   
		}

/*---------------------------------------------------------------------------*/
/* Rutina   	:   Asig_Etiqueta
*  Objetivo 	:   Asigno las etiquetas sobre la estructura E_Etiqueta
*  Entrada 		:
*  Descripcion 	: El string recibido llena la matriz de E_Etiqueta
					el formato recepción es:
  [stx][nodo dest.][nodo origen][long][comando][Nºlinea][Nºetique][Tam 1][Tam 2][Tam 3]......[Tam 19][Tam 20] [ck]
*   0      	1  2        		3  4      	5  6    	7 8       		9        	10     	 11 12   13 14 			............. 47  48    49  50     51

        El largo del string debe ser de 51 bytes.
        [stx]           		==> 1   byte.
        [nodo dest.]    		==> 2   bytes.
        [nodo origen]   		==> 2   bytes.
        [long]          		==> 2   bytes.      45
        [comando]       		==> 2   bytes.      87, 0x57
        [Nºlinea]       		==> 1   byte.       de 1 a 8 le resto 1 para ir en la matriz de 0 a 7
        [Nºetique.]    			==> 1   bytes.      de 1 a 4 le resto 1 para ir en la matriz de 0 a 3
        [Tam 1]         		==> 2   bytes.      01 se etiqueta 00 no se etiqueta
        [Tam 2]         		==> 2   bytes.      02 se etiqueta 00 no se etiqueta
        [Tam 3]         		==> 2   bytes.      03
        [Tam 4]         		==> 2   bytes.      04
          ..
          ..
        [Tam 18]        		==> 2   bytes.      18
        [Tam 19]        		==> 2   bytes.      19
        [Tam 20]        		==> 2   bytes.      20
        [CK]            		==> 1   bytes.      el check sum

    Los datos se guardan en la matriz E_Etiqueta, son 20
    tamanios por cada línea E_Etiqueta[Nºetiquetadora 4][lineas 8][tamaños 22]

-----------------------------------------------------------------------------*/
void  Asig_Etiqueta(void)
    {
	int8_t aux[3];
	int8_t x;    /* Nº de línea  viene en ASCII y lo paso a Int (& 0x000F) */
	int8_t y;    /* Nº etiquetadora */
	int8_t j;

    x = (*(Comunica_PC.buf_rx_PC + 9) & 0x000F) - 1;	/* cargo el Nº de línea */
    y = (*(Comunica_PC.buf_rx_PC + 10) & 0x000F) - 1;   /* Cargo el Nº etiquetadora */

    E_Etiqueta_CCM[y][x][0] = 0;						/* al tamaño T0 lo cargo con "0" */
    for(j = 1; j < Max_Tamanos; j++)
        {
        aux[0] = *(Comunica_PC.buf_rx_PC + 9 + (2*j));
        aux[1] = *(Comunica_PC.buf_rx_PC + 10 + (2*j));
        aux[2] = Null;
        E_Etiqueta_CCM[y][x][j]= mi_atoi(aux,3);
        }
    if((x + 1) == Config.n_lineas )                     // cuando llego a la última línea guardo los datos en la RAM BACKUP
        Flg_Write = true;					            // Pongo el flag para grabar en la flash y Guardo los datos de la ram CCM en la Ram BackUp
    }


