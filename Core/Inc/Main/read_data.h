#include "Datatype/vector.h"
#include "cstdint"

class Read_data
{
    int index; //which element of std::vector
    int total_byte;
    int size_of_data;
    uint8_t ch;
    bool accessible;
    float depth;
    float yaw;
    geometry::Vector v;
    float velocity[3];
    float joint[3];

public:
    Read_data();
    uint8_t receieved_data[45];
    float get_single_num();
    void assign_num();
    void receieve();
    int get_size_of_data(){return size_of_data;};

    float get_depth(){return depth;};
    float get_yaw(){return yaw;};
    bool access_ok(){return accessible;};
    void access_init(){accessible = false;};
    geometry::Vector get_geometry_vector(){return v;};
    float get_joint0(){return joint[0];};
    float get_joint1(){return joint[1];};
    float get_joint2(){return joint[2];};
    float get_vel0(){return velocity[0];};
    float get_vel1(){return velocity[1];};
    float get_vel2(){return velocity[2];};
};
