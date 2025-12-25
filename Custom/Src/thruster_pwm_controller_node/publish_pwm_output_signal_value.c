#include "thruster_pwm_controller_node/publish_pwm_output_signal_value.h"


#include <std_msgs/msg/int32.h>


static struct {
    rcl_publisher_t pwm_output_signal_value_publisher;
    int32_t previous_pwm_output_signal_value_us;
} thrusters_data[8];


static bool initialize_pwm_output_signal_value_publisher(ThrusterNumber thruster_number, const char *topic_name, rcl_node_t *thruster_pwm_controller_node)
{
    rcl_ret_t rc = rclc_publisher_init_default(
        &(thrusters_data[thruster_number].pwm_output_signal_value_publisher),
        thruster_pwm_controller_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        topic_name
    );
    if (rc != RCL_RET_OK) {
        printf("pwm_output_signal_value publisher init failed for %s (rc=%d)\n", topic_name, (int)rc);
        return false;
    }
    return true;
}


bool initialize_pwm_output_signal_value_publisher_for_all_thrusters(rcl_node_t *thruster_pwm_controller_node)
{
    return
        initialize_pwm_output_signal_value_publisher(THRUSTER0, "thruster_0/pwm_output_signal_value_us", thruster_pwm_controller_node) &&
        initialize_pwm_output_signal_value_publisher(THRUSTER1, "thruster_1/pwm_output_signal_value_us", thruster_pwm_controller_node) &&
        initialize_pwm_output_signal_value_publisher(THRUSTER2, "thruster_2/pwm_output_signal_value_us", thruster_pwm_controller_node) &&
        initialize_pwm_output_signal_value_publisher(THRUSTER3, "thruster_3/pwm_output_signal_value_us", thruster_pwm_controller_node) &&
        initialize_pwm_output_signal_value_publisher(THRUSTER4, "thruster_4/pwm_output_signal_value_us", thruster_pwm_controller_node) &&
        initialize_pwm_output_signal_value_publisher(THRUSTER5, "thruster_5/pwm_output_signal_value_us", thruster_pwm_controller_node) &&
        initialize_pwm_output_signal_value_publisher(THRUSTER6, "thruster_6/pwm_output_signal_value_us", thruster_pwm_controller_node) &&
        initialize_pwm_output_signal_value_publisher(THRUSTER7, "thruster_7/pwm_output_signal_value_us", thruster_pwm_controller_node);
}


static void publish_pwm_output_signal_value(ThrusterNumber thruster_number, int32_t pwm_output_signal_value_us)
{
    std_msgs__msg__Int32 pwm_output_signal_value_msg;
    pwm_output_signal_value_msg.data = pwm_output_signal_value_us;

    rcl_ret_t ret = rcl_publish(&(thrusters_data[thruster_number].pwm_output_signal_value_publisher), &pwm_output_signal_value_msg, NULL);
    if (ret != RCL_RET_OK)
    {
        printf("Error publishing (line %d)\n", __LINE__);
    }
}


void publish_pwm_output_signal_value_only_when_updated(ThrusterNumber thruster_number)
{
    int32_t current_pwm_output_signal_value_us = (int32_t)get_pwm_output_signal_value_us(thruster_number);

    if(current_pwm_output_signal_value_us != thrusters_data[thruster_number].previous_pwm_output_signal_value_us)
    {
        publish_pwm_output_signal_value(thruster_number, current_pwm_output_signal_value_us);
        thrusters_data[thruster_number].previous_pwm_output_signal_value_us = current_pwm_output_signal_value_us;
    }
}


static void initialize_previous_pwm_output_signal_value(ThrusterNumber thruster_number)
{
    int32_t current_pwm_output_signal_value_us = (int32_t)get_pwm_output_signal_value_us(thruster_number);

    publish_pwm_output_signal_value(thruster_number, current_pwm_output_signal_value_us);
    thrusters_data[thruster_number].previous_pwm_output_signal_value_us = current_pwm_output_signal_value_us;
}


void initialize_previous_pwm_output_signal_value_for_all_thrusters()
{
    initialize_previous_pwm_output_signal_value(THRUSTER0);
    initialize_previous_pwm_output_signal_value(THRUSTER1);
    initialize_previous_pwm_output_signal_value(THRUSTER2);
    initialize_previous_pwm_output_signal_value(THRUSTER3);
    initialize_previous_pwm_output_signal_value(THRUSTER4);
    initialize_previous_pwm_output_signal_value(THRUSTER5);
    initialize_previous_pwm_output_signal_value(THRUSTER6);
    initialize_previous_pwm_output_signal_value(THRUSTER7);
}
