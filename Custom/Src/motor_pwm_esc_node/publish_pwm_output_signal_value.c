#include "motor_pwm_esc_node/publish_pwm_output_signal_value.h"


#include <std_msgs/msg/int32.h>


static struct {
    rcl_publisher_t pwm_output_signal_value_publisher;
    int32_t previous_pwm_output_signal_value_us;
} motors_data[8];


static void initialize_pwm_output_signal_value_publisher(MotorNumber motor_number, const char *topic_name, rcl_node_t *motor_pwm_esc_node)
{
    rclc_publisher_init_default(
        &(motors_data[motor_number].pwm_output_signal_value_publisher),
        motor_pwm_esc_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        topic_name
    );
}


void initialize_pwm_output_signal_value_publisher_for_all_motors(rcl_node_t *motor_pwm_esc_node)
{
    initialize_pwm_output_signal_value_publisher(MOTOR0, "motor_0/pwm_output_signal_value_us", motor_pwm_esc_node);
    initialize_pwm_output_signal_value_publisher(MOTOR1, "motor_1/pwm_output_signal_value_us", motor_pwm_esc_node);
    initialize_pwm_output_signal_value_publisher(MOTOR2, "motor_2/pwm_output_signal_value_us", motor_pwm_esc_node);
    initialize_pwm_output_signal_value_publisher(MOTOR3, "motor_3/pwm_output_signal_value_us", motor_pwm_esc_node);
    initialize_pwm_output_signal_value_publisher(MOTOR4, "motor_4/pwm_output_signal_value_us", motor_pwm_esc_node);
    initialize_pwm_output_signal_value_publisher(MOTOR5, "motor_5/pwm_output_signal_value_us", motor_pwm_esc_node);
    initialize_pwm_output_signal_value_publisher(MOTOR6, "motor_6/pwm_output_signal_value_us", motor_pwm_esc_node);
    initialize_pwm_output_signal_value_publisher(MOTOR7, "motor_7/pwm_output_signal_value_us", motor_pwm_esc_node);
}


static void publish_pwm_output_signal_value(MotorNumber motor_number, int32_t pwm_output_signal_value_us)
{
    std_msgs__msg__Int32 pwm_output_signal_value_msg;
    pwm_output_signal_value_msg.data = pwm_output_signal_value_us;

    rcl_ret_t ret = rcl_publish(&(motors_data[motor_number].pwm_output_signal_value_publisher), &pwm_output_signal_value_msg, NULL);
    if (ret != RCL_RET_OK)
    {
        printf("Error publishing (line %d)\n", __LINE__);
    }
}


void publish_pwm_output_signal_value_only_when_updated(MotorNumber motor_number)
{
    int16_t current_pwm_output_signal_value_us = get_pwm_output_compare_value(motor_number);

    if(current_pwm_output_signal_value_us != motors_data[motor_number].previous_pwm_output_signal_value_us)
    {
        publish_pwm_output_signal_value(motor_number, current_pwm_output_signal_value_us);
        motors_data[motor_number].previous_pwm_output_signal_value_us = current_pwm_output_signal_value_us;
    }
}


static void initialize_previous_pwm_output_signal_value(MotorNumber motor_number)
{
    bool current_pwm_output_signal_value_us = get_pwm_output_compare_value(motor_number);

    publish_pwm_output_signal_value(motor_number, current_pwm_output_signal_value_us);
    motors_data[motor_number].previous_pwm_output_signal_value_us = current_pwm_output_signal_value_us;
}


void initialize_previous_pwm_output_signal_value_for_all_motors()
{
    initialize_previous_pwm_output_signal_value(MOTOR0);
    initialize_previous_pwm_output_signal_value(MOTOR1);
    initialize_previous_pwm_output_signal_value(MOTOR2);
    initialize_previous_pwm_output_signal_value(MOTOR3);
    initialize_previous_pwm_output_signal_value(MOTOR4);
    initialize_previous_pwm_output_signal_value(MOTOR5);
    initialize_previous_pwm_output_signal_value(MOTOR6);
    initialize_previous_pwm_output_signal_value(MOTOR7);
}