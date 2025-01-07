#include "kill_switch_node.h"

#include <std_msgs/msg/bool.h>

#include "kill_switch_driver.h"


static rcl_node_t kill_switch_node;
static rcl_publisher_t is_kill_switch_closed_publisher;
static std_msgs__msg__Bool is_kill_switch_closed_msg;


void initialize_kill_switch_node(rclc_support_t *support)
{
    // create node
    rclc_node_init_default(&kill_switch_node, "kill_switch_node", "orca_auv", support);

    // create publisher
    rclc_publisher_init_default(
        &is_kill_switch_closed_publisher,
        &kill_switch_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "kill_switch_publisher"
    );
}


void read_and_publish_kill_switch_state()
{
    is_kill_switch_closed_msg.data = is_kill_switch_closed();

    rcl_ret_t ret = rcl_publish(&is_kill_switch_closed_publisher, &is_kill_switch_closed_msg, NULL);
    if (ret != RCL_RET_OK)
    {
        printf("Error publishing (line %d)\n", __LINE__);
    }
}
