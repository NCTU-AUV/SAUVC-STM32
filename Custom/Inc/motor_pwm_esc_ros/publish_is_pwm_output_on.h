#ifndef PUBLISH_IS_PWM_OUTPUT_ON
#define PUBLISH_IS_PWM_OUTPUT_ON

#include "motor_pwm_esc_driver.h"

#include <rclc/rclc.h>

void initialize_is_pwm_output_on_publisher_for_all_motors(rcl_node_t *stm32_node);

void initialize_previous_is_pwm_output_on_state_for_all_motors();

void publish_is_pwm_output_on_only_when_updated(MotorNumber motor_number);

#endif