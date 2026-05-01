#ifndef PRESSURE_SENSOR_NODE
#define PRESSURE_SENSOR_NODE

#include <rclc/rclc.h>
#include <rclc/executor.h>

#include "cmsis_os2.h"

extern const unsigned int PRESSURE_SENSOR_NUM_HANDLES;

void initialize_pressure_sensor_node(rclc_support_t *support, rclc_executor_t *executor, rcl_node_t *stm32_node, osMessageQueueId_t pressureSensorDepthQueueHandle);
void pressure_sensor_on_timer_tick(void);

#endif
