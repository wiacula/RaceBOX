#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <stdint.h>

/** 
 * @file accelerometer.h
 * @brief Plik nag��wkowy sterownika akcelerometru.
 * Zawiera funkcje do inicjalizacji, odczytu danych, zerowania offset�w i obliczania przyspieszenia.
 */

/**
 * @def ACCELEROMETER_ADDR
 * @brief Adres I2C akcelerometru.
 */
#define ACCELEROMETER_ADDR 0x1D

/**
 * @def SAMPLE_SIZE
 * @brief Liczba pr�bek u�ywana do obliczania i zerowania warto�ci offsetu.
 */
#define SAMPLE_SIZE 20

/**
 * @brief Inicjalizuje sprz�t akcelerometru.
 * Konfiguruje urz�dzenie do normalnej pracy.
 */
void Accelerometer_Init(void);

/**
 * @brief Odczytuje surowe warto�ci przyspieszenia z akcelerometru.
 * 
 * @param x_value Wska�nik do przechowywania surowych danych przyspieszenia na osi X.
 * @param y_value Wska�nik do przechowywania surowych danych przyspieszenia na osi Y.
 * @param z_value Wska�nik do przechowywania surowych danych przyspieszenia na osi Z.
 */
void Accelerometer_Read(int16_t *x_value, int16_t *y_value, int16_t *z_value);

/**
 * @brief Zeruje warto�ci offset�w dla wszystkich osi.
 * Wykonuje obliczenia �redniej na podstawie okre�lonej liczby pr�bek.
 */
void Zero_Offset(void);

/**
 * @brief Przekszta�ca surowe dane przyspieszenia na czytelne warto�ci przyspieszenia.
 * 
 * @param x_raw Surowe dane przyspieszenia na osi X.
 * @param y_raw Surowe dane przyspieszenia na osi Y.
 * @param z_raw Surowe dane przyspieszenia na osi Z.
 * @param x_acc Wska�nik na wynikow� warto�� przyspieszenia na osi X.
 * @param y_acc Wska�nik na wynikow� warto�� przyspieszenia na osi Y.
 * @param z_acc Wska�nik na wynikow� warto�� przyspieszenia na osi Z.
 */
void Calculate_Acceleration(int16_t x_raw, int16_t y_raw, int16_t z_raw, float *x_acc, float *y_acc, float *z_acc);

/**
 * @brief Oblicza warto�� wektora przyspieszenia (magnitud�) na podstawie warto�ci przyspiesze�.
 * 
 * @param x_acc Warto�� przyspieszenia na osi X.
 * @param y_acc Warto�� przyspieszenia na osi Y.
 * @return Magnituda wektora przyspieszenia.
 */
float Calculate_Magnitude(float x_acc, float y_acc);

#endif // ACCELEROMETER_H
