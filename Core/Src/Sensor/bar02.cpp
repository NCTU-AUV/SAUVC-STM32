#include "Sensor/bar02.h"
#include "math.h"

// sensor address
const uint8_t MS5837_ADDR = 0x76 << 1;
// sensor command byte
uint8_t MS5837_RESET = 0x1E;
uint8_t MS5837_ADC_READ = 0x00;
uint8_t MS5837_PROM_READ = 0xA0;
uint8_t MS5837_CONVERT_D1_8192 = 0x4A;
uint8_t MS5837_CONVERT_D2_8192 = 0x5A;

const float Bar02::Pa = 100.0f;
const float Bar02::bar = 0.001f;
const float Bar02::mbar = 1.0f;

const uint8_t Bar02::MS5837_30BA = 0;
const uint8_t Bar02::MS5837_02BA = 1;
const uint8_t Bar02::MS5837_UNRECOGNISED = 255;

const uint8_t MS5837_02BA01 = 0x00; // Sensor version: From MS5837_02BA datasheet Version PROM Word 0
const uint8_t MS5837_02BA21 = 0x15; // Sensor version: From MS5837_02BA datasheet Version PROM Word 0
const uint8_t MS5837_30BA26 = 0x1A; // Sensor version: From MS5837_30BA datasheet Version PROM Word 0

Bar02::Bar02()
{
    fluidDensity = 1029;
}

bool Bar02::set(I2C_HandleTypeDef* handler)
{
    h = handler;
    HAL_I2C_Master_Transmit(h, MS5837_ADDR, &(MS5837_RESET), 1, 1000);
    HAL_Delay(10);

    // Read calibration values amd CRC
    for (uint8_t i = 0; i < 7; i++){
        uint8_t buffer[2] = {0};
        uint8_t cmd = MS5837_PROM_READ + i * 2;

        HAL_I2C_Master_Transmit(h, MS5837_ADDR, &cmd, 1, 1000);
        HAL_I2C_Master_Receive(h, MS5837_ADDR, buffer, 2, 1000);
        C[i] = (buffer[0] << 8) | (buffer[1]);
    }

    // Verify CRC
    uint8_t crcRead = C[0] >> 12;
    uint8_t crcCalculated = crc4(C);

    if (crcCalculated != crcRead)
    {
        return false;
    }
        

	uint8_t version = (C[0] >> 5) & 0x7F; // Extract the sensor version from PROM Word 0

	// Set _model according to the sensor version
	if (version == MS5837_02BA01)
	{
		_model = MS5837_02BA;
	}
	else if (version == MS5837_02BA21)
	{
		_model = MS5837_02BA;
	}
	else if (version == MS5837_30BA26)
	{
		_model = MS5837_30BA;
	}
	else
	{
		_model = MS5837_UNRECOGNISED;
	}

    setFluidDensity(997);


    // *****************************************//
    // **** Initialization with offset below ***//
    // *****************************************//
     

    // remove initial sensor value (not accurate)
    depth_offset = 0;
    float temp = 0;
    for(int i = 0; i < 50; i++){
        read_value();
        temp = depth();
    }

    depth_offset = depth();

    return true;
}

void Bar02::setModel(uint8_t model)
{
    _model = model;
}

uint8_t Bar02::getModel()
{
	return (_model);
}

void Bar02::setFluidDensity(float density)
{
	fluidDensity = density;
}

void Bar02::read_value()
{
    uint8_t buffer[3]={0};

    if (h == nullptr)
        return;

    // D1 conversion
    HAL_I2C_Master_Transmit(h, MS5837_ADDR, &MS5837_CONVERT_D1_8192, 1, 1000);
    HAL_Delay(20);
    HAL_I2C_Master_Transmit(h, MS5837_ADDR, &MS5837_ADC_READ, 1, 1000);
    
    HAL_I2C_Master_Receive(h, MS5837_ADDR, buffer, 3, 1000);
    D1_pres = 0;
    D1_pres = buffer[0];
    D1_pres = (D1_pres << 8) | buffer[1];
    D1_pres = (D1_pres << 8) | buffer[2];

    buffer[0]=0; buffer[1]=0; buffer[2]=0;
    // D2 conversion
    HAL_I2C_Master_Transmit(h, MS5837_ADDR, &MS5837_CONVERT_D2_8192, 1, 1000);
    HAL_Delay(20);
    HAL_I2C_Master_Transmit(h, MS5837_ADDR, &MS5837_ADC_READ, 1, 1000);
    
    HAL_I2C_Master_Receive(h, MS5837_ADDR, buffer, 3, 1000);
    D2_temp = 0;
    D2_temp = buffer[0];
    D2_temp = (D2_temp << 8) | buffer[1];
    D2_temp = (D2_temp << 8) | buffer[2]; 

    calculate();
}


void Bar02::calculate()
{
    int32_t dT = 0;   
    int64_t SENS = 0;
    int64_t OFF = 0;  
    int32_t SENSi = 0;
	int32_t OFFi = 0; 
    int32_t Ti = 0; 
    int64_t OFF2 = 0; 
    int64_t SENS2 = 0;

    dT = D2_temp - uint32_t(C[5]) * 256l;
    if (_model == MS5837_02BA)
    {
        SENS = int64_t(C[1]) * 65536l + (int64_t(C[3] )* dT) / 128l;
        OFF = int64_t(C[2]) * 131072l + (int64_t(C[4]) * dT) / 64l;
        P = (D1_pres * SENS / (2097152l) - OFF) / (32768l);
    }
    else
    {
        SENS = int64_t(C[1]) * 32768l + (int64_t(C[3]) * dT) / 256l;
		OFF = int64_t(C[2]) * 65536l + (int64_t(C[4]) * dT) / 128l;
		P = (D1_pres*SENS / (2097152l) - OFF) / (8192l);
    }

    // Temp conversion
	TEMP = 2000l + int64_t(dT) * C[6] / 8388608LL;

    // Second order conversion
    if ( _model == MS5837_02BA )
    {
		if((TEMP / 100) < 20) //Low temp
        {
			Ti = (11 * int64_t(dT) * int64_t(dT)) / (34359738368LL);
			OFFi = (31 * (TEMP - 2000) * (TEMP - 2000)) / 8;
			SENSi = (63 * (TEMP - 2000) * (TEMP - 2000)) / 32;
		}
	}
    else
    {
		if((TEMP / 100) < 20)  //Low temp
        {         
			Ti = (3 * int64_t(dT) * int64_t(dT)) / (8589934592LL);
			OFFi = (3 * (TEMP - 2000) * (TEMP - 2000)) / 2;
			SENSi = (5 * (TEMP - 2000) * (TEMP - 2000)) / 8;
			if((TEMP / 100) < -15)  //Very low temp
            {    
				OFFi = OFFi + 7 * (TEMP + 1500l) * (TEMP + 1500l);
				SENSi = SENSi + 4 * (TEMP + 1500l) * (TEMP + 1500l);
			}
		}
		else if((TEMP / 100) >= 20)  //High temp
        {    
			Ti = 2 * (dT * dT) / (137438953472LL);
			OFFi = (1 * (TEMP - 2000) * (TEMP - 2000)) / 16;
			SENSi = 0;
		}
	}

    //Calculate pressure and temp second order
    OFF2 = OFF - OFFi;           
	SENS2 = SENS - SENSi;
    TEMP = (TEMP - Ti);
    
    if (_model == MS5837_02BA)
    {
		P = (((D1_pres * SENS2) / 2097152l - OFF2) / 32768l);
	}
    else
    {
		P = (((D1_pres * SENS2) / 2097152l - OFF2) / 8192l);
	}
}

float Bar02::pressure(float conversion) 
{
	if (_model == MS5837_02BA) 
    {
		return P * conversion / 100.0f;
	}
	else 
    {
		return P * conversion / 10.0f;
	}
}

float Bar02::temperature() 
{
	return TEMP / 100.0f;
}

float Bar02::depth() 
{
	return (pressure(Bar02::Pa) - 101300) / (fluidDensity * 9.80665) - depth_offset; // from experiment we measure the offset is 0.195m
}

float Bar02::altitude() 
{
	return (1 - pow((pressure() / 1013.25), 0.190284)) * 145366.45 * 0.3048;
}

uint8_t Bar02::crc4(uint16_t n_prom[])
{
	uint16_t n_rem = 0;

	n_prom[0] = ((n_prom[0]) & 0x0FFF);
	n_prom[7] = 0;

	for (uint8_t i = 0; i < 16; i++)
    {
		if (i % 2 == 1)
        {
			n_rem ^= (uint16_t)((n_prom[i >>1 ]) & 0x00FF);
		}
        else
        {
			n_rem ^= (uint16_t)(n_prom[i >> 1] >> 8);
		}
		for (uint8_t n_bit = 8; n_bit > 0; n_bit--)
        {
			if (n_rem & 0x8000)
            {
				n_rem = (n_rem << 1) ^ 0x3000;
			}
            else
            {
				n_rem = (n_rem << 1);
			}
		}
	}

	n_rem = ((n_rem >> 12) & 0x000F);

	return n_rem ^ 0x00;
}


/*

// in main : (USER CODE BEGIN 1)
    Bar02 depth_sensor;

// in USER CODE BEGIN 2
    bool tmp = depth_sensor.set(&hi2c1);

// in while loop  (USER CODE END WHILE)
    uart_buf_len = sprintf(uart_buf, "Depth: %f \r\n", depth_sensor.read_value());
    HAL_UART_Transmit(&huart4, (uint8_t*) uart_buf, uart_buf_len, 1000);
*/


// procedure
/*
    1. initialize Bar02 object
        # Bar02 depth_sensor;

    2. initialize sensor
        # bool tmp = depth_sensor.set(&hi2c1); 

    3. read value
        # depth_sensor.read_value();

    4. get depth value 
        # depth_sensor.depth();
*/