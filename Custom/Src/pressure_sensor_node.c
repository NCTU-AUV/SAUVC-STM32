#include "pressure_sensor_node.h"


#include <rcl/error_handling.h>
#include <std_msgs/msg/float32.h>

#include "debug_logger.h"
#include <stdio.h>


const unsigned int PRESSURE_SENSOR_NUM_HANDLES = 0;


static rcl_publisher_t pressure_sensor_depth_publisher;

static osMessageQueueId_t pressure_sensor_depth_queue_handle;


static void publish_pressure_sensor_depth(float pressure_sensor_depth_m)
{
    std_msgs__msg__Float32 pressure_sensor_depth_msg;
    pressure_sensor_depth_msg.data = pressure_sensor_depth_m;

    rcl_ret_t ret = rcl_publish(&pressure_sensor_depth_publisher, &pressure_sensor_depth_msg, NULL);
    if (ret != RCL_RET_OK)
    {
        char msg[128];
        int n = snprintf(msg, sizeof(msg),
                         "pressure_sensor_node: failed to publish depth (rc=%d): %s\n",
                         (int)ret,
                         rcl_get_error_string().str);
        if (n > 0) {
            (void)debug_logger_publish(msg);
        }
        rcl_reset_error();
    }
}


static void pressure_sensor_timer_callback(rcl_timer_t *, int64_t)
{
    float pressure_sensor_depth_m = 0.0f;
    float latest_pressure_sensor_depth_m = 0.0f;
    bool has_sample = false;

    while (osMessageQueueGet(pressure_sensor_depth_queue_handle, &pressure_sensor_depth_m, NULL, 0U) == osOK) {
        latest_pressure_sensor_depth_m = pressure_sensor_depth_m;
        has_sample = true;
    }

    if (has_sample) {
        publish_pressure_sensor_depth(latest_pressure_sensor_depth_m);
    }
}


void pressure_sensor_on_timer_tick(void)
{
    pressure_sensor_timer_callback(NULL, 0);
}


void initialize_pressure_sensor_node(rclc_support_t *support, rclc_executor_t *executor, rcl_node_t *stm32_node, osMessageQueueId_t pressureSensorDepthQueueHandle)
{
    (void)executor;

    // create publisher
    rcl_ret_t rc = rclc_publisher_init_default(
        &pressure_sensor_depth_publisher,
        stm32_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
        "pressure_sensor_depth_m"
    );
    if (rc != RCL_RET_OK) {
        char msg[128];
        int n = snprintf(msg, sizeof(msg),
                         "pressure_sensor_node: publisher init failed (rc=%d): %s\n",
                         (int)rc,
                         rcl_get_error_string().str);
        if (n > 0) {
            (void)debug_logger_publish(msg);
        }
        rcl_reset_error();
        return;
    }

    pressure_sensor_depth_queue_handle = pressureSensorDepthQueueHandle;
}
