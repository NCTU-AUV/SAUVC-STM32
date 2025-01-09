#ifndef SUBSCRIBE_SET_PWM_OUTPUT_SIGNAL_VALUE
#define SUBSCRIBE_SET_PWM_OUTPUT_SIGNAL_VALUE

#include <rclc/rclc.h>
#include <rclc/executor.h>

void initialize_set_pwm_output_signal_value_subscriber_for_all_thrusters(rcl_node_t *thruster_pwm_controller_node, rclc_executor_t *executor);

#endif