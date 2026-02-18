/*
 * 		Drivers de la máquina
 *
    Fecha inicialización:	01/05/2022
    Fecha actualización :	05/08/2025
    Realizado por	    :	C.E. Colombo
    Compilador utilizado:	ST - Eclipse IDE
    Proyecto	    	:	Phenix 2025
    Archivo		    	: 	Drivers.c
    Versión	   	    	:	1.30.00
    Objetivo	    	:
                Manejo del display 4x40 LCD						ok  8Hs.
                Manejo del teclado de 16/20 teclas 				ok 15Hs.
                Manejo del reloj								ok	8Hs.
                Manejo el sonido de la tecla normal				ok
                Manejo el sonido del error de tecla				ok
                Manejo el sonido del error grave				ok
*/

/* Includes ------------------------------------------------------------------*/
#define PosVariables 1 /* 	0 = Variables propias \
                           1 = Variables externas */

/* USER CODE BEGIN Includes */
#include "main.h"
#include "Variables.h"
#include "Def_LCD.h"
#include <stdint.h>

extern int8_t *str_time(int8_t Actualizar);

/* Private function prototypes -----------------------------------------------*/
/*	LCD			*/
void Ini_LCD(void);
void cmd_ini(uint16_t cmd);
unsigned int status_display(void);
void Clr_LCD(void);
void Print_LCD(uint8_t col, uint8_t fila, int8_t *cadena);
void data_display(uint16_t dato);
void DATA_IN(void);
void DATA_OUT(void);

/*	Teclado			*/
void InitTec(void);
void ScanTec(void);
void Guardo_tecla(uint8_t Tecla);
int8_t Teclado(void);

/*	Rutinas varias		*/
void Beep(void);                      /* Activar Buzzer */
void error_qom(void);                 /* Error de comando o tecla */
void fatal_error_Beep(void);          /* Error grave */
void delay_us(uint32_t microseconds); // Delay en microsegundos por software (NO usa interrupciones)

/*	Rutinas Reloj		*/
int8_t *str_date(int8_t Actualizar);
int8_t *str_time(int8_t Actualizar);
void Pongo_Hora(int8_t Hora, int8_t renglon);

//  void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1);
void Leo_AD(int8_t Canal);
void config_ext_channel_ADC(int8_t channel);

/*****************************************************************************
               RUTINAS DE MANEJO DEL DISPLAY LCD
                 D I S P L A Y   L C D  4 X 20
******************************************************************************
*  Rutina   	: Ini_LCD()
*  Objetivo 	: inicializar el display LCD
*  Entrada  	: -------
*  Comentarios	:
*
* La inicializacion del display LCD consiste en una serie de comandos especificos enviados al modulo
* en una secuencia determinada y con intervalos de espera entre ellos.
*
  COMANDOS					DESCRIPCION
                  7   6   5   4   3   2   1   0
                --------------------------------
DISPLAY CLEAR   | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 |
                ---------------------------------

                  7   6   5   4   3   2   1   0
                ---------------------------------
CURSOR HOME     | 0 | 0 | 0 | 0 | 0 | 0 | 1 | X |
                ---------------------------------

                  7   6   5   4   3   2   1   0
                ---------------------------------
ENTRY MODE      | 0 | 0 | 0 | 0 | 0 | 1 |I/D| SH|
                ---------------------------------
                                           |   |---> 1: display no shiftea
                                           |-------> 1: escribe hacia la izquierda

                  7   6   5   4   3   2   1   0
                ---------------------------------
DISPLAY ON/OFF  | 0 | 0 | 0 | 0 | 1 | D | C | B |
                ---------------------------------
                                      |   |   |---> 1: blink on
                                      |   |-------> 1: cursor on
                                      |-----------> 1: display on

                  7   6   5   4   3   2   1   0
                ---------------------------------
SHIFT           | 0 | 0 | 0 | 1 |S/C|R/L| X | X |   para shiftear display o cursor
                ---------------------------------
                                  |   |-----------> 1: shift right
                                  |---------------> 1: display shift; 0: cursor move

                  7   6   5   4   3   2   1   0
                ---------------------------------
SET FUNCTION    | 0 | 0 | 1 | DL| N | F | X | X |
                ---------------------------------
                              |   |   |-----------> 1: 5x10 dots; 0: 5x7 dots
                              |   |---------------> 1: 2 lineas; 0: 1 linea
                              |-------------------> 1: 8 bits; 0: 4 bits

                  7   6   5   4   3   2   1   0
                ---------------------------------
POSICIONAR      | 1 | A6| A5| A4| A3| A2| A1| A0|
                ---------------------------------
                     \__________________________/
                          direccion cursor

*/
void Ini_LCD(void)
    {
    if (Flg_Display == true) // Si tengo display lo habilito
        {
        Time_Out_disp = 10; // demora > 10 ms
        while (Time_Out_disp > 0)
            ; // Esta primera demora es para el power on

        cmd_ini(Func_set2); // Sistem Set, 8 Bits;2lineas;5x10dots
        Time_Out_disp = 10; // demora > 10 ms
        while (Time_Out_disp > 0)
            ;

        cmd_ini(Func_set2); // Sistem Set, 8 Bits;2lineas;5x10dots
        Time_Out_disp = 10; // demora > 10 ms
        while (Time_Out_disp > 0)
            ;

        cmd_ini(Func_set2); // Sistem Set, 8 Bits;2lineas;5x10dots
        Time_Out_disp = 10; // demora > 10 ms
        while (status_display())
            ; // Espera a que este disponible

        cmd_ini(Func_set2); // Sistem Set, 8 Bits;2lineas;5x10dots
        Time_Out_disp = 10; // demora > 10 ms
        while (status_display())
            ; // Espera a que este disponible  demora > 50 ms

        cmd_ini(Disp_off); // Display OFF,Cursor OFF,Blink Cursor OFF
        Time_Out_disp = 5; // demora > 5 ms
        while (status_display())
            ; // Espera a que este disponible  demora > 5 ms

        cmd_ini(Set);      // Entry mode set
        Time_Out_disp = 5; // demora > 5 ms
        while (status_display())
            ; // Espera a que este disponible  demora > 5 ms

        cmd_ini(Clear);    // Clear display y retorne el cursor a su posicion
        Time_Out_disp = 5; // demora > 5 ms
        while (status_display())
            ; // Espera a que este disponible  demora > 5 ms

        cmd_ini(Home);     // Lleva el cursor al primer caracter y deja el AC en cero
        Time_Out_disp = 5; // demora > 5 ms
        while (status_display())
            ; // Espera a que este disponible  demora > 5 ms

        cmd_ini(Disp_on);  // Display ON,Cursor OFF,Blink Cursor OFF
        Time_Out_disp = 5; // demora > 5 ms
        while (status_display())
            ; // Espera a que este disponible  demora > 5 ms
        }
    }

/*------------------------------------------------------------------------------------------------------------------
 *  Rutina   	:	cmd_ini()
 *  Objetivo 	: 	Carga un comando en el display LCD
 *  Entrada  	: 	cmd = comando para cargar en el display LCD
 *  Comentarios	: 	El display interpreta que es un comando cuando el RS = 0.
 */
void cmd_ini(uint16_t cmd)
    {
    DATA_OUT();                               // direcciono el port PD8....PD15 como salida
    LCD_RS_GPIO_LOW();                        // Pongo a "0" RS
    LCD_WR_GPIO_LOW();                        // Pongo a "0" R/!W
    LCD_EN_GPIO_HIGH();                       // Pongo a "1" E
    GPIOD->ODR = (GPIOD->ODR & 0x00ff) | cmd; // escribo el comando a cargar
    Time_Out_disp = 1;                        // demora < 1 ms
    while (Time_Out_disp > 0)
        ;               // demora
    LCD_EN_GPIO_LOW();  // Pongo a "0" E
    LCD_RS_GPIO_HIGH(); // Pongo a "1" RS
    LCD_WR_GPIO_HIGH(); // Pongo a "1" R/!W
    DATA_IN();          // direcciono el port PD8....PD15 como entrada
    }
/*-----------------------------------------------------------------------------------------------------------------
 *  Rutina   	:    status_display()
 *  Objetivo 	: 	Lee el estado del display LCD
 *  Entrada  	:
 *  Comentarios	: 	el flag de busy esta en el bit mas significativo de la palabra leida
 *  Retorna		:  1 si el display esta ocupado, 0 si puede ser accedido
 */
unsigned int status_display()
    {
    uint16_t dato = 0; // Variable local para retornar el estado del display

    DATA_IN();                                         // direcciono el port PF0...PF7 como entrada
    LCD_RS_GPIO_LOW();                                 // Pongo a "0" RS
    LCD_WR_GPIO_HIGH();                                // Pongo a "1" R/!W
    LCD_EN_GPIO_HIGH();                                // Pongo a "1" E
    while (dato == Busy_flg_disp && Time_Out_disp > 0) // leer el estado del display y sale si el dato es 0,
        {                                              // o si paso el Time out fijado
        dato = (GPIOD->IDR) & 0x8000;
        }
    LCD_EN_GPIO_LOW();  // Pongo a "0" E
    LCD_RS_GPIO_HIGH(); // Pongo a "1" RS

    if (Time_Out_disp <= 0) // Time Out de display ocupado
        dato = 0;           // Si paso el Time out fijado => sale de la espera aunque no lea el estado del display

    return (dato); // retorna el dato del bit de estado
    }
/*------------------------------------------------------------------------------------------------------------------
 *  Rutina   	:    	Clr_LCD(void)
 *  Objetivo 	: 		Borrado del Display LCD y cursor en 0,0
 *  Entrada  	:
 *  Comentarios	:
 *  Retorna		:
 */
void Clr_LCD(void)
    {
    if (Flg_Display == true) // Si tengo display lo habilito
        {
        cmd_ini(Clear);    // Clear display y retorne el cursor a su posicion
        Time_Out_disp = 2; // demora > 1 ms
        while (status_display())
            ; // Espera a que este disponible
        }
    }

/*------------------------------------------------------------------------------------------------------------------
*  Rutina   	:   Print_LCD(uint8_t col,uint8_t fila,uint8_t *cadena)
*  Objetivo 	: 	escribir en el display
*  Entrada  	: 	fila (0..1), columna (0..19), puntero al string a escribir
*  Comentario	: 	primero posiciona el cursor al principio del renglon 1
                    Esta funcion manda al display una cadena ASCII terminada en NULL.
                    Se pasan como parámetros la columna X y la fila Y donde comenzar
                    a imprimirse la cadena.
                    X: 0...19 (col)
                    Y: 0 - 1 (fila)
*/
void Print_LCD(uint8_t col, uint8_t fila, int8_t *cadena)
    {
    uint16_t pos;

    if (Flg_Display == true) // Si tengo display lo habilito
        {
        Time_Out_disp = 5; // demora > 5 ms
        while (status_display())
            ; // Espera a que este disponible  demora > 5 ms

        if (fila == 0 || fila == 1)
            {
            pos = ((fila * LONGFILA) + col) << 8; // primero posiciono el cursor
            cmd_ini(Ddram_low + pos);
            }
        else
            {
            pos = (((fila - 2) * LONGFILA) + col) << 8; // primero posiciono el cursor
            cmd_ini(Ddram_mid + pos);
            }

        Time_Out_disp = 5; // demora > 5 ms
        while (status_display())
            ; // Espera a que este disponible  demora > 5 ms

        while (*cadena != Null)
            data_display(*(cadena++));
        }
    }
/*------------------------------------------------------------------------------------------------------------------
 *  Rutina   	:   data_display(dato)
 *  Objetivo 	: 	Carga un dato en el display LCD
 *  Entrada  	: 	dato (en ASCII)
 *  Comentarios	:   El display interpreta que es un dato cuando el RS = 1.
 */
void data_display(uint16_t dato)
    {
    Time_Out_disp = 5; // demora > 5 ms
    while (status_display())
        ;                                             // espera a que este disponible
                                                      // preparo para escribir
    DATA_OUT();                                       // direcciono el port PD8...PD15 como salida
    LCD_RS_GPIO_HIGH();                               // Pongo a "1" RS
    LCD_WR_GPIO_LOW();                                // Pongo a "0" R/!W
    LCD_EN_GPIO_HIGH();                               // Pongo a "1" E
    GPIOD->ODR = (GPIOD->ODR & 0x00ff) | (dato << 8); // escribo el dato a mostrar
    Time_Out_disp = 1;                                // demora < 1 ms
    while (Time_Out_disp > 0)
        ;               // demora
    LCD_EN_GPIO_LOW();  // Pongo a "0" E
    LCD_WR_GPIO_HIGH(); // Pongo a "1" R/!W
    DATA_IN();          // direcciono el port PD como entrada
    }

/*------------------------------------------------------------------------------------------------------------------
*  Rutina   :   DATA_IN(void)
*  Objetivo :   Pone el port de los datos del LCD como entrada
*  Entrada  :
*  				Configure GPIO pins : 	DD0_Pin DD1_Pin DD2_Pin DD3_Pin
                                        DD4_Pin DD5_Pin DD6_Pin DD7_Pin
                Port PD
**/
void DATA_IN(void)
    {
    GPIO_InitStruct.Pin = DD0_Pin | DD1_Pin | DD2_Pin | DD3_Pin | DD4_Pin | DD5_Pin | DD6_Pin | DD7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    }
/*------------------------------------------------------------------------------------------------------------------
*  Rutina   	:   DATA_OUT(void)
*  Objetivo 	: 	Pone el port de los datos del LCD como salida
*  Entrada  	:
*  				Configure GPIO pins : 	DD0_Pin DD1_Pin DD2_Pin DD3_Pin
                                        DD4_Pin DD5_Pin DD6_Pin DD7_Pin
                Port PD
*/
void DATA_OUT(void)
    {
    GPIO_InitStruct.Pin = DD0_Pin | DD1_Pin | DD2_Pin | DD3_Pin | DD4_Pin | DD5_Pin | DD6_Pin | DD7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    }

/*-----------------------------------------------------------------------------------------------------------------
               D R I V E R   D E   T E C L A D O
-------------------------------------------------------------------------------------------------------------------
El equipo tiene una matriz de teclado 4x5, 20 teclas.
EL TECLADO SE DEBE BARRER CON "0" en la Fila Y SI LEE UN "0" en la columna	=> HAY TECLA PRESIONADA
Ojo se barre del PA.5 a PA.2 (de derecha a izquierda)

FILAS 	 = GPIOA  		(PA.2/PA.3/PA.4/PA.5)		   SALIDAS
COLUMNAS = GPIOA 	  	(PA.6/PA.7/PA.8/PA9/PA.10) ENTRADAS

ALGORITMO PARA OBTENER EL CODIGO DE LA TECLA PRESIONADA EN BASE A LA FILA Y COLUMNA
TECLA = VALOR_ARB + FILA

El valor arbitrario se obtiene por simple inspeccion del valor de la columna.
Luego se le suma el valor de la fila correspondiente.

       FILAS    		PA2		PA3		PA4		PA5
     COLUMNA
    PA6 	  Col0   0	 0		 1		 2		 3		--> CODIGO DE LA TECLA OBTENIDO = VALOR_ARB + NUM_Columna
    PA7  	  Col1   4	 4		 5		 6		 7
    PA8 	  Col2   8	 8		 9		10		11
    PA9  	  Col3	12	12		13		14		15
    PA10  	  Col4	16	16		17		18		19
                |
                --> VALOR_ARB (VALOR ARBITRARIO PARA ARMAR EL CODIGO DE LA TECLA)
                    El valor arbitrario se debe elegir de tal forma que no se generen
                    codigos de teclas repetido.

****************************************************************************
                 Conexionado del teclado 20 teclas
****************************************************************************
                Matriz del Teclado
                (Lo que esta entre parentesis es el puntero a la tabla)
                            PA2  	PA3  	 PA4  	  PA5
                                   |     	|     	 |        |
                                  3|(3)    7|(2)    T|(1)   TL|(0)
                            PA6  ---------------------------------
                                   |     	|     	 |     	  |
                                  2|(7)    6|(6)    C|(5)    +|(4)
                            PA7  ---------------------------------
                                   |     	|    	 |   	  |
                                  1|(11)   5|(10)   9|(9)   AD|(8)
                            PA8 ---------------------------------
                                   |     	|    	 |   	  |
                                  0|(15)   4|(14)   8|(13)   Z|(12)
                            PA9  ---------------------------------
                                   |     	|    	 |    	  |
                                  0|(15)   4|(14)   8|(13)   Z|(12)
                            PA10 ---------------------------------
                                   |     	|    	 |    	  |



   Se definen como const para que la tabla quede en ROM
            Posicion   		 0	  1	   2	3	 4	  5     6	  7	   8	9
const uint8_t Mapeo[20] = { 20,  22,   7,   3,  18,  19,    6,    2,  21 ,   8,
                             4,   0,  17,   9,   5,   1,  255,  255, 255, 255};

 Tabla del teclado 20 Teclas
                   Posicion	 0	  1	   2    3	 4     5	6    7	   8    9
const uint8_t Mapeo[20] = { 24,   9,   6,   3,  23,    8,   5,   2,   20,   7,
                             4,   1,  22,  19,  17,   0,  25,  18,  22,  21};
#define	Enter			20	D
#define	TL 				20	D
#define	Clear_tec		19	C
#define	Tara_T			22	F
#define	Zero			21	E
#define	Mas				18	B
#define	Prt				18	B
#define	AD 				17	A


 Manejo de la cola :
La cola de teclado tiene 3 posiciones , hay dos punteros uno es PongoTec que lo
maneja la int de teclado y va avanzando cada vez que se presiona una tecla,
si la casilla donde tiene que poner la tecla esta ocupada (<> 0) esta tecla se
pierde y nunca el puntero PongoTec va a pasar por encima del puntero de consumo
teclas.
El puntero de consumo de teclas se da cuenta que tiene tecla válida cuando
la posición a donde apunta es distinto de 0xFF, saca la tecla y borra dicha
posición (la deja en 0xFF una vez leida).

*/
void InitTec(void)
    {
    int8_t i; // Variable local para inicializar el buffer de teclado

    if (Flg_Display == true) // Si tengo display lo habilito
        {
        SalScan = 0x0002; // Inicializo el barrido de filas. de derecha a izquierda
        // No hay teclas presionadas => carga todas las posiciones con 0xFF
        Ptr_PongoTec = 0;    // Inicializo el puntero para poner teclas en la cola del teclado
        Ptr_ConsumoTec = 0;  // Inicializo el puntero para leer teclas de la cola del teclado
        Flg_Posible = false; // Indicador de existencia de tecla en un barrido anterior
        Flg_NoTecla = false; // Indicador de que No hay tecla presionada
        Posible_Tecla = 0;   // Posible tecla presionada
        Posible_Columna = 0; // Posible columna de tecla presionada
        Cnt_NoTecla = 0;     // Inicializa el contador de tecla no presionada (espera la finalizacion del rebote)
        Cnt_posible = 0;     // Inicializa el contador de tecla posible (contador de tecla leida en varias oportunidades)
        Cnt_Error = 50;      // Inicializa el contador de error

        for (i = MAXCOLA; i >= 0; i--) // MAXCOLA=20 => máxima cantidad de teclas en la cola
            Buffer_Teclado[i] = FREE;  // Inicializo el buffer de teclado como que
        }
    }

void ScanTec(void)
    {
    int8_t Tecla = 0;    // Variable local para determinar el codigo de la tecla valida
    int16_t Columna = 0; // Variable local para leer el port asignado a las columnas del teclado
    int8_t Aux = 0;
    uint16_t Aux_Scan = 0; // Auxiliar para invertir SalScan
    /*
     *  	PA5...PA2 (FILAS) traigo el catodo a cero ("0") SalScan(0xFFDF, 0xFFEF, 0xFFF7, 0xFFFB)
            PA15...PA11		 PA10	PA9 	PA8 	PA7 	PEA6	PEA5	PA4 	PA3 	PA2 	PA1 	PA0
                              X		 X		 X		 X		 X		  1		 1		 1		 0		 X	 	 X	Escribo
                              X		 X		 X		 X		 X		  1		 1		 1		 0		 X	 	 X	Leo el dato
                              X		 X		 X		 X		 X		  1		 1		 0		 1		 X	 	 X	Escribo
                              X		 X		 X		 X		 X		  1		 1		 0		 1		 X	 	 X	Leo el dato
                              X		 X		 X		 X		 X		  1		 0		 1		 1		 X	 	 X	Escribo
                              X		 X		 X		 X		 X		  1		 0		 1		 1		 X	 	 X	Leo el dato
                              X		 X		 X		 X		 X		  0		 1		 1		 1		 X	 	 X	Escribo
                              X		 X		 X		 X		 X		  0		 1		 1		 1		 X	 	 X	Leo el dato

    Filtro &  				1		 1		 1		 1		 1		  1		 1		 1		 1		 1		 1
    Filtro & 	F|			     F               |               F               |              F				Pongo a uno la Filas _0....3
    Escribo 	 | x		x		x		 x		 x		 x		       Valor de SalScan			 x		 x
    Filtro &  		 1		 1		 1		  1		 1		 1		 0		 1		 1		 0		 0		 0		Leo la entrada y la filtro
    Filtro &  F	|			     F               |               F               |              B					Valor de la tecla
                                    shifteo el valor de la tecla 6 veces

    */

    SalScan = SalScan << 1; // Pasa a la siguiente fila

    if (SalScan > 0x0020) // Si ya barrio las 4 filas (desde PA2 hasta PA5)
        {
        SalScan = 0x0004; // Inicializo el barrido de las filas
        }

    Aux_Scan = ~SalScan;                  // Invierto el dato para barrer con "0"  0x0004 => 0xFFBF
    GPIOA->ODR = (GPIOA->ODR & Aux_Scan); // escribo la fila a revisar en "0"
    Columna = (GPIOA->IDR);               // Leo el Port "A", dejo las 5 columnas de entrada PA10...PA6
    Columna = (~Columna) & 0x07C0;        // Invierto el dato y lo filtro dejo solo las columnas
    Columna = Columna >> 6;               // Shifteo el dato leido 6 posiciones

    if (Columna == 0) // Si no tengo ninguna tecla presionada todas las columnas en "0"
        {
        if (Cnt_NoTecla > 20) // 20mseg sin tecla presionada
            {
            Flg_NoTecla = true;  // No hay tecla presionada
            Cnt_posible = 0;     // Inicializa el contador de espera de repeticion de tecla
            Flg_Posible = false; // Saca el flag de posible tecla valida
            Posible_Tecla = 0;   // Posible tecla presionada
            Posible_Columna = 0; // Posible columna de tecla presionada
            }
        else
            Cnt_NoTecla++; // incrementa el contador de espera de rebote de la tecla (soltar tecla)
        }
    else // Tengo alguna tecla presionada
        {
        Cnt_NoTecla = 0;         // Inicializo el contador de espera de repeticion de la tecla al soltarla
        if (Flg_Posible == true) // Si hubo una tecla anterior verifica si es la misma
            {
            if ((Posible_Columna == Columna) && (Posible_Tecla == SalScan)) // Es la misma tecla que la anterior?
                Cnt_posible++;                                              // Si => incrementa el contador de repeticion de tecla presionada
            else
                {
                Cnt_Error--;
                if (Cnt_Error <= 0)
                    Flg_Posible = false; // Pone el flag para cargar una nueva tecla
                }
            }
        else
            {
            if (Flg_NoTecla == true)       // Si vengo de haber liberado una tecla verifico si hay nueva tecla
                {                          // Si hubo algun posible valor leido de las columnas por primera vez
                Posible_Columna = Columna; // lo guarda para analizar la fila en una segunda, tercera, .... pasada
                Posible_Tecla = SalScan;   // Tambien guarda la posible columna de la tecla presionada
                Cnt_posible = 0;           // Inicializa el contador de espera de repeticion de tecla
                Cnt_Error = 50;            // Inicializa el contador de error
                Flg_Posible = true;        // Pone el flag de una posible tecla valida
                }
            }
        }

    /* Esta parte del programa se ejecuta cuando hubo una tecla presionada durante un tiempo definido
     * por la cantidad de lecturas consecutivas de la misma tecla, el cual esta determinado por el
     * contador 'Cnt_posible'; una vez pasado este tiempo hay que esperar que se suelte la tecla.
     * Este tiempo de demora permite eliminar el problema conocido como rebote del teclado.
     * A su vez, este lapso queda determinado por el contador 'Cnt_NoTecla'
     * Como la revision del teclado se lleva a cabo con un intervalo de aproximadamente 1,3 ms
     * podemos decir que el valor de comparacion del contador 'Cnt_NoTecla' esta expresando la demora
     * en milisegundos, mientras que para el otro contador ('Cnt_posible') el tiempo sera de:
     * Valor de comparacion * cantidad de filas * 1,3 ms
     * Esto se debe a que con cada barrido se pasa de una fila a otra, lo que significa que si debemos
     * volver a revisar la misma fila para validar una tecla hay que esperar para pasar por todas las filas
     * que posea el teclado.
     *
     */

    if ((Cnt_posible >= 5) && (Flg_NoTecla == true)) // Si paso el tiempo de espera de repeticion de tecla y no hubo
        {                                            // una nueva tecla => pasa a analizar la tecla presionada
        switch (Posible_Columna)                     // Analiza primero cual es la fila de la tecla presionada
            {
        case 0x01:     // columna 0 (PA6)
            Tecla = 0; // Asigna el valor arbitrario 0
            break;
        case 0x02:     // columna 1 (PA7)
            Tecla = 4; // Asigna el valor arbitrario 8
            break;
        case 0x04:     // columna 2 (PA8)
            Tecla = 8; // Asigna el valor arbitrario 16
            break;
        case 0x08:      // columna 3 (PA9)
            Tecla = 12; // Asigna el valor arbitrario 24
            break;
        case 0x10:      // columna 4 (PA10)
            Tecla = 16; // Asigna el valor arbitrario 32
            break;
        default:        // Ninguna columna valida
            Tecla = 21; // Asigna el valor arbitrario 50
            break;
            }

        switch (Posible_Tecla) // Analiza primero cual es la columna de la tecla presionada
            {
        case 0x0004: // Fila 0 (PA2)
            Aux = 0;
            break;
        case 0x0008: // Fila 1 (PA3)
            Aux = 1;
            break;
        case 0x0010: // Fila 2 (PA4)
            Aux = 2;
            break;
        case 0x0020: // Fila 3 (PA5)
            Aux = 3;
            break;
        default:
            Aux = 21;
            break;
            }
        Tecla = Tecla + Aux; // Arma el codigo de la tecla sumando al valor obtenido
        // del 'switch' anterior el valor de la fila actual
        if (Tecla < 20) // Si es una tecla valida la carga en la cola del buffer de teclado
            {           // Si no fue una tecla valida (Tecla = 200) => la desecha sin hacer nada
            Guardo_tecla(Tecla);
            }
        Cnt_NoTecla = 0;     // Inicializa el contador de espera de rebote de tecla
        Cnt_posible = 0;     // Inicializa el contador de espera de repeticion de tecla
        Flg_Posible = false; // Saca el flag de posible tecla valida
        Flg_NoTecla = false;
        }

    GPIOA->ODR = (GPIOA->ODR | 0x003C); // escribo todas las filas salidas en "1"

    } // Cierre de la rutina ScanTec

/*---------------------------------------------------------------------------*/
/* Rutina   	:  Guardo_tecla
 *  Objetivo 	:  Guardo la tecla levantada en Buffer_Teclado
 *  Entrada  	:
 *  Descripcion : recibe la tecla y la guarda en el buffer de teclado, pone el
 *				  flag de uso e incrementa el puntero Ptr_PongoTec si corresponde
 *				  lo pone a cero
 */
void Guardo_tecla(uint8_t Tecla)
    {
    Buffer_Teclado[Ptr_PongoTec] = Tecla; // Carga el codigo de la tecla actual en la cola de teclado
    Ptr_PongoTec++;                       // Actualiza el puntero de carga de la cola del teclado
    if (Ptr_PongoTec >= MAXCOLA)          // Verifica si llego al final de la cola de teclado
        Ptr_PongoTec = 0;                 // Si llego al final de la cola de teclado vuelve al principio
    }

/*------------------------------------------------------------------------------------------------------------------
 *  Rutina   	: unsigned int8_t Teclado(void)
 *  Objetivo 	: Devolver el codigo de una tecla valida si fue presionada alguna
 *  Entrada  	: Buffer_Teclado[i]
 *  Descripcion  :
 */
int8_t Teclado(void)
    {
    int8_t Nueva_Tec;
    Nueva_Tec = FREE;                               // Inicializa el codigo de nueva tecla como NO hay tecla (0x00)
    if (Buffer_Teclado[Ptr_ConsumoTec] != FREE)     // Si la cola NO esta vacia
        {                                           // => empieza a analizarla
        Nueva_Tec = Buffer_Teclado[Ptr_ConsumoTec]; // Recupera el codigo de la nueva tecla de la cola del teclado
        Nueva_Tec = Mapeo[Nueva_Tec];               // Recupera el codigo asignado a la tecla leida (decodificacion de teclado)
        Buffer_Teclado[Ptr_ConsumoTec] = FREE;      // Inicializa la cola del teclado (borra el codigo de la tecla leida)
        Ptr_ConsumoTec++;                           // Actualiza el puntero que recorre la cola para levantar las teclas leidas
        if (Ptr_ConsumoTec >= MAXCOLA)              // LLego al final de la cola del teclado?
            Ptr_ConsumoTec = 0;                     // Si=> inicializa el puntero para levantar las teclas leidas

        Beep(); // Sonar buzzer
        }
    return (Nueva_Tec); // Devuelve el valor de la tecla leida y decodificada
    }

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*               FUNCIONES RELACIONADAS CON EL BUZZER                        */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Rutina   	: Beep
 *  Objetivo 	: Hace sonar el buzzer con un período de 1,2ms --> 830Hz.
 *  Entrada  	:
 *  Descripcion 	: se acciona durante 200mseg.
 * 				  Se carga la variable Time_Buzzer con el tiempo que queremos,
 * 				  en la rutina de interrupciones del timer se apaga el Buzzer.
 **/
void Beep(void)
    {
    HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, SET);
    Time_Buzzer = 200; /* Activo el Buzzer por 0,2 seg */
    }
/*---------------------------------------------------------------------------*/
/* Rutina   	: error_qom
 *  Objetivo 	: Hace sonar el buzzer con un período de 2ms --> 500Hz.
 *  Entrada  	:
 *  Descripcion 	: se acciona durante 500mseg.
 * 				  Se carga la variable Time_Buzzer con el tiempo que queremos,
 * 				  en la rutina de interrupciones del timer se apaga el Buzzer.
 */
void error_qom(void)
    {
    HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, SET);
    Time_Buzzer = 500; /* Activo el Buzzer por 0,5 seg */
    }
/*---------------------------------------------------------------------------*/
/* Rutina   	: fatal_error_Beep
 *  Objetivo 	: Hace sonar el buzzer con un período de 2,844mseg --> 351Hz.
 *  Entrada  	:
 *  Descripcion 	: se acciona durante 1000mseg.
 * 				  Se carga la variable Time_Buzzer con el tiempo que queremos,
 * 				  en la rutina de interrupciones del timer se apaga el Buzzer.
 */
void fatal_error_Beep(void)
    {
    HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, SET);
    Time_Buzzer = 1000; /* Activo el Buzzer por 1 seg */
    }

/**
 * @brief Implementación de una función de retardo en microsegundos.
 * @param microseconds El número de microsegundos a esperar.
 * @note Esta es una implementación básica y puede necesitar ser ajustada
 * para tu microcontrolador específico para mayor precisión.
 */
void delay_us(uint32_t microseconds)
    {
    // La implementación de esta función depende en gran medida
    // del compilador y la arquitectura del microcontrolador.
    // Para Arduino, puedes usar delayMicroseconds(microseconds);
    // Para otros sistemas, podrías usar un bucle de NOPs o un temporizador de hardware.
    for (volatile uint32_t i = 0; i < (microseconds * (SystemCoreClock / 1000000) / 5); ++i)
        {
        __asm__("nop");
        }
    }

/* ------------------------------------------------------------------------ */
/*      --------------- 	RUTINAS	 del  RELOJ   ---------------       */
/* ------------------------------------------------------------------------ */
/* Rutina   	: str_date
 *  Objetivo 	: Cargar la nueva fecha si se indica y devolver la misma
 *  Entrada  	: un int8_t * con el dato != 0 si hay que actualizar la fecha
 *  Descripcion 	:  Si el el dato != 0 hay que actualizar la fecha, cargar
 *                 buf_date con el dato actualizado y retornar el puntero
 *                 apuntando a buf_date[0]
 *
 */
int8_t *str_date(int8_t Actualizar)
    {
    int8_t aux;

    HAL_PWR_EnableBkUpAccess(); // enable PWR backup domain access (RTC,BKReg)
    __HAL_RCC_RTC_ENABLE();     // Enable RTC. not created by cube because the RTC can run.
    if (Actualizar == true)
        {
        /*## Configure the Date #################################################*/
        sDate.WeekDay = (Day & 0x0F); //	RTC_WEEKDAY_TUESDAY;

        aux = (buf_date[3] << 4);
        aux = aux | (buf_date[4] & 0x0F);
        sDate.Month = aux; // RTC_MONTH_SEPTEMBER;

        aux = (buf_date[0] << 4);
        aux = aux | (buf_date[1] & 0x0F);
        sDate.Date = aux;

        aux = (buf_date[8] << 4);
        aux = aux | (buf_date[9] & 0x0F);
        sDate.Year = aux;

        if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
            {
            Error_Handler();
            }
        }
    /* Get the RTC current Date */
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

    aux = sDate.Date;
    aux = (aux >> 4) + 0x30;
    buf_date[0] = aux;
    aux = sDate.Date;
    aux = (aux & 0x0F) + 0x30;
    buf_date[1] = aux;
    buf_date[2] = '-';

    aux = sDate.Month;
    aux = (aux >> 4) + 0x30;
    buf_date[3] = aux;
    aux = sDate.Month;
    aux = (aux & 0x0F) + 0x30;
    buf_date[4] = aux;
    buf_date[5] = '-';

    buf_date[6] = '2';
    buf_date[7] = '0';
    aux = sDate.Year;
    aux = (aux >> 4) + 0x30;
    buf_date[8] = aux;
    aux = sDate.Year;
    aux = (aux & 0x0F) + 0x30;
    buf_date[9] = aux;
    buf_date[10] = Null;

    /*  DBP : Disable access to Backup domain */
    HAL_PWR_DisableBkUpAccess();
    return (&buf_date[0]);
    }

/* ------------------------------------------------------------------------- */
/* Rutina   	: str_time
 *  Objetivo 	: Cargar la nueva hora si se indica y devuelve la misma
 *  Entrada  	: un int8_t * con el dato != 0 si hay que actualizar la hora
 *  Descripcion 	:  Si el el dato != 0 hay que actualizar la hora, cargar
 *                 		buf_time con el dato actualizado y retornar el puntero
 *                 		apuntando a buf_time[0]
 *                 	OJO
 *                 		despues de leer la hora se debe leer la fecha para que
 *                 		se pueda actualizar la hora para una nueva lectura
 */
int8_t *str_time(int8_t Actualizar)
    {
    int8_t aux;

    HAL_PWR_EnableBkUpAccess(); // enable PWR backup domain access (RTC,BKReg)
    __HAL_RCC_RTC_ENABLE();     // Enable RTC. not created by cube because the RTC can run.

    if (Actualizar == true)
        {
        /*##-1- Configure the Time #################################################*/
        aux = (buf_time[0] << 4);
        aux = aux | (buf_time[1] & 0x0F);
        sTime.Hours = aux;

        aux = (buf_time[3] << 4);
        aux = aux | (buf_time[4] & 0x0F);
        sTime.Minutes = aux;

        aux = (buf_time[6] << 4);
        aux = aux | (buf_time[7] & 0x0F);
        sTime.Seconds = aux;
        sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        sTime.StoreOperation = RTC_STOREOPERATION_RESET;
        if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
            {
            Error_Handler();
            }
        }
    /* Get the RTC current Time */
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
    aux = sTime.Hours;
    aux = (aux >> 4) + 0x30;
    buf_time[0] = aux;
    aux = sTime.Hours;
    aux = (aux & 0x0F) + 0x30;
    buf_time[1] = aux;
    buf_time[2] = ':';

    aux = sTime.Minutes;
    aux = (aux >> 4) + 0x30;
    buf_time[3] = aux;
    aux = sTime.Minutes;
    aux = (aux & 0x0F) + 0x30;
    buf_time[4] = aux;
    buf_time[5] = ':';

    aux = sTime.Seconds;
    aux = (aux >> 4) + 0x30;
    buf_time[6] = aux;
    aux = sTime.Seconds;
    aux = (aux & 0x0F) + 0x30;
    buf_time[7] = aux;
    buf_time[8] = Null;

    /* Get the RTC current Date */
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
    /*  DBP : Disable access to Backup domain */
    HAL_PWR_DisableBkUpAccess();
    return (&buf_time[0]);
    }

void Pongo_Hora(int8_t Hora, int8_t renglon)
    {
    int8_t Aux_hora[6];
    int8_t x;
    if (Time_Clock < 1)
        {
        Time_Clock = 60000;
        if (Hora == true)
            Print_LCD(0, 3, (int8_t *)str_date(false));

        (int8_t *)str_time(false);

        for (x = 0; x < 5; x++)
            {
            Aux_hora[x] = buf_time[x];
            }
        Aux_hora[5] = Null;

        Print_LCD(15, renglon, (int8_t *)Aux_hora);
        }
    }

/* ------------------------------------------------------------------------ */
/*      -------------- 	RUTINAS	 del  Conversor A/D   ---------------       */
/* ------------------------------------------------------------------------ */

/**
* @brief  Conversion complete callback in non blocking mode
* @param  AdcHandle : AdcHandle handle
* @note   This example shows a simple way to report end of conversion, and
*         you can add your own implementation.
* @retval None

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1)	// Entro una conversion por interrupcion del canal seleccionado
    {
    }
*/
/**
 *   Rutina          :  Leo_AD
 *   Entrada         :  None
 *   Parametros      :  None
 *   Returns         :  None
 *   Preconditions   :  None
 *   Descripcion     :  Lectura del A/D, inicializo las variables
 *                      y activo la secuencia de conversión.
 */
void Leo_AD(int8_t Canal)
    {
    Cont_AD = 0;        // contador para promedios de lectura del A/D
    PromAD[Canal] = 0;  // Valor leido del A/D promediado
    TotalAD[Canal] = 0; // Valor Totalizado del A/D
    Flag_Medicion = false;
    Channel_AD = Canal + 10;

    config_ext_channel_ADC(Channel_AD);
    HAL_ADC_Start_IT(&hadc1); //  Inicia conversion con el A/D
    }

/**
 *   Rutina          :  config_ext_channel_ADC
 *   Entrada         :  None
 *   Parametros      :  None
 *   Returns         :  None
 *   Preconditions   :  None
 *   Descripcion     :  Configuración del canal del AD a medir.
 */
void config_ext_channel_ADC(int8_t channel)
    {

    sConfig.Channel = channel;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Rank = 1;

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    }
