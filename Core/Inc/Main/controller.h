#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Datatype/dynamics.h"

const float weight = 24.3;
const float buoyancy = 25.35;

class Controller
{
private:
    geometry::Vector Kx;
    geometry::Vector Kv;
    geometry::Vector KR;
    geometry::Vector KOmega;

public:
    Controller(geometry::Vector Kx, geometry::Vector Kv, geometry::Vector KR, geometry::Vector KOmega);
    ~Controller();

    Kinematics update(const geometry::Vector &ex, const geometry::Vector &ev,
                      const geometry::Vector &eR, const geometry::Vector &eOmega);
};

#endif