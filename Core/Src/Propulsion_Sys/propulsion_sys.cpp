#include "Propulsion_Sys/propulsion_sys.h"


extern int operate;

Propulsion_Sys::Propulsion_Sys(TIM_HandleTypeDef *tim1, TIM_HandleTypeDef *tim2) //setting timer during construction
{
    //timer2
    motor[0].set(tim1, TIM_CHANNEL_1);
    motor[1].set(tim1, TIM_CHANNEL_2);
    motor[2].set(tim1, TIM_CHANNEL_3);
    motor[3].set(tim1, TIM_CHANNEL_4);

    //timer8
    motor[4].set(tim2, TIM_CHANNEL_1);
    motor[5].set(tim2, TIM_CHANNEL_2);
    motor[6].set(tim2, TIM_CHANNEL_3);
    motor[7].set(tim2, TIM_CHANNEL_4);
    for (int i = 0; i < 8; i++)
        motor[i].output(0);
}

Propulsion_Sys::~Propulsion_Sys()
{
    for (int i = 0; i < 8; i++)
        motor[i].output(0);
}

/**
 * @brief Set motor output
 * @param 
 * @retval none
 */

void Propulsion_Sys::resetAll(){
    
    // reset or  all the motors
    for(int i=0; i < 8; i++)
        motor[i].output(0); // unit: N
}
void Propulsion_Sys::checkAll(){

    //Testing all the motors simultaneously
    for(int i=0; i < 8; i++)
        motor[i].output(operate * 0.07); // unit: N
}
void Propulsion_Sys::checkInOrder(){
    
    //Testing the motors sequentially according to their numbers
    // unit: N   
    for (int i = 0; i < 8; i++){
        for(int k = 0; k < 8; k++)
            motor[k].output(operate * 0);
        if(!operate)
            break;
        for(int k = 0; k < 8; k++){
            if (k == i)
                motor[k].output(operate * 0.07);
            else
                motor[k].output(operate * 0);
        }
        HAL_Delay(800);
    }
}


/**
 * @brief allocate control_input(6D force and moment) .to each motor thrust then output
 * @param ctrl_input Kinematics class
 * @retval none
 */ 
void Propulsion_Sys::allocate(const Kinematics &ctrl_input)
{
    
    
    thrust[0] = ctrl_input.linear.x  * 0 + ctrl_input.linear.y  *  0 + ctrl_input.linear.z  * 0.25 + 
                ctrl_input.angular.x * -1.0549 + ctrl_input.angular.y * -1.6667 + ctrl_input.angular.z * 0;

    thrust[1] = ctrl_input.linear.x  * 0 + ctrl_input.linear.y  * 0 + ctrl_input.linear.z * 0.25 + 
                ctrl_input.angular.x *  1.0549 + ctrl_input.angular.y * -1.6667 + ctrl_input.angular.z * 0;

    thrust[2] = ctrl_input.linear.x  *  0 + ctrl_input.linear.y  *  0 + ctrl_input.linear.z * 0.25 +
                ctrl_input.angular.x * -1.0549 + ctrl_input.angular.y *  1.6667 + ctrl_input.angular.z * 0;

    thrust[3] = ctrl_input.linear.x  *  0 + ctrl_input.linear.y  * 0 + ctrl_input.linear.z * 0.25 +
                ctrl_input.angular.x *  1.0549 + ctrl_input.angular.y *  1.6667 + ctrl_input.angular.z * 0;

    thrust[4] = ctrl_input.linear.x  * 0.3536 + ctrl_input.linear.y   *  0.3536 + ctrl_input.linear.z * 0 +
                ctrl_input.angular.x * 0      + ctrl_input.angular.y  *  0      + ctrl_input.angular.z * 0.6939;

    thrust[5] = ctrl_input.linear.x  * 0.3536 + ctrl_input.linear.y   * -0.3536 + ctrl_input.linear.z * 0 +
                ctrl_input.angular.x * 0      + ctrl_input.angular.y  *  0      + ctrl_input.angular.z * -0.6939;

    thrust[6] = ctrl_input.linear.x  * 0.3536 + ctrl_input.linear.y   * -0.3536 + ctrl_input.linear.z * 0 +
                ctrl_input.angular.x * 0      + ctrl_input.angular.y  *  0      + ctrl_input.angular.z *  0.6939;

    thrust[7] = ctrl_input.linear.x  * 0.3536 + ctrl_input.linear.y   *  0.3536 + ctrl_input.linear.z * 0 +
                ctrl_input.angular.x * 0      + ctrl_input.angular.y  *  0      + ctrl_input.angular.z * -0.6939;
        
    //Adjest each motor
    thrust[0] *= 1;
    thrust[1] *= 1;
    thrust[2] *= 1;
    thrust[3] *= 1;
    thrust[4] *= 1;
    thrust[5] *= 1;
    thrust[6] *= 1;
    thrust[7] *= 1;
    
    //Reverse direction
    thrust[0] *= -1;
    thrust[3] *= -1;
    thrust[4] *= -1;
    thrust[7] *= -1;
    
    //output thurst   
    for(int i=0; i < 8; i++)
        motor[i].output(operate*thrust[0]);  // unit: N

    /** Test Code **/     
    //resetAll();
    //void checkAll();
    //void checkInOrder();
}








