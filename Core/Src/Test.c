/*---------------------------------------------------------------------------*/

/*                            M o d o     T E S T                            */

/*---------------------------------------------------------------------------*/
/*
	Fecha inicialización	:	21/05/2022
	Fecha actualización 	:	22/12/2012
	Realizado por	    	:	C.E. Colombo
	Compilador utilizado	:	ST - Eclipse IDE
	Proyecto	    		:	Phenix 2022
	Archivo		    		: 	Test.c
	Versión	   	    		:	3.00.00
	Objetivo				:	Modo Test
                                Se recibe el comando y se contesta con
							   los datos pedidos.
*/
#define  PosVariables   1   /*   	0 = Variables propias
									1 = Variables externas */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Variables.h"

extern void   	Clr_LCD(void);
extern void   	Print_LCD(uint8_t col,uint8_t fila,int8_t *cadena);
//extern int8_t 	Teclado(void);
extern void   	Pongo_Hora(int8_t Hora, int8_t renglon);
extern void 	Armo_Cabecera(int8_t Usart);
extern void   	Tx_char(uint8_t tx_ch, int8_t Usart);  				// Transmito un caracter
extern void   	TX_Ack(int8_t Usart);								// Transmite ACK a la PC o placa de salida - se elije USART(Comu_PSalidas) o USART(Comu_PC)
extern void 	TX_Nak(int8_t Usart);								// Transmite NACK a la PC o placa de salida - se elije USART(Comu_PSalidas) o USART(Comu_PC)
extern void   	Inicializo_Rx(int8_t Usart);						// Inicializo todas las variables para la rutina de recepción
extern void 	Transmito_string(int16_t Usart, int8_t Flag_Check_Sun);
extern void 	Asig_Salidas(void);        							// Asigno las salidas
extern void 	Asig_Etiqueta(void);       							// Asigno los tamanios a las etiquetadoras
extern void   	Beep(void);											// Activar Buzzer
extern int8_t 	* mi_itoa(uint16_t numero, int8_t *buffer, int8_t Largo, int8_t saco_ceros);	// Convertir un entero en un dato ASCII de 5 caracteres
extern int16_t  mi_atoi(int8_t * s,int8_t Largo);					// Convertir una cadena ASCII de 5 caracteres en un entero
//extern int8_t 	* mi_ltoa(uint32_t numero, int8_t *buffer, int8_t Largo,int8_t saco_ceros);	// Convertir un entero long en un dato ASCII de 10 caracteres
extern void     write_Pagina(void);
extern void    	Init_Variables(void);
extern void   	Init_Variables_Config(int8_t Inicializacion);
extern int8_t 	* str_date (int8_t Actualizar);
extern int8_t 	* str_time (int8_t Actualizar);
extern void 	Proc_Rx_PC(uint8_t LenRx);
extern void 	StartReceivingUART_DMA_PC(void);
extern void   	StartReceivingUART_DMA_Sal(void);
extern void 	fatal_error_Beep(void);								// Error grave
extern void 	Proc_rx_sal(uint8_t LenRx);
/************************* Acciones de la placa A/D *********************************************************/
extern int8_t 	Primer_Cero(int8_t Num_Celda);						// Tomar el primer cero
extern int8_t 	Tomar_Peso(int8_t Num_Celda);						// Tomar el peso
//extern void 	Segundo_Cero(void);									// Tomar el segundo cero
extern void 	Ver_Cuentas(void);									// Ver las cuentas del A/D
extern void 	Toma_Taras(void);									// Tomar las taras
extern void 	Fin_Calib_AD(void);									// Fin de la calibración (grabar en flash coeficientes)
extern void 	Verificar_Taras(void);								// Verificar taras
extern void 	Get_Osciloscopio(void);								// Ver los datos de peso en pantalla
extern void 	Datos_Curvas(void);									// Pido los datos de peso de las celdas
//extern void 	Ver_Peso_Test(void);								// Arma el frame para mostrar y transmitir la velocidad, las cuentas y el peso neto
//extern void 	Ver_Peso_Trab(void);								// Arma el frame para mostrar y transmitir el peso neto y el tamaño
extern void		Toma_Tara_Platillos(void);							// Tomar la tara de los platillos (proceso de medicion luego que recibe el comando desde la PC)

/* Declaración de los prototipos del módulo */
void Test(void);					/* Modo test */
void S_Nodo(void);					/* Cambio el número de nodo de la CPU */
void S_NserieCpu(void);				/* Cargo el número de serie de la CPU */
void G_NserieCpu(void);				/* Cargo el número de serie de la CPU */
void S_Fecha(void);					/* Pongo la fecha inst. y actualizo fecha y hora */
void G_FechaInst(void);				/* transmito la fecha de instalación */
void Set_Datos_Cpu(void);			/* Cargo datos de la CPU */
void Get_Datos_Cpu(void);			/* Informo los datos de la CPU */
void Set_Baud_rate(void);			/* Cargo la velocidad de comunicación TPU */
void Get_Baud_rate(void);			/* Pido los datos de la comunicación TPU */
void Set_Baud_rate_SCI(void);		/* Cargo la velocidad de comunicación SCI */
void Get_Baud_rate_SCI(void);		/* Pido los datos de la comunicación SCI */
void Get_Datos_AD(void);			// Transmite a la PC la version del programa de CPU
void Get_Nserie_AD(void);			// Transmite a la PC el numero de serie y la fecha de la placa CPU
void Set_DatosOffset(void);			// Recibe los datos de offset de comienzo y fin de peso (descarta los dameas datos del frame)
//void VersionAD(void);
//void Retransmitir(void);			/* Pedido de retransmisión del último envio */
void Version(void);					/* Pedido de versión de CPU */
void Get_Num_Disco(void);			/* Pido el Número de Disco para control */
void Set_Num_Disco(void);			/* Cargo el Número de Disco para control */
void Set_Ram_Reloj(void);			/* Guardo en la ram reloj los datos */
void Set_dis_salidas(void);			/* Cargo las distancias de las salidas */
void Get_dis_salidas(void);			/* Informo las distancias de las salidas */
void Placa_salida(void);			/* se retransmiten comandos PC==> placas salidas */

void Verifico_Sensores(void);		// Test de sensores
void Calculo_Velocidad(void);		// Calculo de la velocidad de la maquina

/**----------------------------------------------------------------------------------------------------------
*  Rutina   : Test
*  Objetivo : Se testean las distintas funciones de las placas.
*  Entrada  :
*  Descripcion : Las comunicaciones con la Pc se hacen por la huart1
                1.- Placa de CPU.
                    1.- Set Nodo							ok
                    2.- Set Número de serie.				ok
                    3.- Get Número de serie.				ok
                    4.- Set Fecha y hora.					ok
                    5.- Get Fecha y hora.					ok
                    6.- Set modo trabajo. (Sort On)			ok
                    7.- Set tipo de máquina.				ok
                    8.- Get tipo de máquina.				ok
                    9.- Set Net rate.						ok
                   10.- Get Net rate.						ok
                   11.- Set Distancias salidas				ok
                   12.- Get Distancias salidas				ok
                   13.- Get versión.						ok
                   14.- Guardar datos en la memoria.		ok
                   15.- Escape.								ok
                   16.- Comando desconocido.				ok
                   17.- Retransmite lo último transmitido.	ok
				   18.- Inicializo todos los datos			ok

                2.- Comandos A/D para realizar las tareas analógicas
                    1.- Tomar el primer cero.
                    2.- Tomar el Peso patrón y calcular
							  el coeficiente estático.
                    3.- Tomar el segundo cero.
                    4.- Mostrar las cuentas.
                    5.- Tomar las taras.
                    6.- Mostrar el peso bruto.
                    7.- Fin calibración.
                    8.- Verificación de las taras.
                    9.- Test de sensores.
                   10.- Peso neto en forma dinámica.
                   11.- Tomar el peso dinámico.
                   12.- Calcular el coeficiente dinámico.
                   13.- Peso patron.
                   14.- Offset de peso.
                   15.- Offset de cero.

                3.- Placa Salidas. (Electroválvulas) 		huart8
					Todos los comandos son retransmitidos de la huart1 a la huart8 y
					las respuestas de la huart8 a la huart1.
					Se recibe de la PC por la huart1.
					Se recibe de la placa de salida por la huart8.
					Comandos:
                    1.- Escape.
                    2.- Set Id.
                    3.- Get Id.
                    4.- Set Número de serie.
                    5.- Get Número de serie.
                    6.- Set Offset.
                    7.- Get Offset.
                    8.- On Válvula estática.
                    9.- On Válvula dinámica.
                   10.- Off Válvulas.
                   11.- Get versión.
                   12.- Set Baud rate.
                   13.- Guardar datos en la memoria.
*
    Cuando se dispara el comando Sort_On, se debe dar la orden a la placa de
    salidas para pasar al modo trabajo.
    El led D28 cuando esta prendido esta en modo trabajo, apagado en modo test
-----------------------------------------------------------------------------*/
void Test(void)
	{
	uint8_t Dato[6];
	int8_t 	x;
	uint8_t Contador;

	HAL_GPIO_WritePin(Modo_Test_GPIO_Port, Modo_Test_Pin,SET);// Prendo el led de Modo Test
	Flag_Trabajo = true;									// Pongo el flag de salida
    Flg_Display = true;										// Habilito el display y teclado
    Flag_Ver_Cuentas = false;								// No esta en modo ver cuentas al comienzo del modo test
    Inicializo_Rx(Comu_PC);									// Inicializo las variables de recepción

	Clr_LCD();												// Borro el LCD
	Print_LCD(0,0,(int8_t *) "  Phenix  2025     ");
	Print_LCD(0,1,(int8_t *) " Test");
	Print_LCD(0,2,(int8_t *) "V.Maq.      Nodo   ");
	Print_LCD(18, 2, (int8_t *) Config.Nodo);				// Pongo el Nº de nodo
	Time_Out_G = 1;											// pongo el Time Out en 1 mseg.
	Time_Clock = 0;

	/* Orden para pasar a modo test a la  placa Salidas */
	Dato[0]=0x0F;											// string [SI][2][15][26] en hexa
	Dato[1]=0x02;
	Dato[2]=0x15;
	Dato[3]=0x26;
	Dato[4]=Null;
	Contador = 0;

	HAL_GPIO_WritePin(Dir_485_PC_GPIO_Port, Dir_485_PSalida_Pin,SET);// Pongo la salidas PA11 = 1 (485)
	Time_Out_G=0;											// pongo el Time Out en 2,6 mseg.
	while(Time_Out_G != 0);									// Espero la 485
	while(Dato[Contador]!=Null)
		{
    	Tx_char(Dato[Contador], Comu_PSalidas);
		Contador++;
		}
	HAL_GPIO_WritePin(Dir_485_PC_GPIO_Port, Dir_485_PSalida_Pin,RESET);	// Pongo la salidas PA11 = 0 (485)

	TX_Ack(Comu_PC);
	Beep();
	Inicializo_Rx(Comu_PC);										/* Inicializo las variables de recepción */
	// Iniciar la recepción por DMA con detección de línea IDLE
	StartReceivingUART_DMA_PC();
//	StartReceivingUART_DMA_EMC();
	Time_Out_disp = 250;									// Tiempo para sacar los datos al display

	Velocidad_medida = 0;
    M_Trabajo.Velocidad  = 0;
	for(x = 0; x < 6; x++)
    	Dato[x] = Null;

    Dato[0] = ' ';
    Dato[1] = '0';
    Dato[2] = '.';
    Dato[3] = '0';
    Dato[4] = Null;

	while (Flag_Trabajo==True)								// sin un comando que lo cambie no salgo del Modo TEST
        {													// si recibí algo por la serie el flag pasa a True
		if((Flag_Rx_DMA_PC == true))						// si es por la serie DMA la atiendo y analizo el frame
            {
        	Proc_Rx_PC(RxDataLen_PC);						// Analizo el string recibido desde la PC
        	StartReceivingUART_DMA_PC();					// Habilito el DMA PC
            }
        if(Comunica_PC.flg_rx == true)						// Si la recepcion fue correcta => se ejecuta el comando recibido
        	{
    		Flg_Display = true;										// Habilito el display
        	Print_LCD(0,3,(int8_t *)"R.Comando:          ");	// Pone el comando recibido en el display
        	switch(Comando_recibido)						// analizo el comando recibido
        		{
/*************** Comandos de la CPU **************************************************************/
        		case Set_Nodo:								// Set Numero de nodo  --> VERIFICADO
        			Print_LCD(11, 3,(int8_t *)"Set nodo ");
        			Beep();
       				S_Nodo();
       				Print_LCD(18, 2, (int8_t *) Config.Nodo);// Pongo el Nº de nodo
       			break;
        		case Set_NserieCpu:							// Set Número de serie  --> VERIFICADO
        			Print_LCD(11, 3,(int8_t *)"Set Nser ");
       				Beep();
       				S_NserieCpu();
       				break;
       			case Get_NserieCpu:							// Get Número Nodo y de serie  --> VERIFICADO
       				Print_LCD(11, 3,(int8_t *)"Num.Serie");
       				Beep();
       				G_NserieCpu();
       				break;
       			case Set_Fecha:								// Set Fecha y hora  --> VERIFICADO
       				Print_LCD(11, 3,(int8_t *)"FechaHora");
       				Beep();
       				S_Fecha();
       				break;
       			case Get_Fecha:								// Get Fecha de instalación  --> VERIFICADO
       				Print_LCD(11, 3,(int8_t *)"FechaInst");
       				Beep();
       				G_FechaInst();
       				break;
        		case Sort_On:           					// Modo Trabajo On
        			Beep();
//        			TX_Ack(Comu_PC);
        			Flag_Primer_ingreso = false;
        			Flag_Trabajo = false;					// Pongo el flag de salida
        			break;
        		case Rx_Key:            					// Datos generales de la máquina
        			Print_LCD(11, 3,(int8_t *)"SetDatGen");
        			Beep();
        			Set_Datos_Cpu();
        			break;
        		case Tx_Key:								// Transmite datos generales de la máquina
       				Print_LCD(11, 3,(int8_t *)"GetDatGen");
        			Beep();
        			Get_Datos_Cpu();
        			break;
        		case Net_Rate:          					// Baud rate de la CPU a la PC  --> VERIFICADO
        			Print_LCD(11, 3,(int8_t *)"SetBaudPC");
        			Beep();
        			Set_Baud_rate();
        			break;
        		case Get_Net_Rate:      					// Get baud rate de la CPU a la PC  --> VERIFICADO
        			Print_LCD(11, 3,(int8_t *)"GetBaudPC");
        			Beep();
        			Get_Baud_rate();
        			break;
        		case Net_Rate_SCI:      					// Baud rate de la CPU a las Placas de salida
        			Print_LCD(11, 3,(int8_t *)"SetBaudSa");
        			Beep();
        			Set_Baud_rate_SCI();
        			break;
        		case Get_Net_Rate_SCI:  					// Get baud rate de la CPU a las Placas de salida  --> VERIFICADO
        			Print_LCD(11, 3,(int8_t *)"SetBaudSa");
        			Beep();
        			Get_Baud_rate_SCI();
        			break;
        		case Set_Distancias:						/* Set datos de las distancias de salida */
        			Print_LCD(11, 3,(int8_t *)"Set Dist.");
        			Beep();
        			Set_dis_salidas();
        			break;
        		case Get_Distancias:						/* Get datos de las distancias de salida */
        			Print_LCD(11, 3,(int8_t *)"Get Dist.");
        			Beep();
        			Get_dis_salidas();
        			break;
        		case Asig_Salida:       					/* Actualizo estructura de salidas E_Salidas */
        			Print_LCD(11, 3,(int8_t *)"Asig.Sali");
        			Asig_Salidas();
        			break;
        		case Asig_Etiq:         					/* Actualizo matriz de etiquetadoras */
        			Print_LCD(11, 3,(int8_t *)"Asig.Etiq");
        			Asig_Etiqueta();
        			break;
        		case Version_CPU:       					// Versión de la CPU  --> VERIFICADO
        			Print_LCD(11, 3,(int8_t *)"Ver. CPU.");
        			Beep();
        			Version();
        			break;
        		case Guardar_datos:     					// Guardar datos en memoria  --> VERIFICADO
        		case F_Calib:
        			Print_LCD(11, 3,(int8_t *)"Write    ");
        			Beep();
         			HAL_GPIO_WritePin(CPU_OK_GPIO_Port, CPU_OK_Pin,SET);		// Prendo el led Verde
      				Time_Parpadeo = 50;
         			Time_Parpa = Time_Parpadeo;
         			Flag_Grabo = true;						// Activo el flag para activar el parpadeo
         			write_Pagina();
         			if(Flg_ErrorFlash == false)
        				{
        				TX_Ack(Comu_PC);					// Transmito ACK a la PC informando que se grabo bien la FLASH
        				Print_LCD(17, 3, (int8_t *) "ok ");
               			Flag_Grabo = false;					// Desactivo el flag para activar el parpadeo
            			HAL_GPIO_WritePin(CPU_OK_GPIO_Port, CPU_OK_Pin,SET);		// Prendo el led Verde
            			Beep();
        				}
        			else
        				{
               			Flag_Grabo = false;					// Desactivo el flag para activar el parpadeo
              			HAL_GPIO_WritePin(CPU_OK_GPIO_Port, CPU_OK_Pin,RESET);		// Apago el led Verde
           				HAL_GPIO_WritePin(Error_GPIO_Port, Error_Pin,SET);
        				TX_Nak(Comu_PC);						// Transmito NACK a la PC informando un error de escritura en FLASH
        				Print_LCD(17, 3, (int8_t *) "Mal");
           				Time_Parpadeo = 80;
             			Time_Parpa = Time_Parpadeo;
        				Flag_Error = true;
        				}
       			break;
        		case Esc:               					/* Dejar de hacer una tarea */
        			Print_LCD(11, 3,(int8_t *)"Escape   ");
        			Flag_Ver_Cuentas = false;				// Sale del modo ver cuentas
        			Flag_Enable_Taras = false;				// Deshabilita la toma o verificacion de taras y aborta el proceso
        			HAL_GPIO_WritePin(GPIOC, Reset_Pin, GPIO_PIN_SET);			// Manda un RESET al conversor A/D porque hay veces que se cuelga cuando viene de ver cuentas
        			for (x = 0; x < 2; ++x)										// Hace una demora para que actue el RESET del A/D
        		    	{__asm__("nop");}										//
        			HAL_GPIO_WritePin(GPIOC, Reset_Pin, GPIO_PIN_RESET);		//
        			TX_Ack(Comu_PC);
        			break;
        		case Retransmite:       					/* Retransmite lo último enviado */
        			Print_LCD(11, 3,(int8_t *)"Retransmi");
        			Transmito_string(Comu_PC,true);			/* Re transmito y calculo en check sum */
        			break;
        		case Borrar_M8K:							/* Borro la memoria Flash e inicializo el sistema */
        			Print_LCD(11, 3,(int8_t *)"Cle.Flash");
        			Beep();
        			Init_Variables_Config(true);			/* armo toda la estructura */
        			Init_Variables();						/* copio de la ram_reloj a la de trabajo */
        			TX_Ack(Comu_PC);
        			break;
        		case Get_N_Disco:
        			Print_LCD(11, 3,(int8_t *)"GetN.Disc");
        			Get_Num_Disco();						/* Transmito el número de disco para control */
        			Beep();
        			break;
        		case Set_N_Disco:
        			Print_LCD(11, 3,(int8_t *)"SetN.Disc");
        			Set_Num_Disco();						/* Grabo el número de disco para control */
        			Beep();
        			break;

/************************* Comandos de la placa A/D *********************************************************/
        		case T_Cero:			// Tomar el primer cero
        			Beep();
        			Print_LCD(11, 3,(int8_t *)"Toma Cero");
        			Primer_Cero(8);
        			break;
        		case T_Peso:			// Tomar el peso
					Beep();
        			Print_LCD(11, 3,(int8_t *)"Toma Peso");
					Tomar_Peso(8);
					break;
        		case T_Scero:			// Tomar el segundo cero
        			Beep();
                    Primer_Cero(8);
        			break;
        		case V_Cuentas:			// Ver las cuentas del A/D
        			Print_LCD(11, 3,(int8_t *)"VerCuenta");
        			Print_LCD(0, 2,(int8_t *)"                    ");
        			Ver_Cuentas();
        			break;
        			/**
        			 * CUal es el comando de Tomar Taras que envia la PC?
        			 * T_Taras = 0x05
        			 * T_Taras1 = 0x54
        			 */
        		case T_Taras:			// Tomar las taras
        			Print_LCD(11, 3,(int8_t *)"Toma Tara");
        			Flag_Enable_Taras = true;				// Habilita la toma de taras desde la PC
        			Toma_Taras();
        			Beep();
 //       			TX_Ack(Comu_PC);
        			break;
        		case T_Taras1:			/** Verificar tarea de tomar taras  - CONSULTAR SI SE EJECUTA ESTE COMANDO DESDE NET (EXISTE ???) */
        			Beep();
        			TX_Ack(Comu_PC);
        			break;
        		case P_Bruto_6:			/** Ver el peso bruto - DESDE EL NET SE PIDE V_CUENTAS  - (EXISTE ESTE COMANDO ?)   */
        			Beep();
        			break;
        		case V_Taras:			// Verificar taras
        			Beep();
        			Print_LCD(11, 3,(int8_t *)"Test Tara");
        			Flag_Enable_Taras = true;				// Habilita la verificacion de taras desde la PC
	      			Verificar_Taras();
        			break;
        		case In_Test:			// Test de sensores
        			Beep();
					Verifico_Sensores();
        			break;
        		case Get_Datos_Gen:		// Devuelve la version del programa llamando a la rutina Version()
        			Beep();
        			Get_Datos_AD();
        			break;
        		case Get_NserieAD:		// Transmite a la PC el numero de serie y la fecha
        			Beep();
        			Get_Nserie_AD();
        			break;
        		case Version_AD:		// Transmite a la PC la versión de la CPU
        			Beep();
        			Version();
        			break;
        		case Osciloscopio:		// Ver los datos de peso en pantalla
        			Beep();
        			Print_LCD(11, 3,(int8_t *)"Ver Curvas");
					Get_Osciloscopio();
        			break;
        		case Pido_Curvas:		// Pido los datos de peso de las celdas
        			Beep();
					Datos_Curvas();
        			break;
        		case T_Pdin:			/** Tomar el peso dinamico y sacar coeficiente dinamico (NO IMPLEMENTADO EN VERSION ORIGINAL) */
        			Beep();
//        	    	Cal_Dinamica();
        			break;
        		case Set_Datos_Gen:
        			Beep();
        			Print_LCD(11, 3,(int8_t *)"Set OFFset");
        			Set_DatosOffset();
        			TX_Ack(Comu_PC);
        			break;
        		case Set_OffsetAD:
        			Beep();
        			break;
        		case Get_OffsetAD:		/* Get offset placa A/D */
        			Beep();
//			        Get_Offset_AD();
        			break;
        		case C_Limites:			/* Cargar los limites */
//	 	            Armo_Limites();
//		            Set_Limites();
        			break;

/************** Acciones de la placa de Salidas *****************************************************************************/

        		case P_Salida:
        			Print_LCD(11, 3,(int8_t *)"Pl.Salida");
					Placa_salida();		// retransmito comandos, recibo la respuesta y habilito ls recepción DMA_Sal
        			break;

				/* Si es un Nack, Can, Ack, no hago nada */
        		case ACK_1:
        		case NAK_1:
        		case CAN_1:
        			Print_LCD(11, 3,(int8_t *)"CAN      ");
        			break;

        		case No_nodo:									// No es para este nodo
        			Print_LCD(11, 3,(int8_t *)"No nodo  ");
        			break;

        		case Com_desc:									// Comando desconocido
        		default:										// Comando desconocido
        			Print_LCD(11, 3,(int8_t *)"Descon.  ");
        			break;

        		}   /* cierro el swicth */
    		Inicializo_Rx(Comu_PC);									/* Inicializo las variables de recepción */
        	}
        else		/** No hubo recepcion desde la PC ***/
        	{
        /**************************************************************************************/
         	if(Time_Out_disp <= 1)
        		{
       			Calculo_Velocidad();							// Calculo de la velocidad de la maquina
       			Print_LCD(6, 2, (int8_t *) Dato_Velo);			// Pongo la velocidad de la máquina
				Pongo_Hora(false,0);
          		Time_Out_disp = 250;							/* Tiempo para sacar los datos al display */
        		}
        /**************************************************************************************/
    		if(Flag_Init_Tara == true)				// Pone el flag para habilitar la toma de taras de los platillos)
    			{
    			if(Flag_Enable_Taras == true)				// DEBE esperar que pase el plato 1 para comenzar a tomar las taras
    				{
    				Toma_Tara_Platillos();				// Toma el peso del platillo y lo guarda como tara
    				Print_LCD(11, 3,(int8_t *)"Tomando T");
    				Flag_Error = True;						// Se enciende y apaga el Led rojo cada 1 seg para indicar que se esta tomando la tara
        		if(Conta_Ejes >= Config.n_platillos)	// La cantidad de platos va de 0 a n_platillos - 1
           			{
        			TX_Ack(Comu_PC);					// Transmito ACK a la PC informando que finalizo el proceso de toma de taras
        			Flag_Init_Tara = false;				// Pone el flag para deshabilitar la toma de taras de los platillos
        			Flag_Enable_Taras = false;			// Deshabilita la toma de taras desde la PC
        			Flag_Tara_OK = True;				// Pone el flag indicando que se realizo el proceso de toma de taras
        			Flag_Error = False;					// Apaga el Led rojo indicando que finalizo el proceso de toma de tara
        			Print_LCD(11, 3,(int8_t *)"         ");
           			}
    			}
        	}
        	}
        }           /* cierro el while principal */
	HAL_GPIO_WritePin(Modo_Test_GPIO_Port, Modo_Test_Pin,RESET);/* Apago el led de Modo Test */
	}
/*---------------------------------------------------------------------------*/
/* Rutina   : S_Nodo
*  Objetivo : Cambia el número de nodo de la CPU
*  Entrada  :
*  Descripcion : Se carga en la estructura de datos el número de nodo de la
*               placa CPU.
*               CUIDADO para que no se pierdan los datos hay que guardarlos en
*               la ram_reloj con el comando correspondiente.
*
-----------------------------------------------------------------------------*/
void S_Nodo(void)
	{
	Config.Nodo[0]=Comunica_PC.buf_rx_PC[9];
	Config.Nodo[1]=Comunica_PC.buf_rx_PC[10];
	Config.Nodo[2]=Null;

	TX_Ack(Comu_PC);
	}

/*---------------------------------------------------------------------------*/
/* Rutina   : S_NserieCpu
*  Objetivo : Cargo el número de serie de la CPU
*  Entrada  :
*  Descripcion : Se carga en la estructura de datos el número de serie de la
*               placa CPU.
*               CUIDADO para que no se pierdan los datos hay que guardarlos en
*               la ram_reloj con el comando correspondiente.
*
-----------------------------------------------------------------------------*/
void S_NserieCpu(void)
	{
	Config.n_serie[0]=Comunica_PC.buf_rx_PC[9];
	Config.n_serie[1]=Comunica_PC.buf_rx_PC[10];
	Config.n_serie[2]=Comunica_PC.buf_rx_PC[11];
	Config.n_serie[3]=Comunica_PC.buf_rx_PC[12];
	Config.n_serie[4]=Comunica_PC.buf_rx_PC[13];
	Config.n_serie[5]=Comunica_PC.buf_rx_PC[14];
	Config.n_serie[6]=Comunica_PC.buf_rx_PC[15];
	Config.n_serie[7]=Null;

	TX_Ack(Comu_PC);
	}
/*---------------------------------------------------------------------------*/
/* Rutina   : G_NserieCpu
*  Objetivo : Cargo el número de serie y el número de nodo de la CPU
*  Entrada  :
*  Descripcion : Se carga de la estructura de datos el número de serie de la
*               placa CPU, el número de nodo y se transmite.
*
-----------------------------------------------------------------------------*/
void G_NserieCpu(void)
	{
	uint8_t j;

	/* Armo el string a transferir */
	Armo_Cabecera(Comu_PC);						/* Armo la cabecera de la transmisión */
	Comunica_PC.buf_tx_PC[5] = '1';				/* Longitud a transmitir o recibir */
	Comunica_PC.buf_tx_PC[6] = '0';
	Comunica_PC.buf_tx_PC[7] = Config.Nodo[0]; 	/* Número de nodo */
	Comunica_PC.buf_tx_PC[8] = Config.Nodo[1];

	for(j = 0; j <= 6; j++)
		{
		Comunica_PC.buf_tx_PC[9 + j] =  Config.n_serie[j];
		}
	/* Transmito los datos y calculo el Check sum, lo guardo al final por si
		necesito retransmisión del dato */
	Comunica_PC.buf_tx_PC[9 + j] = Null;       	/* pongo el terminador */
	Transmito_string(Comu_PC,true);				/* transmito y calculo en check sum*/
	}
/*---------------------------------------------------------------------------*/
/* Rutina   : S_Fecha
*  Objetivo : Cargo la fecha y la hora en el reloj de la CPU
*  Entrada  :
*  Descripcion : Se carga en la estructura de datos la fecha de inicialización
*               de la placa CPU si el flag corresponde y se actualiza la hora.
*               CUIDADO para que no se pierdan los datos hay que guardarlos en
*               la ram_reloj con el comando correspondiente.
*
-----------------------------------------------------------------------------*/
void S_Fecha(void)
	{
	// buf_time[] 	-->  hh:mm:ss Null
	// buf_date[]	-->  dd-mm-aaaa Null

	if(Comunica_PC.buf_rx_PC[9]=='1')
		{
		Config.f_instalacion[0]=Comunica_PC.buf_rx_PC[10];
		Config.f_instalacion[1]=Comunica_PC.buf_rx_PC[11];
		Config.f_instalacion[2]='/';
		Config.f_instalacion[3]=Comunica_PC.buf_rx_PC[12];
		Config.f_instalacion[4]=Comunica_PC.buf_rx_PC[13];
		Config.f_instalacion[5]='/';
		Config.f_instalacion[6]=Comunica_PC.buf_rx_PC[14];
		Config.f_instalacion[7]=Comunica_PC.buf_rx_PC[15];
		Config.f_instalacion[8]=Null;
		}
    /* actualizo la Fecha */
	buf_date[0]=Comunica_PC.buf_rx_PC[10];
	buf_date[1]=Comunica_PC.buf_rx_PC[11];
	buf_date[2]='/';
	buf_date[3]=Comunica_PC.buf_rx_PC[12];
	buf_date[4]=Comunica_PC.buf_rx_PC[13];
	buf_date[5]='/';
	buf_date[6]=Comunica_PC.buf_rx_PC[14];
	buf_date[7]=Comunica_PC.buf_rx_PC[15];
	buf_date[8]=Null;
    /* actualizo la Hora */
	buf_time[0]=Comunica_PC.buf_rx_PC[16];
	buf_time[1]=Comunica_PC.buf_rx_PC[17];
	buf_time[2]=':';
	buf_time[3]=Comunica_PC.buf_rx_PC[18];
	buf_time[4]=Comunica_PC.buf_rx_PC[19];
	buf_time[5]=':';
	buf_time[6]='0';
	buf_time[7]='0';
	buf_time[8]=Null;

	(int8_t *) str_time(true);
	(int8_t *) str_date(true);

	TX_Ack(Comu_PC);
	}
/*---------------------------------------------------------------------------*/
/* Rutina   : G_FechaInst
*  Objetivo : Pido la fecha de Instalación
*  Entrada  :
*  Descripcion : Se envía de la estructura de datos la fecha de inicialización
*               de la placa CPU.
*
-----------------------------------------------------------------------------*/
void G_FechaInst(void)
	{
	uint8_t j;

	/* Armo el string a transferir */
	Armo_Cabecera(Comu_PC);						/* Armo la cabecera de la transmisión */
	Comunica_PC.buf_tx_PC[5] = '0';    			/* Longitud a transmitir o recibir */
	Comunica_PC.buf_tx_PC[6] = '9';

	for(j = 0; j <= 7; j++)          			/* Armo el string */
		{
		Comunica_PC.buf_tx_PC[7 + j] =  Config.f_instalacion[j];
		}
	/* Transmito los datos y calculo el Check sum, lo guardo al final por si
		necesito retransmisión del dato */
	Comunica_PC.buf_tx_PC[7 + j] = Null;     	/* pongo el terminador */
	Transmito_string(Comu_PC,true);				/* transmito y calculo en check sum*/
	}

/*---------------------------------------------------------------------------*/
/* Rutina   : Set_Datos_Cpu
*  Objetivo : Recibo todos los datos generales de la máquina.
*  Entrada  :
*  Descripcion : Se reciben los datos generales de la máquina y se guardan en
*               la estructura de configuración de la CPU.
*               CUIDADO para que no se pierdan los datos hay que guardarlos en
*               la ram_reloj con el comando correspondiente.
*
-----------------------------------------------------------------------------*/
void Set_Datos_Cpu(void)
	{
	int8_t aux[4];

	Config.n_lineasASCII[0]=Comunica_PC.buf_rx_PC[9];
	Config.n_lineasASCII[1]=Null;
	Config.n_lineas = Comunica_PC.buf_rx_PC[9] & 0x000F;

	Config.n_salidasASCII[0]=Comunica_PC.buf_rx_PC[10];
	Config.n_salidasASCII[1]=Comunica_PC.buf_rx_PC[11];
	Config.n_salidasASCII[2]=Null;
	aux[0]=Comunica_PC.buf_rx_PC[10];
	aux[1]=Comunica_PC.buf_rx_PC[11];
	aux[2]=Null;
	Config.n_salidas = mi_atoi(aux, 3);

	Config.n_platillosASCII[0]=Comunica_PC.buf_rx_PC[12];
	Config.n_platillosASCII[1]=Comunica_PC.buf_rx_PC[13];
	Config.n_platillosASCII[2]=Comunica_PC.buf_rx_PC[14];
	Config.n_platillosASCII[3]=Null;
	aux[0]=Comunica_PC.buf_rx_PC[12];
	aux[1]=Comunica_PC.buf_rx_PC[13];
	aux[2]=Comunica_PC.buf_rx_PC[14];
	aux[3]=Null;
	Config.n_platillos=mi_atoi(aux, 3);

	Config.n_etiqueASCII[0]=Comunica_PC.buf_rx_PC[15];
	Config.n_lineasASCII[1]=Null;
	Config.n_etique = Comunica_PC.buf_rx_PC[15] & 0x000F;

	Config.peso_patron[0]=Comunica_PC.buf_rx_PC[16];
	Config.peso_patron[1]=Comunica_PC.buf_rx_PC[17];
	Config.peso_patron[2]=Comunica_PC.buf_rx_PC[18];
	Config.peso_patron[3]=Null;

	Config.modo_bandeja = (Comunica_PC.buf_rx_PC[19] - 0x30);

	Config.error_taraASCII[0]=Comunica_PC.buf_rx_PC[20];
	Config.error_taraASCII[1]=Comunica_PC.buf_rx_PC[21];
	Config.error_taraASCII[2]=Null;
	aux[0]=Comunica_PC.buf_rx_PC[20];
	aux[1]=Comunica_PC.buf_rx_PC[21];
	aux[2]=Null;
	Config.error_tara = mi_atoi(aux, 3);

	aux[0]=Comunica_PC.buf_rx_PC[22];
	aux[1]=Comunica_PC.buf_rx_PC[23];
	aux[2]=Null;
	Config.DistEtiq[0]=mi_atoi(aux, 3);

	aux[0]=Comunica_PC.buf_rx_PC[24];
	aux[1]=Comunica_PC.buf_rx_PC[25];
	aux[2]=Null;
	Config.SalidaEtiq[0]= mi_atoi(aux, 3);
	aux[0]=Comunica_PC.buf_rx_PC[26];
	aux[1]=Comunica_PC.buf_rx_PC[27];
	aux[2]=Null;
	Config.DistEtiq[1]= mi_atoi(aux, 3);

	aux[0]=Comunica_PC.buf_rx_PC[28];
	aux[1]=Comunica_PC.buf_rx_PC[29];
	aux[2]=Null;
	Config.SalidaEtiq[1]= mi_atoi(aux, 3);

	aux[0]=Comunica_PC.buf_rx_PC[30];
	aux[1]=Comunica_PC.buf_rx_PC[31];
	aux[2]=Null;
	Config.DistEtiq[2]= mi_atoi(aux, 3);
	aux[0]=Comunica_PC.buf_rx_PC[32];
	aux[1]=Comunica_PC.buf_rx_PC[33];
	aux[2]=Null;
	Config.SalidaEtiq[2]= mi_atoi(aux, 3);

	aux[0]=Comunica_PC.buf_rx_PC[34];
	aux[1]=Comunica_PC.buf_rx_PC[35];
	aux[2]=Null;
	Config.DistEtiq[3]= mi_atoi(aux, 3);

	aux[0]=Comunica_PC.buf_rx_PC[36];
	aux[1]=Comunica_PC.buf_rx_PC[37];
	aux[2]=Null;
	Config.SalidaEtiq[3]= mi_atoi(aux, 3);

	TX_Ack(Comu_PC);

	}
/*---------------------------------------------------------------------------*/
/* Rutina   : Get_Datos_Cpu
*  Objetivo : Envío la fecha y la hora en el reloj de la CPU
*  Entrada  :
*  Descripcion : Se envían los datos generales de la máquina que se guardan en
*               la estructura de configuración de la CPU.
*
-----------------------------------------------------------------------------*/
void Get_Datos_Cpu(void)
	{
	int8_t aux[6];

	/* Armo el string a transferir */
	Armo_Cabecera(Comu_PC);						/* Armo la cabecera de la transmisi�n */
	Comunica_PC.buf_tx_PC[5] = '2';				/* Longitud a transmitir o recibir */
	Comunica_PC.buf_tx_PC[6] = '2';

	/* Armo el string */
	Comunica_PC.buf_tx_PC[7] = Config.n_lineasASCII[0];

	Comunica_PC.buf_tx_PC[8] = Config.n_salidasASCII[0];
	Comunica_PC.buf_tx_PC[9] = Config.n_salidasASCII[1];

	Comunica_PC.buf_tx_PC[10] = Config.n_platillosASCII[0];
	Comunica_PC.buf_tx_PC[11] = Config.n_platillosASCII[1];
	Comunica_PC.buf_tx_PC[12] = Config.n_platillosASCII[2];

	Comunica_PC.buf_tx_PC[13] = Config.n_etiqueASCII[0];

	Comunica_PC.buf_tx_PC[14] = Config.peso_patron[0];
	Comunica_PC.buf_tx_PC[15] = Config.peso_patron[1];
	Comunica_PC.buf_tx_PC[16] = Config.peso_patron[2];

	Comunica_PC.buf_tx_PC[17] = (Config.modo_bandeja + 0x30);

	Comunica_PC.buf_tx_PC[18] = Config.error_taraASCII[0];
	Comunica_PC.buf_tx_PC[19] = Config.error_taraASCII[1];

	mi_itoa (Config.SalidaEtiq[0], (int8_t *) &aux[0], 2, false);  /* Convierto el INT a ASCII */
	Comunica_PC.buf_tx_PC[20] = aux[0];
	Comunica_PC.buf_tx_PC[21] = aux[1];
	mi_itoa (Config.SalidaEtiq[1], (int8_t *) &aux[0], 2, false);  /* Convierto el INT a ASCII */
	Comunica_PC.buf_tx_PC[22] = aux[0];
	Comunica_PC.buf_tx_PC[23] = aux[1];
	mi_itoa (Config.SalidaEtiq[2], (int8_t *) &aux[0], 2, false);  /* Convierto el INT a ASCII */
	Comunica_PC.buf_tx_PC[24] = aux[0];
	Comunica_PC.buf_tx_PC[25] = aux[1];
	mi_itoa (Config.SalidaEtiq[3], (int8_t *) &aux[0], 2, false);  /* Convierto el INT a ASCII */
	Comunica_PC.buf_tx_PC[26] = aux[0];
	Comunica_PC.buf_tx_PC[27] = aux[1];
	/* Transmito los datos y calculo el Check sum, lo guardo al final por si
		necesito retransmisi�n del dato */
	Comunica_PC.buf_tx_PC[28] = Null;      		/* pongo el terminador */
	Transmito_string(Comu_PC,true);				/* transmito y calculo en check sum*/

}
/*---------------------------------------------------------------------------*/
/* Rutina   : Set_Baud_rate
*  Objetivo : Cargo la velocidad de comunicación de la CPU con la PC
*  Entrada  :
*  Descripcion : Se carga en la estructura de datos la velocidad de comunicación
*               de la placa CPU con la PC.
*               CUIDADO para que no se pierdan los datos hay que guardarlos en
*               la ram_reloj con el comando correspondiente, en ese mismo
*               momento se activa la nueva velocidad. (Ini_Serie_TPU())
*
-----------------------------------------------------------------------------*/
void Set_Baud_rate(void)
	{
	switch(Comunica_PC.buf_rx_PC[9])
		{
		case '4':
			Comu_Conf[Comu_PC].velocidad = 4;         /* Baud rate 9600 */
			break;
		case '3':
			Comu_Conf[Comu_PC].velocidad = 3;         /* Baud rate 19200 */
			break;
		case '2':
			Comu_Conf[Comu_PC].velocidad = 2;         /* Baud rate 38400 */
			break;
		case '1':
			Comu_Conf[Comu_PC].velocidad = 1;         /* Baud rate 57600 */
			break;
		default:
			Comu_Conf[Comu_PC].velocidad = 0;         /* Baud rate 115200 */
			break;
		}
	Comu_Conf[Comu_PC].paridad = 0x00F & (Comunica_PC.buf_rx_PC[10]);
	Comu_Conf[Comu_PC].bits_data = 0x00F & (Comunica_PC.buf_rx_PC[11]);		/* largo de la palabra a transmitir */
	Comu_Conf[Comu_PC].Tipo_Comu_H = 0x00F & (Comunica_PC.buf_rx_PC[12]);   /* 0 = RS-232,  1 = RS-485 */

	TX_Ack(Comu_PC);
	}
/*---------------------------------------------------------------------------*/
/* Rutina   : Get_Baud_rate
*  Objetivo : Retorno los datos de la comunicación de la CPU con la PC
*  Entrada  :
*  Descripcion : Se toman de la estructura de datos la velocidad de comunicación
*               de la placa CPU (TPU) con la PC, y se transmiten.
*
-----------------------------------------------------------------------------*/
void Get_Baud_rate(void)
	{
	/* Armo el string a transferir */
	Armo_Cabecera(Comu_PC);							/* Armo la cabecera de la transmisión */
	Comunica_PC.buf_tx_PC[5] = '0';    				/* Longitud a transmitir o recibir */
	Comunica_PC.buf_tx_PC[6] = '5';

	/* Armo el string */
	switch(Comu_Conf[Comu_PC].velocidad)
		{
		case 0:
			Comunica_PC.buf_tx_PC[7] = '0';
			break;
		case 1:
			Comunica_PC.buf_tx_PC[7] = '1';
			break;
		case 2:
			Comunica_PC.buf_tx_PC[7] = '2';
			break;
		case 3:
			Comunica_PC.buf_tx_PC[7] = '3';
			break;
		case 4:
			Comunica_PC.buf_tx_PC[7] = '4';
			break;
		default:
			Comunica_PC.buf_tx_PC[7] = '2';
  	     break;
		}

	Comunica_PC.buf_tx_PC[8] = Comu_Conf[Comu_PC].paridad + '0';
	Comunica_PC.buf_tx_PC[9] = Comu_Conf[Comu_PC].bits_data + '0';
	Comunica_PC.buf_tx_PC[10] = Comu_Conf[Comu_PC].Tipo_Comu_H + '0';
	/* Transmito los datos y calculo el Check sum, lo guardo al final por si
		necesito retransmisión del dato */
	Comunica_PC.buf_tx_PC[11] = Null;      			/* pongo el terminador */
	Transmito_string(Comu_PC,true);					/* transmito y calculo en check sum*/

	}
/*---------------------------------------------------------------------------*/
/* Rutina   : Set_Baud_rate_SCI
*  Objetivo : Cargo la velocidad de comunicación de la CPU con las Placas de salida
*  Entrada  :
*  Descripcion : Se carga en la estructura de datos la velocidad de comunicación
*               de la placa CPU SCI con las Placas de salida.
*               CUIDADO para que no se pierdan los datos hay que guardarlos en
*               la ram_reloj con el comando correspondiente, en ese mismo
*               momento se activa la nueva velocidad. (Ini_Serie_SCI())
*
-----------------------------------------------------------------------------*/
void Set_Baud_rate_SCI(void)
	{
	switch(Comunica_PC.buf_rx_PC[9])
		{
		case '0':
			Comu_Conf[Comu_PSalidas].velocidad = 0;         /* Baud rate 9600 */
			break;
		case '1':
			Comu_Conf[Comu_PSalidas].velocidad = 1;         /* Baud rate 19200 */
			break;
		case '2':
			Comu_Conf[Comu_PSalidas].velocidad = 2;         /* Baud rate 38400 */
			break;
		case '3':
			Comu_Conf[Comu_PSalidas].velocidad = 3;         /* Baud rate 57600 */
			break;
		case '4':
			Comu_Conf[Comu_PSalidas].velocidad = 4;         /* Baud rate 115200 */
			break;
		default:
			Comu_Conf[Comu_PSalidas].velocidad = 2;         /* Baud rate 38400 */
			break;
		}
	Comu_Conf[Comu_PSalidas].paridad=0x00F & (Comunica_PC.buf_rx_PC[10]);
	Comu_Conf[Comu_PSalidas].bits_data=0x00F & (Comunica_PC.buf_rx_PC[11]);		/* largo de la palabra a transmitir */
	Comu_Conf[Comu_PSalidas].Tipo_Comu_H=0x00F & (Comunica_PC.buf_rx_PC[12]);   /* 0 = RS-232,  1 = RS-485 */

	TX_Ack(Comu_PC);
	}
/*---------------------------------------------------------------------------*/
/* Rutina   : Get_Baud_rate_SCI
*  Objetivo : Retorno los datos de la comunicación de las Placas de Salida con la PC
*  Entrada  :
*  Descripcion : Se toman de la estructura de datos la velocidad de comunicación
*               de la placa CPU , y se transmiten.
*
-----------------------------------------------------------------------------*/
void Get_Baud_rate_SCI(void)
	{
	/* Armo el string a transferir */
	Armo_Cabecera(Comu_PSalidas);					/* Armo la cabecera de la transmisión */
	Comunica_PC.buf_tx_PC[5] = '0';    				/* Longitud a transmitir o recibir */
	Comunica_PC.buf_tx_PC[6] = '5';

	/* Armo el string */
	switch(Comu_Conf[Comu_PSalidas].velocidad)
		{
		case 4:
			Comunica_PC.buf_tx_PC[7] = '4';
			break;
		case 3:
			Comunica_PC.buf_tx_PC[7] = '3';
			break;
		case 2:
			Comunica_PC.buf_tx_PC[7] = '2';
			break;
		case 1:
			Comunica_PC.buf_tx_PC[7] = '1';
			break;
		default:
			Comunica_PC.buf_tx_PC[7] = '0';
			break;
		}

	Comunica_PC.buf_tx_PC[8] = Comu_Conf[Comu_PSalidas].paridad + '0';
	Comunica_PC.buf_tx_PC[9] = Comu_Conf[Comu_PSalidas].bits_data + '0';
	Comunica_PC.buf_tx_PC[10] = Comu_Conf[Comu_PSalidas].Tipo_Comu_H + '0';
	/* Transmito los datos y calculo el Check sum, lo guardo al final por si
		necesito retransmisión del dato */
	Comunica_PC.buf_tx_PC[11] = Null;      			/* pongo el terminador */
	Transmito_string(Comu_PC,true);					/* transmito y calculo en check sum*/
	}

/*---------------------------------------------------------------------------*/
/* Rutina   : Version
*  Objetivo : Transmite a la PC la versión de la CPU.
*  Entrada  :
*  Descripcion :
*
-----------------------------------------------------------------------------*/
void Version(void)
	{
	int8_t j;

	/* Armo el string a transferir */
	Armo_Cabecera(Comu_PC);						/* Armo la cabecera de la transmisión */
	Comunica_PC.buf_tx_PC[5] = '1';    			/* Longitud a transmitir o recibir */
	Comunica_PC.buf_tx_PC[6] = '0';

	for(j = 0; j <= 8; j++)           			/* Armo el string */
		{
		Comunica_PC.buf_tx_PC[7 + j] = Config.n_version[j];
		}
	/* Transmito los datos y calculo el Check sum, lo guardo al final por si
    necesito retransmisión del dato */
	Comunica_PC.buf_tx_PC[7+j] = Null;       	/* pongo el terminador */
	Transmito_string(Comu_PC, true);			/* transmito y calculo en check sum */
	}

/*---------------------------------------------------------------------------*/
/* Rutina   : Get_Datos_AD(void)
*  Objetivo : Transmite a la PC la version del programa
*  Entrada  :
*  Descripcion :
*
-----------------------------------------------------------------------------*/
void Get_Datos_AD(void)
	{
	Version();
	}

/*---------------------------------------------------------------------------*/
/* Rutina   : Get_Nserie_AD(void)
*  Objetivo : Transmite a la PC el numero de serie y la fecha
*  Entrada  :
*  Descripcion :  Transmite a la PC datos de N# serie y Fecha
                1.- N# de serie     2 bytes dato en binario (p. alta, p.baja)
                2.- Fecha           8 bytes en ASCII
*
-----------------------------------------------------------------------------*/
void Get_Nserie_AD(void)
	{
	int8_t j;

	Armo_Cabecera(Comu_PC);						/* Armo la cabecera de la transmisión */
	Comunica_PC.buf_tx_PC[5] = '1';				/* Longitud a transmitir o recibir */
	Comunica_PC.buf_tx_PC[6] = '4';

	for(j = 0; j <= 6; j++)
{
		Comunica_PC.buf_tx_PC[7 + j] =  Config.n_serie[j];
		}

	for(j = 0; j <= 7; j++)          			/* Armo el string */
		{
		Comunica_PC.buf_tx_PC[12 + j] =  Config.f_instalacion[j];
}

	/* Transmito los datos y calculo el Check sum, lo guardo al final por si
		necesito retransmisión del dato */
	Comunica_PC.buf_tx_PC[19 + j] = Null;       	/* pongo el terminador */
	Transmito_string(Comu_PC,true);				/* transmito y calculo en check sum*/
	}
/*---------------------------------------------------------------------------*/
/* Rutina   : Set_DatosOffset(void)
*  Objetivo : Recibe los datos de offset de comienzo y fin de peso (descarta los dameas datos del frame)
*  Entrada  :
*  Descripcion :
*
-----------------------------------------------------------------------------*/
void Set_DatosOffset(void)
	{
	int8_t aux[4];
	int8_t j;

    aux[0]=Comunica_PC.buf_rx_PC[22];
    aux[1]=Comunica_PC.buf_rx_PC[23];
    aux[2]=Comunica_PC.buf_rx_PC[24];
    aux[3]=Null;
    j = mi_atoi(aux, 3);
    Config.Enable_AD = j;

    aux[0]=Comunica_PC.buf_rx_PC[25];
    aux[1]=Comunica_PC.buf_rx_PC[26];
    aux[2]=Comunica_PC.buf_rx_PC[27];
    aux[3]=Null;
    j = mi_atoi(aux, 3);
    Config.Disable_AD = j;
	}

/*---------------------------------------------------------------------------*/
/* Rutina   : Get_Datos_AD
*  Objetivo : Get datos generales de la m�quina
*  Entrada  :
*  Descripcion : Transmitir los datos genéricos de la placa A/D
                 Estos son: 1.- Número de línea.        1 byte en binario 1 en ASCII
                            2.- Número de platillos     2 byte en binario 3 en ASCII
                            3.- Peso patron             2 byte en binario 3 en ASCII
                            4.- Error de tara           1 byte en binario 2 en ASCII
                            5.- Peso promedio platillo  2 byte en binario 3 en ASCII
                            6.- Valor mínimo a pesar    1 byte en binario 3 en ASCII
*
-----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/* Rutina   : Get_Num_Disco
*  Objetivo : Transmite a la PC el número de disco.
*  Entrada  :
*  Descripcion :
*
-----------------------------------------------------------------------------*/
void Get_Num_Disco(void)
	{
	int8_t j;

	/* Armo el string a transferir */
	Armo_Cabecera(Comu_PC);						/* Armo la cabecera de la transmisi�n */
	Comunica_PC.buf_tx_PC[5] = '1';    			/* Longitud a transmitir o recibir */
	Comunica_PC.buf_tx_PC[6] = '0';

	for(j = 0; j <= 8; j++)           			/* Armo el string */
		{
		Comunica_PC.buf_tx_PC[7 + j] = Config.NumDisco[j];
		}
	/* Transmito los datos y calculo el Check sum, lo guardo al final por si
		necesito retransmisión del dato */
	Comunica_PC.buf_tx_PC[7 + j] = Null;       	/* pongo el terminador */
	Transmito_string(Comu_PC, true);			/* transmito y calculo en check sum*/
	}
/*---------------------------------------------------------------------------*/
/* Rutina   : Set_Num_Disco
*  Objetivo : Cargo el número de disco de la PC
*  Entrada  :
*  Descripcion : Se carga en la estructura de datos el número de disco de la PC
*               CUIDADO para que no se pierdan los datos hay que guardarlos en
*               la ram_reloj con el comando correspondiente.
*
-----------------------------------------------------------------------------*/
void Set_Num_Disco(void)
	{
	Config.NumDisco[0]=Comunica_PC.buf_rx_PC[9];
	Config.NumDisco[1]=Comunica_PC.buf_rx_PC[10];
	Config.NumDisco[2]=Comunica_PC.buf_rx_PC[11];
	Config.NumDisco[3]=Comunica_PC.buf_rx_PC[12];
	Config.NumDisco[4]=Comunica_PC.buf_rx_PC[13];
	Config.NumDisco[5]=Comunica_PC.buf_rx_PC[14];
	Config.NumDisco[6]=Comunica_PC.buf_rx_PC[16];
	Config.NumDisco[7]=Comunica_PC.buf_rx_PC[17];
	Config.NumDisco[8]=Comunica_PC.buf_rx_PC[18];
	Config.NumDisco[9]=Null;

	Set_Ram_Reloj();		/* se guardan los datos en la ram reloj
							   y se transmite el ACK */
	}
/*---------------------------------------------------------------------------*/
/* Rutina   : Set_Ram_Reloj
*  Objetivo : Guarda los datos de la ram a la ram_reloj.
*  Entrada  :
*  Descripcion : Copia las estructuras a la ram_reloj e inicializa la
*                comunicaci�n de la PC.
*
-----------------------------------------------------------------------------*/
void Set_Ram_Reloj(void)
	{
/*    unsigned short int *Ptr;

Ptr=(unsigned short int *)PORTE0;    Cargo la direcci�n del Port "E" */
/* Copia la configuracion de comunicacion */
//	Copy_ram((unsigned char *)&Comunica_PC,(unsigned char *)ptr_estruc_comu_pc,sizeof(Comunica_PC));
//	Copy_ram((unsigned char *)&Comu_Sal,(unsigned char *)ptr_estruc_comu_sal,sizeof(Comu_Sal));
/* Copia la configuracion de la maquina CPU */
//	Copy_ram((unsigned char *)&Config,(unsigned char *)ptr_estruc_maquina,sizeof(Config));
/* Copia la configuracion de las placas A/d, Salidas y Etiquetadora */
//	Copy_ram((unsigned char *)&P_Salidas,(unsigned char *)ptr_estruc_salida_0,sizeof(P_Salidas));

	TX_Ack(Comu_PC);

//	Ini_Serie_TPU();    /* Inicializo la serie de la TPU para la PC */
//	Ini_SCI();         	/* Inicializo la SCI, comunicaci�n con placa salida */

	}
/*---------------------------------------------------------------------------*/
/* Rutina   : Set_dis_salidas
*  Objetivo : Guarda los datos de las salidas.
*  Entrada  :
*  Descripcion : Copia en las estructuras de la CPU las distancias de las
*               	salidas con respecto a la celda de carga.
*					El formato de recepción es:
			Long. = Cantidad de bytes desde el byte 5 hasta el ck.
            [stx][nodo dest.][nodo origen][long][comando][dist 1]......[dist   32] [ck]
              0     1  2         3  4       5 6   7  8    9 10 11      105 106 107 108
			El largo del string es variable en función de la cantidad de salidas.
			A dato Long. se le deben restar 3 bytes, (2 del comando y 1 del ck).
*
-----------------------------------------------------------------------------*/
void Set_dis_salidas(void)
	{
	uint8_t j;
	int16_t Sal;
	uint8_t aux_Sal;
	int8_t aux[6];

	Sal = mi_atoi(Config.n_salidasASCII, 3); 	/* casteo a int el número de salidas */
	aux[0] = Comunica_PC.buf_rx_PC[5];   		/* Levanto los bytes a recibir y lo casteo */
	aux[1] = Comunica_PC.buf_rx_PC[6];
	aux[2] = Null;
	aux_Sal = (mi_atoi(aux, 4) - 3) / 3;   		 /* a al cantidad de bytes a recibir, le resto el comando, el CK
													y lo divido por 3 (3 bytes por cada dato) */

if(Sal == aux_Sal)          					/* si coinciden las cantidades las guardo */
    {
    for(j=0; j< Sal; j++)
        {
        aux[0]=Comunica_PC.buf_rx_PC[(j*3)+9];   /* Levanto los bytes datos y los casteo */
        aux[1]=Comunica_PC.buf_rx_PC[(j*3)+10];
        aux[2]=Comunica_PC.buf_rx_PC[(j*3)+11];
        aux[3]=Null;
        Config.DisBox[j] = mi_atoi(aux, 4);  	/* Convierto el ASCII a INT */
        }

    TX_Ack(Comu_PC);
    }
else
    {
    TX_Nak(Comu_PC);
    }
}
/*---------------------------------------------------------------------------*/
/* Rutina   : Get_dis_salidas
*  Objetivo : Transmite los datos de las salidas.
*  Entrada  :
*  Descripcion :
*
-----------------------------------------------------------------------------*/
void Get_dis_salidas(void)
	{
	uint8_t x;
	uint8_t j;
	uint8_t Sal;
	int8_t aux[6];

	Sal = mi_atoi(Config.n_salidasASCII, 3); 		/* casteo a int el número de salidas */

	/* Armo el string a transferir */
	Armo_Cabecera(Comu_PC);							/* Armo la cabecera de la transmisi�n */
	x = (Sal * 3) + 1;    							/* calculo la cantidad de bytes a transmitir */
	mi_itoa (x, (int8_t *) &aux[0], 5, false);  	/* Convierto el INT a ASCII */
	Comunica_PC.buf_tx_PC[5] = aux[3];    			/* Longitud a transmitir o recibir */
	Comunica_PC.buf_tx_PC[6] = aux[4];

	for(j = 0; j < Sal; j++)
		{
		mi_itoa (Config.DisBox[j], (int8_t *) &aux[0], 5, false);  /* Convierto el INT a ASCII */
		Comunica_PC.buf_tx_PC[(j*3)+7] = aux[2];
		Comunica_PC.buf_tx_PC[(j*3)+8] = aux[3];
		Comunica_PC.buf_tx_PC[(j*3)+9] = aux[4];
		}
	/* Transmito los datos y calculo el Check sum, lo guardo al final por si
		necesito retransmisión del dato */
	Comunica_PC.buf_tx_PC[8 + x] = Null;       		/* pongo el terminador */
	Transmito_string(Comu_PC,true);					/* transmito y calculo en check sum*/

	}
/*---------------------------------------------------------------------------*/
/* Rutina   : Placa_salida
*  Objetivo : Retransmite un comando de la PC a las placas de salida
*  Entrada  :
*  Descripcion : Cuando se recibe el comando hay que sacarle el [STX] [N# nodo]
*               [cant. caracteres] [comando] y el [check sum] la parte central ya viene
*               procesada por la PC. La respuesta recibida, se la retransmite tal como
*				se la recibe.
*		String recibido:
		[STX][nodo dest.][nodo origen][Cant.caracteres][comando][datos placa salida][ck]
		  0     1  2         3  4          5  6           7 8      9............ n   n+1
*
-----------------------------------------------------------------------------*/
void Placa_salida(void)
	{
	uint8_t 	j;
	int16_t 	Cant_caracteres;
	int8_t 		aux[6];

	// Armo el string a retransmitir
	aux[0]=Comunica_PC.buf_rx_PC[5];			// Levanto los bytes a recibir, lo casteo
	aux[1]=Comunica_PC.buf_rx_PC[6];			// y calculo los bytes a retransmitir
	aux[2]=Null;
	Cant_caracteres= (mi_atoi(aux, 4)) - 3;		// Convierto el ASCII a INT
												// Descuento Byte 7, Byte 8 y el Byte del ck

    HAL_UARTEx_ReceiveToIdle_IT(&huart6, RxBuffer_Sal, Buffer_Rx_Sal);		// Recibo las Placas de salida  por Interrupciones
    Inicializo_Rx(Comu_PSalidas);

	HAL_GPIO_WritePin(Dir_485_PSalida_GPIO_Port, Dir_485_PSalida_Pin,SET);	// Pongo la salidas PA11 = 1 (485)
	Time_Out_485 = 0;							// pongo el Time Out en 0 mseg.
	while(Time_Out_485 != 0);					// Espero la 485
	for(j = 0; j < Cant_caracteres; j++)
		{
		Tx_char(Comunica_PC.buf_rx_PC[9 + j], Comu_PSalidas);	//	Tx_char_sal ((unsigned char) Comunica_PC.buf_rx_PC[9+j]);
		}
	HAL_GPIO_WritePin(Dir_485_PSalida_GPIO_Port, Dir_485_PSalida_Pin,RESET);// Pongo la salidas PA11 = 0 (485)

	// *************	Espero la contestación de la placa de salida    *********************************
	Time_Out_G = 2000;								// pongo el Time Out en 2000 mseg.
	while(Time_Out_G != 0) 							// Espera la respuesta de la placa de salida
		{
//		if((Flag_Rx_DMA_Sal == true))					// si es por la serie DMA la atiendo
//			{
//			Proc_rx_sal(RxDataLen_Sal);					// Analizo el string recibido

		if (Comunica_PSal.flg_rx == True)			// Si recibi la respuesta la transmito a la PC
			{
			if (Comunica_PSal.errores_txrx!=0)
				{
				Print_LCD(0,0,(int8_t *)"error Com.Placa sal.");
				fatal_error_Beep();
				TX_Nak(Comu_PC);
				}
			else
				{
				// Armo el string a transferir
				Armo_Cabecera(Comu_PC);				// Armo la cabecera de la transmisión
				switch(Comunica_PSal.buf_rx_Sal[0])
					{
					case ACK:						// Recibí un Ack
					case NAK:						// Recibí un Nack
					case CAN:						// Recibí un Can
						Comunica_PC.buf_tx_PC[5] = '0';	// Longitud a transmitir o recibir
						Comunica_PC.buf_tx_PC[6] = '3';
						Comunica_PC.buf_tx_PC[7] = '0';
						Comunica_PC.buf_tx_PC[8] = Comunica_PSal.buf_rx_Sal[0];
						Comunica_PC.buf_tx_PC[9] = Null;// pongo el terminador
						break;
					default:
						j = 0;
						while(Comunica_PSal.buf_rx_Sal[j]!=Null)
							{
							Comunica_PC.buf_tx_PC[7+j] = Comunica_PSal.buf_rx_Sal[j];
							j++;
							}
						// calculo la cantidad de bytes a transmitir
						mi_itoa ((j+1), (int8_t *) & aux[0], 5,false);  // Convierto el INT a ASCII
						Comunica_PC.buf_tx_PC[5] = aux[3];		// Longitud a transmitir o recibir
						Comunica_PC.buf_tx_PC[6] = aux[4];
						Comunica_PC.buf_tx_PC[j + 7] = Null;  		// pongo el terminador
						break;
					}
				Transmito_string(Comu_PC,true);				//transmito y calculo en check sum
				Print_LCD(11,3,(int8_t *)"P.Sal. OK");
				Beep();
				break;
				}
			}
		}
	if(Time_Out_G  <= 1)
		{
		Print_LCD(0,3,(int8_t *)"Error Placa sal.T.O.");
		fatal_error_Beep();
		TX_Nak(Comu_PC);
		}
	}

/************************* Acciones de la placa A/D *****************************/
/**----------------------------------------------------------------------------------------------------------
*  Rutina   : Verifico_Sensores
*  Objetivo : Test de sensores
*  Entrada  :
*  Descripcion :
*
------------------------------------------------------------------------------------------------------------*/
void Verifico_Sensores(void)
{

}

/**----------------------------------------------------------------------------------------------------------
*  Rutina   	: void Calculo_Velocidad(void)void Calculo_Velocidad(void)
*  Objetivo 	: Calcular la velocidad de máqiuina
*  Entrada  	: ----------
*  Descripcion 	: Hago la medición cada 200mseg.
*
------------------------------------------------------------------------------------------------------------*/
void Calculo_Velocidad(void)
	{

   	if(Time_Out_Velo <= 1)
   		{
	   	Cont_Velo++;
   		if(Conta_Velo_Maq != 0)					// Maquina funcionando
   			{
   	   		Time_Out_Velo = 200;										// Tiempo para calcular la velocidad
  			Velocidad_medida = (uint32_t)(100000 / Conta_Velo_Maq);		// Frecuencia = 100000 / Contador (en mseg.) para tener 2 decimales

   			mi_itoa(Velocidad_medida,(int8_t *)& Dato_Velo,4, true);
   			Dato_Velo[3] = Dato_Velo[2];
   			Dato_Velo[2] = '.';
   			Dato_Velo[4] = Null;
   			Cont_Velo = 0;
   			Conta_Velo_Maq = 0;
   			}
   		else									// Maquina detenida
   			{
   			if(Cont_Velo > 200)
   				{
   				Velocidad_medida = 0;
   				Conta_Velo_Maq = 0;
   				Dato_Velo[0] = ' ';
   				Dato_Velo[1] = '0';
   				Dato_Velo[2] = '.';
   				Dato_Velo[3] = '0';
   				Dato_Velo[4] = Null;
   				Cont_Velo = 0;
   				}
   			}
    	}
	}

/**----------------------------------------------------------------------------------------------------------
*  Rutina   :
*  Objetivo :
*  Entrada  :
*  Descripcion :
*
------------------------------------------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------------------------------------
*  Rutina   :
*  Objetivo :
*  Entrada  :
*  Descripcion :
*
------------------------------------------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------------------------------------
*  Rutina   :
*  Objetivo :
*  Entrada  :
*  Descripcion :
*
------------------------------------------------------------------------------------------------------------*/


