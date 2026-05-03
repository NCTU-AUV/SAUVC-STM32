#include "kill_switch_node.h"

#include <std_msgs/msg/bool.h>

#include "kill_switch_driver.h"
#include "debug_logger.h"

#include <stdio.h>
#include <rcl/error_handling.h>


const unsigned int KILL_SWITCH_NUM_HANDLES = 0;


static rcl_publisher_t killed_publisher;

static bool previous_killed_state;


static void publish_killed_state(bool killed)
{
    std_msgs__msg__Bool killed_msg;
    killed_msg.data = killed;

    rcl_ret_t ret = rcl_publish(&killed_publisher, &killed_msg, NULL);
    if (ret != RCL_RET_OK)
    {
        char msg[128];
        int n = snprintf(msg, sizeof(msg),
                         "kill_switch_node: failed to publish kill switch state (rc=%d): %s\n",
                         (int)ret,
                         rcl_get_error_string().str);
        if (n > 0) {
            (void)debug_logger_publish(msg);
        }
        rcl_reset_error();
    }
}


static void kill_switch_timer_callback(rcl_timer_t *, int64_t)
{
    bool killed = kill_switch_closed();

    if(killed != previous_killed_state)
    {
        publish_killed_state(killed);
        previous_killed_state = killed;
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
        &killed_publisher,
        stm32_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "sensors/killed"
    );
    if (rc != RCL_RET_OK) {
        char msg[128];
        int n = snprintf(msg, sizeof(msg),
                         "kill_switch_node: publisher init failed (rc=%d): %s\n",
                         (int)rc,
                         rcl_get_error_string().str);
        if (n > 0) {
            (void)debug_logger_publish(msg);
        }
        rcl_reset_error();
        return;
    }

    previous_killed_state = kill_switch_closed();
    publish_killed_state(previous_killed_state);
}
