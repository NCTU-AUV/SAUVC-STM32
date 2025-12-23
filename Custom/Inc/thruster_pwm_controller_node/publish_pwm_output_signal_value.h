#ifndef PUBLISH_PWM_OUTPUT_SIGNAL_VALUE
#define PUBLISH_PWM_OUTPUT_SIGNAL_VALUE

#include "thruster_pwm_control_driver.h"

#include <rclc/rclc.h>

bool initialize_pwm_output_signal_value_publisher_for_all_thrusters(rcl_node_t *thruster_pwm_controller_node);

void initialize_previous_pwm_output_signal_value_for_all_thrusters();

void publish_pwm_output_signal_value_only_when_updated(ThrusterNumber thruster_number);

#endif
