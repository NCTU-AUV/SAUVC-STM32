#include "motor_pwm_controller_node/subscribe_set_pwm_output_signal_value.h"


#include "motor_pwm_control_driver.h"


#include <std_msgs/msg/int32.h>


static struct {
    rcl_subscription_t set_pwm_output_signal_value_subscriber;
    std_msgs__msg__Int32 set_pwm_output_signal_value_msg;
    MotorNumber motor_number;
} motors_data[8];


static void the_set_pwm_output_signal_value_subscriber_callback_with_context(const void * msgin, void * context_void_ptr)
{
    const std_msgs__msg__Int32 * set_pwm_output_signal_value_msg = (const std_msgs__msg__Int32 *)msgin;
    if (set_pwm_output_signal_value_msg == NULL) {
        printf("Callback: msg NULL\n");
        return;
    }
    
    if (context_void_ptr == NULL) {
        printf("Callback: context is empty\n");
        return;
    }
    
    const MotorNumber * motor_number = (const MotorNumber *) context_void_ptr;
    
    int32_t output_signal_value_us = set_pwm_output_signal_value_msg->data;
    if(output_signal_value_us < 0)
    {
        output_signal_value_us = -output_signal_value_us;
    }

    set_motor_pwm_output(*motor_number, (uint32_t)(output_signal_value_us));
}


static void initialize_set_pwm_output_signal_value_subscriber(MotorNumber motor_number, const char *topic_name, rcl_node_t *motor_pwm_controller_node, rclc_executor_t *executor)
{
    motors_data[motor_number].set_pwm_output_signal_value_subscriber = rcl_get_zero_initialized_subscription();
    rcl_ret_t rc = rclc_subscription_init_default(
        &(motors_data[motor_number].set_pwm_output_signal_value_subscriber),
        motor_pwm_controller_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        topic_name
    );
    if (rc != RCL_RET_OK) {
        printf("Failed to create subscriber %s.\n", topic_name);
    } else {
        printf("Created subscriber %s:\n", topic_name);
    }

    std_msgs__msg__Int32__init(&(motors_data[motor_number].set_pwm_output_signal_value_msg));
    motors_data[motor_number].motor_number = motor_number;
    rc = rclc_executor_add_subscription_with_context(
        executor,
        &(motors_data[motor_number].set_pwm_output_signal_value_subscriber),
        &(motors_data[motor_number].set_pwm_output_signal_value_msg),
        &the_set_pwm_output_signal_value_subscriber_callback_with_context,
        (void *) &(motors_data[motor_number].motor_number),
        ON_NEW_DATA
    );
    if (rc != RCL_RET_OK) {
        printf("Error in rclc_executor_add_subscription. \n");
    }
}


void initialize_set_pwm_output_signal_value_subscriber_for_all_motors(rcl_node_t *motor_pwm_controller_node, rclc_executor_t *executor)
{
    initialize_set_pwm_output_signal_value_subscriber(MOTOR0, "motor_0/set_pwm_output_signal_value_us", motor_pwm_controller_node, executor);
    initialize_set_pwm_output_signal_value_subscriber(MOTOR1, "motor_1/set_pwm_output_signal_value_us", motor_pwm_controller_node, executor);
    initialize_set_pwm_output_signal_value_subscriber(MOTOR2, "motor_2/set_pwm_output_signal_value_us", motor_pwm_controller_node, executor);
    initialize_set_pwm_output_signal_value_subscriber(MOTOR3, "motor_3/set_pwm_output_signal_value_us", motor_pwm_controller_node, executor);
    initialize_set_pwm_output_signal_value_subscriber(MOTOR4, "motor_4/set_pwm_output_signal_value_us", motor_pwm_controller_node, executor);
    initialize_set_pwm_output_signal_value_subscriber(MOTOR5, "motor_5/set_pwm_output_signal_value_us", motor_pwm_controller_node, executor);
    initialize_set_pwm_output_signal_value_subscriber(MOTOR6, "motor_6/set_pwm_output_signal_value_us", motor_pwm_controller_node, executor);
    initialize_set_pwm_output_signal_value_subscriber(MOTOR7, "motor_7/set_pwm_output_signal_value_us", motor_pwm_controller_node, executor);
}
