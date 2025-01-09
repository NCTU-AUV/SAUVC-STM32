#ifndef THRUSTER_PWM_CONTROLLER_NODE
#define THRUSTER_PWM_CONTROLLER_NODE

#include <rclc/rclc.h>
#include <rclc/executor.h>

extern const unsigned int THRUSTER_PWM_CONTROLLER_NUM_HANDLES;

void initialize_thruster_pwm_controller_node(rclc_support_t *support, rclc_executor_t *executor);

#endif