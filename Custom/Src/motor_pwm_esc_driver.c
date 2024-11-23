#include "motor_pwm_esc_driver.h"

struct {
    TIM_HandleTypeDef *motor_timer_htim;
    uint32_t motor_channel;
} motor_profiles[8];

void initialize_all_motors(
    TIM_HandleTypeDef *motor_0_timer_htim,
    uint32_t motor_0_channel,

    TIM_HandleTypeDef *motor_1_timer_htim,
    uint32_t motor_1_channel,

    TIM_HandleTypeDef *motor_2_timer_htim,
    uint32_t motor_2_channel,

    TIM_HandleTypeDef *motor_3_timer_htim,
    uint32_t motor_3_channel,

    TIM_HandleTypeDef *motor_4_timer_htim,
    uint32_t motor_4_channel,

    TIM_HandleTypeDef *motor_5_timer_htim,
    uint32_t motor_5_channel,

    TIM_HandleTypeDef *motor_6_timer_htim,
    uint32_t motor_6_channel,

    TIM_HandleTypeDef *motor_7_timer_htim,
    uint32_t motor_7_channel
)
{
    motor_profiles[MOTOR0].motor_timer_htim = motor_0_timer_htim;
    motor_profiles[MOTOR0].motor_channel = motor_0_channel;

    motor_profiles[MOTOR1].motor_timer_htim = motor_1_timer_htim;
    motor_profiles[MOTOR1].motor_channel = motor_1_channel;

    motor_profiles[MOTOR2].motor_timer_htim = motor_2_timer_htim;
    motor_profiles[MOTOR2].motor_channel = motor_2_channel;

    motor_profiles[MOTOR3].motor_timer_htim = motor_3_timer_htim;
    motor_profiles[MOTOR3].motor_channel = motor_3_channel;

    motor_profiles[MOTOR4].motor_timer_htim = motor_4_timer_htim;
    motor_profiles[MOTOR4].motor_channel = motor_4_channel;

    motor_profiles[MOTOR5].motor_timer_htim = motor_5_timer_htim;
    motor_profiles[MOTOR5].motor_channel = motor_5_channel;

    motor_profiles[MOTOR6].motor_timer_htim = motor_6_timer_htim;
    motor_profiles[MOTOR6].motor_channel = motor_6_channel;

    motor_profiles[MOTOR7].motor_timer_htim = motor_7_timer_htim;
    motor_profiles[MOTOR7].motor_channel = motor_7_channel;
}

void set_motor_pwm_output(MotorNumber motor_number, uint32_t compare_value)
{
    __HAL_TIM_SetCompare(motor_profiles[motor_number].motor_timer_htim, motor_profiles[motor_number].motor_channel, compare_value);
}