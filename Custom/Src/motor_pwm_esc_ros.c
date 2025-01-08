#include "motor_pwm_esc_ros.h"

#include "motor_pwm_esc_driver.h"

#include <std_msgs/msg/bool.h>


const unsigned int MOTOR_PWM_ESC_NUM_HANDLES = 1;


rcl_publisher_t is_pwm_output_on_publishers[8];
static rcl_timer_t motor_pwm_esc_timer;


static void initialize_is_pwm_output_on_publisher_for_motor_number(MotorNumber motor_number, const char *topic_name, rcl_node_t *stm32_node)
{
    rclc_publisher_init_default(
        &(is_pwm_output_on_publishers[motor_number]),
        stm32_node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        topic_name
    );
}


static void initialize_is_pwm_output_on_publisher_for_all_motors(rcl_node_t *stm32_node)
{
    initialize_is_pwm_output_on_publisher_for_motor_number(MOTOR0, "motor_0/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher_for_motor_number(MOTOR1, "motor_1/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher_for_motor_number(MOTOR2, "motor_2/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher_for_motor_number(MOTOR3, "motor_3/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher_for_motor_number(MOTOR4, "motor_4/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher_for_motor_number(MOTOR5, "motor_5/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher_for_motor_number(MOTOR6, "motor_6/is_pwm_output_on", stm32_node);
    initialize_is_pwm_output_on_publisher_for_motor_number(MOTOR7, "motor_7/is_pwm_output_on", stm32_node);
}


static void motor_pwm_esc_timer_callback(rcl_timer_t *, int64_t)
{
}


static void initialize_motor_pwm_esc_timer(rclc_support_t *support, rclc_executor_t *executor)
{
    motor_pwm_esc_timer = rcl_get_zero_initialized_timer();
    const unsigned int motor_pwm_esc_timer_timeout = 100;
    rcl_ret_t rc = rclc_timer_init_default(
        &motor_pwm_esc_timer,
        support,
        RCL_MS_TO_NS(motor_pwm_esc_timer_timeout),
        motor_pwm_esc_timer_callback
    );
    if (rc != RCL_RET_OK) {
        printf("Error in rcl_timer_init_default.\n");
    } else {
        printf("Created timer with timeout %d ms.\n", motor_pwm_esc_timer_timeout);
    }

    rclc_executor_add_timer(executor, &motor_pwm_esc_timer);
    if (rc != RCL_RET_OK) {
        printf("Error in rclc_executor_add_timer.\n");
    }
}


void initialize_motor_pwm_esc_ros(rcl_node_t *stm32_node, rclc_support_t *support, rclc_executor_t *executor)
{
    initialize_is_pwm_output_on_publisher_for_all_motors(stm32_node);
    initialize_motor_pwm_esc_timer(support, executor);
}
