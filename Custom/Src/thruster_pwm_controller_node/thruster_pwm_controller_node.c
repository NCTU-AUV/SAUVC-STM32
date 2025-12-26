#include "thruster_pwm_controller_node/thruster_pwm_controller_node.h"

#include "thruster_pwm_controller_node/subscribe_set_pwm_output_on.h"
#include "thruster_pwm_controller_node/subscribe_set_pwm_output_signal_value.h"

const unsigned int THRUSTER_PWM_CONTROLLER_NUM_HANDLES = 8 + 8;



static void thruster_pwm_controller_timer_callback(rcl_timer_t *, int64_t)
{
}


void thruster_pwm_controller_on_timer_tick(void)
{
    thruster_pwm_controller_timer_callback(NULL, 0);
}


void initialize_thruster_pwm_controller_node(rclc_support_t *support, rclc_executor_t *executor, rcl_node_t *stm32_node)
{
    if (!initialize_set_pwm_output_on_subscriber_for_all_thrusters(stm32_node, executor)) {
        printf("thruster_pwm_controller_node: aborting init; failed to set up set_pwm_output_on subscribers\n");
        return;
    }
    if (!initialize_set_pwm_output_signal_value_subscriber_for_all_thrusters(stm32_node, executor)) {
        printf("thruster_pwm_controller_node: aborting init; failed to set up set_pwm_output_signal_value subscribers\n");
        return;
    }
}
