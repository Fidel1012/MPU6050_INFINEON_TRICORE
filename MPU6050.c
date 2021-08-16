/*
 * MPU6050.c
 *
 *  Created on: 6 ago. 2021
 *      Author: Fidel
 */
#include <MPU6050/Inc/MPU6050.h>
#include "MPU6050.h"
#include "stdint.h"
#include "IfxI2c_I2c.h"

#define LOGITUD_DIRECCION   1

MPU6050_Result MPU6050_Init(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores, MPU6050_Dispositivo NumeroDispositivo, MPU6050_Acelerometro_Rango Acelerometro_Rango, MPU6050_Giroscopio_Rango Giroscopio_Rango)
{
    /*Configurar dirección del dispositivo*/
    MPU6050_Valores->Direccion = MPU_I2C_DIR | (uint8_t)NumeroDispositivo;
    I2C_Handler->deviceAddress = MPU6050_Valores->Direccion;

    uint8_t direccionRegistro[LOGITUD_DIRECCION] = {WHO_AM_I};
    uint8_t datosLeidos[2] = {0,0};

    /*Verificar dispositivo*/
    /*-----------------------------------------------------------------------------------------------------*/
    while(IfxI2c_I2c_write(I2C_Handler, direccionRegistro, LOGITUD_DIRECCION) == IfxI2c_I2c_Status_nak);

    while(IfxI2c_I2c_read(I2C_Handler, &datosLeidos[0], 1) == IfxI2c_I2c_Status_nak);

    if(datosLeidos[0] != MPU_I_AM )
    {
        return MPU6050_DispositivoInvalido;
    }
    /*-----------------------------------------------------------------------------------------------------*/

    /*Wake up MPU6050*/
    /*-----------------------------------------------------------------------------------------------------*/
    uint8_t datosEscritos[2] = {PWR_MGMT_1,0x00};

    while(IfxI2c_I2c_write(I2C_Handler, datosEscritos, 2) == IfxI2c_I2c_Status_nak);

    /*-----------------------------------------------------------------------------------------------------*/

    /*Configuracion tasa de muestreo a 1KHz*/
    MPU6050_SampleRateConfig(I2C_Handler, MPU6050_Valores, MPU6050_SampleRate_1KHz);

    /*Configuración acelerometro*/
    MPU6050_AcelerometroConfig(I2C_Handler, MPU6050_Valores, Acelerometro_Rango);

    /*Configuracion giroscopio*/
    MPU6050_GiroscopioConfig(I2C_Handler, MPU6050_Valores, Giroscopio_Rango);

    return MPU6050_OK;
}

MPU6050_Result MPU6050_SampleRateConfig(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores, uint8_t SampleRate)
{
    uint8_t datosEscribir[] = {SMPLRT_DIV, SampleRate};

    if(IfxI2c_I2c_write(I2C_Handler, datosEscribir, 2) != IfxI2c_I2c_Status_ok)
    {
        return MPU6050_ERROR;
    }

    return MPU6050_OK;
}


MPU6050_Result MPU6050_AcelerometroConfig(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores, MPU6050_Acelerometro_Rango Acelerometro_Rango)
{
    uint8_t datos[2] = {ACCEL_CONFIG, 0x00};

    /*Configuración del acelerometro*/

    while(IfxI2c_I2c_write(I2C_Handler, &datos[0], LOGITUD_DIRECCION) == IfxI2c_I2c_Status_nak);

    while(IfxI2c_I2c_read(I2C_Handler, &datos[1], 1) == IfxI2c_I2c_Status_nak);

    datos[1] = (datos[1] & 0xE7) | (uint8_t)(Acelerometro_Rango << 3);

    while(IfxI2c_I2c_write(I2C_Handler, datos, 2) == IfxI2c_I2c_Status_nak);

    /*Establecer multiplicador de sensibilidad*/

    switch (Acelerometro_Rango)
    {
        case MPU6050_Acelerometro_2g:
            MPU6050_Valores->Acelerometro_Multiplicador = (float)1 / MPU6050_ACELEROMETRO_SENSIBILIDAD_2G;
            break;
        case MPU6050_Acelerometro_4g:
            MPU6050_Valores->Acelerometro_Multiplicador = (float)1 / MPU6050_ACELEROMETRO_SENSIBILIDAD_4G;
            break;
        case MPU6050_Acelerometro_8g:
            MPU6050_Valores->Acelerometro_Multiplicador = (float)1 / MPU6050_ACELEROMETRO_SENSIBILIDAD_8G;
            break;
        case MPU6050_Acelerometro_16g:
            MPU6050_Valores->Acelerometro_Multiplicador = (float)1 / MPU6050_ACELEROMETRO_SENSIBILIDAD_16G;
            break;
    }

    return MPU6050_OK;
}

MPU6050_Result MPU6050_GiroscopioConfig(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores, MPU6050_Giroscopio_Rango Giroscopio_Rango)
{
    uint8_t datos[] = {GYRO_CONFIG, 0x00};

    /*Configuración giroscopio*/
    while(IfxI2c_I2c_write(I2C_Handler, &datos[0], LOGITUD_DIRECCION) == IfxI2c_I2c_Status_nak);

    while(IfxI2c_I2c_read(I2C_Handler, &datos[1], 1) == IfxI2c_I2c_Status_nak);

    datos[1] = (datos[1] & 0xE7) | (uint8_t)(Giroscopio_Rango<< 3);

    while(IfxI2c_I2c_write(I2C_Handler, datos, 2) == IfxI2c_I2c_Status_nak);

    /*Establecer multiplicador de sensibilidad*/
    switch (Giroscopio_Rango)
    {
        case MPU6050_Giroscopio_250:
            MPU6050_Valores->Giroscopio_Multiplicador = (float)1 / MPU6050_GIROSCOPIO_SENSIBILIDAD_250;
            break;
        case MPU6050_Giroscopio_500:
            MPU6050_Valores->Giroscopio_Multiplicador = (float)1 / MPU6050_GIROSCOPIO_SENSIBILIDAD_500;
            break;
        case MPU6050_Giroscopio_1000:
            MPU6050_Valores->Giroscopio_Multiplicador = (float)1 / MPU6050_GIROSCOPIO_SENSIBILIDAD_1000;
            break;
        case MPU6050_Giroscopio_2000:
            MPU6050_Valores->Giroscopio_Multiplicador = (float)1 / MPU6050_GIROSCOPIO_SENSIBILIDAD_2000;
            break;
    }

    return MPU6050_OK;

}

MPU6050_Result MPU6050_LeerAcelerometro(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores)
{
    uint8_t datos[7] = {ACCEL_XOUT_H, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    /*Leer datos del acelerometro*/
    while(IfxI2c_I2c_write(I2C_Handler, &datos[0], 1) == IfxI2c_I2c_Status_nak);

    while(IfxI2c_I2c_read(I2C_Handler, &datos[1], 6) == IfxI2c_I2c_Status_nak);

    /*Guardar datos en estructura*/
    MPU6050_Valores->Aceleracion_X.LecturaDividida.LecturaMSB = datos[1];
    MPU6050_Valores->Aceleracion_X.LecturaDividida.LecturaLSB = datos[2];
    MPU6050_Valores->Aceleracion_Y.LecturaDividida.LecturaMSB = datos[3];
    MPU6050_Valores->Aceleracion_Y.LecturaDividida.LecturaLSB = datos[4];
    MPU6050_Valores->Aceleracion_Z.LecturaDividida.LecturaMSB = datos[5];
    MPU6050_Valores->Aceleracion_Z.LecturaDividida.LecturaLSB = datos[6];

    return MPU6050_OK;
}

MPU6050_Result MPU6050_LeerGiroscopio(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores)
{
    uint8_t datos[7] = {GYRO_XOUT_H , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    /*Leer datos del acelerometro*/
    while(IfxI2c_I2c_write(I2C_Handler, &datos[0], 1) == IfxI2c_I2c_Status_nak);

    while(IfxI2c_I2c_read(I2C_Handler, &datos[1], 6) == IfxI2c_I2c_Status_nak);

    /*Guardar datos en la estructura*/
    MPU6050_Valores->Giroscopio_X.LecturaDividida.LecturaMSB = datos[1];
    MPU6050_Valores->Giroscopio_X.LecturaDividida.LecturaLSB = datos[2];
    MPU6050_Valores->Giroscopio_Y.LecturaDividida.LecturaMSB = datos[3];
    MPU6050_Valores->Giroscopio_Y.LecturaDividida.LecturaLSB = datos[4];
    MPU6050_Valores->Giroscopio_Z.LecturaDividida.LecturaMSB = datos[5];
    MPU6050_Valores->Giroscopio_Z.LecturaDividida.LecturaLSB = datos[6];

    return MPU6050_OK;
}

MPU6050_Result MPU6050_LeerTemperatura(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores)
{
    uint8_t datos[3] = {TEMP_OUT_H, 0x00, 0x00};

    /*Leer datos de termometro*/
    while(IfxI2c_I2c_write(I2C_Handler, &datos[0], 1) == IfxI2c_I2c_Status_nak);

    while(IfxI2c_I2c_read(I2C_Handler, &datos[1], 2) == IfxI2c_I2c_Status_nak);

    /*Formato de datos*/
    MPU6050_Valores->LecturaTemperatura.LecturaDividida.LecturaMSB = datos[1];
    MPU6050_Valores->LecturaTemperatura.LecturaDividida.LecturaLSB = datos[2];

    /*Conversión a °C*/
    MPU6050_Valores->Temperatura = (float)((((float)MPU6050_Valores->LecturaTemperatura.LecturaTotal)/340) + 36.53);

    return MPU6050_OK;
}

MPU6050_Result MPU6050_LeerTodo(IfxI2c_I2c_Device *I2C_Handler, MPU6050_Typedef *MPU6050_Valores)
{
    uint8_t datos[15] = {ACCEL_XOUT_H, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    /*Leer datos*/
    while(IfxI2c_I2c_write(I2C_Handler, &datos[0], 1) == IfxI2c_I2c_Status_nak);

    while(IfxI2c_I2c_read(I2C_Handler, &datos[1], 14) == IfxI2c_I2c_Status_nak);

    /*Guardar datos en estructura*/
    MPU6050_Valores->Aceleracion_X.LecturaDividida.LecturaMSB = datos[1];
    MPU6050_Valores->Aceleracion_X.LecturaDividida.LecturaLSB = datos[2];
    MPU6050_Valores->Aceleracion_Y.LecturaDividida.LecturaMSB = datos[3];
    MPU6050_Valores->Aceleracion_Y.LecturaDividida.LecturaLSB = datos[4];
    MPU6050_Valores->Aceleracion_Z.LecturaDividida.LecturaMSB = datos[5];
    MPU6050_Valores->Aceleracion_Z.LecturaDividida.LecturaLSB = datos[6];
    MPU6050_Valores->LecturaTemperatura.LecturaDividida.LecturaMSB = datos[7];
    MPU6050_Valores->LecturaTemperatura.LecturaDividida.LecturaLSB = datos[8];
    MPU6050_Valores->Giroscopio_X.LecturaDividida.LecturaMSB = datos[9];
    MPU6050_Valores->Giroscopio_X.LecturaDividida.LecturaLSB = datos[10];
    MPU6050_Valores->Giroscopio_Y.LecturaDividida.LecturaMSB = datos[11];
    MPU6050_Valores->Giroscopio_Y.LecturaDividida.LecturaLSB = datos[12];
    MPU6050_Valores->Giroscopio_Z.LecturaDividida.LecturaMSB = datos[13];
    MPU6050_Valores->Giroscopio_Z.LecturaDividida.LecturaLSB = datos[14];

    MPU6050_Valores->Temperatura = (float)((((float)MPU6050_Valores->LecturaTemperatura.LecturaTotal)/340) + 36.53);

    return MPU6050_OK;
}




