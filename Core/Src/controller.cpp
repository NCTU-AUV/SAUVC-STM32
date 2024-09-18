#include "controller.h"
#include "math.h"

/**
 * @brief Constructor of Controller
 * @param Kx coefficient for position
 * @param Kv coefficient for velocity 
 * @param KR coefficient for angle
 * @param KOmega coefficient for angular velocity
 */ 
Controller::Controller(geometry::Vector kx, geometry::Vector kv, geometry::Vector kR, geometry::Vector kOmega): Kx(kx), Kv(kv), KR(kR), KOmega(kOmega)
{
}

Controller::~Controller()
{
}

Kinematics Controller::update(const geometry::Vector &ex, const geometry::Vector &ev, 
                        const geometry::Vector &eR, const geometry::Vector &eOmega)
{
    Kinematics control_input;

    /******************************/
    /** calculate control inputs **/
    /******************************/

    // using the y Component to adjust ctrl_input.linear.x
    control_input.linear.x = Kx.x * ex.x + Kv.x * ev.x - 0.099 * abs(ex.y); // unit: N
    control_input.linear.y = Kx.y * ex.y + Kv.y * ev.y;                     // unit: N
    control_input.linear.z = Kx.z * ex.z + Kv.z * ev.z - weight + buoyancy; // unit: N

    control_input.angular.x = KR.x * eR.x + KOmega.x * eOmega.x;            // unit: N*m
    control_input.angular.y = KR.y * eR.y + KOmega.y * eOmega.y;            // unit: N*m

    //Distribute the eR range
    if((eR.z > 90) || (eR.z < -90)){          // yaw(degree): 90 ~ 180 or -90 ~ -180
        //decide direction
        if(eR.z > 0) 
            control_input.angular.z = 0.2;
        else 
            control_input.angular.z = -0.2;
        control_input.linear.x = 0;
        control_input.linear.y = 0;
    }  
    else if((eR.z > 45) || (eR.z < -45)){     // yaw(degree): 45 ~ 90 or -45 ~ -90
        //decide direction
        if(eR.z > 0)
            control_input.angular.z = 0.1;
        else 
            control_input.angular.z = -0.1;
        control_input.linear.x = 0;
        control_input.linear.y = 0;
    }
    else if((eR.z > 5) || (eR.z < -5)){       // yaw(degree): 5 ~ 45 or -5 ~ -45
        //decide direction
        if(eR.z > 0)
            control_input.angular.z = 0.07;
        else 
            control_input.angular.z = -0.07;
        control_input.linear.x = 0;
        control_input.linear.y = 0;
    }
    else if((eR.z < 5) || (eR.z > -5))        // yaw(degree): -5 ~ 5
        control_input.angular.z = KR.z * eR.z + KOmega.z * eOmega.z;

    return control_input;
}