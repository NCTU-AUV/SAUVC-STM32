#ifndef PUBLISH_PWM_OUTPUT_SIGNAL_VALUE
#define PUBLISH_PWM_OUTPUT_SIGNAL_VALUE

#include "motor_pwm_esc_driver.h"

#include <rclc/rclc.h>

void initialize_pwm_output_signal_value_publisher_for_all_motors(rcl_node_t *motor_pwm_esc_node);

void initialize_previous_pwm_output_signal_value_for_all_motors();

void publish_pwm_output_signal_value_only_when_updated(MotorNumber motor_number);

#endif