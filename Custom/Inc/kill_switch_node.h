#ifndef KILL_SWITCH_NODE
#define KILL_SWITCH_NODE

#include <rclc/rclc.h>
#include <rclc/executor.h>

extern const unsigned int KILL_SWITCH_NUM_HANDLES;

void initialize_kill_switch_node(rclc_support_t *support, rclc_executor_t *executor);

void read_and_publish_kill_switch_state();

#endif
