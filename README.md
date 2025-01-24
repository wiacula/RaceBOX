Documentation
RACEBOX
Jakub Wiącek
24.01.2024

1. Introduction
Racebox is a system designed to measure vehicle acceleration times. An additional feature is GPS-based position tracking. The device is based on the FRDM-KL05Z microcontroller, its built-in MMA8451Q accelerometer, a 16x2 LCD display, a NEO-6M GPS module, and buttons. The project allows speed measurement, result display, and reset functionality.

2. Connections
16x2 LCD display:

GND -> GND
VCC -> +5V
SDA -> PTB4
SCL -> PTB3
4x4 Buttons:

C1 -> PTA12
C2 -> PTA11
C3 -> PTA10
C4 -> PTA9
GPS Module:

GND -> GND
VCC -> +5V
RX -> PTB2
TX -> PTB1
3. Libraries Used
main.c (main project file):
lcd1602.h - LCD display handling.
klaw.h - Button handling (S2, S3, S4).
i2c.h - I2C communication with modules.
SysTick.h - SysTick timer handling.
accelerometer.h - Accelerometer handling.
gps_test.c (GPS test file):
uart.h - UART communication for the GPS module.
gps.h - Functionality handling for the NEO-6M GPS module.
4. Key Features
Button Handling
Buttons S2, S3, and S4 allow the user to navigate the menu, start measurements, and reset results. Button handling is implemented in the PORTA_IRQHandler function, which detects interrupts from button presses and sets flags (S2_press, S3_press, S4_press) accordingly.

System Menu
The menu offers navigation between three main options:

Speed Measurement - Starts the test.
Results - Displays saved results (future implementation).
Settings - Configures additional options (future implementation).
The Show_Menu function handles menu display on the LCD.

Acceleration and Speed Measurement
Acceleration measurement is performed using the accelerometer, following these steps:

Zero Offset Calibration:

The Zero_Offset function sets baseline values for each axis (X, Y, Z), removing misalignment errors.
Data Collection:

The Accelerometer_Read function collects raw data from the accelerometer for X, Y, and Z axes.
Data is added to buffers (x_buffer, y_buffer, z_buffer) using the add_to_buffer function.
Acceleration Calculation:

The Calculate_Acceleration function converts raw data into acceleration values (m/s²), considering zero offset calibration.
The resultant acceleration vector is calculated using Calculate_Magnitude (Pythagorean theorem for X and Y).
Measurement Start:

If resultant acceleration exceeds a set threshold (e.g., 1 m/s²), the measurement starts (measurement_active flag set to 1), and the timer (counter_100ms) is reset.
Speed Calculation:

The Update_Velocity function integrates acceleration over time to calculate speed:

velocity
+
=
acceleration
×
delta_time
×
3600
1000
velocity+=acceleration×delta_time× 
1000
3600
​
 
The function also checks if speeds of 30 km/h, 50 km/h, and 100 km/h are reached, recording times (time_0_to_30, time_0_to_50, time_0_to_100).

Results Display:

Once 100 km/h is reached, the system stops measurement and displays results on the LCD using LCD1602_Print.
Result Display
Results are shown on the LCD in the following format:

T30: Time to reach 30 km/h.
T50: Time to reach 50 km/h.
T100: Time to reach 100 km/h.
After completing the measurement, pressing S2 resets the system.

GPS NEO-6M Support
(Not yet integrated into the main program; possible future implementation in the menu.)
RACEBOX integrates the GPS NEO-6M module for position tracking and satellite count. Key functions include:

UART0_IRQHandler - Handles UART communication for receiving GPS data.
parse_gpgga - Extracts NMEA GPGGA data (latitude, longitude, satellite count).
display_gps_data - Displays GPS data on the LCD.
GPS_Init - Initializes the GPS module.
GPS_ProcessChar - Processes incoming NMEA characters in real-time.
Data is displayed on the LCD in the following format:

LAT: Latitude.
LON: Longitude.
5. Program Structure
The main loop (main) performs the following operations:

System Initialization:
Initializes buttons, LCD, accelerometer, and GPS.
Displays a startup message using Display_Startup_Message.
Menu Navigation:
Navigation via S2 and S3 buttons.
Starts measurements via S4.
Acceleration Time Measurement:
Reads accelerometer data, calculates acceleration and speed, displays results on the LCD, and resets using S2.
6. Interrupt and Timer Handling
The device uses interrupts for button handling and the SysTick timer for generating 1ms interrupts. A counter_100ms timer is used to calculate speed and times for 0-30, 0-50, and 0-100 km/h.

7. Reset Measurements
In the Reset_Measurements function:

Variable Reset:
All measurement variables (e.g., velocity, times, counters) are set to zero.
Buffer Reset:
Accelerometer data buffers are cleared.
Zero Offset Calibration:
The Zero_Offset function recalibrates the accelerometer.
User Feedback:
The LCD displays a “Reset complete” message.
8. Conclusions and Future Development
RACEBOX functions as intended and can be used for vehicle acceleration testing. It features a simple, user-friendly interface. Future plans include:

GPS module integration into the menu.
Memory support for saving results.
Enclosure design.
