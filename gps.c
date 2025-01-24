#include "gps.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

static char gps_buffer[GPS_BUFFER_SIZE];
static volatile uint8_t gps_index = 0;


static void remove_leading_zeros(char *str) {
    while (*str == '0' && *(str + 1) != '\0') {
        memmove(str, str + 1, strlen(str)); 
    }
}


void shift_comma_left(char *str) {
    char *comma = strchr(str, '.');
    if (!comma || (comma - str) < 2) return; 


    size_t comma_pos = comma - str;
    str[comma_pos - 2] = '.'; 


    memmove(&str[comma_pos - 1], &str[comma_pos + 1], strlen(&str[comma_pos + 1]) + 1);
}


static bool parse_gpgga(const char *nmea, gps_data_t *data) {
    const char *gpgga_tag = "$GPGGA";
    if (strncmp(nmea, gpgga_tag, strlen(gpgga_tag)) != 0) {
        return false; 
    }

    char buffer[GPS_BUFFER_SIZE];
    strcpy(buffer, nmea); 

    char *token = strtok(buffer, ",");
    int field_index = 0;

    while (token != NULL) {
        switch (field_index) {
        case 2: // Latitude
            strcpy(data->latitude, token);
            remove_leading_zeros(data->latitude);
            shift_comma_left(data->latitude); 
            break;
        case 4: // Longitude
            strcpy(data->longitude, token);
            remove_leading_zeros(data->longitude);
            shift_comma_left(data->longitude);
            break;
        case 7: // Satellites
            strcpy(data->satellites, token);
            break;
        }
        token = strtok(NULL, ",");
        field_index++;
    }

    return true;
}


void GPS_Init(void) {
    gps_index = 0;
    memset(gps_buffer, 0, sizeof(gps_buffer));
}


void GPS_ProcessChar(char received) {
    if (received == '\n' || gps_index >= GPS_BUFFER_SIZE - 1) {
        gps_buffer[gps_index] = '\0';
        gps_index = 0;
    } else {
        gps_buffer[gps_index++] = received;
    }
}


bool GPS_ParseLocation(gps_data_t *data) {
    return parse_gpgga(gps_buffer, data);
}
