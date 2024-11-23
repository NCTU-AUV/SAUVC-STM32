#include "kill_switch_driver.h"

#include "stm32f4xx_hal.h"

GPIO_TypeDef *const KILL_SWITCH_GPIOX = GPIOE;
const uint16_t KILL_SWITCH_GPIO_PIN = GPIO_PIN_2;
const GPIO_PinState KILL_SWITCH_CLOSED_STATE = GPIO_PIN_SET;

bool is_kill_switch_closed()
{
    return HAL_GPIO_ReadPin(KILL_SWITCH_GPIOX, KILL_SWITCH_GPIO_PIN) == KILL_SWITCH_CLOSED_STATE;
}