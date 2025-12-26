#include "thruster_pwm_controller_node/subscribe_set_pwm_output_on.h"


#include "thruster_pwm_control_driver.h"


#include <std_msgs/msg/bool.h>
#include <rcl/error_handling.h>


static struct {
    rcl_subscription_t set_pwm_output_on_subscriber;
    std_msgs__msg__Bool set_pwm_output_on_msg;
    ThrusterNumber thruster_number;
} thrusters_data[8];


static void the_set_pwm_output_on_subscriber_callback_with_context(const void * msgin, void * context_void_ptr)
{
    const std_msgs__msg__Bool * set_pwm_output_on_msg = (const std_msgs__msg__Bool *)msgin;
    if (set_pwm_output_on_msg == NULL) {
        printf("set_pwm_output_on callback received null message payload\n");
        return;
    }
    
    if (context_void_ptr == NULL) {
        printf("set_pwm_output_on callback missing thruster context\n");
        return;
    }
    
    const ThrusterNumber * thruster_number = (const ThrusterNumber *) context_void_ptr;
    
    if(set_pwm_output_on_msg->data == true)
    {
        start_thruster_pwm_output(*thruster_number);
    } else {
        stop_thruster_pwm_output(*thruster_number);
    }
}


static bool initialize_set_pwm_output_on_subscriber(ThrusterNumber thruster_number, const char *topic_name, rcl_node_t *thruster_pwm_controller_node, rclc_executor_t *executor)
{
    thrusters_data[thruster_number].set_pwm_output_on_subscriber = rcl_get_zero_initialized_subscription();
    rcl_ret_t rc = rclc_subscription_init_default(
        &(thrusters_data[thruster_number].set_pwm_output_on_subscriber),
        thruster_pwm_controller_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        topic_name
    );
    if (rc != RCL_RET_OK) {
        printf("thruster %d: failed to create set_pwm_output_on subscriber for %s (rc=%d): %s\n",
               (int)thruster_number,
               topic_name,
               (int)rc,
               rcl_get_error_string().str);
        rcl_reset_error();
        return false;
    }

    std_msgs__msg__Bool__init(&(thrusters_data[thruster_number].set_pwm_output_on_msg));
    thrusters_data[thruster_number].thruster_number = thruster_number;
    rc = rclc_executor_add_subscription_with_context(
        executor,
        &(thrusters_data[thruster_number].set_pwm_output_on_subscriber),
        &(thrusters_data[thruster_number].set_pwm_output_on_msg),
        &the_set_pwm_output_on_subscriber_callback_with_context,
        (void *) &(thrusters_data[thruster_number].thruster_number),
        ON_NEW_DATA
    );
    if (rc != RCL_RET_OK) {
        printf("thruster %d: failed to register set_pwm_output_on callback for %s (rc=%d): %s\n",
               (int)thruster_number,
               topic_name,
               (int)rc,
               rcl_get_error_string().str);
        rcl_reset_error();
        return false;
    }

    return true;
}


bool initialize_set_pwm_output_on_subscriber_for_all_thrusters(rcl_node_t *thruster_pwm_controller_node, rclc_executor_t *executor)
{
    return
        initialize_set_pwm_output_on_subscriber(THRUSTER0, "thruster_0/set_pwm_output_on", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_on_subscriber(THRUSTER1, "thruster_1/set_pwm_output_on", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_on_subscriber(THRUSTER2, "thruster_2/set_pwm_output_on", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_on_subscriber(THRUSTER3, "thruster_3/set_pwm_output_on", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_on_subscriber(THRUSTER4, "thruster_4/set_pwm_output_on", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_on_subscriber(THRUSTER5, "thruster_5/set_pwm_output_on", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_on_subscriber(THRUSTER6, "thruster_6/set_pwm_output_on", thruster_pwm_controller_node, executor) &&
        initialize_set_pwm_output_on_subscriber(THRUSTER7, "thruster_7/set_pwm_output_on", thruster_pwm_controller_node, executor);
}
