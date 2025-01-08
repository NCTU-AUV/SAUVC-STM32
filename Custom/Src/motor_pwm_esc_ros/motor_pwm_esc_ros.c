#include "motor_pwm_esc_ros/motor_pwm_esc_ros.h"

#include "motor_pwm_esc_ros/publish_is_pwm_output_on.h"

#include <std_msgs/msg/bool.h>


const unsigned int MOTOR_PWM_ESC_NUM_HANDLES = 1;


static rcl_timer_t motor_pwm_esc_timer;


static void motor_pwm_esc_timer_callback(rcl_timer_t *, int64_t)
{
    publish_is_pwm_output_on_only_when_updated(MOTOR0);
    publish_is_pwm_output_on_only_when_updated(MOTOR1);
    publish_is_pwm_output_on_only_when_updated(MOTOR2);
    publish_is_pwm_output_on_only_when_updated(MOTOR3);
    publish_is_pwm_output_on_only_when_updated(MOTOR4);
    publish_is_pwm_output_on_only_when_updated(MOTOR5);
    publish_is_pwm_output_on_only_when_updated(MOTOR6);
    publish_is_pwm_output_on_only_when_updated(MOTOR7);
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

    initialize_previous_is_pwm_output_on_state_for_all_motors();
}
