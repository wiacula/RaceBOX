#ifndef GPS_H
#define GPS_H

#include <stdbool.h>
#include <stdint.h>

#define GPS_BUFFER_SIZE 128

// Structure to hold parsed GPS data
typedef struct {
    char latitude[16];
    char longitude[16];
    char satellites[8];
} gps_data_t;

/**
 * @brief Initialize the GPS module.
 */
void GPS_Init(void);

/**
 * @brief Process incoming GPS characters.
 *
 * This function is typically called from the UART interrupt handler.
 *
 * @param received The received character.
 */
void GPS_ProcessChar(char received);

/**
 * @brief Parse GPS data for location information from a $GPGGA sentence.
 *
 * @param data Pointer to gps_data_t structure to store parsed data.
 * @return true if parsing is successful, false otherwise.
 */
bool GPS_ParseLocation(gps_data_t *data);

#endif // GPS_H
