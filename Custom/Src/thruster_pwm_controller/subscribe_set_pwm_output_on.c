#include "thruster_pwm_controller/subscribe_set_pwm_output_on.h"


#include "thruster_pwm_driver.h"
#include "debug_logger.h"


#include <std_msgs/msg/bool.h>
#include <rcl/error_handling.h>
#include <stdio.h>


static rcl_subscription_t set_pwm_output_on_subscriber;
static std_msgs__msg__Bool set_pwm_output_on_msg;
static rcl_publisher_t pwm_output_enabled_publisher;


static bool are_all_thrusters_pwm_output_on(void)
{
    for (size_t i = 0; i < 8; i++)
    {
        if (!is_pwm_output_on((ThrusterNumber)i)) {
            return false;
        }
    }

    return true;
}


static void publish_pwm_output_enabled_state(bool enabled)
{
    std_msgs__msg__Bool msg;
    msg.data = enabled;

    rcl_ret_t ret = rcl_publish(&pwm_output_enabled_publisher, &msg, NULL);
    if (ret != RCL_RET_OK)
    {
        char log_msg[128];
        int n = snprintf(log_msg, sizeof(log_msg),
                         "failed to publish thrusters/enabled status (rc=%d): %s\n",
                         (int)ret,
                         rcl_get_error_string().str);
        if (n > 0) {
            (void)debug_logger_publish(log_msg);
        }
        rcl_reset_error();
    }
}


static void the_set_pwm_output_on_subscriber_callback(const void * msgin)
{
    const std_msgs__msg__Bool * set_pwm_output_on_msg = (const std_msgs__msg__Bool *)msgin;
    if (set_pwm_output_on_msg == NULL) {
        (void)debug_logger_publish("set_pwm_output_on callback received null message payload\n");
        return;
    }
    
    if(set_pwm_output_on_msg->data == true)
    {
        start_all_thrusters_pwm_output();
    } else {
        stop_all_thrusters_pwm_output();
    }

    publish_pwm_output_enabled_state(are_all_thrusters_pwm_output_on());
}


bool initialize_set_pwm_output_on_subscriber(rcl_node_t *thruster_pwm_controller, rclc_executor_t *executor)
{
    set_pwm_output_on_subscriber = rcl_get_zero_initialized_subscription();
    rcl_ret_t rc = rclc_subscription_init_default(
        &set_pwm_output_on_subscriber,
        thruster_pwm_controller,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "thrusters/set_enabled"
    );
    if (rc != RCL_RET_OK) {
        char msg[128];
        int n = snprintf(msg, sizeof(msg),
                         "failed to create thrusters/set_enabled subscriber (rc=%d): %s\n",
                         (int)rc,
                         rcl_get_error_string().str);
        if (n > 0) {
            (void)debug_logger_publish(msg);
        }
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
        char msg[128];
        int n = snprintf(msg, sizeof(msg),
                         "failed to register thrusters/set_enabled callback (rc=%d): %s\n",
                         (int)rc,
                         rcl_get_error_string().str);
        if (n > 0) {
            (void)debug_logger_publish(msg);
        }
        rcl_reset_error();
        return false;
    }

    pwm_output_enabled_publisher = rcl_get_zero_initialized_publisher();
    rc = rclc_publisher_init_default(
        &pwm_output_enabled_publisher,
        thruster_pwm_controller,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "thrusters/enabled"
    );
    if (rc != RCL_RET_OK) {
        char msg[128];
        int n = snprintf(msg, sizeof(msg),
                         "failed to create thrusters/enabled status publisher (rc=%d): %s\n",
                         (int)rc,
                         rcl_get_error_string().str);
        if (n > 0) {
            (void)debug_logger_publish(msg);
        }
        rcl_reset_error();
        return false;
    }

    publish_pwm_output_enabled_state(are_all_thrusters_pwm_output_on());

    return true;
}
