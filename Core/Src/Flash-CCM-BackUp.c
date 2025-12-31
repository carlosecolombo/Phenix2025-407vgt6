/*---------------------------------------------------------------------------
      M A N E J O   M E M O R I A S    F L A S H ,  C C M ,  B A C K U P
---------------------------------------------------------------------------*/
/*	Fecha inicialización	:	23/05/2022
	Fecha actualización 	:	22/12/2025
	Realizado por			:	C.E. Colombo
	Compilador utilizado	:	ST - Eclipse IDE
	Proyecto	    		:	Phenix 2025
	Versión					:	3.00
	Objetivo				:	Rutinas de manejo de las Memorias
*/
#define  PosVariables   1   /*   	0 = Variables propias
									1 = Variables externas */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "Variables.h"
#include <string.h>

extern void Print_LCD(uint8_t col, uint8_t fila, int8_t *cadena);
extern void Clr_LCD(void);

/*Variable used for Erase procedure*/
FLASH_EraseInitTypeDef EraseInitStruct;

void Init_Variables_Config(void);
void write_Pagina(void);					// Graba los datos de la Ram CCM en la Flask
void Grabo_Def_to_FL(void);					// Carga las definiciones originales en la Ram CCM y los guarda en la Flash
void Grabo_FL_to_CCM(void);					// Pasa los datos de la Flash  a la Ram CCM
/*	Rutinas BKPSRAM --> 4-kBytes backup ram library for STM32F4xx.  */
void BKPSRAM_Init(void);
void Grabo_BackUp_to_CCM(void);				// Leo los datos de la BackUp SRAM y los paso a la Ram CCM
void Write_Ram_BK(void);					// Guardo los datos de la Ram CCM en la BackUp SRAM


/*--------------------------------------------------------------------------------------------------
      R U T I N A S   D E   M A N E J O   D E    M E M O R I A   F L A S H
*---------------------------------------------------------------------------------------------------*/
/*
*  Rutina  		: write_Pagina()
*  Objetivo		: Escribe en la FLASH
*  Entrada 		:
*  Comentarios	: OJO los datos en la memoria Flash y la CCM  deben estar definidos iguales y consecutivos
*  				  la copia se realiza no por nombre de la  variable aino en forma masiva
*  Retorna		:
*/
void write_Pagina(void)
	{
	uint32_t Start_Address_Flash = 0;
	uint32_t Start_Address_Ram = 0;
	uint32_t SECTORError = 0;
	uint8_t  Dato8;
	uint8_t  DatoFlash;
	uint32_t FLASH_USER_END_ADDR;
	int16_t  MemoryProgramStatus;


	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();

	/* Get the 1st sector to erase
		Sector 11 --> 0x080E0000
		NbfSectors = 1;					Borro  1 sector solo
	*/
	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector        = FLASH_SECTOR_11;		// Sector 11 --> 0x080E0000
	EraseInitStruct.NbSectors     = 1;						// NbfSectors;

	if(HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
		{
		/*  Error occurred while sector erase.  */
		/* Infinite loop */
		while (1)
			{
			Time_Parpadeo = 500;						// Prendo y apago el led de ERROR
			Time_Parpa = Time_Parpadeo;
	    	Clr_LCD();													/* Borro el LCD */
	    	Print_LCD(0,0,(int8_t *) "    Phenix  2025    ");
	    	Print_LCD(0,1,(int8_t *) "ERROR Borrado Flash ");
			}
		}

	  /* Program the user Flash area Byte by Byte
		(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	Start_Address_Ram   = RAM_CCM_START_ADDR;			// Comienzo de la Ram a guardar
	Start_Address_Flash = FLASH_USER_START_ADDR;		// Comienzo de la Flash a grabar

	FLASH_USER_END_ADDR = Start_Address_Flash + 0x1FFF;	// Fin de la Flash a grabar

	while(Start_Address_Flash < FLASH_USER_END_ADDR)
		{
		Dato8 = *(uint8_t *)Start_Address_Ram;			//	Address_Dato[i];
		Start_Address_Ram++;							//	i++;
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, Start_Address_Flash, Dato8) == HAL_OK)
			Start_Address_Flash = Start_Address_Flash + 1;
		else
			{
			/* Error occurred while writing data in Flash memory.
			 User can add here some code to deal with this error */
			while (1)
				{
				Time_Parpadeo = 500;					// Prendo y apago el led de ERROR
				Time_Parpa = Time_Parpadeo;
		    	Clr_LCD();													/* Borro el LCD */
		    	Print_LCD(0,0,(int8_t *) "    Phenix  2025    ");
		    	Print_LCD(0,1,(int8_t *) " ERROR  Copia Flash ");
				}
			}
		}
	/* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
	HAL_FLASH_Lock();

 	/* Check if the programmed data is OK
		MemoryProgramStatus = 0: data programmed correctly
		MemoryProgramStatus != 0: number of words not programmed correctly ******
		Start_Address_Flash = FLASH_USER_START_ADDR;
		MemoryProgramStatus = 0x0;
	*/

	Start_Address_Ram   = RAM_CCM_START_ADDR;			// Comienzo de la Ram a guardar
	Start_Address_Flash = FLASH_USER_START_ADDR;		// Comienzo de la Flash a grabar
	MemoryProgramStatus = 0;

	while(Start_Address_Flash < FLASH_USER_END_ADDR)
		{
		DatoFlash = *(uint8_t *)Start_Address_Flash;
		Dato8     = *(uint8_t *)Start_Address_Ram;		//Address_Dato;

		if (Dato8 != DatoFlash)
			{
			MemoryProgramStatus++;
			}
		Start_Address_Flash = Start_Address_Flash + 1;
		Start_Address_Ram++;							//Address_Dato = Address_Dato + 1;
		}

	/*	Check if there is an issue to program data  */
	if(MemoryProgramStatus == 0)
		Flg_ErrorFlash = false;
	else
		{
		Time_Parpadeo = 500;					// Prendo y apago el led de ERROR
		Time_Parpa = Time_Parpadeo;
    	Clr_LCD();													/* Borro el LCD */
    	Print_LCD(0,0,(int8_t *) "    Phenix  2025    ");
    	Print_LCD(0,1,(int8_t *) " ERROR Verif. Flash ");
		Flg_ErrorFlash = true;
		}
	}

/*----------------------------------------------------------------------------
*  Rutina 		: void init_Variables_Config(void)
*  Objetivo 	: Inicialización general de las variables de configuración
*  					guardadas en Flash
*  Entrada  	:
*  Descripcion 	:	Los valores por defecto se definen en Definiciones.h
*  					Al llamar a la funcion se pasa el parametro TRUE o FALSE
*  					TRUE: graba en FLASH los valores de las variables almacenados en RAM
*  					FALSE: lee de la FLASH los valores y los almacena en RAM
*
*/
void Init_Variables_Config(void)
	{
	Print_LCD(15,2,(int8_t *) "Grab");
	Time_Out_G = 500;						// pongo el Time Out en 0,5 seg
	Flag_Grabo = true;						// Activo el flag para activar el parpadeo
	Time_Parpadeo = 80;
	Time_Parpa = Time_Parpadeo;
	/* Variables Públicas de la configuracion de la maquina que se graban en Flash */
	if(Firma_FL != Firma_Def) 				// Si la Firma no es igual a la definida en FLASH
		{
		Print_LCD(14,2,(int8_t *) "D>FCCMB");
		Grabo_Def_to_FL();					// Inicializo toda la flash, Ram CCM y Ram BAckUp
		}									// con los valores originales definidos.
	else if(Numero_Grabacion_FL == boot_count_Bk)// Si el Nº de la flash es == al Nº de la Ram BackUp
			{
			Print_LCD(14,2,(int8_t *) "F>CCMB");
			Grabo_FL_to_CCM();				// levanto los datos de la FLASH y los pongo en la ram CCM y en la Ram BackUp
			Write_Ram_BK();					// Guardo los datos de la ram CCM en la back SRAM (Es redundante)
			}
		else 								// El Nº de la Numero_Grabacion_FL es != a boot_count_Bk
			{
			Print_LCD(14,2,(int8_t *) "B>CCMF");
			Grabo_FL_to_CCM();				// levanto los datos de la FLASH y los pongo en la ram CCM
			Grabo_BackUp_to_CCM();			// Guardo los datos de la back SRAM en la CCM
			write_Pagina();					// Actualizo la Flash
		}

	Time_Out_G = 2000;						// pongo el Time Out en 2 seg
	while(Time_Out_G > 0);					// Espero para ver
	Print_LCD(14,2,(int8_t *) "  OK  ");
	Time_Out_G = 1000;						// pongo el Time Out en 1 seg.
	while(Time_Out_G > 0);					// Espero para ver
	Flag_Grabo = false;						// Desactivo el flag para activar el parpadeo
	Time_Parpadeo = 80;
	}

/*----------------------------------------------------------------------------
*  Rutina   	: void Grabo_Def_to_FL(void)
*  Objetivo 	: Inicialización general de las variables de configuración
*  				   guardadas en Flash
*  Entrada  	:
*  Descripcion 	:	Los valores por defecto se definen en Definiciones.h
*
*/
void   Grabo_Def_to_FL(void)
	{
	int32_t Limpio_Ram;
	int16_t x;
	int16_t y;
	int16_t j;

	Time_Out_G = 1500;						// pongo el Time Out en 0,5 seg.
	while(Time_Out_G > 0)					// Espero para ver

	Limpio_Ram = RAM_CCM_START_ADDR;
	while(Limpio_Ram < RAM_CCM_END_ADDR)	// Limpio toda la memoria .ccmram
		{									// y .user_data_myram
		*(uint8_t*)Limpio_Ram = Null;
		Limpio_Ram++;
		}
	strcpy((char *)FirmaASE_CCM,(const char *) Firma_ASE_Def);	// Copio el dato de la firma
	Firma_CCM = Firma_Def;					// Firma numérica
											//	Coeficientes de las mediciones de Tensión

	// Datos de comunicación --- Los valores por defecto se definen en Definiciones.h
	Comu_Conf[Comu_PC].velocidad = velocidad_def_PC;		// Comunicación PC UART0
	Comu_Conf[Comu_PC].bits_data = bits_data_def_PC;
	Comu_Conf[Comu_PC].paridad = paridad_def_PC;
	Comu_Conf[Comu_PC].Tipo_Comu_H = Tipo_Comu_def_PC;

	Comu_Conf[Comu_PSalidas].velocidad = velocidad_def_PSal;// Comunicación Placas salida UART1
	Comu_Conf[Comu_PSalidas].bits_data = bits_data_def_PSal;
	Comu_Conf[Comu_PSalidas].paridad = paridad_def_PSal;
	Comu_Conf[Comu_PSalidas].Tipo_Comu_H = Tipo_Comu_def_PSal;

	Comu_Conf[Comu_ESP].velocidad = velocidad_def_ECP;		// Comunicación Módulo ECP8266 UART2
	Comu_Conf[Comu_ESP].bits_data = bits_data_def_ECP;
	Comu_Conf[Comu_ESP].paridad = paridad_def_ECP;
	Comu_Conf[Comu_ESP].Tipo_Comu_H = Tipo_Comu_def_ECP;

	// Inicializacion general de la maquina--- Los valores por defecto se definen en Definiciones.h
	Config.Numero_Maquina = Num_CPU;                 				// Numero de placa
	strcpy((char *)Config.n_serie,(const char *)Numero_CPU_ASCII);  // Numero de placa
	strcpy((char *)Config.Nodo,(const char *)Nodo_def);             // Numero de Nodo
	strcpy((char *)Config.denominacion,(const char *)Nombre_CPU);   // Nombre de la placa CPU
	strcpy((char *)Config.n_version,(const char *)Vers_CPU);        // Numero de versiÛn de la CPU
	strcpy((char *)Config.NumDisco,(const char *)Num_Disco);      	// Número de Disco de la PC

	strcpy((char *)Config.f_instalacion,(const char *)Fecha_Inicial); 	// Fecha de instalacion
	strcpy((char *)Config.n_lineasASCII,(const char *)MaxLineasASCII);	// Numero de lineas
	Config.n_lineas = MaxLineas;
	strcpy((char *)Config.n_salidasASCII,(const char *)SalidasASCII);  	// Numero de salidas
	Config.n_salidas = Salidas;
	strcpy((char *)Config.n_platillosASCII,(const char *)MaxPlatillosASCII);// Numero de platillos
	Config.n_platillos = MaxPlatillos;
	strcpy((char *)Config.n_etiqueASCII,(const char *)MaxEtiquetaASCII);// Numero de etiquetadoras
	Config.n_etique = MaxEtiqueta;
	strcpy((char *)Config.peso_patron,(const char *)PesoPatronASCII);  	// valor del peso patron
	Config.modo_bandeja = M_Bandeja;                					// Modo Normal sin bandeja "0"
	strcpy((char *)Config.error_taraASCII,(const char *)dif_taraASCII);	// Error aceptable de la tara

	Config.error_tara = dif_tara;
	Config.Cant_Pulsos_Sincro = Cant_Pul_Sincro;
	Config.Enable_AD = EnableAD;
	Config.Disable_AD = DisableAD;

	for(x = 0; x < Salidas ; x++)
		{
		Config.DisBox[x] = (x * 2) + 5;								// Distancias entre la celda y las salidas
		Tot_Frut_Salidas[x] = 0;									// borro los totalizadores de salidas
		}

	for(x = 0; x < MaxEtiqueta ; x++)
		{
		Config.DistEtiq[x] = 1 + x;									// Distancia de la celda a las etiquetadoras
		Config.SalidaEtiq[x] = x + Salidas + 1;						// Número de salida asignado a cada Etiquetadora
		}
	// Inicializo la estructura de las Placas de Salidas a 0
	for(x = 0; x < MaxLineas; x++)
		{
		for(y = 0; y < Max_Tamanos; y++)
			{
			E_Salidas_CCM[x][y].Cant_Boxes = 0;
			E_Salidas_CCM[x][y].Salida_Actual = 0;
			E_Salidas_CCM[x][y].Puntero=0;
			E_Salidas_CCM[x][y].Total_frutos = 0;
			for(j = 0; j < 4; j++)
				{
				E_Salidas_CCM[x][y].Sal[j] = 0;
				E_Salidas_CCM[x][y].Tot_Frutos_Box[j] = 10;
				}
			}
		}

	// Inicializo la variable E_Etiqueta a 0, Asignación de las etiquetadoras
	for(x = 0; x < MaxEtiqueta; x++)
		{
		for(y = 0; y < MaxLineas; y++)
			{
			for(j = 0; j < Max_Tamanos; j++)
				{
				E_Etiqueta_CCM[x][y][j] = 0;
				}
			}
		}

	// Inicializo los límites de las distintas líneas
	for(y = 0; y < MaxLineas; y++)
		{
		for(j = 0; j < Max_Tamanos; j++)
			{
            Tabla_Limites_CCM[j][y] = j * 30;	// le cargo una tabla cualquiera 0, 30, 60,.....660 gr
            }
        }

	// Inicializo el cero de la calibracion
	for(x = 0; x < 8; x++)
		{
		Cero_CH_CCM[x] = Cero_ini;
		}

	// Inicializo el coeficiente de la calibracion
	for(x = 0; x < 8; x++)
		{
		Coeficiente_CH_CCM[x] = Coeficiente_ini;
		}


	Fin_Ram_CCM = 123;
	Numero_Grabacion_CCM = 1;
	write_Pagina();								// Graba en FLASH los valores Iniciales de la CCM
	Write_Ram_BK();								// Guardo los datos de la ram CCM en la BackUp SRAM
	}

/************************************************************************/
/* Rutina   	: Grabo_FL_to_CCM
*  Objetivo 	: copio los datos de Flash a la Ram CCM y a la Ram BackUp
*  Entrada  	:
*  Descripcion 	:
*
*************************************************************************/
void Grabo_FL_to_CCM(void)
	{
	int16_t x;
	int16_t y;
	int16_t j;

	strcpy((char *)FirmaASE_CCM,(const char *) Firma_ASE_Def);	// Copio el dato de la firma
	Firma_CCM = Firma_FL;											// Firma numérica

	// Datos de comunicación
	Comu_Conf[Comu_PC].velocidad = Comu_Conf_FL[Comu_PC].velocidad;				// Primero carga la UART1
	Comu_Conf[Comu_PC].bits_data = Comu_Conf_FL[Comu_PC].bits_data;
	Comu_Conf[Comu_PC].paridad = Comu_Conf_FL[Comu_PC].paridad;
	Comu_Conf[Comu_PC].Tipo_Comu_H = Comu_Conf_FL[Comu_PC].Tipo_Comu_H;

	Comu_Conf[Comu_PSalidas].velocidad = Comu_Conf_FL[Comu_PSalidas].velocidad;	// Luego carga la UART2
	Comu_Conf[Comu_PSalidas].bits_data = Comu_Conf_FL[Comu_PSalidas].bits_data;
	Comu_Conf[Comu_PSalidas].paridad =  Comu_Conf_FL[Comu_PSalidas].paridad;
	Comu_Conf[Comu_PSalidas].Tipo_Comu_H = Comu_Conf_FL[Comu_PSalidas].Tipo_Comu_H;

	Comu_Conf[Comu_ESP].velocidad = Comu_Conf_FL[Comu_ESP].velocidad;			// Luego carga la UART2
	Comu_Conf[Comu_ESP].bits_data = Comu_Conf_FL[Comu_ESP].bits_data;
	Comu_Conf[Comu_ESP].paridad = Comu_Conf_FL[Comu_ESP].paridad;
	Comu_Conf[Comu_ESP].Tipo_Comu_H = Comu_Conf_FL[Comu_ESP].Tipo_Comu_H;


		// Inicializacion general de la maquina
		Config.Numero_Maquina = Config_FL.Numero_Maquina ;               // Numero de placa

	strcpy((char *)Config.n_serie,(const char *)Config_FL.n_serie);      // Numero de placa
	strcpy((char *)Config.Nodo,(const char *)Config_FL.Nodo);            // Numero de Nodo
	strcpy((char *)Config.denominacion,(const char *)Config_FL.denominacion); // Nombre de la placa CPU
	strcpy((char *)Config.n_version,(const char *)Config_FL.n_version);  // Numero de versiÛn de la CPU
	strcpy((char *)Config.NumDisco,(const char *)Config_FL.NumDisco);    // Número de Disco de la PC

	strcpy((char *)Config.f_instalacion,(const char *)Config_FL.f_instalacion);  	// Fecha de instalacion
	strcpy((char *)Config.n_lineasASCII,(const char *)Config_FL.n_lineasASCII);  	// Numero de lineas
	Config.n_lineas = Config_FL.n_lineas;
	strcpy((char *)Config.n_salidasASCII,(const char *)Config_FL.n_salidasASCII); 	// Numero de salidas
	Config.n_salidas = Config_FL.n_salidas;
	strcpy((char *)Config.n_platillosASCII,(const char *)Config_FL.n_platillosASCII);// Numero de platillos
	Config.n_platillos = Config_FL.n_platillos;
	strcpy((char *)Config.n_etiqueASCII,(const char *)Config_FL.n_etiqueASCII);		// Numero de etiquetadoras
	Config.n_etique = Config_FL.n_etique;
	strcpy((char *)Config.peso_patron,(const char *)Config_FL.peso_patron);   		// valor del peso patron
	Config.modo_bandeja = Config_FL.modo_bandeja;                					// Modo Normal sin bandeja "0"
	strcpy((char *)Config.error_taraASCII,(const char *)Config_FL.error_taraASCII); // Error aceptable de la tara

	Config.error_tara = Config_FL.error_tara;
	Config.Cant_Pulsos_Sincro = Config_FL.Cant_Pulsos_Sincro;
	Config.Enable_AD = Config_FL.Enable_AD;
	Config.Disable_AD = Config_FL.Disable_AD;

	for(x = 0; x < Salidas ; x++)
		{
		Config.DisBox[x] = Config_FL.DisBox[x];					// Distancias entre la celda y las salidas
		}

	for(x = 0; x < MaxEtiqueta ; x++)
		{
		Config.DistEtiq[x] = Config_FL.DistEtiq[x];				// Distancia de la celda a las etiquetadoras
		Config.SalidaEtiq[x] = Config_FL.SalidaEtiq[x];			// Número de salida asignado a cada Etiquetadora
		}

	// Inicializo la estructura de las Placas de Salidas
	for(x = 0; x < MaxLineas; x++)
		{
		for(y = 0; y < Max_Tamanos; y++)
			{
			E_Salidas_CCM[x][y].Cant_Boxes = E_Salidas_FL[x][y].Cant_Boxes;
			E_Salidas_CCM[x][y].Salida_Actual = E_Salidas_FL[x][y].Salida_Actual;
			E_Salidas_CCM[x][y].Puntero = E_Salidas_FL[x][y].Puntero;
			E_Salidas_CCM[x][y].Total_frutos = E_Salidas_FL[x][y].Total_frutos;
			for(j = 0; j < 4; j++)
				{
				E_Salidas_CCM[x][y].Sal[j] = E_Salidas_FL[x][y].Sal[j];
				E_Salidas_CCM[x][y].Tot_Frutos_Box[j] = E_Salidas_FL[x][y].Tot_Frutos_Box[j];
				}
			}
		}
	// Inicializo la variable E_Etiqueta a 0, AsignaciÛn de las etiquetadoras
	for(x = 0; x < MaxEtiqueta; x++)
		{
		for(y = 0; y < MaxLineas; y++)
			{
			for(j = 0; j < Max_Tamanos; j++)
				{
				E_Etiqueta_CCM[x][y][j] = E_Etiqueta_FL[x][y][j];
				}
			}
		}

	// Inicializo los lÌmites de las distintas lÌneas
	for(y = 0; y < MaxLineas; y++)
		{
		for(j = 0; j < Max_Tamanos; j++)
			{
			Tabla_Limites_BK[j][y] = Tabla_Limites_FL[j][y];			// le cargo una table cualquiera 0, 30, 60,.....660 gr
			}
		}

	// borro los totalizadores de salidas
	for(x = 0; x < Salidas; x++)
		{
		Tot_Frut_Salidas[x] = 0;
		}
	Numero_Grabacion_CCM = Numero_Grabacion_FL;

	// Recupero el cero de la calibracion
	for(x = 0; x < 8; x++)
		{
		Cero_CH_CCM[x] = Cero_CH_FL[x];
		}

	// Recupero el coeficiente de la calibracion
	for(x = 0; x < 8; x++)
		{
		Coeficiente_CH_CCM[x] = Coeficiente_CH_FL[x];
		}

	Fin_Ram_CCM = 123;

	}


/**************************************************************************************************/
/* ------------------------------------------------------------------------ */
/*      -------------- 	RUTINAS	 para usar la BKPSRAM   ---------------
 *   4-kBytes backup ram library for STM32F4xx -
 *   https://stm32f4-discovery.net/2014/12/library-45-interface-backup-sram-on-stm32f4/
  * ------------------------------------------------------------------------ */
/**
 *   Rutina          :  BKPSRAM_Init(void)
 *   Entrada         :  None
 *   Parametros      :  None
 *   Returns         :  None
 *   Preconditions   :  None
 *   Descripcion     :  Inicialización de los 4K de memoria Ram para BackUp.
*/
void BKPSRAM_Init1(void)
	{
	/* Enable PWR clock */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	/* Enable backup SRAM Clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_BKPSRAMEN;

	/* Allow access to backup domain */
	PWR->CR |= PWR_CR_DBP;

	/* Enable the Backup SRAM low power Regulator */
	/* This will allow data to stay when using VBat mode */
	PWR->CSR |= PWR_CSR_BRE;

	/* Backup domain SRAM enable, and with it, the regulator */
	/* Wait for backup regulator to be ready  */
	while ((PWR->CSR & PWR_CSR_BRR) == 0);
	}

/*
 *	Para inicializar a travez de la biblioteca HAL  *
   Habilitar el reloj de la interfaz de energía */
void BKPSRAM_Init(void)
	{
    /* DBP bit 1, habilitar el acceso al dominio de respaldo */
    HAL_PWR_EnableBkUpAccess();

	__HAL_RCC_PWR_CLK_ENABLE();

   /* Habilite el reloj SRAM de respaldo configurando el bit BKPSRAMEN en el
    * registro de habilitación de reloj periférico RCC AHB1 (RCC_AHB1ENR) */
   __HAL_RCC_BKPSRAM_CLK_ENABLE();

   /* La aplicación debe esperar a que se establezca el indicador Listo del
    * regulador de respaldo (BRR),lo que indica que los datos escritos en la
    * RAM permanecerán en modo de espera y VBAT. */
    HAL_PWREx_EnableBkUpReg();

    /*  DBP : Disable access to Backup domain */
    HAL_PWR_DisableBkUpAccess();
	}
 /*
 */

/**
 *   Rutina          :  Grabo_BackUp_to_CCM(void)
 *   Entrada         :  None
 *   Parametros      :  None
 *   Returns         :  None
 *   Preconditions   :  None
 *   Descripcion     :  Leo los datos de la BackUp SRAM y los paso a la CCM Ram
*/
void Grabo_BackUp_to_CCM(void)
	{
	uint16_t Indice;
	uint8_t Dato;
	uint16_t * Start_Address_Ram;
	uint16_t * End_Address_Ram;

	Start_Address_Ram = & Firma_CCM; 			// Comienzo de la Ram a grabar
	End_Address_Ram = & Fin_BK_CCM; 			// Fin de la Ram a grabar

	Indice = 0;
	while(Start_Address_Ram < End_Address_Ram)
		{
		Dato = BKPSRAM_Read8(Indice);		// Leo el dato de la BKPSRAM
		* Start_Address_Ram = Dato;			// Lo guardo en la Ram de trabajo
	    Indice++;							// Incremento el indice y dirección
		Start_Address_Ram++;				// donde lo guardo
		}
	}

/**
 *   Rutina          :  Write_Ram_BK(void)
 *   Entrada         :  None
 *   Parametros      :  None
 *   Returns         :  None
 *   Preconditions   :  None
 *   Descripcion     :  Guardo los datos de la ram en la back SRAM.
 *   					el tiempo de ejecución de la rutina es de
 *   		SOLO SE GUARDAN LOS SIGUIENTES DATOS
	 int16_t  Tabla_Limites[Max_Tamanos][MaxLineas] __attribute__((__section__(".backup_sram")));	 		    // 352 bytes largo. Para calcular los tamaños
	 struct   reg_Sal  	E_Salidas[MaxLineas][Max_Tamanos] __attribute__((__section__(".backup_sram")));	// 3168 bytes largo de la estructura
*/
void Write_Ram_BK(void)
	{
	uint8_t x;
	uint8_t y;
	uint8_t j;

	HAL_PWR_EnableBkUpAccess();							//enable PWR backup domain access (RTC,BKReg)
	/* Enable clock to BKPSRAM */
	__HAL_RCC_BKPSRAM_CLK_ENABLE();

	Firma_BK = Firma_CCM;
	boot_count_Bk = Numero_Grabacion_CCM;
	   /* Guardo la estructura de las Salidas  */
	    for(x = 0; x < MaxLineas; x++)
	        {
	        for(y = 0; y < Max_Tamanos; y++)
	            {
	        	E_Salidas_BK[x][y].Cant_Boxes = E_Salidas_CCM[x][y].Cant_Boxes;
	        	E_Salidas_BK[x][y].Salida_Actual = E_Salidas_CCM[x][y].Salida_Actual;
	        	E_Salidas_BK[x][y].Puntero = E_Salidas_CCM[x][y].Puntero;
	        	E_Salidas_BK[x][y].Total_frutos = E_Salidas_CCM[x][y].Total_frutos;
	            for(j = 0; j < 4; j++)
	                {
	            	E_Salidas_BK[x][y].Sal[j] = E_Salidas_CCM[x][y].Sal[j];
	            	E_Salidas_BK[x][y].Tot_Frutos_Box[j] = E_Salidas_CCM[x][y].Tot_Frutos_Box[j];
	                }
	            }
	        }
	    /* Guardo los lÌmites de las distintas lÌneas */
	    for(y = 0; y < MaxLineas; y++)
	    	{
	        for(x = 0; x < Max_Tamanos; x++)
	        	{
	        	Tabla_Limites_BK[x][y] = Tabla_Limites_CCM[x][y];
	            }
	    	}
		Fin_BK = Fin_Ram_CCM;
	/* Disable clock to BKPSRAM */
//	__HAL_RCC_BKPSRAM_CLK_DISABLE();
	HAL_PWR_DisableBkUpAccess();
	}

/****************************************************************/
/* Rutina   : Copy_ram
*  Objetivo : copio de un origen dato a un destino una cierta cant. de caracteres
*  Entrada  :
*  Descripcion :
*
***************************************************************
void Copy_ram(uint8_t *origen ,uint8_t *destino,int8_t longitud)
    {
    int8_t i;

    for(i = 0; i < longitud; i++)
	    *(destino + i) = *(origen + i);
    }
*/
