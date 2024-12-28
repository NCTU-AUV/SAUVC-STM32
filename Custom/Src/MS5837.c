#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "i2c.h" // Replace with your I2C implementation header

#define MS5837_ADDR 0x76
#define MS5837_RESET 0x1E
#define MS5837_ADC_READ 0x00
#define MS5837_PROM_READ 0xA0
#define MS5837_CONVERT_D1_8192 0x4A
#define MS5837_CONVERT_D2_8192 0x5A

const float Pa = 100.0f;
const float bar = 0.001f;
const float mbar = 1.0f;

const uint8_t MS5837_30BA = 0;
const uint8_t MS5837_02BA = 1;
const uint8_t MS5837_UNRECOGNISED = 255;

const uint8_t MS5837_02BA01 = 0x00;
const uint8_t MS5837_02BA21 = 0x15;
const uint8_t MS5837_30BA26 = 0x1A;

static uint16_t C[8];
static uint32_t D1_pres, D2_temp;
static int32_t TEMP;
static int32_t P;
static uint8_t model;
static float fluidDensity = 1029;

static uint8_t crc4(uint16_t n_prom[]);
static void calculate();

bool MS5837_init() {
    // Send reset command
    i2c_start(MS5837_ADDR);
    i2c_write(MS5837_RESET);
    i2c_stop();

    // Wait for reset to complete
    delay(10);

    // Read calibration values and CRC
    for (uint8_t i = 0; i < 7; i++) {
        i2c_start(MS5837_ADDR);
        i2c_write(MS5837_PROM_READ + i * 2);
        i2c_stop();

        i2c_request(MS5837_ADDR, 2);
        C[i] = (i2c_read() << 8) | i2c_read();
    }

    // Verify CRC
    uint8_t crcRead = C[0] >> 12;
    uint8_t crcCalculated = crc4(C);

    if (crcCalculated != crcRead) {
        return false; // CRC fail
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
    i2c_start(MS5837_ADDR);
    i2c_write(MS5837_CONVERT_D1_8192);
    i2c_stop();

    delay(20);

    // Read D1
    i2c_start(MS5837_ADDR);
    i2c_write(MS5837_ADC_READ);
    i2c_stop();

    i2c_request(MS5837_ADDR, 3);
    D1_pres = (i2c_read() << 16) | (i2c_read() << 8) | i2c_read();

    // Request D2 conversion
    i2c_start(MS5837_ADDR);
    i2c_write(MS5837_CONVERT_D2_8192);
    i2c_stop();

    delay(20);

    // Read D2
    i2c_start(MS5837_ADDR);
    i2c_write(MS5837_ADC_READ);
    i2c_stop();

    i2c_request(MS5837_ADDR, 3);
    D2_temp = (i2c_read() << 16) | (i2c_read() << 8) | i2c_read();

    calculate();
}

float MS5837_pressure(float conversion) {
    return (model == MS5837_02BA) ? P * conversion / 100.0f : P * conversion / 10.0f;
}

float MS5837_temperature() {
    return TEMP / 100.0f;
}

float MS5837_depth() {
    return (MS5837_pressure(Pa) - 101300) / (fluidDensity * 9.80665);
}

float MS5837_altitude() {
    return (1 - pow((MS5837_pressure() / 1013.25), .190284)) * 145366.45 * .3048;
}

static void calculate() {
    int32_t dT = D2_temp - (uint32_t)C[5] * 256;
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

    if (model == MS5837_02BA && TEMP / 100 < 20) {
        int32_t Ti = (11 * dT * dT) / 34359738368;
        int64_t OFFi = (31 * (TEMP - 2000) * (TEMP - 2000)) / 8;
        int64_t SENSi = (63 * (TEMP - 2000) * (TEMP - 2000)) / 32;

        OFF -= OFFi;
        SENS -= SENSi;
        TEMP -= Ti;
    }

    P = (D1_pres * SENS / 2097152 - OFF) / ((model == MS5837_02BA) ? 32768 : 8192);
}

static uint8_t crc4(uint16_t n_prom[]) {
    uint16_t n_rem = 0;
    n_prom[0] = n_prom[0] & 0x0FFF;
    n_prom[7] = 0;

    for (uint8_t i = 0; i < 16; i++) {
        n_rem ^= (i % 2) ? (n_prom[i >> 1] & 0x00FF) : (n_prom[i >> 1] >> 8);

        for (uint8_t n_bit = 8; n_bit > 0; n_bit--) {
            n_rem = (n_rem & 0x8000) ? (n_rem << 1) ^ 0x3000 : (n_rem << 1);
        }
    }

    return (n_rem >> 12) & 0x000F;
}
