#ifndef ORCA_STM32_ELECTROMAGNET_CONTROLLER_H
#define ORCA_STM32_ELECTROMAGNET_CONTROLLER_H

#include <rclc/rclc.h>
#include <rclc/executor.h>

extern const unsigned int ELECTROMAGNET_CONTROLLER_NUM_HANDLES;

void initialize_orca_stm32_electromagnet_controller(rclc_support_t *support, rclc_executor_t *executor, rcl_node_t *orca_stm32_bridge);

#endif
