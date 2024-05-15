#ifndef KILLSWITCH_H
#define KILLSWITCH_H

#include "gpio.h"

class KillSwitch
{
    private:
        bool operate;

    public:
        KillSwitch();
        ~KillSwitch();
        void read_state();
        bool get_state();

};

#endif