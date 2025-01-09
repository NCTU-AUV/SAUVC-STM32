#include "motor_pwm_esc_driver.h"


#include "kill_switch_driver.h"


struct {
    TIM_HandleTypeDef *motor_htim;
    uint32_t motor_channel;
    bool is_pwm_output_on;
    uint32_t pwm_output_compare_value;
} motor_profiles[8];

void initialize_motor(MotorNumber motor_number, TIM_HandleTypeDef *motor_htim, uint32_t motor_channel)
{
    motor_profiles[motor_number].motor_htim = motor_htim;
    motor_profiles[motor_number].motor_channel = motor_channel;
    motor_profiles[motor_number].is_pwm_output_on = false;
    motor_profiles[motor_number].pwm_output_compare_value = 0;
}

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
    initialize_motor(MOTOR0, motor_0_htim, motor_0_channel);
    initialize_motor(MOTOR1, motor_1_htim, motor_1_channel);
    initialize_motor(MOTOR2, motor_2_htim, motor_2_channel);
    initialize_motor(MOTOR3, motor_3_htim, motor_3_channel);
    initialize_motor(MOTOR4, motor_4_htim, motor_4_channel);
    initialize_motor(MOTOR5, motor_5_htim, motor_5_channel);
    initialize_motor(MOTOR6, motor_6_htim, motor_6_channel);
    initialize_motor(MOTOR7, motor_7_htim, motor_7_channel);
}

void set_motor_pwm_output(MotorNumber motor_number, uint32_t compare_value)
{
    motor_profiles[motor_number].pwm_output_compare_value = compare_value;
    __HAL_TIM_SetCompare(motor_profiles[motor_number].motor_htim, motor_profiles[motor_number].motor_channel, compare_value);
}

void start_motor_pwm_output(MotorNumber motor_number)
{
    if(is_kill_switch_closed())
    {
        return;
    }

    motor_profiles[motor_number].is_pwm_output_on = true;
    HAL_TIM_PWM_Start(motor_profiles[motor_number].motor_htim, motor_profiles[motor_number].motor_channel);
}

void stop_motor_pwm_output(MotorNumber motor_number)
{
    motor_profiles[motor_number].is_pwm_output_on = false;
    HAL_TIM_PWM_Stop(motor_profiles[motor_number].motor_htim, motor_profiles[motor_number].motor_channel);
}


bool is_pwm_output_on(MotorNumber motor_number)
{
    return motor_profiles[motor_number].is_pwm_output_on;
}


uint32_t get_pwm_output_compare_value(MotorNumber motor_number)
{
    return motor_profiles[motor_number].pwm_output_compare_value;
}


void start_all_motors_pwm_output()
{
    start_motor_pwm_output(MOTOR0);
    start_motor_pwm_output(MOTOR1);
    start_motor_pwm_output(MOTOR2);
    start_motor_pwm_output(MOTOR3);
    start_motor_pwm_output(MOTOR4);
    start_motor_pwm_output(MOTOR5);
    start_motor_pwm_output(MOTOR6);
    start_motor_pwm_output(MOTOR7);
}

void stop_all_motors_pwm_output()
{
    stop_motor_pwm_output(MOTOR0);
    stop_motor_pwm_output(MOTOR1);
    stop_motor_pwm_output(MOTOR2);
    stop_motor_pwm_output(MOTOR3);
    stop_motor_pwm_output(MOTOR4);
    stop_motor_pwm_output(MOTOR5);
    stop_motor_pwm_output(MOTOR6);
    stop_motor_pwm_output(MOTOR7);
}