#include "SysTick.h"

volatile uint32_t systick_elapsed_time_ms = 0;
volatile uint8_t systick_flag = 0;

void SysTick_Timer_Init(uint32_t interval_ms) {
    uint32_t load_value = (SystemCoreClock / 1000) * interval_ms; // Przelicz na ticki
    SysTick->LOAD = load_value - 1;         // Ustaw wartoœæ ³adowania
    SysTick->VAL = 0;                       // Wyzeruj licznik
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | // U¿yj zegara rdzenia
                    SysTick_CTRL_TICKINT_Msk |   // W³¹cz przerwania
                    SysTick_CTRL_ENABLE_Msk;     // W³¹cz SysTick
}

void SysTick_Handler(void) {
    systick_elapsed_time_ms++;               // Zwiêksz czas o 1 ms
    systick_flag = 1;                        // Ustaw flagê SysTick
}

uint32_t SysTick_GetElapsedTime(void) {
    return systick_elapsed_time_ms;
}

void SysTick_ResetElapsedTime(void) {
    systick_elapsed_time_ms = 0;
}
