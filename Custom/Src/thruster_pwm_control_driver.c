#include "thruster_pwm_control_driver.h"


#include "kill_switch_driver.h"


static struct {
    TIM_HandleTypeDef *thruster_htim;
    uint32_t thruster_channel;
    bool is_pwm_output_on;
    uint32_t pwm_output_signal_value_us;
} thruster_profiles[8];

void initialize_thruster(ThrusterNumber thruster_number, TIM_HandleTypeDef *thruster_htim, uint32_t thruster_channel)
{
    thruster_profiles[thruster_number].thruster_htim = thruster_htim;
    thruster_profiles[thruster_number].thruster_channel = thruster_channel;
    
    stop_thruster_pwm_output(thruster_number);
    set_thruster_pwm_output(thruster_number, 0);
}

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
)
{
    initialize_thruster(THRUSTER0, thruster_0_htim, thruster_0_channel);
    initialize_thruster(THRUSTER1, thruster_1_htim, thruster_1_channel);
    initialize_thruster(THRUSTER2, thruster_2_htim, thruster_2_channel);
    initialize_thruster(THRUSTER3, thruster_3_htim, thruster_3_channel);
    initialize_thruster(THRUSTER4, thruster_4_htim, thruster_4_channel);
    initialize_thruster(THRUSTER5, thruster_5_htim, thruster_5_channel);
    initialize_thruster(THRUSTER6, thruster_6_htim, thruster_6_channel);
    initialize_thruster(THRUSTER7, thruster_7_htim, thruster_7_channel);
}

void set_thruster_pwm_output(ThrusterNumber thruster_number, uint32_t output_signal_value_us)
{
    thruster_profiles[thruster_number].pwm_output_signal_value_us = output_signal_value_us;
    __HAL_TIM_SetCompare(thruster_profiles[thruster_number].thruster_htim, thruster_profiles[thruster_number].thruster_channel, output_signal_value_us);
}

void start_thruster_pwm_output(ThrusterNumber thruster_number)
{
    if(is_kill_switch_closed())
    {
        return;
    }

    thruster_profiles[thruster_number].is_pwm_output_on = true;
    HAL_TIM_PWM_Start(thruster_profiles[thruster_number].thruster_htim, thruster_profiles[thruster_number].thruster_channel);
}

void stop_thruster_pwm_output(ThrusterNumber thruster_number)
{
    thruster_profiles[thruster_number].is_pwm_output_on = false;
    HAL_TIM_PWM_Stop(thruster_profiles[thruster_number].thruster_htim, thruster_profiles[thruster_number].thruster_channel);
}


bool is_pwm_output_on(ThrusterNumber thruster_number)
{
    return thruster_profiles[thruster_number].is_pwm_output_on;
}


uint32_t get_pwm_output_signal_value_us(ThrusterNumber thruster_number)
{
    return thruster_profiles[thruster_number].pwm_output_signal_value_us;
}


void start_all_thrusters_pwm_output()
{
    start_thruster_pwm_output(THRUSTER0);
    start_thruster_pwm_output(THRUSTER1);
    start_thruster_pwm_output(THRUSTER2);
    start_thruster_pwm_output(THRUSTER3);
    start_thruster_pwm_output(THRUSTER4);
    start_thruster_pwm_output(THRUSTER5);
    start_thruster_pwm_output(THRUSTER6);
    start_thruster_pwm_output(THRUSTER7);
}

void stop_all_thrusters_pwm_output()
{
    stop_thruster_pwm_output(THRUSTER0);
    stop_thruster_pwm_output(THRUSTER1);
    stop_thruster_pwm_output(THRUSTER2);
    stop_thruster_pwm_output(THRUSTER3);
    stop_thruster_pwm_output(THRUSTER4);
    stop_thruster_pwm_output(THRUSTER5);
    stop_thruster_pwm_output(THRUSTER6);
    stop_thruster_pwm_output(THRUSTER7);
}