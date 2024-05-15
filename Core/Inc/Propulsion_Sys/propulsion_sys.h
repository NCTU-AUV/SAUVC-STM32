#ifndef PROPULSION_SYS
#define PROPULSION_SYS

#include "stm32f4xx.h"
#include "t200.h"
#include "Datatype/dynamics.h"

class Propulsion_Sys
{
private:
    float thrust[8];
public:
    T200 motor[8];
    Propulsion_Sys(TIM_HandleTypeDef *tim1, TIM_HandleTypeDef *tim2);
    ~Propulsion_Sys();

    void allocate(const Kinematics &ctrl_input);
    void resetAll();
    void checkAll();
    void checkInOrder();
};

#endif