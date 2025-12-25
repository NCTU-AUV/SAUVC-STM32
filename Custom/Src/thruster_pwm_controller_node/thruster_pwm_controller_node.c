#include "thruster_pwm_controller_node/thruster_pwm_controller_node.h"

#include "thruster_pwm_controller_node/publish_is_pwm_output_on.h"
#include "thruster_pwm_controller_node/subscribe_set_pwm_output_on.h"
#include "thruster_pwm_controller_node/publish_pwm_output_signal_value.h"
#include "thruster_pwm_controller_node/subscribe_set_pwm_output_signal_value.h"

#include <std_msgs/msg/bool.h>


const unsigned int THRUSTER_PWM_CONTROLLER_NUM_HANDLES = 1 + 8 + 8;


static rcl_timer_t thruster_pwm_controller_timer;


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


static void initialize_thruster_pwm_controller_timer(rclc_support_t *support, rclc_executor_t *executor)
{
    thruster_pwm_controller_timer = rcl_get_zero_initialized_timer();
    const unsigned int thruster_pwm_controller_timer_timeout = 100;
    rcl_ret_t rc = rclc_timer_init_default(
        &thruster_pwm_controller_timer,
        support,
        RCL_MS_TO_NS(thruster_pwm_controller_timer_timeout),
        thruster_pwm_controller_timer_callback
    );
    if (rc != RCL_RET_OK) {
        printf("Error in rcl_timer_init_default.\n");
    } else {
        printf("Created timer with timeout %d ms.\n", thruster_pwm_controller_timer_timeout);
    }

    rc = rclc_executor_add_timer(executor, &thruster_pwm_controller_timer);
    if (rc != RCL_RET_OK) {
        printf("Error in rclc_executor_add_timer.\n");
        return;
    }
}


void initialize_thruster_pwm_controller_node(rclc_support_t *support, rclc_executor_t *executor, rcl_node_t *stm32_node)
{
    if (!initialize_is_pwm_output_on_publisher_for_all_thrusters(stm32_node)) {
        printf("thruster_pwm_controller_node: publisher init failed\n");
        return;
    }
    if (!initialize_pwm_output_signal_value_publisher_for_all_thrusters(stm32_node)) {
        printf("thruster_pwm_controller_node: pwm value publisher init failed\n");
        return;
    }
    initialize_thruster_pwm_controller_timer(support, executor);

    if (!initialize_set_pwm_output_on_subscriber_for_all_thrusters(stm32_node, executor)) {
        printf("thruster_pwm_controller_node: set_pwm_output_on subscriber init failed\n");
        return;
    }
    if (!initialize_set_pwm_output_signal_value_subscriber_for_all_thrusters(stm32_node, executor)) {
        printf("thruster_pwm_controller_node: pwm value subscriber init failed\n");
        return;
    }

    initialize_previous_is_pwm_output_on_state_for_all_thrusters();
    initialize_previous_pwm_output_signal_value_for_all_thrusters();
}
