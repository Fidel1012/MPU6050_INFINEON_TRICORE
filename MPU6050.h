/*
 * MPU6050.h
 *
 *  Created on: 6 ago. 2021
 *      Author: Fidel
 */

#ifndef MPU6050_INC_MPU6050_H_
#define MPU6050_INC_MPU6050_H_

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxI2c_I2c.h"
#include "IfxI2c.h"
#include "stdint.h"

/*Baudrate*/
#define     MPU6050_BAUDRATE    400000

/**
 * @defgroup    mpu_6050_c Resgitros
 * @brief       Registros del MPU6050.
 * @{
 */

/*Dirección I2C por default */
#define     MPU_I2C_DIR         0xD0

/*Dirección del registro WHO_AM_I*/
#define     WHO_AM_I            0x75

/*Valor del registro WHO_AM_I*/
#define     MPU_I_AM            0x68

#define     SELF_TEST_X         0x0D
#define     SELF_TEST_Y         0x0E
#define     SELF_TEST_Z         0x0F
#define     SELF_TEST_A         0x10
#define     SMPLRT_DIV          0x19
#define     CONFIG              0x1A
#define     GYRO_CONFIG         0x1B
#define     ACCEL_CONFIG        0x1C
#define     FIFO_EN             0x23
#define     I2C_MST_CTRL        0x24
#define     I2C_SLV0_ADDR       0X25
#define     I2C_SLV0_REG        0X26
#define     I2C_SLV0_CTRL       0x27
#define     I2C_SLV1_ADDR       0x28

#define     ACCEL_XOUT_H        0x3B
#define     ACCEL_XOUT_L        0x3C
#define     ACCEL_YOUT_H        0x3D
#define     ACCEL_YOUT_L        0x3E
#define     ACCEL_ZOUT_H        0x3F
#define     ACCEL_ZOUT_L        0x40
#define     TEMP_OUT_H          0x41
#define     TEMP_OUT_L          0x42
#define     GYRO_XOUT_H         0x43
#define     GYRO_XOUT_L         0x44
#define     GYRO_YOUT_H         0x45
#define     GYRO_YOUT_L         0x46
#define     GYRO_ZOUT_H         0x47
#define     GYRO_ZOUT_L         0x48

#define     USER_CTRL           0x6A
#define     PWR_MGMT_1          0x6B
#define     PWR_MGMT_2          0x6C

/**
 * @}
 */

/**
 * @defgroup    MPU_6060_Tasas de muestreo
 * @brief       Valores definidos para la tasa de muestreo.
 * @{
 */
#define     MPU6050_SampleRate_8KHz     0
#define     MPU6050_SampleRate_4KHz     1
#define     MPU6050_SampleRate_2KHz     3
#define     MPU6050_SampleRate_1KHz     7
#define     MPU6050_SampleRate_500Hz    15
#define     MPU6050_SampleRate_250Hz    31
#define     MPU6050_SampleRate_125Hz    63
#define     MPU6050_SampleRate_100Hz    79

/**
 * @defgroup    MPU_6060_Sensibilidades del acelerometro
 * @brief       Se definen los valores de las sensibilidades del acelerometro, en LSB/g para los multiplicadores de las lecturas.
 * @{
 */
#define     MPU6050_ACELEROMETRO_SENSIBILIDAD_2G    ((float)16384)
#define     MPU6050_ACELEROMETRO_SENSIBILIDAD_4G    ((float)8192)
#define     MPU6050_ACELEROMETRO_SENSIBILIDAD_8G    ((float)4096)
#define     MPU6050_ACELEROMETRO_SENSIBILIDAD_16G   ((float)2048)

/**
 * @}
 */

/**
 * @defgroup    MPU_6060_Sensibilidades del giroscopio
 * @brief       Se definen los valores de las sensibilidades del giroscopio, en LSB/°/S para los multiplicadores de las lecturas.
 * @{
 */
#define     MPU6050_GIROSCOPIO_SENSIBILIDAD_250    ((float)131)
#define     MPU6050_GIROSCOPIO_SENSIBILIDAD_500    ((float)65.5)
#define     MPU6050_GIROSCOPIO_SENSIBILIDAD_1000   ((float)32.8)
#define     MPU6050_GIROSCOPIO_SENSIBILIDAD_2000   ((float)16.4)

/**
 * @}
 */

/**
 * @defgroup    MPU_6060_Typedefs
 * @brief       Se definen los typedefs de la libreria.
 * @{
 */

/**
 * @brief   MPU_6050 Valores de retorno de las funciones.
 */
typedef enum
{
    MPU6050_OK  =   0x01,                   /*No hay problema*/
    MPU6050_ERROR,                          /*Error desconocido*/
    MPU6050_DispositivoDesconectado,        /*El disposotivo no se encuentra conectado*/
    MPU6050_DispositivoInvalido,            /*El dispositivo tiene una direccion diferente a la que se desea conectar*/
}MPU6050_Result;

/**
 * @brief   MPU_6050 Permite conectar dos acelerometros, dependiendo del valor logico del pin AD0, cuando AD0 esta en bajo
 *          la direccion, de 7 bits, es b1101000. Cuando el pin AD0 esta en alto, la dirección es b1101001.
 */
typedef enum
{
    MPU6050_Dispositivo_1   =   0x00U,
    MPU6050_Dispositivo_2   =   0x02U
}MPU6050_Dispositivo;

/**
 * @brief   Rangos de operación del acelerometro en g; 1g = 9.81 m/s2.
 */
typedef enum
{
    MPU6050_Acelerometro_2g =   0x00,   /*Rango +-2g */
    MPU6050_Acelerometro_4g =   0x01,   /*Rango +-4g */
    MPU6050_Acelerometro_8g =   0x02,   /*Rango +-8g */
    MPU6050_Acelerometro_16g=   0x03    /*Rango +-16g */
}MPU6050_Acelerometro_Rango;

/**
 * @brief   Rango del giroscopio en °/s.
 */
typedef enum
{
    MPU6050_Giroscopio_250  =   0x00,   /*Rango +- 250 °/s */
    MPU6050_Giroscopio_500,             /*Rango +- 500 °/s */
    MPU6050_Giroscopio_1000,            /*Rango +- 1000 °/s */
    MPU6050_Giroscopio_2000             /*Rango +- 2000 °/s */
}MPU6050_Giroscopio_Rango;

/**
 * @brief   Union que almacena las lecturas realizadas por el acelerometro o giroscopio. La lectura total se almacena en dos registros de 8 bits.
 */
typedef union {
        /*Publico*/
   int16_t LecturaTotal;                /*Almacena la lectura total, es decir los 16 bits*/
   struct {
           /*Privado*/
           uint8_t LecturaLSB;           /*Modifica los 8 bits menos significativos de la lectura*/
           uint8_t LecturaMSB;           /*Modifica los 8 bits mas significativos de la lectura*/
   } LecturaDividida;
}Lectura;

/**
 * @brief   Estructura donde se almacenan los datos referentes a las mediciones
 */
typedef struct
{
   /*Privado*/
   uint8_t Direccion;
   float Acelerometro_Multiplicador;
   float Giroscopio_Multiplicador;
   Lectura LecturaTemperatura;

   /*Publico*/
   Lectura Aceleracion_X;
   Lectura Aceleracion_Y;
   Lectura Aceleracion_Z;
   Lectura Giroscopio_X;
   Lectura Giroscopio_Y;
   Lectura Giroscopio_Z;
   float Temperatura;
}MPU6050_Typedef;

/*
 * @}
 */

/**
 * @defgroup    MPU_6060_Funciones
 * @brief       Funciones de la libreria
 * @{
 */

/**
 * @brief   Funcion para inicializar y configurar el dispositivo
 * @param   I2C_Handler: Puntero al controlador I2C nativo de iLLD library
 * @param   MPU6050_Valores: Puntero a estructura vacias @ref MPU6050_Typedef
 * @param   NumeroDispositivo: Indica la dirección de dispositivo @ref MPU6050_Dispositivo
 * @param   Acelerometro_Rango: Indica el rango de operación del acelerometro @ref MPU6050_Acelerometro_Rango
 * @param   Giroscopio_Rango: Indica el rango de operación del giroscopio @ref MPU6050_Giroscopio_Rango
 *
 * @retval  Miembro de la enumeracion @ref MPU6050_Result
 */
MPU6050_Result MPU6050_Init(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores, MPU6050_Dispositivo NumeroDispositivo, MPU6050_Acelerometro_Rango Acelerometro_Rango, MPU6050_Giroscopio_Rango Giroscopio_Rango);

/**
 * @brief   Función para configurar la frecuencia de muestreo, sample rate
 * @param   I2C_Handler: Puntero al controlador I2C nativo de iLLD library
 * @param   MPU6050_Valores: Puntero a estructura vacias @ref MPU6050_Typedef
 * @param   SampleRate: Frecuencia de muestreo @defgroup MPU_6060_Tasas de muestreo
 *
 * @retval  Miembro de la enumeracion @ref MPU6050_Result
 */
MPU6050_Result MPU6050_SampleRateConfig(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores, uint8_t SampleRate);

/**
 * @brief   Funcion para configurar el acelerometro
 * @param   I2C_Handler: Puntero al controlador I2C nativo de iLLD library
 * @param   MPU6050_Valores: Puntero a estructura vacias @ref MPU6050_Typedef
 * @param   Acelerometro_Rango: Rango de valores que puede tomar el acelerometro, valores aceptados son @ref MPU6050_Acelerometro_Rango
 *
 * @retval  Miembro de la enumeracion @ref MPU6050_Result
 */
MPU6050_Result MPU6050_AcelerometroConfig(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores, MPU6050_Acelerometro_Rango Acelerometro_Rango);

/**
 * @brief   Funcion para configurar el giroscopio
 * @param   I2C_Handler: Puntero al controlador I2C nativo de iLLD library
 * @param   MPU6050_Valores: Puntero a estrictura vacias @ref MPU6050_Typedef
 * @param   Acelerometro_Rango: Rango de valores que puede tomar el giroscopio, valores aceptados son @ref MPU6050_Giroscopio_Rango
 *
 * @retval  Miembro de la enumeracion @ref MPU6050_Result
 */
MPU6050_Result MPU6050_GiroscopioConfig(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores, MPU6050_Giroscopio_Rango Giroscopio_Rango);


/**
 * @brief   Función para recibir lecturas de acelerometro
 * @param   I2C_Handler: Puntero al controlador I2C nativo de iLLD library
 * @param   MPU6050_Valores: Puntero a estrictura vacias @ref MPU6050_Typedef
 *
 * @retval  Miembro de la enumeracion @ref MPU6050_Result
 */
MPU6050_Result MPU6050_LeerAcelerometro(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores);


/**
 * @brief   Función para recibir lecturas de giroscopio
 * @param   I2C_Handler: Puntero al controlador I2C nativo de iLLD library
 * @param   MPU6050_Valores: Puntero a estrictura vacias @ref MPU6050_Typedef
 *
 * @retval  Miembro de la enumeracion @ref MPU6050_Result
 */
MPU6050_Result MPU6050_LeerGiroscopio(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores);

/**
 * @brief   Función para recibir lecturas de termometro
 * @param   I2C_Handler: Puntero al controlador I2C nativo de iLLD library
 * @param   MPU6050_Valores: Puntero a estrictura vacias @ref MPU6050_Typedef
 *
 * @retval  Miembro de la enumeracion @ref MPU6050_Result
 */
MPU6050_Result MPU6050_LeerTemperatura(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores);

/**
 * @brief   Función para recibir lecturas de acelerometro, termometro y giroscopio a la vez
 * @param   I2C_Handler: Puntero al controlador I2C nativo de iLLD library
 * @param   MPU6050_Valores: Puntero a estrictura vacias @ref MPU6050_Typedef
 *
 * @retval  Miembro de la enumeracion @ref MPU6050_Result
 */
MPU6050_Result MPU6050_LeerTodo(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores);

/**
 * @}
 */

#endif /* MPU6050_INC_MPU6050_H_ */
