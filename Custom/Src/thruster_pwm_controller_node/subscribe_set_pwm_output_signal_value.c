#include "thruster_pwm_controller_node/subscribe_set_pwm_output_signal_value.h"


#include "thruster_pwm_control_driver.h"


#include <std_msgs/msg/int32.h>


static struct {
    rcl_subscription_t set_pwm_output_signal_value_subscriber;
    std_msgs__msg__Int32 set_pwm_output_signal_value_msg;
    ThrusterNumber thruster_number;
} thrusters_data[8];


static void the_set_pwm_output_signal_value_subscriber_callback_with_context(const void * msgin, void * context_void_ptr)
{
    const std_msgs__msg__Int32 * set_pwm_output_signal_value_msg = (const std_msgs__msg__Int32 *)msgin;
    if (set_pwm_output_signal_value_msg == NULL) {
        printf("Callback: msg NULL\n");
        return;
    }
    
    if (context_void_ptr == NULL) {
        printf("Callback: context is empty\n");
        return;
    }
    
    const ThrusterNumber * thruster_number = (const ThrusterNumber *) context_void_ptr;
    
    int32_t output_signal_value_us = set_pwm_output_signal_value_msg->data;
    if(output_signal_value_us < 0)
    {
        output_signal_value_us = -output_signal_value_us;
    }

    set_thruster_pwm_output(*thruster_number, (uint32_t)(output_signal_value_us));
}


static bool initialize_set_pwm_output_signal_value_subscriber(ThrusterNumber thruster_number, const char *topic_name, rcl_node_t *thruster_pwm_controller_node, rclc_executor_t *executor)
{
    thrusters_data[thruster_number].set_pwm_output_signal_value_subscriber = rcl_get_zero_initialized_subscription();
    rcl_ret_t rc = rclc_subscription_init_default(
        &(thrusters_data[thruster_number].set_pwm_output_signal_value_subscriber),
        thruster_pwm_controller_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        topic_name
    );
    if (rc != RCL_RET_OK) {
        printf("Failed to create subscriber %s.\n", topic_name);
        return false;
    }

    std_msgs__msg__Int32__init(&(thrusters_data[thruster_number].set_pwm_output_signal_value_msg));
    thrusters_data[thruster_number].thruster_number = thruster_number;
    rc = rclc_executor_add_subscription_with_context(
        executor,
        &(thrusters_data[thruster_number].set_pwm_output_signal_value_subscriber),
        &(thrusters_data[thruster_number].set_pwm_output_signal_value_msg),
        &the_set_pwm_output_signal_value_subscriber_callback_with_context,
        (void *) &(thrusters_data[thruster_number].thruster_number),
        ON_NEW_DATA
    );
    if (rc != RCL_RET_OK) {
        printf("Error in rclc_executor_add_subscription. \n");
        return false;
    }

    return true;
}


bool initialize_set_pwm_output_signal_value_subscriber_for_all_thrusters(rcl_node_t *thruster_pwm_controller_node, rclc_executor_t *executor)
{
    return
        initialize_set_pwm_output_signal_value_subscriber(THRUSTER0, "thruster_0/set_pwm_output_signal_value_us", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_signal_value_subscriber(THRUSTER1, "thruster_1/set_pwm_output_signal_value_us", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_signal_value_subscriber(THRUSTER2, "thruster_2/set_pwm_output_signal_value_us", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_signal_value_subscriber(THRUSTER3, "thruster_3/set_pwm_output_signal_value_us", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_signal_value_subscriber(THRUSTER4, "thruster_4/set_pwm_output_signal_value_us", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_signal_value_subscriber(THRUSTER5, "thruster_5/set_pwm_output_signal_value_us", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_signal_value_subscriber(THRUSTER6, "thruster_6/set_pwm_output_signal_value_us", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_signal_value_subscriber(THRUSTER7, "thruster_7/set_pwm_output_signal_value_us", thruster_pwm_controller_node, executor);
}
