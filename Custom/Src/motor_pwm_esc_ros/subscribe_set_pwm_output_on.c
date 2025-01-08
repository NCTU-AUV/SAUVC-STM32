#include "motor_pwm_esc_ros/subscribe_set_pwm_output_on.h"


#include "motor_pwm_esc_driver.h"


#include <std_msgs/msg/bool.h>


static rcl_subscription_t set_pwm_output_on_subscribers[8];


static void initialize_set_pwm_output_on_subscriber(MotorNumber motor_number, const char *topic_name, rcl_node_t *stm32_node)
{
    set_pwm_output_on_subscribers[motor_number] = rcl_get_zero_initialized_subscription();
    rcl_ret_t rc = rclc_subscription_init_default(
        &(set_pwm_output_on_subscribers[motor_number]),
        stm32_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        topic_name
    );
    if (rc != RCL_RET_OK) {
        printf("Failed to create subscriber %s.\n", topic_name);
    } else {
        printf("Created subscriber %s:\n", topic_name);
    }
}


void initialize_set_pwm_output_on_subscriber_for_all_motors(rcl_node_t *stm32_node)
{
    initialize_set_pwm_output_on_subscriber(MOTOR0, "motor_0/set_pwm_output_on", stm32_node);
    initialize_set_pwm_output_on_subscriber(MOTOR1, "motor_1/set_pwm_output_on", stm32_node);
    initialize_set_pwm_output_on_subscriber(MOTOR2, "motor_2/set_pwm_output_on", stm32_node);
    initialize_set_pwm_output_on_subscriber(MOTOR3, "motor_3/set_pwm_output_on", stm32_node);
    initialize_set_pwm_output_on_subscriber(MOTOR4, "motor_4/set_pwm_output_on", stm32_node);
    initialize_set_pwm_output_on_subscriber(MOTOR5, "motor_5/set_pwm_output_on", stm32_node);
    initialize_set_pwm_output_on_subscriber(MOTOR6, "motor_6/set_pwm_output_on", stm32_node);
    initialize_set_pwm_output_on_subscriber(MOTOR7, "motor_7/set_pwm_output_on", stm32_node);
}
