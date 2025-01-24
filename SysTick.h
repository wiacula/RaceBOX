#ifndef SYSTICK_TIMER_H
#define SYSTICK_TIMER_H

#include "MKL05Z4.h"
#include <stdint.h>

extern volatile uint32_t systick_elapsed_time_ms;
extern volatile uint8_t systick_flag;

/**
 * @brief Inicjalizuje timer SysTick z okreœlonym interwa³em czasowym.
 *
 * @param interval_ms Interwa³ przerwania w milisekundach.
 */
void SysTick_Timer_Init(uint32_t interval_ms);

/**
 * @brief Obs³uguje przerwanie SysTick.
 */
void SysTick_Handler(void);

/**
 * @brief Zwraca ca³kowity czas up³ywu w milisekundach od momentu startu.
 *
 * @return Czas w milisekundach.
 */
uint32_t SysTick_GetElapsedTime(void);

/**
 * @brief Zeruje licznik up³ywu czasu SysTick.
 */
void SysTick_ResetElapsedTime(void);

#endif // SYSTICK_TIMER_H
