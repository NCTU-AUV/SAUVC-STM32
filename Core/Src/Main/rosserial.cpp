/*
 * main.cpp

 *
 *  Created on: 2018/01/17
 *      Author: yoneken
 */
#include "Main/rosserial.h"
#include "std_msgs/Float32MultiArray.h"

// ros variables
ros::NodeHandle nh;
std_msgs::Float32MultiArray pub_msg;

// parameters to "main.cpp"
geometry::Vector ex;     // position error
geometry::Vector ev = {0 , 0, 0};     // velocity error
geometry::Vector eR;     // angle    error  
geometry::Vector eOmega = {0 , 0, 0}; // angular velocity error 

// other variables to "main.cpp"
int Servo_State = 0;
int CurrentMotor_State = 0;
float desired_depth = 0;

/* ----subscriber parameters
- 0-2:   error angle x,y,z
- 3-4:   ex          x,y
- 5:     desire depth
- 6:     robot arm state

*/


// callback for "rpi_to_stm32"
void callback(const std_msgs::Float32MultiArray& msg){
  

  // error data for angle: row, yaw, pitch ; position: x, y 
  eR.x = msg.data[0];
  eR.y = msg.data[1];
  eR.z = msg.data[2];

  ex.x = msg.data[3];
  ex.y = msg.data[4];
  
  // desired depth underwater
  desired_depth = msg.data[5];

  // operation info for the arm motors controlling 
  Servo_State = msg.data[6];
  CurrentMotor_State = msg.data[7];
  
}

// declaration
ros::Publisher pub("stm32_to_rpi", &pub_msg);
ros::Subscriber<std_msgs::Float32MultiArray> sub("rpi_to_stm32", callback);



// subscribe topic: "rpi_to_stm32" 
void rosserial_subscribe(){
    nh.spinOnce();
}


// publish topic: "stm32_to_rpi"
void rosserial_publish(float data0, float data1, float data2, float data3){
  
  // publish data
  // check quaternion
  pub_msg.data_length = 4;
  float array[4] = {};
  
  array[0] = data0;
  array[1] = data1;
  array[2] = data2;
  array[3] = data3;
  
  pub_msg.data = array;
  pub.publish(&pub_msg);
  nh.spinOnce();
}

void rosserial_init()
{
  
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(pub);
  
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->flush();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->reset_rbuf();
}


// Create class of rosserial
