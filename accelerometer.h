#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <stdint.h>

/** 
 * @file accelerometer.h
 * @brief Plik nag³ówkowy sterownika akcelerometru.
 * Zawiera funkcje do inicjalizacji, odczytu danych, zerowania offsetów i obliczania przyspieszenia.
 */

/**
 * @def ACCELEROMETER_ADDR
 * @brief Adres I2C akcelerometru.
 */
#define ACCELEROMETER_ADDR 0x1D

/**
 * @def SAMPLE_SIZE
 * @brief Liczba próbek u¿ywana do obliczania i zerowania wartoœci offsetu.
 */
#define SAMPLE_SIZE 20

/**
 * @brief Inicjalizuje sprzêt akcelerometru.
 * Konfiguruje urz¹dzenie do normalnej pracy.
 */
void Accelerometer_Init(void);

/**
 * @brief Odczytuje surowe wartoœci przyspieszenia z akcelerometru.
 * 
 * @param x_value WskaŸnik do przechowywania surowych danych przyspieszenia na osi X.
 * @param y_value WskaŸnik do przechowywania surowych danych przyspieszenia na osi Y.
 * @param z_value WskaŸnik do przechowywania surowych danych przyspieszenia na osi Z.
 */
void Accelerometer_Read(int16_t *x_value, int16_t *y_value, int16_t *z_value);

/**
 * @brief Zeruje wartoœci offsetów dla wszystkich osi.
 * Wykonuje obliczenia œredniej na podstawie okreœlonej liczby próbek.
 */
void Zero_Offset(void);

/**
 * @brief Przekszta³ca surowe dane przyspieszenia na czytelne wartoœci przyspieszenia.
 * 
 * @param x_raw Surowe dane przyspieszenia na osi X.
 * @param y_raw Surowe dane przyspieszenia na osi Y.
 * @param z_raw Surowe dane przyspieszenia na osi Z.
 * @param x_acc WskaŸnik na wynikow¹ wartoœæ przyspieszenia na osi X.
 * @param y_acc WskaŸnik na wynikow¹ wartoœæ przyspieszenia na osi Y.
 * @param z_acc WskaŸnik na wynikow¹ wartoœæ przyspieszenia na osi Z.
 */
void Calculate_Acceleration(int16_t x_raw, int16_t y_raw, int16_t z_raw, float *x_acc, float *y_acc, float *z_acc);

/**
 * @brief Oblicza wartoœæ wektora przyspieszenia (magnitudê) na podstawie wartoœci przyspieszeñ.
 * 
 * @param x_acc Wartoœæ przyspieszenia na osi X.
 * @param y_acc Wartoœæ przyspieszenia na osi Y.
 * @return Magnituda wektora przyspieszenia.
 */
float Calculate_Magnitude(float x_acc, float y_acc);

#endif // ACCELEROMETER_H
