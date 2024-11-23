#ifndef MOTOR_PWM_ESC_DRIVER
#define MOTOR_PWM_ESC_DRIVER

#include "stm32f4xx_hal.h"

typedef enum {
    MOTOR0 = 0,
    MOTOR1 = 1,
    MOTOR2 = 2,
    MOTOR3 = 3,
    MOTOR4 = 4,
    MOTOR5 = 5,
    MOTOR6 = 6,
    MOTOR7 = 7,
} MotorNumber;

void initialize_all_motors(
    TIM_HandleTypeDef *motor_0_htim,
    uint32_t motor_0_channel,

    TIM_HandleTypeDef *motor_1_htim,
    uint32_t motor_1_channel,

    TIM_HandleTypeDef *motor_2_htim,
    uint32_t motor_2_channel,

    TIM_HandleTypeDef *motor_3_htim,
    uint32_t motor_3_channel,

    TIM_HandleTypeDef *motor_4_htim,
    uint32_t motor_4_channel,

    TIM_HandleTypeDef *motor_5_htim,
    uint32_t motor_5_channel,

    TIM_HandleTypeDef *motor_6_htim,
    uint32_t motor_6_channel,

    TIM_HandleTypeDef *motor_7_htim,
    uint32_t motor_7_channel
);

void set_motor_pwm_output(MotorNumber motor_number, uint32_t compare_value);
void start_motor_pwm_output(MotorNumber motor_number);
void stop_motor_pwm_output(MotorNumber motor_number);

#endif