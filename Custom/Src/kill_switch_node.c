#include "kill_switch_node.h"

#include <std_msgs/msg/bool.h>

#include "kill_switch_driver.h"


const unsigned int KILL_SWITCH_NUM_HANDLES = 1;


static rcl_node_t kill_switch_node;
static rcl_publisher_t is_kill_switch_closed_publisher;
static rcl_timer_t kill_switch_timer;

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


static void initialize_kill_switch_timer(rclc_support_t *support, rclc_executor_t *executor)
{
    kill_switch_timer = rcl_get_zero_initialized_timer();
    const unsigned int kill_switch_timer_timeout = 100;
    rcl_ret_t rc = rclc_timer_init_default(
        &kill_switch_timer,
        support,
        RCL_MS_TO_NS(kill_switch_timer_timeout),
        kill_switch_timer_callback
    );
    if (rc != RCL_RET_OK) {
        printf("Error in rcl_timer_init_default.\n");
    } else {
        printf("Created timer with timeout %d ms.\n", kill_switch_timer_timeout);
    }

    rclc_executor_add_timer(executor, &kill_switch_timer);
    if (rc != RCL_RET_OK) {
        printf("Error in rclc_executor_add_timer.\n");
    }
}


void initialize_kill_switch_node(rclc_support_t *support, rclc_executor_t *executor)
{
    rclc_node_init_default(&kill_switch_node, "kill_switch_node", "orca_auv", support);

    // create publisher
    rclc_publisher_init_default(
        &is_kill_switch_closed_publisher,
        &kill_switch_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "is_kill_switch_closed"
    );

    initialize_kill_switch_timer(support, executor);

    previous_kill_switch_state = is_kill_switch_closed();
    publish_kill_switch_state(previous_kill_switch_state);
}
