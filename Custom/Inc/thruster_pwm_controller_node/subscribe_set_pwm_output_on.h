#ifndef SUBSCRIBE_SET_PWM_OUTPUT_ON
#define SUBSCRIBE_SET_PWM_OUTPUT_ON

#include <rclc/rclc.h>
#include <rclc/executor.h>

bool initialize_set_pwm_output_on_subscriber(rcl_node_t *thruster_pwm_controller_node, rclc_executor_t *executor);

#endif
