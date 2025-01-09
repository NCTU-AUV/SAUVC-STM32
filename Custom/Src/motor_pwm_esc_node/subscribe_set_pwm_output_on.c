#include "motor_pwm_esc_node/subscribe_set_pwm_output_on.h"


#include "motor_pwm_control_driver.h"


#include <std_msgs/msg/bool.h>


static struct {
    rcl_subscription_t set_pwm_output_on_subscriber;
    std_msgs__msg__Bool set_pwm_output_on_msg;
    MotorNumber motor_number;
} motors_data[8];


static void the_set_pwm_output_on_subscriber_callback_with_context(const void * msgin, void * context_void_ptr)
{
    const std_msgs__msg__Bool * set_pwm_output_on_msg = (const std_msgs__msg__Bool *)msgin;
    if (set_pwm_output_on_msg == NULL) {
        printf("Callback: msg NULL\n");
        return;
    }
    
    if (context_void_ptr == NULL) {
        printf("Callback: context is empty\n");
        return;
    }
    
    const MotorNumber * motor_number = (const MotorNumber *) context_void_ptr;
    
    if(set_pwm_output_on_msg->data == true)
    {
        start_motor_pwm_output(*motor_number);
    } else {
        stop_motor_pwm_output(*motor_number);
    }
}


static void initialize_set_pwm_output_on_subscriber(MotorNumber motor_number, const char *topic_name, rcl_node_t *motor_pwm_esc_node, rclc_executor_t *executor)
{
    motors_data[motor_number].set_pwm_output_on_subscriber = rcl_get_zero_initialized_subscription();
    rcl_ret_t rc = rclc_subscription_init_default(
        &(motors_data[motor_number].set_pwm_output_on_subscriber),
        motor_pwm_esc_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        topic_name
    );
    if (rc != RCL_RET_OK) {
        printf("Failed to create subscriber %s.\n", topic_name);
    } else {
        printf("Created subscriber %s:\n", topic_name);
    }

    std_msgs__msg__Bool__init(&(motors_data[motor_number].set_pwm_output_on_msg));
    motors_data[motor_number].motor_number = motor_number;
    rc = rclc_executor_add_subscription_with_context(
        executor,
        &(motors_data[motor_number].set_pwm_output_on_subscriber),
        &(motors_data[motor_number].set_pwm_output_on_msg),
        &the_set_pwm_output_on_subscriber_callback_with_context,
        (void *) &(motors_data[motor_number].motor_number),
        ON_NEW_DATA
    );
    if (rc != RCL_RET_OK) {
        printf("Error in rclc_executor_add_subscription. \n");
    }
}


void initialize_set_pwm_output_on_subscriber_for_all_motors(rcl_node_t *motor_pwm_esc_node, rclc_executor_t *executor)
{
    initialize_set_pwm_output_on_subscriber(MOTOR0, "motor_0/set_pwm_output_on", motor_pwm_esc_node, executor);
    initialize_set_pwm_output_on_subscriber(MOTOR1, "motor_1/set_pwm_output_on", motor_pwm_esc_node, executor);
    initialize_set_pwm_output_on_subscriber(MOTOR2, "motor_2/set_pwm_output_on", motor_pwm_esc_node, executor);
    initialize_set_pwm_output_on_subscriber(MOTOR3, "motor_3/set_pwm_output_on", motor_pwm_esc_node, executor);
    initialize_set_pwm_output_on_subscriber(MOTOR4, "motor_4/set_pwm_output_on", motor_pwm_esc_node, executor);
    initialize_set_pwm_output_on_subscriber(MOTOR5, "motor_5/set_pwm_output_on", motor_pwm_esc_node, executor);
    initialize_set_pwm_output_on_subscriber(MOTOR6, "motor_6/set_pwm_output_on", motor_pwm_esc_node, executor);
    initialize_set_pwm_output_on_subscriber(MOTOR7, "motor_7/set_pwm_output_on", motor_pwm_esc_node, executor);
}
