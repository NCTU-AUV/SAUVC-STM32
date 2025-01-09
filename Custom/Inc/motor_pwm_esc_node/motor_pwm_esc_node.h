#ifndef MOTOR_PWM_ESC_NODE
#define MOTOR_PWM_ESC_NODE

#include <rclc/rclc.h>
#include <rclc/executor.h>

extern const unsigned int MOTOR_PWM_ESC_NUM_HANDLES;

void initialize_motor_pwm_esc_node(rclc_support_t *support, rclc_executor_t *executor);

#endif