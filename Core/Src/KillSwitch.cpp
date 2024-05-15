#include "KillSwitch.h"

KillSwitch::KillSwitch()
{
    operate = true;
}

KillSwitch::~KillSwitch()
{
    operate = false;
}


void KillSwitch::read_state()
{
    operate = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2);
}


bool KillSwitch::get_state()
{
    return operate;
}