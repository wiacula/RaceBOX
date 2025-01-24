#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "klaw.h"
#include "i2c.h"
#include "SysTick.h"
#include "accelerometer.h"
#include <stdio.h>
#include <math.h>

volatile uint8_t S2_press = 0;  
volatile uint8_t S3_press = 0;  
volatile uint8_t S4_press = 0;  
uint8_t current_menu = 0;       

// Bufory i zmienne dla akcelerometru
volatile uint32_t counter_100ms = 0; 
volatile uint32_t tick_counter = 0;  
int16_t x_buffer[SAMPLE_SIZE];  
int16_t y_buffer[SAMPLE_SIZE];  
int16_t z_buffer[SAMPLE_SIZE];  
int buffer_index = 0;           
float velocity = 0.0;  
float time_0_to_30 = 0.0;
float time_0_to_50 = 0.0;
float time_0_to_100 = 0.0;
int measurement_active = 0;   
int results_displayed = 0; // Flaga, czy wyniki zosta³y wyœwietlone wyniki pomiaru

//PRZYCISKI

void PORTA_IRQHandler(void) {
    uint32_t buf = PORTA->ISFR & (S2_MASK | S3_MASK | S4_MASK);

    if (buf & S2_MASK) {
        DELAY(100);
        if (!(PTA->PDIR & S2_MASK)) {
            S2_press = 1;
        }
    }
    if (buf & S3_MASK) {
        DELAY(100);
        if (!(PTA->PDIR & S3_MASK)) {
            S3_press = 1;
        }
    }
    if (buf & S4_MASK) {
        DELAY(100);
        if (!(PTA->PDIR & S4_MASK)) {
            S4_press = 1;
        }
    }

    PORTA->ISFR |= S2_MASK | S3_MASK | S4_MASK;
    NVIC_ClearPendingIRQ(PORTA_IRQn);
}

//MENU

void Show_Menu(uint8_t menu) {
    LCD1602_ClearAll();
    switch (menu) {
        case 0:
            LCD1602_SetCursor(0, 0);
            LCD1602_Print("Pomiar predkosci");
            LCD1602_SetCursor(0, 1);
            LCD1602_Print("-> Start testu");
            break;
        case 1:
            LCD1602_SetCursor(0, 0);
            LCD1602_Print("Wyniki");
            LCD1602_SetCursor(0, 1);
            LCD1602_Print("-> Historia");
            break;
        case 2:
            LCD1602_SetCursor(0, 0);
            LCD1602_Print("Ustawienia");
            LCD1602_SetCursor(0, 1);
            LCD1602_Print("-> Opcje");
            break;
        default:
            LCD1602_SetCursor(0, 0);
            LCD1602_Print("Brak opcji");
            break;
    }
}

void Display_Startup_Message(void) {
    LCD1602_ClearAll();
    LCD1602_SetCursor(0, 0);
    LCD1602_Print("RACEBOX");
    LCD1602_SetCursor(0, 1);
    LCD1602_Print("by Jakub Wiacek");
    DELAY(3000);
    LCD1602_ClearAll();
}

//AKCELEROMETR

void add_to_buffer(int16_t *buffer, int16_t value) {
    buffer[buffer_index] = value; 
}

void Update_Velocity(float acceleration, float delta_time) {
    velocity += acceleration * delta_time * 3600 / 1000;  
    if (measurement_active) {
        if (velocity >= 30.0 && time_0_to_30 == 0.0) {
            time_0_to_30 = counter_100ms * 0.1;
        }
        if (velocity >= 50.0 && time_0_to_50 == 0.0) {
            time_0_to_50 = counter_100ms * 0.1;
        }
        if (velocity >= 100.0 && time_0_to_100 == 0.0) {
            time_0_to_100 = counter_100ms * 0.1;
            measurement_active = 0;
        }
    }
}

void float_to_string(float value, char *buffer) {
    int int_part = (int)value;
    int decimal_part = (int)((value - int_part) * 100);
    if (decimal_part < 0) decimal_part = -decimal_part;
    sprintf(buffer, "%d.%02d", int_part, decimal_part);
}

void Reset_Measurements() {
    velocity = 0.0;
    time_0_to_30 = 0.0;
    time_0_to_50 = 0.0;
    time_0_to_100 = 0.0;
    measurement_active = 0;
    results_displayed = 0; // Resetowanie flagi wyœwietlenia wyników
    counter_100ms = 0;
    Zero_Offset();
	  LCD1602_ClearAll();
    LCD1602_SetCursor(0, 0);
    LCD1602_Print("Reset pomiaru");
		DELAY(5000);
}

void Run_Measurement() {
        I2C_Init();
    LCD1602_Init();
    Accelerometer_Init();
    SystemCoreClockUpdate();
    SysTick_Timer_Init(1);
    Klaw_Init();
    Klaw_S2_4_Int();

    int16_t x_raw, y_raw, z_raw;
    float x_acc, y_acc, z_acc, magnitude;
    char display_buffer[16];
    float delta_time = 0.1;

    for (int i = 0; i < SAMPLE_SIZE; i++) {
        x_buffer[i] = 0;
        y_buffer[i] = 0;
        z_buffer[i] = 0;
    }

    Zero_Offset();

    while (1) {
        if (systick_flag) {
            systick_flag = 0;

            tick_counter++;
            if (tick_counter >= 100) {
                tick_counter = 0;
                counter_100ms++;

                Accelerometer_Read(&x_raw, &y_raw, &z_raw);
                add_to_buffer(x_buffer, x_raw);
                add_to_buffer(y_buffer, y_raw);
                add_to_buffer(z_buffer, z_raw);

                Calculate_Acceleration(x_raw, y_raw, z_raw, &x_acc, &y_acc, &z_acc);
                magnitude = Calculate_Magnitude(x_acc, y_acc);

                if (magnitude > 1.0 && !measurement_active) {
                    measurement_active = 1;
                    counter_100ms = 0; // Zerowanie licznika czasu
                    velocity = 0.0; // Zerowanie prêdkoœci
                }

                if (measurement_active) {
                    Update_Velocity(magnitude, delta_time);
                }

                if (!measurement_active && time_0_to_100 > 0.0 && !results_displayed) {
                    LCD1602_ClearAll();
                    LCD1602_SetCursor(0, 0);
                    LCD1602_Print("T30:");
                    float_to_string(time_0_to_30, display_buffer);
                    LCD1602_Print(display_buffer);

                    LCD1602_SetCursor(0, 1);
                    LCD1602_Print("T50:");
                    float_to_string(time_0_to_50, display_buffer);
                    LCD1602_Print(display_buffer);

                    LCD1602_SetCursor(8, 1);
                    LCD1602_Print("T100:");
                    float_to_string(time_0_to_100, display_buffer);
                    LCD1602_Print(display_buffer);

                    results_displayed = 1; // Ustawienie flagi wyœwietlenia wyników
                }
								
								if (results_displayed) {
									measurement_active = 0;}

                if (measurement_active || !results_displayed) {
                    LCD1602_ClearAll();
                    LCD1602_SetCursor(0, 0);
                    float_to_string(x_acc, display_buffer);
                    LCD1602_Print("X:");
                    LCD1602_Print(display_buffer);

                    float_to_string(y_acc, display_buffer);
                    LCD1602_Print(" Y:");
                    LCD1602_Print(display_buffer);

                    LCD1602_SetCursor(0, 1);
                    float_to_string(magnitude, display_buffer);
                    LCD1602_Print("Mag:");
                    LCD1602_Print(display_buffer);

                    LCD1602_SetCursor(8, 1);
                    float_to_string(velocity, display_buffer);
                    LCD1602_Print("Vel:");
                    LCD1602_Print(display_buffer);
                }
            }
        }

        if (S2_press) { // Obs³uga resetu
            S2_press = 0;
            Reset_Measurements();
        }
    }
}

int main(void) {
    Klaw_Init();
    Klaw_S2_4_Int();
    LCD1602_Init();
    LCD1602_Backlight(1);

    Display_Startup_Message();
    Show_Menu(current_menu);

    while (1) {
        if (S2_press) {
            S2_press = 0;
            current_menu = (current_menu + 1) % 3;
            Show_Menu(current_menu);
        }
        if (S3_press) {
            S3_press = 0;
            current_menu = (current_menu + 2) % 3;
            Show_Menu(current_menu);
        }
        if (S4_press) {
            S4_press = 0;
            if (current_menu == 0) {
                Run_Measurement();
            }
        }
    }

    return 0;
}
