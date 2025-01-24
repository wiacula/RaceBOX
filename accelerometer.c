#include "accelerometer.h"
#include "i2c.h"
#include <math.h>
#include <stdio.h>


float STEP = 0.001197*2;           // Przelicznik dla zakresu ±2g w m/s2/count
// Offsety do zerowania
int16_t x_offset = 0;  
int16_t y_offset = 0;  
int16_t z_offset = 0;  

void Accelerometer_Init(void) {
    // Ustaw zakres akcelerometru na ±2g (ustawienie rejestru XYZ_DATA_CFG)
    I2C_WriteReg(ACCELEROMETER_ADDR, 0x0E, 0x00); // 0x00 = ±2g, 0x01 = ±4g, 0x02 = ±8g

    // Ustaw CTRL_REG1 na 0x01 (aktywuj akcelerometr)
    I2C_WriteReg(ACCELEROMETER_ADDR, 0x2A, 0x01); 
}

void Accelerometer_Read(int16_t *x_value, int16_t *y_value, int16_t *z_value) {
    uint8_t x_high, x_low, y_high, y_low, z_high, z_low;

    // Odczyt X
    I2C_ReadReg(ACCELEROMETER_ADDR, 0x01, &x_high);
    I2C_ReadReg(ACCELEROMETER_ADDR, 0x02, &x_low);
    *x_value = (int16_t)((x_high << 8) | x_low);
    *x_value = *x_value >> 2; // Przesuniêcie w prawo o 2 bity, bo 14-bitowe dane

    // Odczyt Y
    I2C_ReadReg(ACCELEROMETER_ADDR, 0x03, &y_high);
    I2C_ReadReg(ACCELEROMETER_ADDR, 0x04, &y_low);
    *y_value = (int16_t)((y_high << 8) | y_low);
    *y_value = *y_value >> 2;

    // Odczyt Z
    I2C_ReadReg(ACCELEROMETER_ADDR, 0x05, &z_high);
    I2C_ReadReg(ACCELEROMETER_ADDR, 0x06, &z_low);
    *z_value = (int16_t)((z_high << 8) | z_low);
    *z_value = *z_value >> 2;
}

void Zero_Offset(void) {
    int16_t x_raw, y_raw, z_raw;
    long x_sum = 0, y_sum = 0, z_sum = 0;

    // Uœrednianie próbek w celu obliczenia offsetu
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        Accelerometer_Read(&x_raw, &y_raw, &z_raw);
        x_sum += x_raw;
        y_sum += y_raw;
        z_sum += z_raw;
        DELAY(10);  // Krótkie opó¿nienie miêdzy odczytami
    }

    // Obliczenie œredniego odczytu (offsetu)
    x_offset = x_sum / SAMPLE_SIZE;
    y_offset = y_sum / SAMPLE_SIZE;
    z_offset = z_sum / SAMPLE_SIZE;
}

void Calculate_Acceleration(int16_t x_raw, int16_t y_raw, int16_t z_raw, float *x_acc, float *y_acc, float *z_acc) {
    *x_acc = (x_raw - x_offset) * STEP; 
    *y_acc = (y_raw - y_offset) * STEP;
    *z_acc = (z_raw - z_offset) * STEP;
}

float Calculate_Magnitude(float x_acc, float y_acc) {
    return sqrt(x_acc * x_acc + y_acc * y_acc);  // Obliczanie wartoœci wektora wypadkowego
}