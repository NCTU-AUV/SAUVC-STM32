#include "thruster_pwm_controller_node/subscribe_set_pwm_output_on.h"


#include "thruster_pwm_control_driver.h"


#include <std_msgs/msg/bool.h>
#include <rcl/error_handling.h>


static rcl_subscription_t set_pwm_output_on_subscriber;
static std_msgs__msg__Bool set_pwm_output_on_msg;


static void the_set_pwm_output_on_subscriber_callback(const void * msgin)
{
    const std_msgs__msg__Bool * set_pwm_output_on_msg = (const std_msgs__msg__Bool *)msgin;
    if (set_pwm_output_on_msg == NULL) {
        printf("set_pwm_output_on callback received null message payload\n");
        return;
    }
    
    if(set_pwm_output_on_msg->data == true)
    {
        start_all_thrusters_pwm_output();
    } else {
        stop_all_thrusters_pwm_output();
    }
}


bool initialize_set_pwm_output_on_subscriber(rcl_node_t *thruster_pwm_controller_node, rclc_executor_t *executor)
{
    set_pwm_output_on_subscriber = rcl_get_zero_initialized_subscription();
    rcl_ret_t rc = rclc_subscription_init_default(
        &set_pwm_output_on_subscriber,
        thruster_pwm_controller_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "thrusters/set_pwm_output_on"
    );
    if (rc != RCL_RET_OK) {
        printf("failed to create set_pwm_output_on subscriber (rc=%d): %s\n",
               (int)rc,
               rcl_get_error_string().str);
        rcl_reset_error();
        return false;
    }

    std_msgs__msg__Bool__init(&set_pwm_output_on_msg);
    rc = rclc_executor_add_subscription(
        executor,
        &set_pwm_output_on_subscriber,
        &set_pwm_output_on_msg,
        &the_set_pwm_output_on_subscriber_callback,
        ON_NEW_DATA
    );
    if (rc != RCL_RET_OK) {
        printf("failed to register set_pwm_output_on callback (rc=%d): %s\n",
               (int)rc,
               rcl_get_error_string().str);
        rcl_reset_error();
        return false;
    }

    return true;
}
