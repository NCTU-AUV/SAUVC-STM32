#include "thruster_pwm_controller/thruster_pwm_controller.h"

#include "thruster_pwm_controller/subscribe_set_pwm_output_on.h"
#include "thruster_pwm_controller/subscribe_set_pwm_output_signal_value.h"
#include "debug_logger.h"

const unsigned int THRUSTER_PWM_CONTROLLER_NUM_HANDLES = 1 + 1;



static void thruster_pwm_controller_timer_callback(rcl_timer_t *, int64_t)
{
}


void thruster_pwm_controller_on_timer_tick(void)
{
    thruster_pwm_controller_timer_callback(NULL, 0);
}


void initialize_orca_stm32_thruster_pwm_controller(rclc_support_t *support, rclc_executor_t *executor, rcl_node_t *orca_stm32_bridge)
{
    if (!initialize_set_pwm_output_on_subscriber(orca_stm32_bridge, executor)) {
        (void)debug_logger_publish("orca_stm32_thruster_pwm_controller: aborting init; failed to set up set_pwm_output_on subscriber\n");
        return;
    }
    if (!initialize_set_pwm_output_signal_value_subscriber(orca_stm32_bridge, executor)) {
        (void)debug_logger_publish("orca_stm32_thruster_pwm_controller: aborting init; failed to set up set_pwm_output_signal_value subscriber\n");
        return;
    }
}
