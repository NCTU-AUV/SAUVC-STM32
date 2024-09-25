#include "app.h"

#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include <stdio.h>
#include "rosserial.h"
#include "Propulsion_Sys/propulsion_sys.h"
#include "Datatype/dynamics.h"
#include "robot_arm.h"
#include "Sensor/mpu9250.h"
#include "dvl_reader.h"
#include "read_data.h"
#include "controller.h"
#include "Sensor/bar02.h"

void Interrupt_Handle();

// robot arm
extern int Servo_State;
extern int CurrentMotor_State;
int AdjustSpeed = 30;  // PWM(microsecond): AdjustSpeed+1500 for current motor

// KillSwitch operate variable
int operate = 1;

//  error data for position: x,y; velocity: x, y, z; 
extern geometry::Vector ex;     // position error
extern geometry::Vector ev;     // velocity error
extern geometry::Vector eR;     // angle    error  
extern geometry::Vector eOmega; // angular velocity error  

double depth = 0;
extern float desired_depth;  //desired depth

//  coefficient K for controller 
geometry::Vector Kx = {0.6, 0.6, 1};
geometry::Vector Kv = {0, 0, 0};
geometry::Vector KR = {0.004, 0.0028, 0.014};
geometry::Vector KOmega = {0, 0, 0};

// other variables
int done = 0;

int app_main(void)
{
  // rosserial communication 
  rosserial_init();
  rosserial_subscribe(); 
  

  // Sensor Initialization
  Mpu9250 imu_no_use;   // the old imu no use 
  imu_no_use.set(&hspi2, GPIOB, GPIO_PIN_12);

  Bar02 depth_sensor;   // pressure senser
  if (!depth_sensor.set(&hi2c1))
      Interrupt_Handle();
  
  // Controller Initialization 
  Controller controller(Kx, Kv, KR, KOmega); 

  // Propulsion system Initialization
  //For Motor Output
  Propulsion_Sys propulsion_sys(&htim2, &htim8);
  
  //Robot Arm
  Robot_Arm arm(&htim4, AdjustSpeed+1500); // unit: PWM (ms)
  //Wait for motor to setup
  HAL_Delay(3000);

  /* Infinite loop */
  while (!done)
  {
    //Depth Sensor
    depth_sensor.read_value(); // calculate the pressure internally
    depth = depth_sensor.depth(); // calculate the depth via offset
    ex.z = desired_depth - depth; // the error to the desired depth
    //ex.z = 0; // (Use when depth sensor stop working)
    
    //Controller
    // input ex, ev, eR, eOmega; output control_input
    // Control inputs for propulsion system
    // force: x, y, z; moment: x, y, z
    Kinematics control_input = controller.update(ex, ev, eR, eOmega);
    
    // KillSwitch read the "operate" value
    // operate 0 stop the motor
    //         1 run  the motor
    operate = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2);

    /**  Underwater Motors control  **/ 
    // Allocate and Output
    propulsion_sys.allocate(control_input);  //T200 Motor Output

    /**  Servo control  **/
    /*if(Servo_State){
      arm.Servo_SetAngle(1,0);   // (number of Servo, unit: degree)
    }
    else{
      arm.Servo_SetAngle(1, 85); // (number of Servo, unit: degree)
    }*/

    /**  CurrentMotor contrl  **/
    //CurrentMotor_State = 1;
    //arm.CurrentMotor_SetMode(CurrentMotor_State);
    arm.CurrentMotor_Test();

    // publisher topic: stm32_to_rpi 
    rosserial_publish(control_input.angular.x, control_input.angular.y, 0, depth);
  }
}

void Interrupt_Handle()
{
  done = 1;
}
