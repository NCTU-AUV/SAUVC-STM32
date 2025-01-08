#ifndef KILL_SWITCH_ROS
#define KILL_SWITCH_ROS

#include <rclc/rclc.h>
#include <rclc/executor.h>

extern const unsigned int KILL_SWITCH_NUM_HANDLES;

void initialize_kill_switch_ros(rcl_node_t *stm32_node, rclc_support_t *support, rclc_executor_t *executor);

#endif
