#include "Main/dvl_reader.h"

Dvl_reader::Dvl_reader()
{
	velX = 0;
	velY = 0;
	velZ1 = 0;
	velZ2 = 0;
	index = -1;
	delim = 165;
	size_of_data = 222;
	for(int i = 0; i<size_of_data; i++)
		input[i] = 0;
}

void Dvl_reader::filling()
{
	index++;
	input[index] = receieve_char;
	if(index == (size_of_data - 1))
		update();
}
float Dvl_reader::convert(int start_byte)
{
	float f;
	for(int i=0;i<4;i++)
		((char*)&f)[i] = input[start_byte+i];
	return f;	
}

void  Dvl_reader::update()
{
	velX = convert(start_of_velX);
	velY = convert(start_of_velY);
	velZ1 = convert(start_of_velZ1);
	velZ2 = convert(start_of_velZ2);
	index = -1;
}	