#ifndef THRUSTER_PWM_CONTROL_DRIVER
#define THRUSTER_PWM_CONTROL_DRIVER

#include "stm32f4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    THRUSTER0 = 0,
    THRUSTER1 = 1,
    THRUSTER2 = 2,
    THRUSTER3 = 3,
    THRUSTER4 = 4,
    THRUSTER5 = 5,
    THRUSTER6 = 6,
    THRUSTER7 = 7,
} ThrusterNumber;

void initialize_all_thrusters(
    TIM_HandleTypeDef *thruster_0_htim,
    uint32_t thruster_0_channel,

    TIM_HandleTypeDef *thruster_1_htim,
    uint32_t thruster_1_channel,

    TIM_HandleTypeDef *thruster_2_htim,
    uint32_t thruster_2_channel,

    TIM_HandleTypeDef *thruster_3_htim,
    uint32_t thruster_3_channel,

    TIM_HandleTypeDef *thruster_4_htim,
    uint32_t thruster_4_channel,

    TIM_HandleTypeDef *thruster_5_htim,
    uint32_t thruster_5_channel,

    TIM_HandleTypeDef *thruster_6_htim,
    uint32_t thruster_6_channel,

    TIM_HandleTypeDef *thruster_7_htim,
    uint32_t thruster_7_channel
);

void set_thruster_pwm_output(ThrusterNumber thruster_number, uint32_t output_signal_value_us);

void start_thruster_pwm_output(ThrusterNumber thruster_number);
void stop_thruster_pwm_output(ThrusterNumber thruster_number);

void start_all_thrusters_pwm_output();
void stop_all_thrusters_pwm_output();

bool is_pwm_output_on(ThrusterNumber thruster_number);
uint32_t get_pwm_output_signal_value_us(ThrusterNumber thruster_number);

#endif