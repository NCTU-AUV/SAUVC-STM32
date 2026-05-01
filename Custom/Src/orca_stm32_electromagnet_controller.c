#include "orca_stm32_electromagnet_controller.h"

#include "debug_logger.h"
#include "main.h"

#include <rcl/error_handling.h>
#include <std_msgs/msg/bool.h>
#include <stdio.h>


const unsigned int ELECTROMAGNET_CONTROLLER_NUM_HANDLES = 1;


static rcl_subscription_t electromagnet_set_on_subscriber;
static std_msgs__msg__Bool electromagnet_set_on_msg;


static void set_electromagnet_enabled(bool enabled)
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, enabled ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


static void electromagnet_set_on_callback(const void *msgin)
{
    const std_msgs__msg__Bool *msg = (const std_msgs__msg__Bool *)msgin;
    if (msg == NULL) {
        (void)debug_logger_publish("electromagnet callback received null message payload\n");
        return;
    }

    set_electromagnet_enabled(msg->data);
}


void initialize_orca_stm32_electromagnet_controller(rclc_support_t *support, rclc_executor_t *executor, rcl_node_t *orca_stm32_bridge)
{
    (void)support;

    set_electromagnet_enabled(false);

    electromagnet_set_on_subscriber = rcl_get_zero_initialized_subscription();
    rcl_ret_t rc = rclc_subscription_init_default(
        &electromagnet_set_on_subscriber,
        orca_stm32_bridge,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "actuators/electromagnet/enabled"
    );
    if (rc != RCL_RET_OK) {
        char msg[128];
        int n = snprintf(msg, sizeof(msg),
                         "failed to create actuators/electromagnet/enabled subscriber (rc=%d): %s\n",
                         (int)rc,
                         rcl_get_error_string().str);
        if (n > 0) {
            (void)debug_logger_publish(msg);
        }
        rcl_reset_error();
        return;
    }

    std_msgs__msg__Bool__init(&electromagnet_set_on_msg);
    rc = rclc_executor_add_subscription(
        executor,
        &electromagnet_set_on_subscriber,
        &electromagnet_set_on_msg,
        &electromagnet_set_on_callback,
        ON_NEW_DATA
    );
    if (rc != RCL_RET_OK) {
        char msg[128];
        int n = snprintf(msg, sizeof(msg),
                         "failed to register actuators/electromagnet/enabled callback (rc=%d): %s\n",
                         (int)rc,
                         rcl_get_error_string().str);
        if (n > 0) {
            (void)debug_logger_publish(msg);
        }
        rcl_reset_error();
    }
}
