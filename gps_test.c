#include "i2c.h"
#include "lcd1602.h"
#include "uart0.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GPS_UART_CHANNEL 0  
#define GPS_BUFFER_SIZE 128 

static char gps_buffer[GPS_BUFFER_SIZE];
static int gps_index = 0;

// Prototypy funkcji
void UART0_IRQHandler(void);
void parse_gpgga(char *nmea_sentence, char *latitude, char *longitude, int *satellite_count);
void display_gps_data(char *latitude, char *longitude, int satellite_count);

void UART0_IRQHandler(void) {
    if (UART0->S1 & UART0_S1_RDRF_MASK) { 
        char ch = UART0->D;
        if (ch == '\n' || ch == '\r') {
            gps_buffer[gps_index] = '\0';
            gps_index = 0; 
        } else {
            if (gps_index < sizeof(gps_buffer) - 1) {
                gps_buffer[gps_index++] = ch;
            }
        }
    }
}

void parse_gpgga(char *nmea_sentence, char *latitude, char *longitude, int *satellite_count) {
    char *token;
    int field_index = 0;

    token = strtok(nmea_sentence, ",");
    while (token != NULL) {
        field_index++;
        
        if (field_index == 3) { 
            strncpy(latitude, token, 10);
            latitude[10] = '\0'; 
        }
        
        if (field_index == 5) { 
            strncpy(longitude, token, 11); 
            longitude[11] = '\0'; 
        }

        if (field_index == 8) { 
            *satellite_count = atoi(token); 
        }

        token = strtok(NULL, ",");
    }
}

void display_gps_data(char *latitude, char *longitude, int satellite_count) {
    char display_buffer[16];

    LCD1602_ClearAll();
    LCD1602_SetCursor(0, 0);
    snprintf(display_buffer, sizeof(display_buffer), "LAT:%s", latitude);
    LCD1602_Print(display_buffer);
    
    LCD1602_SetCursor(0, 1);
    snprintf(display_buffer, sizeof(display_buffer), "LON:%s SAT:%d", longitude, satellite_count);
    LCD1602_Print(display_buffer);
}
