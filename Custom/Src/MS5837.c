#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "i2c.h" // Replace with your platform-specific I2C library

#define MS5837_ADDR 0x76
#define MS5837_RESET 0x1E
#define MS5837_ADC_READ 0x00
#define MS5837_PROM_READ 0xA0
#define MS5837_CONVERT_D1_8192 0x4A
#define MS5837_CONVERT_D2_8192 0x5A

#define PA_CONVERSION 100.0f
#define BAR_CONVERSION 0.001f
#define MBAR_CONVERSION 1.0f

#define MS5837_30BA 0
#define MS5837_02BA 1
#define MS5837_UNRECOGNISED 255

#define MS5837_02BA01 0x00
#define MS5837_02BA21 0x15
#define MS5837_30BA26 0x1A

static uint16_t C[7];
static uint32_t D1_pres, D2_temp;
static int32_t TEMP;
static int32_t P;
static uint8_t model;
static float fluidDensity = 1029;

static uint8_t crc4(uint16_t n_prom[]);
static void calculate(void);

bool MS5837_init() {
    // Reset the MS5837
    I2C_WriteByte(MS5837_ADDR, MS5837_RESET);
    delay(10);

    // Read calibration values
    for (uint8_t i = 0; i < 7; i++) {
        I2C_WriteByte(MS5837_ADDR, MS5837_PROM_READ + i * 2);
        uint8_t data[2];
        I2C_ReadBytes(MS5837_ADDR, data, 2);
        C[i] = (data[0] << 8) | data[1];
    }

    // Verify CRC
    uint8_t crcRead = C[0] >> 12;
    uint8_t crcCalculated = crc4(C);

    if (crcCalculated != crcRead) {
        return false;
    }

    uint8_t version = (C[0] >> 5) & 0x7F;
    if (version == MS5837_02BA01 || version == MS5837_02BA21) {
        model = MS5837_02BA;
    } else if (version == MS5837_30BA26) {
        model = MS5837_30BA;
    } else {
        model = MS5837_UNRECOGNISED;
    }

    return true;
}

void MS5837_setFluidDensity(float density) {
    fluidDensity = density;
}

void MS5837_read() {
    // Request D1 conversion
    I2C_WriteByte(MS5837_ADDR, MS5837_CONVERT_D1_8192);
    delay(20);

    I2C_WriteByte(MS5837_ADDR, MS5837_ADC_READ);
    uint8_t data[3];
    I2C_ReadBytes(MS5837_ADDR, data, 3);
    D1_pres = (data[0] << 16) | (data[1] << 8) | data[2];

    // Request D2 conversion
    I2C_WriteByte(MS5837_ADDR, MS5837_CONVERT_D2_8192);
    delay(20);

    I2C_WriteByte(MS5837_ADDR, MS5837_ADC_READ);
    I2C_ReadBytes(MS5837_ADDR, data, 3);
    D2_temp = (data[0] << 16) | (data[1] << 8) | data[2];

    calculate();
}

float MS5837_pressure(float conversion) {
    return model == MS5837_02BA ? P * conversion / 100.0f : P * conversion / 10.0f;
}

float MS5837_temperature() {
    return TEMP / 100.0f;
}

float MS5837_depth() {
    return (MS5837_pressure(PA_CONVERSION) - 101300) / (fluidDensity * 9.80665);
}

float MS5837_altitude() {
    return (1 - pow((MS5837_pressure() / 1013.25), 0.190284)) * 145366.45 * 0.3048;
}

static void calculate() {
    int32_t dT = D2_temp - ((uint32_t)C[5] << 8);
    int64_t SENS, OFF;

    if (model == MS5837_02BA) {
        SENS = (int64_t)C[1] * 65536 + ((int64_t)C[3] * dT) / 128;
        OFF = (int64_t)C[2] * 131072 + ((int64_t)C[4] * dT) / 64;
        P = (D1_pres * SENS / 2097152 - OFF) / 32768;
    } else {
        SENS = (int64_t)C[1] * 32768 + ((int64_t)C[3] * dT) / 256;
        OFF = (int64_t)C[2] * 65536 + ((int64_t)C[4] * dT) / 128;
        P = (D1_pres * SENS / 2097152 - OFF) / 8192;
    }

    TEMP = 2000 + ((int64_t)dT * C[6]) / 8388608;
    // Additional compensation logic here if needed
}

static uint8_t crc4(uint16_t n_prom[]) {
    uint16_t n_rem = 0;
    n_prom[0] = n_prom[0] & 0x0FFF;
    n_prom[7] = 0;

    for (uint8_t i = 0; i < 16; i++) {
        if (i % 2 == 1) {
            n_rem ^= (n_prom[i >> 1] & 0x00FF);
        } else {
            n_rem ^= (n_prom[i >> 1] >> 8);
        }

        for (uint8_t n_bit = 8; n_bit > 0; n_bit--) {
            if (n_rem & 0x8000) {
                n_rem = (n_rem << 1) ^ 0x3000;
            } else {
                n_rem = (n_rem << 1);
            }
        }
    }

    return (n_rem >> 12) & 0xF;
}
