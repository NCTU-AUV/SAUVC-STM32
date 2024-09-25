#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f4xx.h"

class Motor
{
private:
    TIM_HandleTypeDef* timer;
    uint32_t channel;

public:
    Motor();
    ~Motor();
    void set(TIM_HandleTypeDef* t, const uint32_t c);
    void output(const int signal);
};

#endif