/*---------------------------------------------------------------------------
		DEFINICIONES GENERALES Y CONSTANTES del LCD
-----------------------------------------------------------------------------

   	Fecha inicialización	:	28/06/2025
	Fecha actualización 	:	28/06/2025
	Realizado por	    	:	C.E. Colombo
	Compilador utilizado	:	ST - Eclipse IDE
	Proyecto	    		:	Phenix 2025 - 407VGT6
	Archivo		    		: 	Def_LCD.h
	Versión	   	    		:	1.00.00
	Objetivo	    		:	Definiciones generales del LCD
------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
			Definiciones del display LCD
			   Port usado --> GPIOD
			   A0...A7    --> Buss de datos
			   RS, W_R, EN --> Control LCD

Definiciones en Define_Pin.h

#define BACKLIGHT_LCD_Pin 		GPIO_PIN_13
#define BACKLIGHT_LCD_GPIO_Port GPIOC
#define D0_Pin 					GPIO_PIN_8
#define D0_GPIO_Port 			GPIOD
#define D1_Pin 					GPIO_PIN_9
#define D1_GPIO_Port 			GPIOD
#define D2_Pin 					GPIO_PIN_10
#define D2_GPIO_Port 			GPIOD
#define D3_Pin 					GPIO_PIN_11
#define D3_GPIO_Port 			GPIOD
#define D4_Pin 					GPIO_PIN_12
#define D4_GPIO_Port 			GPIOD
#define D5_Pin 					GPIO_PIN_13
#define D5_GPIO_Port 			GPIOD
#define D6_Pin 					GPIO_PIN_14
#define D6_GPIO_Port 			GPIOD
#define D7_Pin 					GPIO_PIN_14
#define D7_GPIO_Port 			GPIOD
#define RS_LCD_Pin 				GPIO_PIN_5			original-->  GPIO_PIN_5
#define RS_LCD_GPIO_Port 		GPIOD				original-->  GPIOD_
#define RW_LCD_Pin 				GPIO_PIN_6			original-->  GPIO_PIN_5
#define RW_LCD_GPIO_Port 		GPIOD				original-->  GPIOD_
#define EN_LCD_Pin 				GPIO_PIN_7			original-->  GPIO_PIN_5
#define EN_LCD_GPIO_Port 		GPIOD				original-->  GPIOD_

------------------------------------------------------------------------*/

/*	Macros para setear los pines de control		*/
#define LCD_EN_GPIO_HIGH()	HAL_GPIO_WritePin(LCD_EN_GPIO_Port , LCD_EN_Pin , GPIO_PIN_SET);		// Pongo a "1" E
#define LCD_EN_GPIO_LOW() 	HAL_GPIO_WritePin(LCD_EN_GPIO_Port , LCD_EN_Pin , GPIO_PIN_RESET);		// Pongo a "0" E

#define LCD_RS_GPIO_HIGH()	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin , GPIO_PIN_SET);			// Pongo a "1" RS
#define LCD_RS_GPIO_LOW()	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin , GPIO_PIN_RESET);		// Pongo a "0" RS


#define LCD_WR_GPIO_HIGH()	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin , GPIO_PIN_SET);			// Pongo a "1" R/!W
#define LCD_WR_GPIO_LOW()	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin , GPIO_PIN_RESET);		// Pongo a "0" R/!W

/**********************************************************************************/
/* 		Definiciones de los controles del LCD		*/
#define  BitsAltos   1   /*   		0 = Comandos del bit 8 al 15
									1 = Comandos del bit 0 al 7 */

#if BitsAltos == 0		                /* Variables externas */

#define Clear				0x0001		// Borra todo el display y deja el cursor en el
										// primer caracter de la primer fila
#define Home				0x0002		// Lleva el cursor al primer caracter y deja el AC en cero
#define Set					0x0006		// Entry mode set =	%000001 I/D S	I/D=1 => Incrementa => Cursor derecha
										//									S=1   => Incrementa => Display der.
										// Desplaza cursor a la derecha y display fijo

/*	DISP_on_off	=	%0000 1 D C B	D=1 => Display ON
									C=1 => Cursor ON
									B=1 => Blinking ON
*/
#define Disp_on				0x000C		// Display ON, Cursor ON, Blink Cursor OFF => D=1/C=1/B=0
#define Disp_off			0x0008		// Display OFF, Cursor OFF, Blink Cursor OFF => D=0/C=0/B=0
#define Cursor_on			0x000A		// Cursor on
#define Cursor_off			0x0008		// Cursor off
#define Blink_on			0x0009		// Blink on
#define Blink_off			0x0008		// Blink off
#define Disp_Blink_on		0x000D		// Display ON + Blink on
#define Disp_Blink_Cur_on	0x000F		// Display ON + Blink on + Cursor on

/* CURSOR_DISP_SHFT =	%0001 S/C R/L 00
						S/C R/L
						0   0	Shift cursor left
						0   1  	Shift cursor right
						1   0  	Shift display left
						1   1  	Shift display right
*/
#define Disp_shft_l			0x0018		// Display shift left (No modifica el AC)
#define Disp_shft_h			0x001C		// Display shift right (No modifica el AC)
#define Cur_shft_l			0x0010		// Cursor shift left (AC=AC-1)
#define Cur_shft_r			0x0014		// Cursor shift right (AC=AC+1)


#define LONGFILA 			0x0040 		// aunque el display es de 2x20 el controlador es
                         			// de 2x64 ==> la longitud de la fila es 64

/* DDRAM	=	% 1 A6A5A4A3A2A1A0	Carga la direccion del contador
										de direccion de la DDRAM (AC)
*/
#define Ddram_low			0x0080		// Direccion: 00 (1er. carac./ 1a. linea)
#define Ddram_mid			0x0094		// Direccion: 40 (1er. carac./ 2a. linea)

/* Busy_flag	equ	$ BF A6..A0		donde:	BF=1 => Sistema ocupado
										A6..A0= contenido del contador de direccion
*/
#define Busy_flg_disp		0x0080

/* FUNCTION_SET	=	%001 IF N F 00		IF=1 => Interfase 8 bits
										N=1 => 2 lineas de display
										F=0 => Font int8_t= 5x7
*/
#define Func_set1			0x0038		// 8 bits/ 2 lineas/font= 5x7
#define Func_set2			0x003C		// 8 bits/ 2 lineas/font= 5x10

#else

#define Clear				0x0100		// Borra todo el display y deja el cursor en el
										// primer caracter de la primer fila
#define Home				0x0200		// Lleva el cursor al primer caracter y deja el AC en cero
#define Set					0x0600		// Entry mode set =	%000001 I/D S	I/D=1 => Incrementa => Cursor derecha
										//									S=1   => Incrementa => Display der.
										// Desplaza cursor a la derecha y display fijo

/*	DISP_on_off	=	%00001 D C B	D=1 => Display ON
						C=1 => Cursor ON
						B=1 => Blinking ON
*/
#define Disp_on				0x0C00		// Display ON,Cursor ON,Blink Cursor OFF => D=1/C=1/B=0
#define Disp_off			0x0800		// Display OFF,Cursor OFF,Blink Cursor OFF => D=0/C=0/B=0
#define Cursor_on			0x0A00		// Cursor on
#define Cursor_off			0x0800		// Cursor off
#define Blink_on			0x0900		// Blink on
#define Blink_off			0x0800		// Blink off
#define Disp_Blink_on		0x0D00		// Display ON + Blink on
#define Disp_Blink_Cur_on	0x0F00		// Display ON + Blink on + Cursor on

/* CURSOR_DISP_SHFT =	%0001 S/C R/L 00
						S/C R/L
						0   0	Shift cursor left
						0   1  	Shift cursor right
						1   0  	Shift display left
						1   1  	Shift display right
*/
#define Disp_shft_l			0x1800		// Display shift left (No modifica el AC)
#define Disp_shft_h			0x1C00		// Display shift right (No modifica el AC)
#define Cur_shft_l			0x1000		// Cursor shift left (AC=AC-1)
#define Cur_shft_r			0x1400		// Cursor shift right (AC=AC+1)


#define LONGFILA 			0x0040 		// aunque el display es de 2x20 el controlador es
                         				// de 2x64 ==> la longitud de la fila es 64
										// Cuando estan los bits altos la lonfila la shiteo
										// cuando cargo la poscición

/* DDRAM	=	% 1 A6A5A4A3A2A1A0	Carga la direccion del contador
										de direccion de la DDRAM (AC)
*/
#define Ddram_low			0x8000		// Direccion: 00 (1er. carac./ 1a. linea)
#define Ddram_mid			0x9400		// Direccion: 40 (1er. carac./ 2a. linea)

/* Busy_flag	equ	$ BF A6..A0		donde:	BF=1 => Sistema ocupado
										A6..A0= contenido del contador de direccion
*/
#define Busy_flg_disp		0x8000

/* FUNCTION_SET	=	%001 IF N F 00		IF=1 => Interfase 8 bits
										N=1 => 2 lineas de display
										F=0 => Font int8_t= 5x7
*/
#define Func_set1			0x3800		// 8 bits/ 2 lineas/font= 5x7
#define Func_set2			0x3C00		// 8 bits/ 2 lineas/font= 5x10

#endif
