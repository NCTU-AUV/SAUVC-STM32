#ifndef ROBOT_ARM_H
#define ROBOT_ARM_H

#include "motor.h"
#include "gpio.h"

class Robot_Arm
{
private:
    Motor motor[4];
    int CurrentMotor_PWM;

public:
    Robot_Arm();
    Robot_Arm(TIM_HandleTypeDef *t, const int sp);
    ~Robot_Arm();
    void Servo_SetAngle(const int &num, const int &angle);
    void Servo_Test(const int &num);
    void CurrentMotor_SetMode(const int &mode);
    void CurrentMotor_Test();
    void CurrentMotor_SetDistance(float distance);
    void reset();
};

#endif