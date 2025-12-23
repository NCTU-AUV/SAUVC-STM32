#include "thruster_pwm_controller_node/publish_is_pwm_output_on.h"


#include <std_msgs/msg/bool.h>


static struct {
    rcl_publisher_t is_pwm_output_on_publisher;
    bool previous_is_pwm_output_on_state;
} thrusters_data[8];


static bool initialize_is_pwm_output_on_publisher(ThrusterNumber thruster_number, const char *topic_name, rcl_node_t *thruster_pwm_controller_node)
{
    rcl_ret_t rc = rclc_publisher_init_default(
        &(thrusters_data[thruster_number].is_pwm_output_on_publisher),
        thruster_pwm_controller_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        topic_name
    );
    if (rc != RCL_RET_OK) {
        printf("is_pwm_output_on publisher init failed for %s (rc=%d)\n", topic_name, (int)rc);
        return false;
    }
    return true;
}


bool initialize_is_pwm_output_on_publisher_for_all_thrusters(rcl_node_t *thruster_pwm_controller_node)
{
    return
        initialize_is_pwm_output_on_publisher(THRUSTER0, "thruster_0/is_pwm_output_on", thruster_pwm_controller_node) &&
        initialize_is_pwm_output_on_publisher(THRUSTER1, "thruster_1/is_pwm_output_on", thruster_pwm_controller_node) &&
        initialize_is_pwm_output_on_publisher(THRUSTER2, "thruster_2/is_pwm_output_on", thruster_pwm_controller_node) &&
        initialize_is_pwm_output_on_publisher(THRUSTER3, "thruster_3/is_pwm_output_on", thruster_pwm_controller_node) &&
        initialize_is_pwm_output_on_publisher(THRUSTER4, "thruster_4/is_pwm_output_on", thruster_pwm_controller_node) &&
        initialize_is_pwm_output_on_publisher(THRUSTER5, "thruster_5/is_pwm_output_on", thruster_pwm_controller_node) &&
        initialize_is_pwm_output_on_publisher(THRUSTER6, "thruster_6/is_pwm_output_on", thruster_pwm_controller_node) &&
        initialize_is_pwm_output_on_publisher(THRUSTER7, "thruster_7/is_pwm_output_on", thruster_pwm_controller_node);
}


static void publish_is_pwm_output_on(ThrusterNumber thruster_number, bool is_pwm_output_on_state)
{
    std_msgs__msg__Bool is_pwm_output_on_msg;
    is_pwm_output_on_msg.data = is_pwm_output_on_state;

    rcl_ret_t ret = rcl_publish(&(thrusters_data[thruster_number].is_pwm_output_on_publisher), &is_pwm_output_on_msg, NULL);
    if (ret != RCL_RET_OK)
    {
        printf("Error publishing (line %d)\n", __LINE__);
    }
}


void publish_is_pwm_output_on_only_when_updated(ThrusterNumber thruster_number)
{
    bool current_is_pwm_output_on_state = is_pwm_output_on(thruster_number);

    if(current_is_pwm_output_on_state != thrusters_data[thruster_number].previous_is_pwm_output_on_state)
    {
        publish_is_pwm_output_on(thruster_number, current_is_pwm_output_on_state);
        thrusters_data[thruster_number].previous_is_pwm_output_on_state = current_is_pwm_output_on_state;
    }
}


static void initialize_previous_is_pwm_output_on_state(ThrusterNumber thruster_number)
{
    bool current_is_pwm_output_on_state = is_pwm_output_on(thruster_number);

    publish_is_pwm_output_on(thruster_number, current_is_pwm_output_on_state);
    thrusters_data[thruster_number].previous_is_pwm_output_on_state = current_is_pwm_output_on_state;
}


void initialize_previous_is_pwm_output_on_state_for_all_thrusters()
{
    initialize_previous_is_pwm_output_on_state(THRUSTER0);
    initialize_previous_is_pwm_output_on_state(THRUSTER1);
    initialize_previous_is_pwm_output_on_state(THRUSTER2);
    initialize_previous_is_pwm_output_on_state(THRUSTER3);
    initialize_previous_is_pwm_output_on_state(THRUSTER4);
    initialize_previous_is_pwm_output_on_state(THRUSTER5);
    initialize_previous_is_pwm_output_on_state(THRUSTER6);
    initialize_previous_is_pwm_output_on_state(THRUSTER7);
}
