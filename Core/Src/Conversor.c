/*---------------------------------------------------------------------------*/
/*                     Medicion con el conversor A/D                         */
/*---------------------------------------------------------------------------*/
/*
    Fecha inicialización	:	11/08/2025
    Fecha actualización 	:	16/01/2026
    Realizado por	    	:	Pablo M. Kuziw
    Compilador utilizado	:	ST - Eclipse IDE
    Proyecto	    		:	Phenix 2025
    Archivo		    		: 	Conversor.c
    Versión	   	    		:	3.10.00
    Objetivo				:	Manejo del conversor A/D y rutinas auxiliares
*/
#define PosVariables 1 /*   	0 = Variables propias \
                               1 = Variables externas */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Variables.h"

extern int8_t *mi_itoa(uint16_t numero, int8_t *buffer, int8_t Largo, int8_t saco_ceros); // Convertir un entero en un dato ASCII de 5 caracteres
extern int16_t mi_atoi(int8_t *s, int8_t Largo);                                          // Convertir una cadena ASCII de 5 caracteres en un entero
extern void Print_LCD(uint8_t col, uint8_t fila, int8_t *cadena);
extern void Armo_Cabecera(int8_t Usart);
extern void TX_Ack(int8_t Usart); // Transmite ACK a la PC o placa de salida - se elije USART(Comu_PSalidas) o USART(Comu_PC)
extern void TX_Nak(int8_t Usart); // Transmite NACK a la PC o placa de salida - se elije USART(Comu_PSalidas) o USART(Comu_PC)
extern void TX_Can(int8_t Usart); // Transmite CAN a la PC o placa de salida - se elije USART(Comu_PSalidas) o USART(Comu_PC)
extern void Transmito_string(int16_t Usart, int8_t Flag_Check_Sun);
extern void Transmito_oscilos();                           //
extern void Calculo_Velocidad(void);                       //
extern void Clr_Buffer(uint8_t *Buffer, uint8_t longitud); //

void Medicion(void);       // Medicion de los 8 canales del conversor AD: devuelve peso neto
void Medicion_Bruto(void); // Medicion de los 8 canales del conversor AD: devuelve peso bruto
// void 	Medicion_Cuentas(void);			// Medicion en cuentas de los 8 canales del conversor AD
void Init_Medicion(void);               // Inicializa las variables utilizadas durante el proceso de medicion
int8_t Primer_Cero(int8_t Num_Celda);   // Tomar el primer cero
int8_t Tomar_Peso(int8_t Num_Celda);    // Tomar el peso
int8_t Vcuentas_Cero(int8_t Num_Celda); // Tomar el cero al ingresar a ver cuentas
void Ver_Cuentas(void);                 // Ver las cuentas del A/D
void Hago_Tara_Inicial(void);           // hago la tara la 1º vez
void Toma_Taras(void);                  // Proceso de toma de taras solicitado desde la PC
void Toma_Tara_Platillos(void);         // Tomar las taras delos platillos
void Estado_Tara(void);                 // Respuesta sobre el estado de la toma de taras a la consulta que envia la PC cada 1 seg
void Verificar_Taras(void);             // Verificar taras
void Get_Osciloscopio(void);            // Ver los datos de peso en pantalla
void Datos_Curvas(void);                // Pido los datos de peso de las celdas
void Ver_Peso_Test(void);               // Arma el frame para mostrar y transmitir la velocidad, las cuentas y el peso neto
void Ver_Peso_Test_ON(void);            //

/**-------------------------------------------------------------------------------------------------------------------------
*  Rutina   : Medicion
*  Objetivo :
*  Entrada  :
*  Descripcion : Mide el peso con la maquina funcionando
*
*
---------------------------------------------------------------------------------
 *
 * Explicacion del proceso de medicion:
 * La conversion se inicia poniendo en 1 la entrada CONVST y bajandola a 0
 * Luego la salida BUSY se pone en 1 indicando que dio comienzo a la conversion
 * Cuando la salida BUSY pasa a 0, indica que finalizo la conversion de todos los canales
 * El tiempo que demora la conversion (Tconv) se define desde el instante que se sube a 1 la entrada CONVST hasta
 * que la salida BUSY pasa a 0
 * Este tiempo (por hoja de datos) es de: Tconv = 1,7uS
 * Segun la hoja de datos, este conversor admite como maximo 510kSPS => 1,96uS x conversion
 * Considerando que se logre el maximo de 510kSPS y siendo cada valor 16bits ($FFFF) el resultado de sumar
 * todos los valores tendría un valor maximo de: 33.422.850.000 = $7C82837D0
 * Pero dado que la maquina no funciona a menos de 3 platillos por segundo, la cantidad maxima de conversiones seria entonces
 * de la tercera parte, o sea: 11.140.950.000 => $2980D67F0
 * Usando una variable uint32, llegamos a 4.294.967.296
 * El problema es con la medicion en modo calibracion, porque se mide durante un lapso de 5 segundos
 * Para no tener inconvenientes con la limitacion del numero mas grande, se utiliza una variable de 64bits, lo que permite
 * llegar al numero 18.446.744.073.709.551.616 = (2^64)
 * Con ese limite definido, y tomando el maximo de muestras por segundo (510kSPS) se podrian realizar mediciones durante
 * un lapso de (551.920.140 segundos = 153.311 horas = 6387 dias) sin interrupcion
 *
 *
 */
void Medicion(void)
    {
    uint8_t i; // Indice para recorrer los canales

    if (Flag_Enable_AD == false)    // Si llego al valor de fin de medicion del sincro fino, debe calcular el promedio
        {                           // de los valores acumulados
        if (Flag_Conv_FIN == true)  // Debe esperar a recibir la última conversión
            {                       // Esto se realiza con la salida BUSY_INT del conversor por la entrada de interrupcion
                                    // ETXI10 con flanco de bajada.
            Flag_Conv_FIN = false;  // Prepara el flag para una nueva medicion
            for (i = 0; i < 8; i++) // Calcula el promedio de los canales en forma sucesiva
                {
                Peso_Bruto_CH[i] = Suma_Canal_AD[i] / Cantidad_Mediciones;         // Es un promedio simple de cada canal
                Peso_Bruto_CH[i] = Peso_Bruto_CH[i] >> 2;                          // Vengo con el dato en 15 bits y lo dejo en 14 Bits (16384 cuentas)
                if (Peso_Bruto_CH[i] >= Tara[i][Conta_Ejes])                       // El Peso bruto DEBE ser mayor a la sumatoria de la tara mas el cero
                    {                                                              // porque de otra forma daria un valor negativo generando un peso neto erroneo
                    if ((Peso_Bruto_CH[i] - Tara[i][Conta_Ejes]) < Correcion_cero) // Si la diferencia entre el Peso Bruto y la Tara es menor a un valor definido
                        Tara[i][Conta_Ejes] = Peso_Bruto_CH[i];                    // procede a actualizar la Tara actual para compensar el corrimiento por temperatura

                    Peso_Neto_CH[i] = (Peso_Bruto_CH[i] - Tara[i][Conta_Ejes]) * 10000; // Calcula el peso neto restando la tara al valor medido (todo esto se realiza en cuentas)
                    Peso_Neto_CH[i] = Peso_Neto_CH[i] / Coeficiente_CH_CCM[i];          // Calcula el peso en gramos haciendo la correccion con el coeficiente correspondiente
                    // a cada canal. Este coeficiente se obtiene durante el proceso de calibracion de celda
                    Flag_Signo_Neto[i] = True; // Pone el flag indicando que el peso neto es positivo
                    }
                else
                    {                                                              // Como el peso bruto es menor a la sumatoria de la tara mas el cero se pone
                    if ((Tara[i][Conta_Ejes] - Peso_Bruto_CH[i]) < Correcion_cero) // Si la diferencia entre el Peso Bruto y la Tara es menor a un valor definido
                        Tara[i][Conta_Ejes] = Peso_Bruto_CH[i];                    // procede a actualizar la Tara actual para compensar el corrimiento por temperatura

                    Peso_Neto_CH[i] = (Tara[i][Conta_Ejes] - Peso_Bruto_CH[i]) * 10000; // Calcula el peso neto restando la tara al valor medido (todo esto se realiza en cuentas)
                    Peso_Neto_CH[i] = Peso_Neto_CH[i] / Coeficiente_CH_CCM[i];          // Calcula el peso en gramos haciendo la correccion con el coeficiente correspondiente
                    // a cada canal. Este coeficiente se obtiene durante el proceso de calibracion de celda
                    Flag_Signo_Neto[i] = False; // Pone el flag indicando que el peso neto es negativo
                    }
                }
            }
        }
    }

/**------------------------------------------------------------------------------
*  Rutina   : Medicion_Bruto
*  Objetivo :
*  Entrada  :
*  Descripcion : Mide el peso con la maquina datenida
*
*
---------------------------------------------------------------------------------*/
void Medicion_Bruto(void)
    {
    uint8_t i; // Indice para recorrer los canales

    if (Flag_Enable_AD == false)    // Si llego al valor de fin de medicion del sincro fino, debe calcular el promedio
        {                           // de los valores acumulados
        if (Flag_Conv_FIN == true)  // Debe esperar a recibir la última conversión
            {                       // Esto se realiza con la salida BUSY_INT del conversor por la entrada de interrupcion
                                    // ETXI10 con flanco de bajada.
            Flag_Conv_FIN = false;  // Prepara el flag para una nueva medicion
            for (i = 0; i < 8; i++) // Calcula el promedio de los canales en forma sucesiva
                {
                Peso_Bruto_CH[i] = Suma_Canal_AD[i] / Cantidad_Mediciones;         // Es un promedio simple de cada canal sin restar la tara ni ajustar por coeficiente
                Peso_Bruto_CH[i] = Peso_Bruto_CH[i] >> 2;                          // Vengo con el dato en 15 bits y lo dejo en 14 Bits (16384 cuentas)
                if (Peso_Bruto_CH[i] >= Cero_CH_CCM[i])                            // El Peso bruto DEBE ser mayor al cero
                    {                                                              // porque de otra forma daria un valor negativo generando un peso neto erroneo
                    Peso_Neto_CH[i] = (Peso_Bruto_CH[i] - Cero_CH_CCM[i]) * 10000; // Calcula el peso en gramos haciendo la correccion con el coeficiente correspondiente
                    Peso_Neto_CH[i] = Peso_Neto_CH[i] / Coeficiente_CH_CCM[i];     // Calcula el peso en gramos haciendo la correccion con el coeficiente correspondiente
                    // a cada canal. Este coeficiente se obtiene durante el proceso de calibracion de celda
                    Flag_Signo_Neto[i] = True; // Pone el flag indicando que el peso neto es positivo
                    }
                else
                    {                                                              // Como el peso bruto es menor al cero, hace la resta invirtiendo los operadores y luego
                                                                                   // se agrega el flag de signo negativo
                    Peso_Neto_CH[i] = (Cero_CH_CCM[i] - Peso_Bruto_CH[i]) * 10000; // Calcula el peso en gramos haciendo la correccion con el coeficiente correspondiente
                    Peso_Neto_CH[i] = Peso_Neto_CH[i] / Coeficiente_CH_CCM[i];     // Calcula el peso en gramos haciendo la correccion con el coeficiente correspondiente
                    // a cada canal. Este coeficiente se obtiene durante el proceso de calibracion de celda
                    Flag_Signo_Neto[i] = False; // Pone el flag indicando que el peso neto es negativo
                    }
                }
            }
        }
    }

/**---------------------------------------------------------------------------
*  Rutina   	: Init_Medicion
*  Objetivo 	: Incializa las variables de medicion
*  Entrada  	:
*  Descripcion 	:
*
-----------------------------------------------------------------------------*/

void Init_Medicion(void)
    {
    uint8_t i;
    uint8_t j;

    Cantidad_Mediciones = 0; // Inicializa el contador de mediciones
    for (i = 0; i < 8; i++)  // se baren todos los acumuladores
        {
        Suma_Canal_AD[i] = 0; // Inicializa la variable de suma acumulada utilizadas en la medicion
        }

    HAL_GPIO_WritePin(CONVST_GPIO_Port, CONVST_Pin, SET); // Pone la entrada CONVST del AD en 1 y la baja a 0 para dar inicio al ciclo de conversion
    for (j = 0; j < 2; ++j)
        {
        __asm__("nop");
        }
    Flag_Enable_AD = true;                                  // Se activa el flag para iniciar un nuevo ciclo de medicion del peso
    Flag_Conv_FIN = false;                                  // Prepara el flag para una nueva medicion
    HAL_GPIO_WritePin(CONVST_GPIO_Port, CONVST_Pin, RESET); //
    }
/************************* Acciones de la placa A/D *****************************/
/**------------------------------------------------------------------------------
*  Rutina   : Primer_Cero
*  Objetivo : Medir el primer cero
*  Entrada  :
*  Descripcion :
*  			El formato de recepción es:
*			Long. = Cantidad de bytes desde el byte 5 hasta el chksum
*           [stx][nodo dest.][nodo origen][long][comando][celda a medir][chksum]
*             0     1  2         3  4       5 6   7  8          9          10
*
*
---------------------------------------------------------------------------------*/
int8_t Primer_Cero(int8_t Num_Celda)
    {
    uint8_t aux_celda1 = 0;
    uint8_t i;
    uint16_t j;
    int8_t resultado = false;
    int8_t aux[2];
    uint64_t Contador_Mediciones;

    if (Num_Celda > 7)                                // Esto se hace para saber si es un comando de la PC o se entra por teclado
        aux_celda1 = Comunica_PC.buf_rx_PC[9] & 0x0F; // Recupero el numero de linea/celda a calibrar
    else
        aux_celda1 = Num_Celda;

    Print_LCD(0, 3, (int8_t *)"L x -   Toma Cero   ");
    mi_itoa(aux_celda1, (int8_t *)&aux[0], 2, true);
    Print_LCD(1, 3, (int8_t *)&aux[0]);

    HAL_GPIO_WritePin(GPIOC, Reset_Pin, GPIO_PIN_SET); // Manda un RESET al conversor A/D porque hay veces que se cuelga cuando viene de ver cuentas
    for (i = 0; i < 2; ++i)                            // Hace una demora para que actue el RESET del A/D
        {
        __asm__("nop");
        } //
    HAL_GPIO_WritePin(GPIOC, Reset_Pin, GPIO_PIN_RESET); //

    Flag_Ver_Cuentas = false;  // Sale del modo ver cuentas
    Flag_Enable_Taras = false; // Deshabilita la toma o verificacion de taras y aborta el proceso

    Init_Medicion();         // Inicializa la suma acumulativa de valores medidos y el contador de cantidad de mediciones
    Contador_Mediciones = 0; // Pone el contador de mediciones en una cantidad que equivale a un minuto

    while (Cantidad_Mediciones <= Cant_Med_Cal) // Se queda esperando que realice la cantidad de mediciones definida
        {
        Contador_Mediciones++; // Esta variable NO se utiliza para ninguna funcion, es solamente para hacer una tarea
        } // mientras se realizan las conversiones de la medicion
    Flag_Enable_AD = false; // Se inicializa el flag indicador de un nuevo ciclo de medicion del peso

    // Va a calcular el promedio de valores medidos y devuelve el valor en Peso_Bruto_CH[i]
    if (Flag_Conv_FIN == true) // Debe esperar a recibir la última conversión
        // Esto se realiza con la salida BUSY_INT del conversor por la entrada de interrupcion
        // ETXI10 con flanco de bajada.
        Flag_Conv_FIN = false; // Prepara el flag para una nueva medicion

    for (i = 0; i < 8; i++) // Calcula el promedio de los canales en forma sucesiva
        {
        Peso_Bruto_CH[i] = Suma_Canal_AD[i] / Cantidad_Mediciones; // Es un promedio simple de cada canal
        Peso_Bruto_CH[i] = Peso_Bruto_CH[i] >> 2;                  // Vengo con el dato en 15 bits y lo dejo en 13 Bits (8192 cuentas)
        }

    aux_celda1 = aux_celda1 - 1;                 // Ajusta el numero de celda porque durante las mediciones se cuentan las celdas desde cero
    if (Peso_Bruto_CH[aux_celda1] > Limite_cero) // Si el valor medido en cuentas es mayor a un limite definido => no se toma el cero
        {
        TX_Nak(Comu_PC); // Devuelve NACK a la PC porque hubo un error en la medicion del cero
        Print_LCD(4, 3, (int8_t *)"- Error CERO    ");
        }
    else
        {
        Cero_CH_CCM[aux_celda1] = Peso_Bruto_CH[aux_celda1]; // Transfiere el valor medido a una variable auxiliar para calcular el coeficiente
        // despues de tomar el peso
        TX_Ack(Comu_PC); // Devuelve ACk a la PC porque se midio correctamente el cero
        Print_LCD(4, 3, (int8_t *)"-   CERO OK     ");
        resultado = true;

        if (Flag_Tara_OK == false) // Si nunca se tomaron las taras carga todas las taras con el valor del cero medido
            {
            for (i = 0; i < 8; i++)
                {
                for (j = 0; j < 500; j++)
                    {
                    Tara[i][j] = Cero_CH_CCM[i]; // Inicializa el sector de variables de las Taras de los platillos
                    }
                }
            }
        }
    return (resultado);
    }

/**----------------------------------------------------------------------------------------------------------
*  Rutina   : Tomar_Peso(int8_t Num_Celda)
*  Objetivo : Medir el Peso patrón y calcular el coeficiente estático
*  Entrada  : Num_Celda --> si es mayor que 7 es el comando enviado por la serie, sino es  el Nº de  celda del
*  			  módulo de calibración de testeos
*  Descripcion :  Asi es como lo hago en las balanzas
*
*  	int32_t aux_Peso_AD;
    int32_t F_Escala;
    int32_t aux_calculos;
    int16_t Aux;
    int8_t aux_Calculo1[9];
    int32_t resto1;
*  			// verificaciones y calculo del coeficiente
    aux_calculos = F_Escala * (aux_Peso_AD - Cero_AD) / Config_Balanza.Ppatron;
    if(aux_calculos <= 160000 && ((aux_calculos / 50) < (aux_Peso_AD - Cero_AD)))
        {
        Config_Balanza.Coef = (aux_Peso_AD - Cero_AD) * 10000 / Config_Balanza.Ppatron;
        resto1 = ((aux_Peso_AD - Cero_AD) * 10000) % Config_Balanza.Ppatron;
        if(resto1 >= (Config_Balanza.Ppatron / 2))
            Config_Balanza.Coef += 1;
*
*
------------------------------------------------------------------------------------------------------------*/
int8_t Tomar_Peso(int8_t Num_Celda)
    {
    uint8_t aux_celda1 = 0;
    int8_t aux[6];
    uint32_t Peso_patron = 0;
    uint8_t i;
    int8_t resultado;
    uint64_t Contador_Mediciones;

    if (Num_Celda > 7)
        aux_celda1 = (Comunica_PC.buf_rx_PC[9] & 0x0F) - 1; // Recupero el numero de linea/celda a calibrar
    else
        aux_celda1 = Num_Celda;

    aux[0] = Config.peso_patron[0]; // COnvierte el peso patron a entero
    aux[1] = Config.peso_patron[1];
    aux[2] = Config.peso_patron[2];
    aux[3] = Config.peso_patron[3];
    aux[4] = Null;
    Peso_patron = mi_atoi(aux, 5);

    HAL_GPIO_WritePin(GPIOC, Reset_Pin, GPIO_PIN_SET); // Manda un RESET al conversor A/D porque hay veces que se cuelga cuando viene de ver cuentas
    for (i = 0; i < 2; ++i)                            // Hace una demora para que actue el RESET del A/D
        {
        __asm__("nop");
        }
    HAL_GPIO_WritePin(GPIOC, Reset_Pin, GPIO_PIN_RESET);

    Flag_Ver_Cuentas = false;  // Sale del modo ver cuentas
    Flag_Enable_Taras = false; // Deshabilita la toma o verificacion de taras y aborta el proceso

    Init_Medicion();         // Inicializa la suma acumulativa de valores medidos y el contador de cantidad de mediciones
    Contador_Mediciones = 0; // Pone el contador de mediciones en una cantidad que equivale a un minuto

    while (Cantidad_Mediciones <= Cant_Med_Cal) // Se queda esperando que realice la cantidad de mediciones definida
        {
        Contador_Mediciones++; // Esta variable NO se utiliza para ninguna funcion, es solamente para hacer una tarea
        } // mientras se realizan las conversiones de la medicion
    Flag_Enable_AD = false; // Se inicializa el flag indicador de un nuevo ciclo de medicion del peso

    if (Flag_Conv_FIN == true) // Debe esperar a recibir la última conversión
        // Esto se realiza con la salida BUSY_INT del conversor por la entrada de interrupcion
        // ETXI10 con flanco de bajada.
        Flag_Conv_FIN = false; // Prepara el flag para una nueva medicion

    for (i = 0; i < 8; i++) // Calcula el promedio de los canales en forma sucesiva
        {
        Peso_Bruto_CH[i] = Suma_Canal_AD[i] / Cantidad_Mediciones; // Es un promedio simple de cada canal
        Peso_Bruto_CH[i] = Peso_Bruto_CH[i] >> 2;                  // Vengo con el dato en 15 bits y lo dejo en 14 Bits (16384 cuentas)
        }
    Chk_Coeficiente = ((Peso_Bruto_CH[aux_celda1] - Cero_CH_CCM[aux_celda1]) * Capacidad_Maxima);
    Chk_Coeficiente = Chk_Coeficiente / Peso_patron;
    Chk_Coeficiente = Chk_Coeficiente + Cero_CH_CCM[aux_celda1];

    if (Chk_Coeficiente >= 31000) // Si el valor calculado es mayor a 31000 => Ganancia ALTA => Error de ganancia
        {
        TX_Nak(Comu_PC); // Devuelve NACK a la PC porque la ganancia es demasiado ALTA
        Print_LCD(0, 3, (int8_t *)" ERR.Ganancia ALTA  ");
        resultado = 0;
        }
    else
        {
        Coeficiente_CH_CCM[aux_celda1] = (Peso_Bruto_CH[aux_celda1] - Cero_CH_CCM[aux_celda1]) * 10000;
        Coeficiente_CH_CCM[aux_celda1] = Coeficiente_CH_CCM[aux_celda1] / Peso_patron;

        /** Para no trabajar con punto flotante se multiplica x100 el dividendo, para lograr que el resultado
         * de la division tenga dos digitos enteros de precision, y de esta forma que el coeficiente tenga
         * mayor resolucion mejorando notablemente el calculo del peso Neto en modo trabajo
         * Ejemplo numerico:
         * Coeficiente = (Peso_Bruto - Cero) / ((Peso_patron * Divisor) / Div_Min)
         * Peso Neto en gramos = (Neto en cuentas / Coef) / Divisor
         * Como del conversor solo utilizamos 15 bits, el valor en cuentas mas alto posible sera 32767
         * Cuentas: 32767 => Tension de entrada: 3,28V (maxima tensión de entrada)
         *
         * Para compensar la deriva termica de la celda y circuito, vamos a usar 31000 cuentas como limite superior para
         * una carga maxima de 1Kg
         * Para calibrar se utiliza un peso patron de 500g => 15500 cuentas
         * Para esta maquina se definen los siguientes parametros:
         * Peso Patron = 1000
         * Divisor = 4
         * Div_Min = 1
         * 14000 ---> 500
         * 28000 ---> 1000
         *
         * Neto en cuentas = 15500 => Coef = 13,9575 ==> Coef=13 (tomando la parte entera) => Pesogr = (27915 / 13) / 4 = 536 gr
         * En el ejemplo anterior se observa un error significativo en el valor final del peso
         *
         * Neto en cuentas = 27915 => Coef = 13,9575 ==> Coef=1395 (multiplico x 100) => Pesogr = ((27915*100)) / 1395) / 4 = 500 gr
         * En el ejmplo anterior se observa que el peso obtenido es el esperado (500gr)
         *
         * En caso de requerir mayor precision en el calculo del peso en gramos, se puede multiplicar x1000
         *
         */
        /** El resultado de la operacion DEBE dar mayor a 650, en caso que de menor la ganancia es BAJA */

        if (Coeficiente_CH_CCM[aux_celda1] <= 600)
            {
            TX_Can(Comu_PC); // Devuelve CAN a la PC porque  la ganancia es demasiado BAJA
            Print_LCD(0, 3, (int8_t *)" ERR.Ganancia BAJA  ");
            resultado = 1;
            }
        else
            {
            TX_Ack(Comu_PC); // Devuelve ACk a la PC porque se midio correctamente el cero
            Print_LCD(0, 3, (int8_t *)"    Ganancia OK     ");
            resultado = true;
            }
        }
    return (resultado);
    }

/**----------------------------------------------------------------------------------------------------------
*  Rutina   : Vcuentas_Cero(int8_t Num_Celda)
*  Objetivo : Medir el cero al entrar a Ver Cuentas
*  Entrada  :
*  Descripcion :
*
------------------------------------------------------------------------------------------------------------*/
int8_t Vcuentas_Cero(int8_t Num_Celda)
    {
    uint8_t aux_celda1 = 0;
    uint8_t i;
    uint16_t j;
    int8_t resultado = false;
    uint64_t Contador_Mediciones;

    if (Num_Celda > 7)                                // Esto se hace para saber si es un comando de la PC o se entra por teclado
        aux_celda1 = Comunica_PC.buf_rx_PC[9] & 0x0F; // Recupero el numero de linea/celda a calibrar
    else
        aux_celda1 = Num_Celda;

    HAL_GPIO_WritePin(GPIOC, Reset_Pin, GPIO_PIN_SET); // Manda un RESET al conversor A/D porque hay veces que se cuelga cuando viene de ver cuentas
    for (i = 0; i < 2; ++i)                            // Hace una demora para que actue el RESET del A/D
        {
        __asm__("nop");
        }
    HAL_GPIO_WritePin(GPIOC, Reset_Pin, GPIO_PIN_RESET);

    Flag_Ver_Cuentas = false;  // Sale del modo ver cuentas
    Flag_Enable_Taras = false; // Deshabilita la toma o verificacion de taras y aborta el proceso

    Init_Medicion();         // Inicializa la suma acumulativa de valores medidos y el contador de cantidad de mediciones
    Contador_Mediciones = 0; // Pone el contador de mediciones en una cantidad que equivale a un minuto

    while (Cantidad_Mediciones <= Cant_Med_Cero) // Se queda esperando que realice la cantidad de mediciones definida
        {
        Contador_Mediciones++; // Esta variable NO se utiliza para ninguna funcion, es solamente para hacer una tarea
        } // mientras se realizan las conversiones de la medicion
    Flag_Enable_AD = false; // Se inicializa el flag indicador de un nuevo ciclo de medicion del peso

    // Va a calcular el promedio de valores medidos y devuelve el valor en Peso_Bruto_CH[i]
    if (Flag_Conv_FIN == true) // Debe esperar a recibir la última conversión
        // Esto se realiza con la salida BUSY_INT del conversor por la entrada de interrupcion
        // ETXI10 con flanco de bajada.
        Flag_Conv_FIN = false; // Prepara el flag para una nueva medicion

    for (i = 0; i < 8; i++) // Calcula el promedio de los canales en forma sucesiva
        {
        Peso_Bruto_CH[i] = Suma_Canal_AD[i] / Cantidad_Mediciones; // Es un promedio simple de cada canal
        Peso_Bruto_CH[i] = Peso_Bruto_CH[i] >> 2;                  // Vengo con el dato en 15 bits y lo dejo en 13 Bits (8192 cuentas)
        }

    aux_celda1 = aux_celda1 - 1;                 // Ajusta el numero de celda porque durante las mediciones se cuentan las celdas desde cero
    if (Peso_Bruto_CH[aux_celda1] > Limite_cero) // Si el valor medido en cuentas es mayor a un limite definido => no se toma el cero
        {
        resultado = false; // El valor de cero medido NO es aceptable, es demasiado alto => se mantiene el valor actual
        }
    else
        {
        Cero_CH_CCM[aux_celda1] = Peso_Bruto_CH[aux_celda1]; // Transfiere el valor medido a una variable auxiliar para calcular el coeficiente
        // despues de tomar el peso
        resultado = true; // El valor de cero medido es aceptable y se toma como nuevo cero

        if (Flag_Tara_OK == false) // Si nunca se tomaron las taras carga todas las taras con el valor del cero medido
            {
            for (i = 0; i < 8; i++)
                {
                for (j = 0; j < 500; j++)
                    {
                    Tara[i][j] = Cero_CH_CCM[i]; // Inicializa el sector de variables de las Taras de los platillos
                    }
                }
            }
        }
    return (resultado);
    }

/*======================================================================================================================
 *
 *  NOTA DE ARQUITECTURA – MEDICION DE PESO Y ENVIO A PC (MODO MAQUINA DETENIDA)
 *
 *  Estado actual del sistema:
 *  --------------------------
 *  - La medición de peso con máquina detenida se realiza por temporización (Timer / Time_Medicion).
 *  - La PC solicita el valor de peso aproximadamente cada 250 ms para mostrarlo en pantalla.
 *
 *  Se observó que:
 *    - Cuando la CPU transmite el peso de forma periódica por tiempo,
 *      y la PC también lo solicita por su propio período,
 *      se genera un desacople productor/consumidor que puede provocar
 *      retardos variables (“IPO” visual), aunque sin afectar el funcionamiento real.
 *
 *
 *  POSIBLE CAUSA
 *  -------------
 *  La CPU y la PC generan eventos de transmisión sin sincronización explícita:
 *
 *      CPU: mide y transmite cada X ms
 *      PC : pide peso cada Y ms
 *
 *  Esto puede producir solapamientos de TX, mensajes pendientes pisados
 *  y latencias visibles, especialmente cuando la máquina está detenida
 *  (baja actividad de interrupciones).
 *
 *
 *  ARQUITECTURA RECOMENDADA (pendiente de implementación en banco)
 *  ---------------------------------------------------------------
 *
 *  Separar claramente:
 *
 *    1) ADQUISICION
 *       - La CPU mide el peso por tiempo.
 *       - Guarda el resultado en variables internas.
 *       - NO transmite.
 *
 *    2) TRANSMISION
 *       - La CPU transmite el peso únicamente cuando la PC lo solicita.
 *
 *  Implementación sugerida:
 *
 *      volatile uint8_t Flag_Nuevo_Peso;
 *
 *      En Timer / Time_Medicion:
 *          Medicion_Bruto();
 *          Flag_Nuevo_Peso = 1;
 *
 *      Cuando PC pide peso:
 *          Si Flag_Nuevo_Peso == 1:
 *              Armar frame con último peso
 *              Transmito_string()
 *              Flag_Nuevo_Peso = 0;
 *
 *
 *======================================================================================================================
 */

/**----------------------------------------------------------------------------------------------------------
*  Rutina   : Ver_Cuentas
*  Objetivo : Ver las cuentas del A/D
*  Entrada  :
*  Descripcion :
*
*	Frame a transmitir a la PC:
*   Comunica_PC.buf_tx_PC[0] = STX						 Cargo el chr de comienzo de Tx (STX)
*   Comunica_PC.buf_tx_PC[1] = NodoOrRe[0]				 Nodo destino
*   Comunica_PC.buf_tx_PC[2] = NodoOrRe[1]				 Nodo destino
*   Comunica_PC.buf_tx_PC[3] = Config.Nodo[0]			 Nodo origen
*   Comunica_PC.buf_tx_PC[4] = Config.Nodo[1]			 Nodo origen
*	Comunica_PC.buf_tx_PC[5] = '1'				Longitud frame - byte alto en ASCII
*	Comunica_PC.buf_tx_PC[6] = '8'				Longitud frame - byte bajo en ASCII
*	Comunica_PC.buf_tx_PC[7] = 'n'				Velocidad_medida byte mas alto en ASCII
*	Comunica_PC.buf_tx_PC[8] = 'n'				Velocidad_medida 2do byte alto en ASCII
*	Comunica_PC.buf_tx_PC[9] = 'n'				Velocidad_medida 3er byte alto en ASCII
*	Comunica_PC.buf_tx_PC[10] = 'n'				Velocidad_medida 4to byte alto en ASCII
*	Comunica_PC.buf_tx_PC[11] = 'n'				Velocidad_medida byte mas bajo en ASCII
*	Comunica_PC.buf_tx_PC[12] = 'n'				Peso Neto byte mas alto en ASCII
*	Comunica_PC.buf_tx_PC[13] = 'n'				Peso Neto 2do byte alto en ASCII
*	Comunica_PC.buf_tx_PC[14] = 'n'				Peso Neto 3er byte alto en ASCII
*	Comunica_PC.buf_tx_PC[15] = 'n'				Peso Neto 4to byte alto en ASCII
*	Comunica_PC.buf_tx_PC[16] = 'n'				Peso Neto byte mas bajo en ASCII
*	Comunica_PC.buf_tx_PC[17] = 'n'				Peso Bruto byte mas alto en ASCII
*	Comunica_PC.buf_tx_PC[18] = 'n'				Peso Bruto 2do byte alto en ASCII
*	Comunica_PC.buf_tx_PC[19] = 'n'				Peso Bruto 3er byte alto en ASCII
*	Comunica_PC.buf_tx_PC[20] = 'n'				Peso Bruto 4to byte alto en ASCII
*	Comunica_PC.buf_tx_PC[21] = 'n'				Peso Bruto byte mas bajo en ASCII
*	Comunica_PC.buf_tx_PC[21] = Null			Terminador del frame NULL en ASCII
*
* La longitud del frame se mide comenzando por el byte alto de la longitud (Comunica_PC.buf_tx_PC[5]) y se cuenta
* hasta el Null al final del frame, INCLUSIVE
* Cuando se ejecuta el proceso de transmision del string, se calcula el CHKSUM byte a byte, hasta llegar al Null
* Luego se reemplaza en el frame el Null por el resultado del CHKSUM y se agrega un byte con Null al final del frame
* Recien cuando esta armado el frame con el CHKSUM se procede a enviarlo
*
------------------------------------------------------------------------------------------------------------*/
void Ver_Cuentas(void)
    {
    int8_t j;

    if (Flag_Ver_Cuentas == false)
        {
        Aux_celda = Comunica_PC.buf_rx_PC[9] & 0x0F; // Recupero el numero de linea/celda a revisar
        if (Flag_Maq_Fun == false)                   // Máquina parada envia un dato cada 150ms => Peso neto y cuentas del CH solicitado
            {
            Vcuentas_Cero(Aux_celda); // Ejecuta una toma de cero de la linea a mostrar
            }
        Flag_Ver_Cuentas = true;   // Activo el indicador del modo ver cuentas
        Aux_celda = Aux_celda - 1; // Corrije el numero de celda restando 1
        Flag_Datos_Display = false;
        if (Flag_Errores_Display == false)
            Print_LCD(0, 2, (int8_t *)"Br:      Neto:      ");
        TX_Ack(Comu_PC);    // La PC espera un ACK para luego enviar la solicitud de ver cuentas cada 200ms
        Time_Medicion = 50; // pongo el Time Out en 50 mseg.
        Init_Medicion();    // Inicializa la suma acumulativa de valores medidos y el contador de cantidad de mediciones
        }
    /**
     * Dependiendo si la maquina está funcionando o detenida la tarea a realizar y los datos a mostrar en el display
     *
     */
    else // Ya dio comienzo al modo ver cuentas y tiene que analizar la medicion del A/D
        {
        /**		Maquina DETENIDA	****************************************************************************************************************/

        if (Flag_Maq_Fun == false) // Máquina parada envia un dato cada 250ms => Peso neto y cuentas del CH solicitado
            {
            if (Time_Medicion <= 0) // Cuando pasaron los 150ms hago la medicion del peso y la envio a la PC y al display
                {
                Flag_Enable_AD = false; // Se inicializa el flag indicador de un nuevo ciclo de medicion del peso
                Flag_Conv_FIN = true;
                Medicion_Bruto();               // Calcula el Peso bruto (cuentas) y Peso Neto (Gramos)
                Armo_Cabecera(Comu_PC);         // Armo la cabecera de la transmisión
                Comunica_PC.buf_tx_PC[5] = '1'; // Longitud a transmitir
                Comunica_PC.buf_tx_PC[6] = '8';
                Velocidad_medida = 0; // Maquina detenida => Velocidad = 0
                Ver_Peso_Test();      // Arma el frame para mandar a la PC y al display
                j = 1;
                while (Veo_Peso[j] != Null)
                    {
                    Comunica_PC.buf_tx_PC[6 + j] = Veo_Peso[j]; // Transfiero la informacion al buffer de Tx
                    j++;
                    }
                Comunica_PC.buf_tx_PC[j + 6] = Null; // Agrego el terminador del frame
                Transmito_string(Comu_PC, true);     // transmito y calculo en check sum
                Time_Medicion = 150;                 // pongo el Time Out en 150 mseg.
                Init_Medicion();                     // Inicializa la suma acumulativa de valores medidos y el contador de cantidad de mediciones
                }
            }
        /*******		Maquina FUNCIONANDO	     *******/
        else // Maquina funcionando => envia velocidad, peso neto y cuentas (peso bruto)
            {
            Medicion();                     // Calcula el Peso bruto (cuentas) y Peso Neto (Gramos)
            Armo_Cabecera(Comu_PC);         // Armo la cabecera de la transmisión
            Comunica_PC.buf_tx_PC[5] = '1'; // Longitud a transmitir
            Comunica_PC.buf_tx_PC[6] = '8';
            Calculo_Velocidad(); // El calculo de la velocidad se realiza en el lazo principal de TEST
            Ver_Peso_Test_ON();  // Arma el frame para mandar a la PC y al display
            j = 1;
            while (Veo_Peso[j] != Null)
                {
                Comunica_PC.buf_tx_PC[6 + j] = Veo_Peso[j]; // Transfiero la informacion al buffer de Tx
                j++;
                }
            Comunica_PC.buf_tx_PC[j + 6] = Null; // Agrego el terminador del frame
            Transmito_string(Comu_PC, true);     // transmito y calculo en check sum
            }
        } // Cierro else luego del primer ingreso a la funcion
    }

/*
void Ver_Cuentas(void)
        {
    int8_t j;

    if (Flag_Ver_Cuentas == false)
        {
        Aux_celda = Comunica_PC.buf_rx_PC[9] & 0x0F;	// Recupero el numero de linea/celda a revisar
        if (Flag_Maq_Fun == false)                   	// Máquina parada envia un dato cada 150ms => Peso neto y cuentas del CH solicitado
                {
            Vcuentas_Cero(Aux_celda); 					// Ejecuta una toma de cero de la linea a mostrar
                }
        Flag_Ver_Cuentas = true;   						// Activo el indicador del modo ver cuentas
        Aux_celda = Aux_celda - 1; 						// Corrije el numero de celda restando 1
        Flag_Datos_Display = false;
        if (Flag_Errores_Display == false)
            Print_LCD(0, 2, (int8_t *)"Br:      Neto:      ");
        TX_Ack(Comu_PC); 								// La PC espera un ACK para luego enviar la solicitud de ver cuentas cada 200ms
        Time_Medicion = 150;       						// pongo el Time Out en 150 mseg.
        Init_Medicion(); 								// Inicializa la suma acumulativa de valores medidos y el contador de cantidad de mediciones
        }

     // Dependiendo si la maquina está funcionando o detenida la tarea a realizar y los datos a mostrar en el display es distinta


    else 			// Ya dio comienzo al modo ver cuentas y tiene que analizar la medicion del A/D
        {
        //		Maquina DETENIDA	*************************************************************************************

        if (Flag_Maq_Fun == false) 				// Máquina parada envia un dato => Peso neto y cuentas del CH solicitado
                {
                        Flag_Maq_Estatico = true;			// Pongo el flag para medir en forma estática
            Armo_Cabecera(Comu_PC);         	// Armo la cabecera de la transmisión
            Comunica_PC.buf_tx_PC[5] = '1'; 	// Longitud a transmitir
            Comunica_PC.buf_tx_PC[6] = '8';
            Velocidad_medida = 0; 				// Maquina detenida => Velocidad = 0
            Ver_Peso_Test();      				// Arma el frame para mandar a la PC y al display
            j = 1;
            while (Veo_Peso[j] != Null)
                {
                Comunica_PC.buf_tx_PC[6 + j] = Veo_Peso[j]; // Transfiero la informacion al buffer de Tx
                j++;
                }
            Comunica_PC.buf_tx_PC[j + 6] = Null; // Agrego el terminador del frame
            Transmito_string(Comu_PC, true);     // transmito y calculo en check sum
            Time_Medicion = 150;                 // pongo el Time Out en 150 mseg.
            Init_Medicion();                     // Inicializa la suma acumulativa de valores medidos y el contador de cantidad de mediciones
            }
        }
        //  ******		Maquina FUNCIONANDO	     *******
        else 										// Maquina funcionando => envia velocidad, peso neto y cuentas (peso bruto)
                {
                        Flag_Maq_Estatico = false;				// Paso a la medición normal
            Medicion();                     		// Calcula el Peso bruto (cuentas) y Peso Neto (Gramos)
            Armo_Cabecera(Comu_PC);         		// Armo la cabecera de la transmisión
            Comunica_PC.buf_tx_PC[5] = '1'; 		// Longitud a transmitir
            Comunica_PC.buf_tx_PC[6] = '8';
            Calculo_Velocidad(); 					// El calculo de la velocidad se realiza en el lazo principal de TEST
            Ver_Peso_Test_ON();  					// Arma el frame para mandar a la PC y al display
            j = 1;
            while (Veo_Peso[j] != Null)
                {
                Comunica_PC.buf_tx_PC[6 + j] = Veo_Peso[j]; // Transfiero la informacion al buffer de Tx
                j++;
                }
            Comunica_PC.buf_tx_PC[j + 6] = Null; 	// Agrego el terminador del frame
            Transmito_string(Comu_PC, true);     	// transmito y calculo en check sum
                }
        } // Cierro else luego del primer ingreso a la funcion
        }

 *
 */

/*---------------------------------------------------------------------------*/
/* Rutina   : Ver_Peso_Estatico
*  Objetivo : Leer el peso y las cuentas del A/D
*  Entrada  :
*  Descripcion : Activa la medición en forma estática y deja el dato en Peso bruto (cuentas) y Peso Neto (Gramos)


void Ver_Peso_Estatico(void)
        {
        if (Flag_Maq_Estatico == true) 				// Máquina parada, quiero ver el peso en forma estática
                {
                if(Flag_Med_Estatica == true)
                        {
                        Flag_Med_Estatica = false;
                        if (Time_Medicion <= 0) 			// Cuando pasaron los 150ms hago la medicion del peso
                                {
                                Flag_Enable_AD = false; 		// Se inicializa el flag indicador de un nuevo ciclo de medicion del peso
                                Flag_Conv_FIN = true;
                                Medicion_Bruto();              	// Calcula el Peso bruto (cuentas) y Peso Neto (Gramos)
                                Time_Medicion = 50;             // pongo el Time Out en 50 mseg.
                                Init_Medicion();            	// Inicializa la suma acumulativa de valores medidos y el contador de cantidad de mediciones
                                }
                        }
                else
                        {
                        Flag_Med_Estatica = true;
                        Time_Medicion = 50;                 // pongo el Time Out en 50 mseg.
                        Init_Medicion();                    // Inicializa la suma acumulativa de valores medidos y el contador de cantidad de mediciones
                        }
                }
        }
*/
/*---------------------------------------------------------------------------*/
/* Rutina   : Ver_Peso_Test
*  Objetivo : Leer el peso y las cuentas de la placa A/D
*  Entrada  :
*  Descripcion : Arma los datos recibidos para mostrarlos y transmitirlos
                 Forma de recepción: de cada placa A/D
                    1.- Velocidad máquina      		2 Bytes en binario (baja,alta)
                    2.- Peso del canal Pedido  		2 Bytes en binario (baja,alta)
                    3.- Cuentas del canal Pedido	2 Bytes en binario (baja,alta)
*/
void Ver_Peso_Test(void)
    {
    char aux[6];
    char aux1[6];
    char aux2[6];

    Veo_Peso[0] = ' '; // Dejo el lugar libre para el STX
    mi_itoa(Velocidad_medida, (int8_t *)&aux[0], 5, true);
    Veo_Peso[1] = aux[0];
    Veo_Peso[2] = aux[1];
    Veo_Peso[3] = aux[2];
    Veo_Peso[4] = aux[3];
    Veo_Peso[5] = aux[4];

    mi_itoa(Peso_Neto_CH[Aux_celda], (int8_t *)&aux[0], 5, true);
    Veo_Peso[6] = '/';
    Veo_Peso[7] = aux[0];
    Veo_Peso[8] = aux[1];
    Veo_Peso[9] = aux[2];
    Veo_Peso[10] = aux[3];
    Veo_Peso[11] = aux[4];

    aux1[0] = aux[0];
    aux1[1] = aux[1];
    aux1[2] = aux[2];
    aux1[3] = aux[3];
    aux1[4] = aux[4];
    aux1[5] = null;

    if (Flag_Signo_Neto[Aux_celda] == False) // Si el signo del Peso Neto es negativo lo presenta en el display
        Veo_Peso[7] = '-';

    mi_itoa(Peso_Bruto_CH[Aux_celda], (int8_t *)&aux[0], 5, true);
    Veo_Peso[12] = '/';
    Veo_Peso[13] = aux[0];
    Veo_Peso[14] = aux[1];
    Veo_Peso[15] = aux[2];
    Veo_Peso[16] = aux[3];
    Veo_Peso[17] = aux[4];
    Veo_Peso[18] = Null;

    aux2[0] = aux[0];
    aux2[1] = aux[1];
    aux2[2] = aux[2];
    aux2[3] = aux[3];
    aux2[4] = aux[4];
    aux2[5] = null;

    if (HAL_GPIO_ReadPin(C_S_Display_GPIO_Port, C_S_Display_Pin) != 0) // Miro el jumper de display
        Flg_Display = false;                                           // Desahabilito el display
    else
        Flg_Display = true; // Habilito el display

    if (Flag_Errores_Display == false)
        {
        Print_LCD(3, 2, (int8_t *)aux2);  // Ajustar salida al display de los 3 datos
        Print_LCD(14, 2, (int8_t *)aux1); // Ajustar salida al display de los 3 datos
        }
    }

/*---------------------------------------------------------------------------*/
/* Rutina   : Ver_Peso_Test_ON
*  Objetivo : Leer el peso y numero de platillo
*  Entrada  :
*  Descripcion : Arma los datos recibidos para mostrarlos y transmitirlos
                 Forma de recepción: de cada placa A/D
                    1.- Velocidad máquina      		2 Bytes en binario (baja,alta)
                    2.- Peso del canal Pedido  		2 Bytes en binario (baja,alta)
                    3.- Numero de platillo			2 Bytes en binario (baja,alta)
*/
void Ver_Peso_Test_ON(void)
    {
    char aux[6];

    Veo_Peso[0] = ' '; // Dejo el lugar libre para el STX
    mi_itoa(Velocidad_medida, (int8_t *)&aux[0], 5, true);
    Veo_Peso[1] = aux[0];
    Veo_Peso[2] = aux[1];
    Veo_Peso[3] = aux[2];
    Veo_Peso[4] = aux[3];
    Veo_Peso[5] = aux[4];

    mi_itoa(Peso_Neto_CH[Aux_celda], (int8_t *)&aux[0], 5, true);
    Veo_Peso[6] = '/';
    Veo_Peso[7] = aux[0];
    Veo_Peso[8] = aux[1];
    Veo_Peso[9] = aux[2];
    Veo_Peso[10] = aux[3];
    Veo_Peso[11] = aux[4];

    if (Flag_Signo_Neto[Aux_celda] == False) // Si el signo del Peso Neto es negativo lo presenta en el display
        Veo_Peso[7] = '-';

    mi_itoa(Conta_Ejes, (int8_t *)&aux[0], 5, true);
    Veo_Peso[12] = '/';
    Veo_Peso[13] = aux[0];
    Veo_Peso[14] = aux[1];
    Veo_Peso[15] = aux[2];
    Veo_Peso[16] = aux[3];
    Veo_Peso[17] = aux[4];
    Veo_Peso[18] = Null;
    }

/**----------------------------------------------------------------------------------------------------------
*  Rutina   : Hago_Tara_Inicial
*  Objetivo : Tomar las taras
*  Entrada  :
*  Descripcion :
*
------------------------------------------------------------------------------------------------------------*/
void Hago_Tara_Inicial(void)
    {
    if (Flag_Enable_Taras == true) // DEBE esperar que pase el plato 1 para comenzar a tomar las taras
        {
        Toma_Tara_Platillos(); // Toma el peso del platillo y lo guarda como tara
        if (Flag_Errores_Display == false)
            Print_LCD(11, 3, (int8_t *)"Tomando T");
        if (Flag_Parpa1 == false)
            {
            Time_Parpa = 100; // tiempo de parpadeo 1 seg.
            Flag_Parpa1 = true;
            Flag_Error = True; // Se enciende y apaga el Led rojo cada 1 seg para indicar que se esta tomando la tara
            }
        if (Flag_Final_Taras == true) // La cantidad de platos va de 0 a n_platillos
            {
            Flag_Init_Tara = false;    // Pone el flag para deshabilitar la toma de taras de los platillos
            Flag_Enable_Taras = false; // Deshabilita la toma de taras desde la PC
            Flag_Tara_OK = True;       // Pone el flag indicando que se realizo el proceso de toma de taras
            Flag_Error = False;        // Apaga el Led rojo indicando que finalizo el proceso de toma de tara
            Flag_Tara_Inicial = true;  // no vuelvo a realizar la tara inicial
            if (Flag_Errores_Display == false)
                Print_LCD(11, 3, (int8_t *)"         ");
            }
        }
    else
        {
        if (Flag_Parpa == false)
            {
            Time_Parpa = 1000; // tiempo de parpadeo 1 seg.
            Flag_Parpa = true;
            Flag_Error = True; // Se enciende y apaga el Led rojo cada 1 seg para indicar que se esta tomando la tara
            }
        if (Flag_Errores_Display == false)
            Print_LCD(11, 3, (int8_t *)"Esp.Indi.");
        }
    }

/**----------------------------------------------------------------------------------------------------------
*  Rutina   : Toma_Tara_Platillos
*  Objetivo : Tomar las taras
*  Entrada  :
*  Descripcion :
*
------------------------------------------------------------------------------------------------------------*/
void Toma_Tara_Platillos(void)
    {
    uint8_t i; // Indice para recorrer los canales

    if (Flag_Enable_AD == false)    // Si llego al valor de fin de medicion del sincro fino, debe calcular el promedio
        {                           // de los valores acumulados
        if (Flag_Conv_FIN == true)  // Debe esperar a recibir la última conversión
            {                       // Esto se realiza con la salida BUSY_INT del conversor por la entrada de interrupcion
                                    // ETXI10 con flanco de bajada.
            Flag_Conv_FIN = false;  // Prepara el flag para una nueva medicion
            for (i = 0; i < 8; i++) // Calcula el promedio de los canales en forma sucesiva
                {
                Peso_Bruto_CH[i] = Suma_Canal_AD[i] / Cantidad_Mediciones; // Es un promedio simple de cada canal
                Peso_Bruto_CH[i] = Peso_Bruto_CH[i] >> 2;                  // Vengo con el dato en 15 bits y lo dejo en 13 Bits (8142 cuentas)
                Tara[i][Conta_Ejes] = Peso_Bruto_CH[i];                    // Almacena la tara del plato medido por cada linea
                }
            }
        }
    }

/**----------------------------------------------------------------------------------------------------------
*  Rutina   : Toma_Taras
*  Objetivo : Tomar las taras
*  Entrada  :
*  Descripcion :primero mando Escape, datos generales (79), despues comando (81) alta, Nº de serie, fecha,
*  				despues mando un escape y por último mando (0511) es porque antiguamente se mandaba Nº de línea
*  				y cantidad de vueltas.
*
------------------------------------------------------------------------------------------------------------*/
void Toma_Taras(void)
    {
    Flag_Tara_Inicial = true;
    if (Flag_Maq_Fun == false) // Máquina parada => aborta el proceso de toma de taras
        {
        TX_Nak(Comu_PC);           // Transmito NACK a la PC informando que no se pueden tomar las taras
        Flag_Enable_Taras = false; // Deshabilita la toma de taras desde la PC
        Flag_Init_Tara = false;    // Deshabilita la toma de taras de los platillos desde el comando recibido por la PC
        }
    else
        {
        Flag_Init_Tara = true; // Pone el flag para habilitar la toma de taras de los platillos
        Flag_SecuEje1 = false;
        Flag_Enable_Taras = false;
        }
    }

/**----------------------------------------------------------------------------------------------------------
*  Rutina   : Estado_Tara
*  Objetivo : Respuesta a la PC sobre la tarea detoma de tara
*  Entrada  :
*  Descripcion :
*
*
------------------------------------------------------------------------------------------------------------*/
void Estado_Tara(void)
    {
    if (Flag_Maq_Fun == false) // Máquina parada => aborta el proceso de toma de taras
        {
        Armo_Cabecera(Comu_PC);         // Armo la cabecera de la transmisión (Pone el STX, Nodo Destino y Nodo Origen)
        Comunica_PC.buf_tx_PC[5] = '0'; // Longitud a transmitir
        Comunica_PC.buf_tx_PC[6] = '9';
        Comunica_PC.buf_tx_PC[7] = 'M'; // Devuelve a la PC "M.Parada"
        Comunica_PC.buf_tx_PC[8] = '.';
        Comunica_PC.buf_tx_PC[9] = 'P';
        Comunica_PC.buf_tx_PC[10] = 'a';
        Comunica_PC.buf_tx_PC[11] = 'r';
        Comunica_PC.buf_tx_PC[12] = 'a';
        Comunica_PC.buf_tx_PC[13] = 'd';
        Comunica_PC.buf_tx_PC[14] = 'a';
        Comunica_PC.buf_tx_PC[15] = Null; // Agrego el terminador del frame
        Transmito_string(Comu_PC, true);  // Calculo en check sum y transmito el frame

        // si usamos la toma de tara desde el NET)
        Flag_Enable_Taras = false; // Deshabilita la toma de taras desde la PC
        Flag_Init_Tara = false;    // Deshabilita la toma de taras de los platillos desde el comando recibido por la PC
        }

    else // Maquina funcionando => Continua con la toma de taras
        {
        if (Flag_Tara_OK == false) // Mientras esta realizando el proceso de toma de taras, debe enviar un "Wait" a la PC
            {
            Armo_Cabecera(Comu_PC);         // Armo la cabecera de la transmisión (Pone el STX, Nodo Destino y Nodo Origen)
            Comunica_PC.buf_tx_PC[5] = '0'; // Longitud a transmitir
            Comunica_PC.buf_tx_PC[6] = '5';
            Comunica_PC.buf_tx_PC[7] = 'W'; // Devuelve a la PC "Wait"
            Comunica_PC.buf_tx_PC[8] = 'a';
            Comunica_PC.buf_tx_PC[9] = 'i';
            Comunica_PC.buf_tx_PC[10] = 't';
            Comunica_PC.buf_tx_PC[11] = Null; // Agrego el terminador del frame
            Transmito_string(Comu_PC, true);  // Calculo en check sum y transmito el frame
            }
        else // Cuando finaliza el proceso de toma de taras debe enviar un "Fin" a la PC
            {
            Armo_Cabecera(Comu_PC);         // Armo la cabecera de la transmisión (Pone el STX, Nodo Destino y Nodo Origen)
            Comunica_PC.buf_tx_PC[5] = '0'; // Longitud a transmitir
            Comunica_PC.buf_tx_PC[6] = '4';
            Comunica_PC.buf_tx_PC[7] = 'F'; // Devuelve a la PC "Wait"
            Comunica_PC.buf_tx_PC[8] = 'i';
            Comunica_PC.buf_tx_PC[9] = 'n';
            Comunica_PC.buf_tx_PC[10] = Null; // Agrego el terminador del frame
            Transmito_string(Comu_PC, true);  // Calculo en check sum y transmito el frame
            }
        }
    }

/**----------------------------------------------------------------------------------------------------------
*  Rutina   : Verificar_Taras
*  Objetivo : Verificar taras
*  Entrada  :
*  Descripcion : Se mide el peso bruto (cuentas), luego se verifica si esta dentro del error aceptable de tara
*  definido en la configuracion de la maquina (Config.error_tara) y luego se arma un frame para enviar a la PC
*  el numero de platillo y el resultado de la verificacion por linea, indicando con 0 si hay error de tara
*  y con 1 si la tara esta dentro del valor aceptable
*
*	Frame a transmitir a la PC:
*   Comunica_PC.buf_tx_PC[0] = STX				Cargo el chr de comienzo de Tx (STX)
*   Comunica_PC.buf_tx_PC[1] = NodoOrRe[0]		Nodo destino
*   Comunica_PC.buf_tx_PC[2] = NodoOrRe[1]		Nodo destino
*   Comunica_PC.buf_tx_PC[3] = Config.Nodo[0]	Nodo origen
*   Comunica_PC.buf_tx_PC[4] = Config.Nodo[1]	Nodo origen
*	Comunica_PC.buf_tx_PC[5] = '1'				Longitud frame - byte alto en ASCII
*	Comunica_PC.buf_tx_PC[6] = '2'				Longitud frame - byte bajo en ASCII
*	Comunica_PC.buf_tx_PC[7] = 'n'				Numero de platillo byte mas alto en ASCII
*	Comunica_PC.buf_tx_PC[8] = 'n'				Numero de platillo 2do byte alto en ASCII
*	Comunica_PC.buf_tx_PC[9] = 'n'				Numero de platillo byte mas bajo en ASCII
*	Comunica_PC.buf_tx_PC[10] = 'n'				Linea 1 - Resultado de la verificacion de tara (0 - error de tara / 1 - Tara correcta)
*	Comunica_PC.buf_tx_PC[11] = 'n'				Linea 2 - Resultado de la verificacion de tara (0 - error de tara / 1 - Tara correcta)
*	Comunica_PC.buf_tx_PC[12] = 'n'				Linea 3 - Resultado de la verificacion de tara (0 - error de tara / 1 - Tara correcta)
*	Comunica_PC.buf_tx_PC[13] = 'n'				Linea 4 - Resultado de la verificacion de tara (0 - error de tara / 1 - Tara correcta)
*	Comunica_PC.buf_tx_PC[14] = 'n'				Linea 5 - Resultado de la verificacion de tara (0 - error de tara / 1 - Tara correcta)
*	Comunica_PC.buf_tx_PC[15] = 'n'				Linea 6 - Resultado de la verificacion de tara (0 - error de tara / 1 - Tara correcta)
*	Comunica_PC.buf_tx_PC[16] = 'n'				Linea 7 - Resultado de la verificacion de tara (0 - error de tara / 1 - Tara correcta)
*	Comunica_PC.buf_tx_PC[17] = 'n'				Linea 8 - Resultado de la verificacion de tara (0 - error de tara / 1 - Tara correcta)
*	Comunica_PC.buf_tx_PC[18] = Null			Terminador del frame NULL en ASCII
*
* La longitud del frame se mide comenzando con el byte siguiente al byte bajo de la longitud (Comunica_PC.buf_tx_PC[7]) y se cuenta
* hasta el Null al final del frame, INCLUSIVE
* Cuando se ejecuta el proceso de transmision del string, se calcula el CHKSUM byte a byte, hasta llegar al Null
* Luego se reemplaza en el frame el Null por el resultado del CHKSUM y se agrega un byte con Null al final del frame
* Recien cuando esta armado el frame con el CHKSUM se procede a enviarlo
*
*
------------------------------------------------------------------------------------------------------------*/
void Verificar_Taras(void)
    {
    int8_t j;
    char aux[4];
    int16_t Peso_temp; // Auxiliar para calcular la diferencia del peso bruto con la tara

    if (Flag_Enable_Taras == true) // Verifica si esta habilitada la toma de taras desde la PC
        {
        if (Flag_Maq_Fun == false) // Máquina parada => aborta el proceso de toma de taras
            {
            TX_Nak(Comu_PC);           // Transmito NACK a la PC informando que no se pueden tomar las taras
            Flag_Enable_Taras = false; // Deshabilita la toma de taras desde la PC
            }
        else // Maquina funcionando => se pueden verificar las taras
            {
            Medicion_Bruto();       // Calcula el Peso bruto (cuentas) y lo devuelve en Peso_Bruto_CH[j]
            for (j = 0; j < 8; j++) // Verifica la tara de los 8 canales con cada platillo que avanza
                {
                Peso_temp = Peso_Bruto_CH[j] - Tara[j][Cant_Ejes_Med];
                if (Peso_temp >= 0)
                    {
                    if (Peso_temp > Config_FL.error_tara)
                        Comunica_PC.buf_tx_PC[j + 10] = '0';
                    else
                        Comunica_PC.buf_tx_PC[j + 10] = '1';
                    }
                else
                    {
                    Peso_temp = Peso_temp * (-1);
                    if (Peso_temp > Config_FL.error_tara)
                        Comunica_PC.buf_tx_PC[j + 10] = '0';
                    else
                        Comunica_PC.buf_tx_PC[j + 10] = '1';
                    }
                }
            Armo_Cabecera(Comu_PC);         // Armo la cabecera de la transmisión (Pone el STX, Nodo Destino y Nodo Origen)
            Comunica_PC.buf_tx_PC[5] = '1'; // Longitud a transmitir
            Comunica_PC.buf_tx_PC[6] = '2';
            mi_itoa(Cant_Ejes_Med, (int8_t *)&aux[0], 3, true); // Convierte el numero de platillo a ASCII
            Comunica_PC.buf_tx_PC[7] = aux[0];
            Comunica_PC.buf_tx_PC[8] = aux[1];
            Comunica_PC.buf_tx_PC[9] = aux[2];
            Comunica_PC.buf_tx_PC[18] = Null; // Agrego el terminador del frame
            Transmito_string(Comu_PC, true);  // Calculo en check sum y transmito el frame
            }
        }
    }

/**----------------------------------------------------------------------------------------------------------
*  Rutina   : Get_Osciloscopio
*  Objetivo : Ver los datos de conversion en cuentas en un grafico en la PC
*  Entrada  :
*  Descripcion :
*
*  La variable Canal_AD[i][j] se define en Var_Ram.h con la siguiente capacidad: Canal_AD[8][250]
*  COmo se utiliza un encoder de 100 pulsos y se miden dos platillos mas 20 pulsos adicionales, serían 220 mediciones
*  con lo cual quedan cubiertas todas las mediciones
*  Pero si se pone un encoder de myor cantidad de pulsos, hay que modificar la capacidad de esta variable
*
------------------------------------------------------------------------------------------------------------*/
void Get_Osciloscopio(void)
    {
    uint8_t i; // Indice para recorrer los canales

    if (Flag_Osciloscopio == false)
        {                          // Entra por primera vez al modo osciloscopio y activa el flag de modo osciloscopio
        if (Flag_Maq_Fun == false) // Máquina parada => no puede ejecutar el modo osciloscopio
            {
            TX_Nak(Comu_PC);            // Transmito NACK a la PC informando que no se pueden tomar las taras
            Flag_Osciloscopio = false;  // Deshabilita la toma de taras desde la PC
            Flag_Oscilos_Start = false; // Inicializa el indicador de comienzo del modo osciloscopio
            Flag_Oscilos_End = false;   // Inicializa el indicador de fin del modo osciloscopio
            }
        else // Maquina funcionando => da comienzo a las mediciones para el modo osciloscopio
            {
            Flag_Osciloscopio = true;                           // Activo el indicador del modo ver cuentas para hacer las mediciones de dos platillos consecutivos
            Flag_Oscilos_End = false;                           // Inicializa el indicador de fin del modo osciloscopio
            Flag_Oscilos_Start = false;                         // Inicializa el indicador de comienzo del modo osciloscopio (se activa cuando ingresa la interrupcion de plato)
            contador_mediciones = 0;                            // Inicializo el indice para almacenar las mediciones con cada pulso de sincronismo fino
            Cant_Pulsos = (Config.Cant_Pulsos_Sincro * 2) + 20; // Calculo la cantidad de pulsos para 2 platillos + 20 pulsos adicionales
                                                                // Init_Medicion();					// Inicializa la suma acumulativa en interrupciones
            }
        }
    else                           // Esta en modo osciloscopio y tiene que esperar que haga una cantidad de mediciones para promediar y
        {                          // guardar en memoria para transferir luego a la PC todas las mediciones juntas
        if (Flag_Maq_Fun == false) // Máquina parada => aborta el modo osciloscopio
            {
            TX_Nak(Comu_PC);            // Transmito NACK a la PC informando que no se puede hacer el modo osciloscopio
            Flag_Osciloscopio = false;  // Deshabilita el modo osciloscopio
            Flag_Oscilos_Start = false; // Inicializa el indicador de comienzo del modo osciloscopio
            Flag_Oscilos_End = false;   // Inicializa el indicador de fin del modo osciloscopio
            }

        else                                             // Maquina funcionando => hace una cantidad de mediciones con cada pulso de sincro fino,
            {                                            // calcula el promedio y almacena en memoria el promedio
            if (Cantidad_Mediciones >= Cant_Med_Oscilos) // La Cantidad_Mediciones se contabiliza con cada interrupcion de una medicion (ver stm32f4xx_it.c)
                {
                Flag_Enable_AD = false; // Cuando llega a la cantidad de mediciones definida en Cant_Med_Oscilos calcula el promedio
                Medicion_Bruto();       // Cuando llegó a realizar la cantidad de conversiones requerida, calcula el promedio y lo devuelve en Peso_Bruto_CH[]
                for (i = 0; i < 8; i++) // Transfiere la medicion de cada canal a una memoria temporal para luego transferir a la PC los valores
                    {
                    Canal_AD[i][contador_mediciones] = Peso_Bruto_CH[i]; // Transfiere el resultado de una medicion a una memoria intermedia para luego enviar a la PC todas juntas
                    }
                // Init_Medicion();				// Inicializa la suma acumulativa en interrupciones
                }

            if (Flag_Oscilos_End == true) // Termino de medir todos los pulsos de sincro fino?
                {
                Flag_Osciloscopio = false;  // Deshabilita el modo osciloscopio
                Flag_Oscilos_Start = false; // Inicializa el indicador de comienzo del modo osciloscopio
                TX_Ack(Comu_PC);            // Transmito ACK a la PC informando que finalizo el proceso de toma de mediciones para el osciloscopio
                Comando_recibido = null;    // Borra el comando actual para que no vuelva a entrar a la rutina de osciloscopio
                }
            }
        }
    }

/**----------------------------------------------------------------------------------------------------------
*  Rutina   : Datos_Curvas
*  Objetivo : Se transfieren a la PC los valores medidos con la rutina Get_Osciloscopio
*  Entrada  : Recibo el pedido de la PC ( 10 bytes )

                [stx][N# destino][N# Origen][largo string][comando][cant. datos][ck]
                  02     80          81          05          5F         xx        y
                  comando		=	"0x5F"	--> Pido_Curvas
                  N_Paquete		=	xx		--> Valor en ASCII de "00" a "49"

*  Descripcion : Se transmiten a la PC los datos medidos con la funcion Get_Osciloscopio
*  				que fueron almacenados en la variable Canal_AD[i][j]
*
                [stx][largo string][dato 0].......[dato 95][ck]		--- VERIFICAR SI SE DEBE ENVIAR NODO ORIGEN Y NODO DESTINO EN EL STRING

                Se transmiten 2 bytes por pulso de encoder y por línea => 8 lineas * 2bytes = 16bytes
                Se transmiten N_paquetes en cada pedido que hace la PC
                La comunicacion permite enviar un frame de hasta 99 bytes => Frame = 16 bytes * 6 puntos = 96 bytes
                Para completar los 200 valores requeridos para armar la curva del osciloscopio (suponiendo 1 dato por cada pulso del encoder):
                200 valores en total / 6 datos por transmision = 34 transmisiones de 96 bytes cada una => 3264bytes de datos

                Los  encoder pueden ser de 100, 128 ó 200 pulsos
                Para la maquina de 6 líneas (version anterior del programa):
                100 pulsos --> 200 valores --> 50 paquetes de 96 bytes = 4800 bytes (12 bytes por punto medido: 6 lineas * 2 bytes)
                128 pulsos --> 256 valores --> 32 paquetes de 96 bytes = 3072 bytes
                200 pulsos --> 400 valores --> 25 paquetes de 96 bytes = 2400 bytes
*
------------------------------------------------------------------------------------------------------------*/
void Datos_Curvas(void)
    {
    int8_t Dato[6];
    uint8_t N_Paquete;
    uint16_t Puntero_Tx = 0;
    uint8_t j;
    uint8_t Canal_AD_LOW;
    uint8_t Canal_AD_HIGH;

    if (Flag_Oscilos_End == false) // Si no completo la rutina de medicion de valores => no hay nada que transferir
        {
        TX_Nak(Comu_PC); // Transmito NACK a la PC informando que no hay nada que transferir
        }
    else
        {
        Flag_Oscilos_End = false;           // Inicializa el indicador de fin del modo osciloscopio para que no vuelva a enviar los mismos datos
        Dato[0] = Comunica_PC.buf_rx_PC[9]; // Recupera la cantidad de paquetes solicitados desde la PC
        Dato[1] = Comunica_PC.buf_rx_PC[10];
        Dato[2] = Null;
        N_Paquete = mi_atoi(Dato, 3); // Convierte la cantidad de paquetes a transmitir a la PC a entero

        Armo_Cabecera(Comu_PC);         // Armo la cabecera de la transmisión
        Comunica_PC.buf_tx_PC[5] = '9'; // Longitud a transmitir es siempre la misma, independiente de la cantidad de pulsos del encoder
        Comunica_PC.buf_tx_PC[6] = '7'; // (VER EN LA DESCRIPCION DEL COMANDO AL COMIENZO)

        while (Puntero_Tx < 8)
            {
            for (j = 0; j < 6; j++) // Transmite 6 lineas porque el programa de la PC está preparado para recibir 6 lineas
                {
                Canal_AD_LOW = (Canal_AD[j][Puntero_Tx + (N_Paquete * 8)]) & 0x00FF; // Se selecciona la parte baja de la medicion
                Canal_AD_HIGH = (Canal_AD[j][Puntero_Tx + (N_Paquete * 8)]) >> 0x08; // Se selecciona la parte alta de la medicion

                Comunica_PC.buf_tx_PC[7 + (j * 2) + (Puntero_Tx * 12)] = Canal_AD_HIGH; // Arma el frame para enviar 6 lineas
                Comunica_PC.buf_tx_PC[8 + (j * 2) + (Puntero_Tx * 12)] = Canal_AD_LOW;
                }
            Puntero_Tx++;
            }

        Comunica_PC.buf_tx_PC[Frame_Oscilos] = Null; // Agrego el terminador del frame (luego lo reemplazara el Chksum)
        Transmito_oscilos();                         // Transmito el frame del modo osciloscopio
        }
    }
