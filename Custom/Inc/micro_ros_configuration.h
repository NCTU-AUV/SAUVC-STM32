#ifndef MICRO_ROS_CONFIGURATION
#define MICRO_ROS_CONFIGURATION

#include "stm32f4xx_hal.h"

void configure_micro_ros(UART_HandleTypeDef *const ros_transport_huart);

#endif