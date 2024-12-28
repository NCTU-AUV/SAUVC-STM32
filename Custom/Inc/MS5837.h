/* Blue Robotics Arduino MS5837-30BA Pressure/Temperature Sensor Library
------------------------------------------------------------

Title: Blue Robotics Arduino MS5837-30BA Pressure/Temperature Sensor Library

Description: This library provides utilities to communicate with and to
read data from the Measurement Specialties MS5837-30BA pressure/temperature
sensor.

Authors: Rustom Jehangir, Blue Robotics Inc.
         Adam Šimko, Blue Robotics Inc.

-------------------------------
The MIT License (MIT)

Copyright (c) 2015 Blue Robotics Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-------------------------------*/

#ifndef MS5837_H_BLUEROBOTICS
#define MS5837_H_BLUEROBOTICS

#include <stdint.h>
#include <stdbool.h>

extern const float MS5837_Pa;
extern const float MS5837_bar;
extern const float MS5837_mbar;

extern const uint8_t MS5837_MS5837_30BA;
extern const uint8_t MS5837_MS5837_02BA;
extern const uint8_t MS5837_MS5837_UNRECOGNISED;

void MS5837_MS5837();

bool MS5837_init(TwoWire &wirePort = Wire);
bool MS5837_begin(TwoWire &wirePort = Wire); // Calls init()

/** Set model of MS5837 sensor. Valid options are MS5837::MS5837_30BA (default)
 * and MS5837::MS5837_02BA.
 */
void MS5837_setModel(uint8_t model);
uint8_t MS5837_getModel();

/** Provide the density of the working fluid in kg/m^3. Default is for
 * seawater. Should be 997 for freshwater.
 */
void MS5837_setFluidDensity(float density);

/** The read from I2C takes up to 40 ms, so use sparingly is possible.
 */
void MS5837_read();

/** Pressure returned in mbar or mbar*conversion rate.
 */
float MS5837_pressure(float conversion = 1.0f);

/** Temperature returned in deg C.
 */
float MS5837_temperature();

/** Depth returned in meters (valid for operation in incompressible
 *  liquids only. Uses density that is set for fresh or seawater.
 */
float MS5837_depth();

/** Altitude returned in meters (valid for operation in air only).
 */
float MS5837_altitude();

#endif
