#include "motor_pwm_esc_ros/publish_is_pwm_output_on.h"


#include <std_msgs/msg/bool.h>


static struct {
    rcl_publisher_t is_pwm_output_on_publishers;
    bool previous_is_pwm_output_on_state;
} motors_data[8];


static void initialize_is_pwm_output_on_publisher(MotorNumber motor_number, const char *topic_name, rcl_node_t *stm32_node)
{
    rclc_publisher_init_default(
        &(motors_data[motor_number].is_pwm_output_on_publishers),
        stm32_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        topic_name
    );
}


void initialize_is_pwm_output_on_publisher_for_all_motors(rcl_node_t *stm32_node)
{
    initialize_is_pwm_output_on_publisher(MOTOR0, "motor_0/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher(MOTOR1, "motor_1/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher(MOTOR2, "motor_2/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher(MOTOR3, "motor_3/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher(MOTOR4, "motor_4/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher(MOTOR5, "motor_5/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher(MOTOR6, "motor_6/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher(MOTOR7, "motor_7/is_pwm_output_on", stm32_node);
}


static void publish_is_pwm_output_on(MotorNumber motor_number, bool is_pwm_output_on_state)
{
    std_msgs__msg__Bool is_pwm_output_on_msg;
    is_pwm_output_on_msg.data = is_pwm_output_on_state;

    rcl_ret_t ret = rcl_publish(&(motors_data[motor_number].is_pwm_output_on_publishers), &is_pwm_output_on_msg, NULL);
    if (ret != RCL_RET_OK)
    {
        printf("Error publishing (line %d)\n", __LINE__);
    }
}


void publish_is_pwm_output_on_only_when_updated(MotorNumber motor_number)
{
    bool current_is_pwm_output_on_state = is_pwm_output_on(motor_number);

    if(current_is_pwm_output_on_state != motors_data[motor_number].previous_is_pwm_output_on_state)
    {
        publish_is_pwm_output_on(motor_number, current_is_pwm_output_on_state);
        motors_data[motor_number].previous_is_pwm_output_on_state = current_is_pwm_output_on_state;
    }
}


static void initialize_previous_is_pwm_output_on_state(MotorNumber motor_number)
{
    bool current_is_pwm_output_on_state = is_pwm_output_on(motor_number);

    publish_is_pwm_output_on(motor_number, current_is_pwm_output_on_state);
    motors_data[motor_number].previous_is_pwm_output_on_state = current_is_pwm_output_on_state;
}


void initialize_previous_is_pwm_output_on_state_for_all_motors()
{
    initialize_previous_is_pwm_output_on_state(MOTOR0);
    initialize_previous_is_pwm_output_on_state(MOTOR1);
    initialize_previous_is_pwm_output_on_state(MOTOR2);
    initialize_previous_is_pwm_output_on_state(MOTOR3);
    initialize_previous_is_pwm_output_on_state(MOTOR4);
    initialize_previous_is_pwm_output_on_state(MOTOR5);
    initialize_previous_is_pwm_output_on_state(MOTOR6);
    initialize_previous_is_pwm_output_on_state(MOTOR7);
}