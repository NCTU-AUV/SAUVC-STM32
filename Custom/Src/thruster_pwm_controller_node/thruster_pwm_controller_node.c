#include "thruster_pwm_controller_node/thruster_pwm_controller_node.h"

#include "thruster_pwm_controller_node/publish_is_pwm_output_on.h"
#include "thruster_pwm_controller_node/subscribe_set_pwm_output_on.h"
#include "thruster_pwm_controller_node/publish_pwm_output_signal_value.h"
#include "thruster_pwm_controller_node/subscribe_set_pwm_output_signal_value.h"

#include <std_msgs/msg/bool.h>


const unsigned int THRUSTER_PWM_CONTROLLER_NUM_HANDLES = 8 + 8;



static void thruster_pwm_controller_timer_callback(rcl_timer_t *, int64_t)
{
    publish_is_pwm_output_on_only_when_updated(THRUSTER0);
    publish_is_pwm_output_on_only_when_updated(THRUSTER1);
    publish_is_pwm_output_on_only_when_updated(THRUSTER2);
    publish_is_pwm_output_on_only_when_updated(THRUSTER3);
    publish_is_pwm_output_on_only_when_updated(THRUSTER4);
    publish_is_pwm_output_on_only_when_updated(THRUSTER5);
    publish_is_pwm_output_on_only_when_updated(THRUSTER6);
    publish_is_pwm_output_on_only_when_updated(THRUSTER7);

    publish_pwm_output_signal_value_only_when_updated(THRUSTER0);
    publish_pwm_output_signal_value_only_when_updated(THRUSTER1);
    publish_pwm_output_signal_value_only_when_updated(THRUSTER2);
    publish_pwm_output_signal_value_only_when_updated(THRUSTER3);
    publish_pwm_output_signal_value_only_when_updated(THRUSTER4);
    publish_pwm_output_signal_value_only_when_updated(THRUSTER5);
    publish_pwm_output_signal_value_only_when_updated(THRUSTER6);
    publish_pwm_output_signal_value_only_when_updated(THRUSTER7);
}


void thruster_pwm_controller_on_timer_tick(void)
{
    thruster_pwm_controller_timer_callback(NULL, 0);
}


void initialize_thruster_pwm_controller_node(rclc_support_t *support, rclc_executor_t *executor, rcl_node_t *stm32_node)
{
    if (!initialize_is_pwm_output_on_publisher_for_all_thrusters(stm32_node)) {
        printf("thruster_pwm_controller_node: aborting init; failed to set up is_pwm_output_on publishers\n");
        return;
    }
    if (!initialize_pwm_output_signal_value_publisher_for_all_thrusters(stm32_node)) {
        printf("thruster_pwm_controller_node: aborting init; failed to set up pwm_output_signal_value publishers\n");
        return;
    }

    if (!initialize_set_pwm_output_on_subscriber_for_all_thrusters(stm32_node, executor)) {
        printf("thruster_pwm_controller_node: aborting init; failed to set up set_pwm_output_on subscribers\n");
        return;
    }
    if (!initialize_set_pwm_output_signal_value_subscriber_for_all_thrusters(stm32_node, executor)) {
        printf("thruster_pwm_controller_node: aborting init; failed to set up set_pwm_output_signal_value subscribers\n");
        return;
    }

    initialize_previous_is_pwm_output_on_state_for_all_thrusters();
    initialize_previous_pwm_output_signal_value_for_all_thrusters();
}
