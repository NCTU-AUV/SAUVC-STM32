#ifndef PRESSURE_SENSOR_NODE
#define PRESSURE_SENSOR_NODE

#include <rclc/rclc.h>
#include <rclc/executor.h>

#include "cmsis_os2.h"

extern const unsigned int PRESSURE_SENSOR_NUM_HANDLES;

void initialize_pressure_sensor_node(rclc_support_t *support, rclc_executor_t *executor, osMessageQueueId_t pressureSensorDepthQueueHandle);

#endif
