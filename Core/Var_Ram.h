/*-------------------------------------------------------------------------------------------------------
        DEFINICION DE PROTOTIPOS Y VARIABLES EN MEMORIA FLASH
---------------------------------------------------------------------------------------------------------*/
/*
    Fecha inicialización	:	28/06/2025
    Fecha actualización 	:	22/12/2025
    Realizado por	    	:	C.E. Colombo
    Compilador utilizado	:	ST - Eclipse IDE
    Proyecto	    		:	Phenix 2025 - 407VGT6
    Archivo		    		: 	Var_Ram.c
    Versión					:	3.00.00
    Objetivo				:	Definicion de las variables en memoria Ram
                                Definiciones generales de las distintas variables
                                en data_ram fijas para grabar también en la memoria Flash
*/
/*-----------------------------------------------------------------------------------------
            DEFINICION DE VARIABLES EN Ram_BackUp
-----------------------------------------------------------------------------------------*/
/* 		El micro STM32F407 tiene una Flash de 1Mb particionada en secciones de 16K, 64K y 128K
 * 		el inicio de la memoria ram esta en 		0x2000 0000
 * 		el inicio de la memoria Ram_BackUp  		0x4002 4000 	// Largo de 4K
 *
 *		 Variable stored in the memory section '.backup_sram'
 *
 *		 OJO Hay que agregar en el archivo *.Id
 *		MEMORY
            {
            CCMRAM      (xrw)    : ORIGIN = 0x10000000, LENGTH = 64K
            RAM         (xrw)    : ORIGIN = 0x20000000, LENGTH = 128K
            FLASH       (rx)     : ORIGIN = 0x8000000,  LENGTH = 900K
            USER_FLASH  (xrw)    : ORIGIN = 0x080E0000, LENGTH = 128K 		// xrw para permitir escritura, si es solo lectura, usa rx
            BKPSRAM     (xrw)    : ORIGIN = 0x40024000, LENGTH = 4K
            }
*
   * User data to be stored in the RAM memory goes into USER_Ram *
  .backup_sram : 				// Nueva sección para la Backup SRAM
      {
        . = ALIGN(4);
        *(.backup_sram) 			// Esto vinculará todas las variables con __attribute__((section(".backup_sram"))) aquí
      } >BKPSRAM

 *
        // Variable global sin inicializar (irá al .bss de BKPSRAM)
    uint32_t myBackupVar __attribute__((section(".backup_sram")));

        // Variable global inicializada (irá al .data de BKPSRAM, y necesitaría
        // una copia de inicio si no es NOLOAD, pero en BKPSRAM suele ser NOLOAD)
    uint16_t backupCounter __attribute__((section(".backup_sram"))) = 0xABCD;

     .ccmram :
      {
        . = ALIGN(4);
        _sccmram = .;       // create a global symbol at ccmram start
        *(.ccmram)
        *(.ccmram*)

        . = ALIGN(4);
        _eccmram = .;       // create a global symbol at ccmram end
      } >CCMRAM AT> FLASH

        // Variable global sin inicializar (irá al .bss de CCMRAM si no se inicializa en el startup)

    uint32_t ccmram_data_uninit __attribute__((section(".ccmram")));

        // Variable global inicializada (irá al .data de CCMRAM)
    uint16_t ccmram_status_word __attribute__((section(".ccmram"))) = 0xAA55;


    typedef struct
        {
        uint8_t  state;
        uint32_t counter;
        float    measurement;
        } CcmramConfig_t;

CcmramConfig_t fastConfig __attribute__((section(".ccmram")));

uint8_t ccmram_buffer[256] __attribute__((section(".ccmram"))); */

/* -------------------------      Includes      --------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "Definiciones.h"

/*------------------------------------------------------------
 * (contenido original de cabecera / comentarios)
 *------------------------------------------------------------*/

#ifndef __VAR_RAM_H
#define __VAR_RAM_H

/* ... includes y defines que ya tengas ... */

#if PosVariables != 0 // Variables externas

/***************************************************************...***************************************************************/
/*	+++			Sector reservado para las configuraciones generales de...kUp_Ram                                                       */
/***************************************************************...***************************************************************/

extern uint16_t Firma_BK __attribute__((__section__(".backup_sram")));                                   // Firma BackUp Ram
extern uint16_t boot_count_Bk __attribute__((__section__(".backup_sram")));                              // 2 bytes el Check Up de la Ram BackUp
extern int16_t Tabla_Limites_BK[Max_Tamanos][MaxLineas] __attribute__((__section__(".backup_sram")));    // 352 bytes largo. Para calcular los tamaños
extern struct reg_Sal E_Salidas_BK[MaxLineas][Max_Tamanos] __attribute__((__section__(".backup_sram"))); // 3168 bytes largo de la estructura
extern uint16_t Fin_BK __attribute__((__section__(".backup_sram")));                                     // Fin BackUp Ram

/***************************************************************...***************************************************************/
/*	+++			Sector reservado para las configuraciones generales de...n SRAM-CCM                                                     */
/***************************************************************...***************************************************************/

extern uint16_t Firma_CCM __attribute__((__section__(".ccmram")));                                   // Firma SRAM-CCM
extern uint16_t Numero_Grabacion_CCM __attribute__((__section__(".ccmram")));                        // 2 bytes el Check Up de la Ram BackUp
extern int16_t Tabla_Limites_CCM[Max_Tamanos][MaxLineas] __attribute__((__section__(".ccmram")));    // 352 bytes largo. Para calcular los tamaños
extern struct reg_Sal E_Salidas_CCM[MaxLineas][Max_Tamanos] __attribute__((__section__(".ccmram"))); // 3168 bytes largo de la estructura
extern uint16_t Fin_BK_CCM __attribute__((__section__(".ccmram")));                                  // Fin SRAM-CCM = BackUp Ram

extern int16_t E_Etiqueta_CCM[MaxEtiqueta][MaxLineas][Max_Tamanos] __attribute__((__section__(".ccmram"))); // 704 bytes
extern int8_t FirmaASE_CCM[22] __attribute__((__section__(".ccmram")));                                     // Firma del equipo

extern struct Comunicacion_conf Comu_Conf[3] __attribute__((__section__(".ccmram"))); // Configuración Gral de las series
extern struct Config_Maquina Config __attribute__((__section__(".ccmram")));          // Configuración de Máquina
extern uint16_t Cero_CH_CCM[8] __attribute__((__section__(".ccmram")));               // Cero en cuentas de cada canal
extern uint32_t Coeficiente_CH_CCM[8] __attribute__((__section__(".ccmram")));        // Coeficiente de cada canal
extern uint16_t Fin_Ram_CCM __attribute__((__section__(".ccmram")));                  // Final de grabación

/**------------------------------------------------------------------------------------------------------------------------
            VARIABLES DE INTERRUPCIONES DE ENTRADAS RELACIONADAS CON LA MEDICION Y PROCESAMIENTO DEL PESO
--------------------------------------------------------------------------------------------------------------------------*/
extern uint_fast16_t Canal_AD[8][250] __attribute__((__section__(".ccmram")));               // Canal medido por al AD
extern volatile uint_fast16_t Canal_AD_check[8][50] __attribute__((__section__(".ccmram"))); // Canal medido por al AD
extern uint_fast32_t Peso_Bruto_CH[8] __attribute__((__section__(".ccmram")));               // Peso Bruto en cuentas de cada canal
extern int_fast32_t Peso_Neto_CH[8] __attribute__((__section__(".ccmram")));                 // Peso Neto en cuentas de cada canal
extern volatile uint_fast64_t Suma_Canal_AD[8] __attribute__((__section__(".ccmram")));      // Suma acumulada de mediciones por canal del AD
extern volatile uint_fast32_t Cantidad_Mediciones __attribute__((__section__(".ccmram")));   // Cantidad de mediciones realizadas
extern uint_fast16_t Tara[8][500] __attribute__((__section__(".ccmram")));                   // Taras de toda la maquina (8 lineas x 500 platillos)
extern volatile uint_fast16_t Velocidad_medida __attribute__((__section__(".ccmram")));      // Velocidad de la maquina, Cantidad de frutos por seg.
extern volatile uint_fast32_t Chk_Coeficiente __attribute__((__section__(".ccmram")));       // Chequeo del coeficiente de correcion de peso
extern volatile int_fast8_t State __attribute__((__section__(".ccmram")));                   // Flag para medir velocidad
extern volatile int_fast16_t Conta_Velo __attribute__((__section__(".ccmram")));             // Contador tiempo para calcular la velocidad en mseg.
extern volatile int_fast16_t Conta_Velo_Maq __attribute__((__section__(".ccmram")));         // Contador tiempo para calcular la velocidad en mseg.
extern volatile int_fast16_t Conta_Sincro __attribute__((__section__(".ccmram")));           // Contador de los pulsos de sincronismo
extern volatile int_fast16_t Conta_Ejes __attribute__((__section__(".ccmram")));             // Contador de platos o ejes
extern volatile int_fast16_t Cant_Ejes_Med __attribute__((__section__(".ccmram")));          // Cantidad de Ejes medidos
extern volatile int_fast16_t Cant_Pulsos_Med __attribute__((__section__(".ccmram")));        // Cantidad de Pulsos recibidos del encoder
extern volatile uint_fast8_t Aux_celda __attribute__((__section__(".ccmram")));              // Auxiliar para guardar celda a calibrar/ver cuentas
extern volatile uint16_t Cant_Pulsos __attribute__((__section__(".ccmram")));                // Cantidad de pulsos a medir en modo osciloscopio

extern volatile int_fast8_t Flag_Maq_Enable __attribute__((__section__(".ccmram")));   // Maquina habilitada
extern volatile int_fast8_t Flag_Maq_Fun __attribute__((__section__(".ccmram")));      // Maquina en funcionamiento
extern volatile int_fast8_t Flag_Sinc_eje __attribute__((__section__(".ccmram")));     // Flag para comenzar a contar pulsos
extern volatile int_fast8_t Flag_Secuencia __attribute__((__section__(".ccmram")));    // Secuencia de pulsos correcta
extern volatile int_fast8_t Flag_SecuenEje __attribute__((__section__(".ccmram")));    // Secuencia de pulsos correcta
extern volatile int_fast8_t Flag_SecuEje1 __attribute__((__section__(".ccmram")));     // Secuencia de pulsos correcta
extern volatile int_fast8_t Flag_Convert_ON __attribute__((__section__(".ccmram")));   // Se mandó a convertir
extern volatile int_fast8_t Flag_Conv_FIN __attribute__((__section__(".ccmram")));     // Finalizó conversión
extern volatile int_fast8_t Flag_Ver_Cuentas __attribute__((__section__(".ccmram")));  // Modo ver cuentas
extern volatile int_fast8_t Flag_Osciloscopio __attribute__((__section__(".ccmram"))); // Modo osciloscopio
extern volatile int_fast8_t Flag_Oscilos_End __attribute__((__section__(".ccmram")));  // Fin osciloscopio
extern volatile int_fast8_t Flag_Tara_OK __attribute__((__section__(".ccmram")));      // Toma de taras OK

#else // Variables propias

/***************************************************************...***************************************************************/
/*	+++			Sector reservado para las configuraciones generales de...kUp_Ram                                                       */
/***************************************************************...***************************************************************/
uint16_t Firma_BK __attribute__((__section__(".backup_sram")));                                   // Firma SRAM-CCM
uint16_t boot_count_Bk __attribute__((__section__(".backup_sram")));                              // 2 bytes el Check Up de la Ram BackUp
int16_t Tabla_Limites_BK[Max_Tamanos][MaxLineas] __attribute__((__section__(".backup_sram")));    // 352 bytes largo. Para calcular los tamaños
struct reg_Sal E_Salidas_BK[MaxLineas][Max_Tamanos] __attribute__((__section__(".backup_sram"))); // 3168 bytes largo de la estructura
uint16_t Fin_BK __attribute__((__section__(".backup_sram")));                                     // Fin BackUp Ram

/***************************************************************...***************************************************************/
/*	+++			Sector reservado para las configuraciones generales de...n SRAM-CCM                                                     */
/***************************************************************...***************************************************************/
uint16_t Firma_CCM __attribute__((__section__(".ccmram")));                                   // Firma SRAM-CCM
uint16_t Numero_Grabacion_CCM __attribute__((__section__(".ccmram")));                        // 2 bytes el Check Up de la Ram BackUp
int16_t Tabla_Limites_CCM[Max_Tamanos][MaxLineas] __attribute__((__section__(".ccmram")));    // 352 bytes largo. Para calcular los tamaños
struct reg_Sal E_Salidas_CCM[MaxLineas][Max_Tamanos] __attribute__((__section__(".ccmram"))); // 3168 bytes largo de la estructura
uint16_t Fin_BK_CCM __attribute__((__section__(".ccmram")));                                  // Fin SRAM-CCM = BackUp Ram

int16_t E_Etiqueta_CCM[MaxEtiqueta][MaxLineas][Max_Tamanos] __attribute__((__section__(".ccmram"))); // 704 bytes
int8_t FirmaASE_CCM[22] __attribute__((__section__(".ccmram")));                                     // Firma del equipo

struct Comunicacion_conf Comu_Conf[3] __attribute__((__section__(".ccmram"))); // Configuración Gral de las series
struct Config_Maquina Config __attribute__((__section__(".ccmram")));          // Configuración de Máquina
uint16_t Cero_CH_CCM[8] __attribute__((__section__(".ccmram")));               // Cero en cuentas de cada canal
uint32_t Coeficiente_CH_CCM[8] __attribute__((__section__(".ccmram")));        // Coeficiente de cada canal
uint16_t Fin_Ram_CCM __attribute__((__section__(".ccmram")));                  // Final de grabación

/**------------------------------------------------------------------------------------------------------------------------
            VARIABLES DE INTERRUPCIONES DE ENTRADAS RELACIONADAS CON LA MEDICION Y PROCESAMIENTO DEL PESO
--------------------------------------------------------------------------------------------------------------------------*/
uint_fast16_t Canal_AD[8][250] __attribute__((__section__(".ccmram")));               // Canal medido por al AD
volatile uint_fast16_t Canal_AD_check[8][50] __attribute__((__section__(".ccmram"))); // Canal medido por al AD
uint_fast32_t Peso_Bruto_CH[8] __attribute__((__section__(".ccmram")));               // Peso Bruto en cuentas de cada canal
int_fast32_t Peso_Neto_CH[8] __attribute__((__section__(".ccmram")));                 // Peso Neto en cuentas de cada canal
volatile uint_fast64_t Suma_Canal_AD[8] __attribute__((__section__(".ccmram")));      // Suma acumulada de mediciones por canal del AD
volatile uint_fast32_t Cantidad_Mediciones __attribute__((__section__(".ccmram")));   // Cantidad de mediciones realizadas
uint_fast16_t Tara[8][500] __attribute__((__section__(".ccmram")));                   // Taras de toda la maquina (8 lineas x 500 platillos)
volatile uint_fast16_t Velocidad_medida __attribute__((__section__(".ccmram")));      // Velocidad de la maquina, Cantidad de frutos por seg.
volatile uint_fast32_t Chk_Coeficiente __attribute__((__section__(".ccmram")));       // Chequeo del coeficiente de correcion de peso
volatile int_fast8_t State __attribute__((__section__(".ccmram")));                   // Flag para medir velocidad
volatile int_fast16_t Conta_Velo __attribute__((__section__(".ccmram")));             // Contador tiempo para calcular la velocidad en mseg.
volatile int_fast16_t Conta_Velo_Maq __attribute__((__section__(".ccmram")));         // Contador tiempo para calcular la velocidad en mseg.
volatile int_fast16_t Conta_Sincro __attribute__((__section__(".ccmram")));           // Contador de los pulsos de sincronismo
volatile int_fast16_t Conta_Ejes __attribute__((__section__(".ccmram")));             // Contador de platos o ejes
volatile int_fast16_t Cant_Ejes_Med __attribute__((__section__(".ccmram")));          // Cantidad de Ejes medidos
volatile int_fast16_t Cant_Pulsos_Med __attribute__((__section__(".ccmram")));        // Cantidad de Pulsos recibidos del encoder
volatile uint_fast8_t Aux_celda __attribute__((__section__(".ccmram")));              // Auxiliar para guardar celda a calibrar/ver cuentas
volatile uint16_t Cant_Pulsos __attribute__((__section__(".ccmram")));                // Cantidad de pulsos a medir en modo osciloscopio

volatile int_fast8_t Flag_Maq_Enable __attribute__((__section__(".ccmram")));   // Maquina habilitada
volatile int_fast8_t Flag_Maq_Fun __attribute__((__section__(".ccmram")));      // Maquina en funcionamiento
volatile int_fast8_t Flag_Sinc_eje __attribute__((__section__(".ccmram")));     // Flag para comenzar a contar pulsos
volatile int_fast8_t Flag_Secuencia __attribute__((__section__(".ccmram")));    // Secuencia de pulsos correcta
volatile int_fast8_t Flag_SecuenEje __attribute__((__section__(".ccmram")));    // Secuencia de pulsos correcta
volatile int_fast8_t Flag_SecuEje1 __attribute__((__section__(".ccmram")));     // Secuencia de pulsos correcta
volatile int_fast8_t Flag_Convert_ON __attribute__((__section__(".ccmram")));   // Se mandó a convertir
volatile int_fast8_t Flag_Conv_FIN __attribute__((__section__(".ccmram")));     // Finalizó conversión
volatile int_fast8_t Flag_Ver_Cuentas __attribute__((__section__(".ccmram")));  // Modo ver cuentas
volatile int_fast8_t Flag_Osciloscopio __attribute__((__section__(".ccmram"))); // Modo osciloscopio
volatile int_fast8_t Flag_Oscilos_End __attribute__((__section__(".ccmram")));  // Fin osciloscopio
volatile int_fast8_t Flag_Tara_OK __attribute__((__section__(".ccmram")));      // Toma de taras OK

#endif

#endif /* __VAR_RAM_H */
