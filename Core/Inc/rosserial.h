/*
 * rosserial.h
 *
 *  Created on: 2018/01/17
 *      Author: yoneken
 */
#include "ros.h"
#include <string>
#include <sstream>
#include "Datatype/dynamics.h"
#include "controller.h"
#include "std_msgs/Float64MultiArray.h"


#ifndef MAINPP_H_
#define MAINPP_H_

#ifdef __cplusplus
extern "C"
{
#endif
    void rosserial_init();
    //void rosserial_update(Dynamics&);
    void rosserial_publish(float q_w, float q_x, float q_y, float q_z);
    void rosserial_subscribe();
    //void loop_sub(void);

#ifdef __cplusplus
}
#endif

#endif /* MAINPP_H_ */

/*
#ifndef ROSSERIAL_H
#define ROSSERIAL_H
class Rosserial{

    public:
        std_msgs::Float64 depth_msg;
        float yaw_angle;
        float ex[3];
        ros::NodeHandle nh;
        //ros::Publisher depth_pub;
        ros::Subscriber<std_msgs::Float64> yaw_sub;
        //ros::Subscriber<geometry_msgs::Vector3> ex_sub;
        
        Rosserial();
        void yaw_callback(const std_msgs::Float64&);
        void ex_callback(const geometry_msgs::Vector3&);

        void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
        void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
};

#endif

*/