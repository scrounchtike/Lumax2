
#ifndef PIPE_HPP
#define PIPE_HPP

#include "../../math.hpp"

struct PipeSection{
	unsigned int liters;
	PipeSection* next;
	PipeSection* previous;
	char numNextConnections;
	
	PipeSection(){
		next = previous = nullptr;
	}
};

class Pipe{
public:
	Pipe();

	void update();
private:
	PipeSection start;
};

#endif
