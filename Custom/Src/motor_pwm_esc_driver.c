#include "motor_pwm_esc_driver.h"

struct {
    TIM_HandleTypeDef *motor_htim;
    uint32_t motor_channel;
} motor_profiles[8];

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
)
{
    motor_profiles[MOTOR0].motor_htim = motor_0_htim;
    motor_profiles[MOTOR0].motor_channel = motor_0_channel;

    motor_profiles[MOTOR1].motor_htim = motor_1_htim;
    motor_profiles[MOTOR1].motor_channel = motor_1_channel;

    motor_profiles[MOTOR2].motor_htim = motor_2_htim;
    motor_profiles[MOTOR2].motor_channel = motor_2_channel;

    motor_profiles[MOTOR3].motor_htim = motor_3_htim;
    motor_profiles[MOTOR3].motor_channel = motor_3_channel;

    motor_profiles[MOTOR4].motor_htim = motor_4_htim;
    motor_profiles[MOTOR4].motor_channel = motor_4_channel;

    motor_profiles[MOTOR5].motor_htim = motor_5_htim;
    motor_profiles[MOTOR5].motor_channel = motor_5_channel;

    motor_profiles[MOTOR6].motor_htim = motor_6_htim;
    motor_profiles[MOTOR6].motor_channel = motor_6_channel;

    motor_profiles[MOTOR7].motor_htim = motor_7_htim;
    motor_profiles[MOTOR7].motor_channel = motor_7_channel;
}

void set_motor_pwm_output(MotorNumber motor_number, uint32_t compare_value)
{
    __HAL_TIM_SetCompare(motor_profiles[motor_number].motor_htim, motor_profiles[motor_number].motor_channel, compare_value);
}