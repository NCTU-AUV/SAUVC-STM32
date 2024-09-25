#ifndef Dvl_reader_H
#define Dvl_reader_H

#include <stdint.h>

const int start_of_velX = 142;
const int start_of_velY = 146;
const int start_of_velZ1 = 150;
const int start_of_velZ2 = 154;

class Dvl_reader
{
	float velX,velY,velZ1,velZ2;
	int index;
    char delim;
	int size_of_data; //how mant data
	float convert(int );	

public:
	Dvl_reader();
	uint8_t receieve_char;
	uint8_t input[222];	//the raw input data will be stored here and waited to be converted
	void filling(); //put receieve_char into input[] 
	void update();
    char get_delim(){return delim;}
	float get_x(){return velX;}
	float get_y(){return velY;}
	float get_z1(){return velZ1;}
	float get_z2(){return velZ2;}
};

#endif