 /*
 *    	Fecha inicialización	:	28/06/2025
	Fecha actualización 		:	28/06/2025
	Realizado por	    		:	C.E. Colombo
	Compilador utilizado		:	ST - Eclipse IDE
	Proyecto	    			:	Phenix 2025 - 407VGT6


                                                                      		*/
/*      	DEFINICION DE ESTRUCTURAS PARA LAS COMUNICACIONES SERIE         	*/
/*                                                                      		*/

/*--------------------------------------------------------------------------------------------------------------
      DEFINICION DE LA ESTRUCTURA PARA LAS COMUNICACIONES SERIE
----------------------------------------------------------------------------------------------------------------*/

	struct Comunicacion_PC					/* 316 bytes largo de la estructura */
		{
		int16_t		flg_rx;						/* True = recepcion OK   */
		int16_t		flg_tx;						/* True = transmision OK */
    	int16_t		cant_Tx;					/* Cant. Bytes transmitidos */
    	int16_t		cant_Rx;					/* Cant. Bytes recibidos */
    	int16_t		tot_Tx;						/* Total Bytes a transmitir */
    	int16_t		tot_Rx;						/* Total Bytes a recibir */
    	uint8_t  	buf_rx_PC[Buffer_Rx];		/* Puntero al Buffer de recepción */
    	uint8_t  	buf_tx_PC[Buffer_Tx];		/* Puntero al Buffer de transmisión */
    	int16_t		busy;						/* Flag indicador de recepcion en progreso */
    	int16_t		errores_txrx;				/* Copia del registro de error de la USART */
		};

	struct Comunicacion_PSal				/* 116 bytes largo de la estructura */
		{
		int16_t		flg_rx;						/* True = recepcion OK   */
		int16_t		flg_tx;						/* True = transmision OK */
    	int16_t		cant_Tx;					/* Cant. Bytes transmitidos */
    	int16_t		cant_Rx;					/* Cant. Bytes recibidos */
    	int16_t		tot_Tx;						/* Total Bytes a transmitir */
    	int16_t		tot_Rx;						/* Total Bytes a recibir */
    	uint8_t  	buf_rx_Sal[Buffer_Rx_Sal];	/* Puntero al Buffer de recepción */
    	uint8_t  	buf_tx_Sal[Buffer_Rx_Sal];	/* Puntero al Buffer de transmisión */
    	int16_t		busy;						/* Flag indicador de recepcion en progreso */
    	int16_t		errores_txrx;				/* Copia del registro de error de la USART */
		};

	struct Comunicacion_ECP					/* 316 bytes largo de la estructura */
		{
		int16_t		flg_rx;						/* True = recepcion OK   */
		int16_t		flg_tx;						/* True = transmision OK */
    	int16_t		cant_Tx;					/* Cant. Bytes transmitidos */
    	int16_t		cant_Rx;					/* Cant. Bytes recibidos */
    	int16_t		tot_Tx;						/* Total Bytes a transmitir */
    	int16_t		tot_Rx;						/* Total Bytes a recibir */
    	uint8_t  	buf_rx_ECP[Buffer_Rx];		/* Puntero al Buffer de recepción */
    	uint8_t  	buf_tx_ECP[Buffer_Tx];		/* Puntero al Buffer de transmisión */
    	int16_t		busy;						/* Flag indicador de recepcion en progreso */
    	int16_t		errores_txrx;				/* Copia del registro de error de la USART */
		};

	struct Comunicacion_conf				/* 8 bytes x 3 estructuras = 24 Bytes de largo */
		{
		int16_t		velocidad;					/* puntero a la leyenda Baud rate */
		int16_t		bits_data;					/* puntero a la leyenda largo de la palabra a transmitir */
		int16_t		paridad;					/* puntero a la leyenda Paridad de la comunicación */
		int16_t		Tipo_Comu_H;				/* puntero al tipo de comunicación 232/485 */
		};

	/*--------------------------------------------------------------------------------------------------------------
		DEFINICION DE LA ESTRUCTURA DE LA MAQUINA
		  1.- Chequeo de versión			integer
    	  5.- Número de Nodo:				81
    	  6.- Número de Equipo  			0.....65000 (En Hex)
----------------------------------------------------------------------------------------------------------------*/
    struct Config_Maquina							/* 164 bytes largo de la estructura */
        {		/* Definida la estructura como Config */
         int16_t  	Numero_Maquina;     			/* Número de placa */
         int8_t     n_serie[8];         			/* Número de serie */
         int8_t     Nodo[3];            			/* Número de Nodo */
         int8_t     denominacion[6];   				/* Nombre de la placa CPU */
         int8_t     n_version[10];      			/* Número de versión de la CPU */
         int8_t     NumDisco[10];       			/* Número de Disco */
         int8_t     f_instalacion[9];   			/* Fecha de instalacion */
         int8_t     n_lineasASCII[2];       		/* Número de lineas en ASCII */
         int16_t  	n_lineas;         				/* Número de lineas en binario */
         int8_t     n_salidasASCII[3];      		/* Número de salidas */
         int16_t  	n_salidas;        				/* Número de salidas en binario */
         int8_t     n_platillosASCII[4];    		/* Número de platillos */
         int16_t  	n_platillos;      				/* Número de platillos en binario */
         int8_t     n_etiqueASCII[2];				/* Número de etiquetadoras */
         int16_t  	n_etique;         				/* Número de etiquetadoras en binario */
         int8_t     peso_patron[4];     			/* valor del peso patron */
         int8_t   	modo_bandeja;       			/* flag de modo bandeja 10/04/2017 */
         int8_t     error_taraASCII[3];    			/* Error aceptable de la tara */
         int16_t  	error_tara;          			/* Error aceptable de la tara en binario */
         int16_t	Cant_Pulsos_Sincro;				/* Cantidad de pulsos de sincronismo */
         int16_t	Enable_AD;						/* Cantidad de pulsos Enable A/D */
         int16_t	Disable_AD;						/* Cantidad de pulsos Disable A/D */
         int16_t  	DisBox[Salidas];    			/* 0 - 255 -> Distancias entre la celda y las salidas */
         int16_t	DistEtiq[MaxEtiqueta];  		/* 0 - 255 -> Distancia de la celda a las etiquetadoras */
         int16_t 	SalidaEtiq[MaxEtiqueta];		/* Número de salida asignado a cada Etiquetadora */
        };

/*    struct Config_PSalidas						// 64 bytes largo de la estructura
        {
   		 uint8_t	Nombre_Plaqueta[6]; 			// Nombre de la placa de salida
		 int16_t	Numero_Plaqueta;				// Número de placa
		 uint8_t	Version_Plaqueta[10];			// Número de versión
		 uint8_t	Fecha_alta_Plaqueta[9];			// Fecha de alta
		 int8_t		Numero_Nodo[3];					// Número de nodo
		 int16_t	OffsetOn[8];					// Offset para activar la salida (Cada placa tiene 8 salidas)
		 int16_t	OffsetOff[8];					// Offset para desactivar salida
		 int8_t		Habilitacion[2];
        };
*/

    struct reg_Trabajo						/* 1602 bytes largo de la estructura */
        {
        uint32_t   TotalKg_Tamano[Max_Tamanos][MaxLineas];		/* Total de Kg Tamano X Línea*/
        uint32_t   TotalKg_Calidad[16];							/* Total de Kg por calidad */
        uint32_t   TotalKg_Linea[MaxLineas];					/* Total de Kg por linea */
        uint32_t   Tot_Plato_Vacio[MaxLineas];					/* Total de platillos vacios por Línea */
        uint32_t   Tot_Platillos[MaxLineas];					/* Total de platillos por Línea */
        uint32_t   Tot_Frutos_Tamanos[Max_Tamanos][MaxLineas];	/* Total de frutos por Línea */
        uint32_t   Total_Frutos[MaxLineas];						/* Total de frutos por Línea */
        int16_t    Velocidad;									/* Velocidad de máquina */
        };

    struct reg_Sal							/* (17 + 1 (alineación)) x 8 x 22 = 3168 bytes largo de la estructura */
        {
    	int8_t   	Salida_Actual;      					/* 1 Bytes Salida en curso, 1 a 32 */
   		int16_t   	Total_frutos;       					/* 2 Bytes Cantidad de frutos de la salida en curso, 1 a 9999  18/04/2017 */
   		int8_t   	Puntero;            					/* 1 Bytes Puntero a la próxima salida, 1 a 4 */
   		int8_t   	Cant_Boxes;         					/* 1 Bytes Cantidad de salidas asociadas, 1 a 4 */
   		int8_t   	Sal[4];             					/* 4 Bytes Primer salida asociada, 1 a 32*/
   		int16_t   	Tot_Frutos_Box[4];  					/* 8 Bytes Total de frutos de cada salida salida, 1 a 9999  18/04/2017 */
        };

