#include "robot_arm.h"

Robot_Arm::Robot_Arm()
{
    CurrentMotor_PWM = 0;
}

Robot_Arm::Robot_Arm(TIM_HandleTypeDef *t, const int CurrentMotor_PWM):CurrentMotor_PWM(CurrentMotor_PWM)
{
    motor[0].set(t, TIM_CHANNEL_1);  // For current motor 
    motor[1].set(t, TIM_CHANNEL_2);  // For Servo motor 1
    motor[2].set(t, TIM_CHANNEL_3);  // For Servo motor 2
    motor[3].set(t, TIM_CHANNEL_4);  // For Servo motor 3

    // setting for 1 current motor & 3 servo motors
    this->CurrentMotor_SetMode(0);
    this->Servo_SetAngle(1, 0);
    this->Servo_SetAngle(2, 0);
    this->Servo_SetAngle(3, 0);
}

Robot_Arm::~Robot_Arm()
{
    this->CurrentMotor_SetMode(0);
}


/**
 * @brief a function to move joint to desired angle
 * @param angle angle in degree from -90 to 90
 * @retval none
 */

void Robot_Arm::Servo_SetAngle(const int &num, const int &angle) // unit: degree
{
    motor[num].output(angle * 100 / 9 + 1500); //unit: PWM (ms)
}
void Robot_Arm::Servo_Test(const int &num){
    // Servo motor test   
    // state: 0 -> 90 -> 0 -> -90 -> 0
    this->Servo_SetAngle(num, 0);
    HAL_Delay(1000);
    this->Servo_SetAngle(num, 90);
    HAL_Delay(1000);
    this->Servo_SetAngle(num, 0);
    HAL_Delay(1000);
    this->Servo_SetAngle(num, -90);
    HAL_Delay(1000);  
}

//
void Robot_Arm::CurrentMotor_SetMode(const int &mode)
{
    // mode 0 stop 
    //      1 turn CW
    //      2 turn CCW
    //      otherwise short brake
    // PD9 -> AIN2 
    // PD10 -> AIN1
    // PD11 -> STBY 
    // PD12 -> PWMA
    // black -> AO1 
    // red -> A02

    motor[0].output(CurrentMotor_PWM); // controlling current motor with CurrentMotor_PWM 
    // select mode
    if (mode == 0){
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
    }
    else if(mode == 1){
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
    }
    else if(mode == 2){
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
    }
    else{
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
    }
}

void Robot_Arm::CurrentMotor_Test(){
    
    this->CurrentMotor_SetMode(1); // turn CW
    HAL_Delay(5000);
    this->CurrentMotor_SetMode(2); // turn CCW
    HAL_Delay(5000);
    this->CurrentMotor_SetMode(0); // stop
}

void Robot_Arm::CurrentMotor_SetDistance(float distance)
{
    this->reset();

    if(distance > 0)
    {
        this->CurrentMotor_SetMode(1); 
    }
    else
    {
        this->CurrentMotor_SetMode(2);
        distance *= -1;
    }

    int num_pulse = distance * 2.27;
    int cnt = 0;

    int prev_val = 0;
    int hall_val = 0;
    while(1){
        hall_val = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8);
        if(hall_val == 1 && prev_val == 0)
            cnt++;
    
        if(cnt == num_pulse)
        {
            this->CurrentMotor_SetMode(0);
            break;
        }
        prev_val = hall_val;
}
}

void Robot_Arm::reset()
{
    //servo motor
    motor[1].output(0);
    motor[2].output(0);
    motor[3].output(0);
    //current motor
    this->CurrentMotor_SetMode(0);
    int prev_val, current_val;
    current_val = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8);
    prev_val = current_val;

    while(1){
        current_val = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8);
        if(current_val == 0 && prev_val == 1)
            return;
        prev_val = current_val;
    }
}
