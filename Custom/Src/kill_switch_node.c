#include "kill_switch_node.h"

#include <std_msgs/msg/bool.h>

#include "kill_switch_driver.h"


const unsigned int KILL_SWITCH_NUM_HANDLES = 0;


static rcl_publisher_t is_kill_switch_closed_publisher;

static bool previous_kill_switch_state;


static void publish_kill_switch_state(bool current_kill_switch_state)
{
    std_msgs__msg__Bool is_kill_switch_closed_msg;
    is_kill_switch_closed_msg.data = current_kill_switch_state;

    rcl_ret_t ret = rcl_publish(&is_kill_switch_closed_publisher, &is_kill_switch_closed_msg, NULL);
    if (ret != RCL_RET_OK)
    {
        printf("Error publishing (line %d)\n", __LINE__);
    }
}


static void kill_switch_timer_callback(rcl_timer_t *, int64_t)
{
    bool current_kill_switch_state = is_kill_switch_closed();

    if(current_kill_switch_state != previous_kill_switch_state)
    {
        publish_kill_switch_state(current_kill_switch_state);
        previous_kill_switch_state = current_kill_switch_state;
    }
}

void kill_switch_on_timer_tick(void)
{
    kill_switch_timer_callback(NULL, 0);
}


void initialize_kill_switch_node(rclc_support_t *support, rclc_executor_t *executor, rcl_node_t *stm32_node)
{
    (void)executor;

    // create publisher
    rcl_ret_t rc = rclc_publisher_init_default(
        &is_kill_switch_closed_publisher,
        stm32_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "is_kill_switch_closed"
    );
    if (rc != RCL_RET_OK) {
        printf("kill_switch_node: publisher init failed (rc=%d)\n", (int)rc);
        return;
    }

    previous_kill_switch_state = is_kill_switch_closed();
    publish_kill_switch_state(previous_kill_switch_state);
}
