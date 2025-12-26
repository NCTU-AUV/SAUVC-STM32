#include "thruster_pwm_controller_node/subscribe_set_pwm_output_signal_value.h"


#include "thruster_pwm_control_driver.h"

#include <stddef.h>

#include <std_msgs/msg/int32_multi_array.h>
#include <rcl/error_handling.h>


static rcl_subscription_t set_pwm_output_signal_value_subscriber;
static std_msgs__msg__Int32MultiArray set_pwm_output_signal_value_msg;
static int32_t set_pwm_output_signal_value_buffer[8];


static void the_set_pwm_output_signal_value_subscriber_callback(const void * msgin)
{
    const std_msgs__msg__Int32MultiArray * set_pwm_output_signal_value_msg = (const std_msgs__msg__Int32MultiArray *)msgin;
    if (set_pwm_output_signal_value_msg == NULL) {
        printf("set_pwm_output_signal_value callback received null message payload\n");
        return;
    }
    
    size_t values_received = set_pwm_output_signal_value_msg->data.size;
    for (size_t i = 0; i < 8; i++)
    {
        int32_t output_signal_value_us = 0;
        if (i < values_received) {
            output_signal_value_us = set_pwm_output_signal_value_msg->data.data[i];
            if(output_signal_value_us < 0)
            {
                output_signal_value_us = -output_signal_value_us;
            }
        }

        set_thruster_pwm_output((ThrusterNumber)i, (uint32_t)(output_signal_value_us));
    }
}


bool initialize_set_pwm_output_signal_value_subscriber(rcl_node_t *thruster_pwm_controller_node, rclc_executor_t *executor)
{
    set_pwm_output_signal_value_subscriber = rcl_get_zero_initialized_subscription();
    rcl_ret_t rc = rclc_subscription_init_default(
        &set_pwm_output_signal_value_subscriber,
        thruster_pwm_controller_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32MultiArray),
        "thrusters/set_pwm_output_signal_value_us"
    );
    if (rc != RCL_RET_OK) {
        printf("failed to create set_pwm_output_signal_value subscriber (rc=%d): %s\n",
               (int)rc,
               rcl_get_error_string().str);
        rcl_reset_error();
        return false;
    }

    std_msgs__msg__Int32MultiArray__init(&set_pwm_output_signal_value_msg);
    set_pwm_output_signal_value_msg.data.data = set_pwm_output_signal_value_buffer;
    set_pwm_output_signal_value_msg.data.capacity = 8;
    set_pwm_output_signal_value_msg.data.size = 0;
    rc = rclc_executor_add_subscription(
        executor,
        &set_pwm_output_signal_value_subscriber,
        &set_pwm_output_signal_value_msg,
        &the_set_pwm_output_signal_value_subscriber_callback,
        ON_NEW_DATA
    );
    if (rc != RCL_RET_OK) {
        printf("failed to register set_pwm_output_signal_value callback (rc=%d): %s\n",
               (int)rc,
               rcl_get_error_string().str);
        rcl_reset_error();
        return false;
    }

    return true;
}
