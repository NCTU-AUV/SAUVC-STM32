#ifndef KILL_SWITCH_NODE
#define KILL_SWITCH_NODE

#include <rclc/rclc.h>

void initialize_kill_switch_node(rclc_support_t *support);

void read_and_publish_kill_switch_state();

#endif
