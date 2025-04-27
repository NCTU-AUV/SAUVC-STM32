#include "pressure_sensor_node.h"


#include <std_msgs/msg/float32.h>


const unsigned int PRESSURE_SENSOR_NUM_HANDLES = 1;


static rcl_node_t pressure_sensor_node;
static rcl_publisher_t pressure_sensor_depth_publisher;
static rcl_timer_t pressure_sensor_timer;

static osMessageQueueId_t pressure_sensor_depth_queue_handle;


static void publish_pressure_sensor_depth(float pressure_sensor_depth_m)
{
    std_msgs__msg__Float32 pressure_sensor_depth_msg;
    pressure_sensor_depth_msg.data = pressure_sensor_depth_m;

    rcl_ret_t ret = rcl_publish(&pressure_sensor_depth_publisher, &pressure_sensor_depth_msg, NULL);
    if (ret != RCL_RET_OK)
    {
        printf("Error publishing (line %d)\n", __LINE__);
    }
}


static void pressure_sensor_timer_callback(rcl_timer_t *, int64_t)
{
    float pressure_sensor_depth_m = (float) 432.1;
    publish_pressure_sensor_depth(pressure_sensor_depth_m);

    // float pressure_sensor_depth_m;

    // osStatus_t status = osMessageQueueGet(pressure_sensor_depth_queue_handle, &pressure_sensor_depth_m, NULL, 0U);

    // if (status == osOK) {
    //     publish_pressure_sensor_depth(pressure_sensor_depth_m);
    // }
}


static void initialize_pressure_sensor_timer(rclc_support_t *support, rclc_executor_t *executor)
{
    pressure_sensor_timer = rcl_get_zero_initialized_timer();
    const unsigned int pressure_sensor_timer_timeout = 100;
    rcl_ret_t rc = rclc_timer_init_default(
        &pressure_sensor_timer,
        support,
        RCL_MS_TO_NS(pressure_sensor_timer_timeout),
        pressure_sensor_timer_callback
    );
    if (rc != RCL_RET_OK) {
        printf("Error in rcl_timer_init_default.\n");
    } else {
        printf("Created timer with timeout %d ms.\n", pressure_sensor_timer_timeout);
    }

    rclc_executor_add_timer(executor, &pressure_sensor_timer);
    if (rc != RCL_RET_OK) {
        printf("Error in rclc_executor_add_timer.\n");
    }
}


void initialize_pressure_sensor_node(rclc_support_t *support, rclc_executor_t *executor, osMessageQueueId_t pressureSensorDepthQueueHandle)
{
    rclc_node_init_default(&pressure_sensor_node, "pressure_sensor_node", "orca_auv", support);

    // create publisher
    rclc_publisher_init_default(
        &pressure_sensor_depth_publisher,
        &pressure_sensor_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
        "pressure_sensor_depth_m"
    );

    initialize_pressure_sensor_timer(support, executor);

    pressure_sensor_depth_queue_handle = pressureSensorDepthQueueHandle;
}
