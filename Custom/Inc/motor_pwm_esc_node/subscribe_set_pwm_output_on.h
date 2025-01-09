#ifndef SUBSCRIBE_SET_PWM_OUTPUT_ON
#define SUBSCRIBE_SET_PWM_OUTPUT_ON

#include <rclc/rclc.h>
#include <rclc/executor.h>

void initialize_set_pwm_output_on_subscriber_for_all_motors(rcl_node_t *motor_pwm_esc_node, rclc_executor_t *executor);

#endif