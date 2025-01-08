#include "motor_pwm_esc_node.h"


static rcl_node_t motor_pwm_esc_node;


void initialize_motor_pwm_esc_node(rclc_support_t *support)
{
    rclc_node_init_default(&motor_pwm_esc_node, "motor_pwm_esc_node", "orca_auv", support);
}
