#ifndef PUBLISH_IS_PWM_OUTPUT_ON
#define PUBLISH_IS_PWM_OUTPUT_ON

#include "thruster_pwm_control_driver.h"

#include <rclc/rclc.h>

bool initialize_is_pwm_output_on_publisher_for_all_thrusters(rcl_node_t *thruster_pwm_controller_node);

void initialize_previous_is_pwm_output_on_state_for_all_thrusters();

void publish_is_pwm_output_on_only_when_updated(ThrusterNumber thruster_number);

#endif
