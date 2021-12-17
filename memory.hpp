#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>

#include "peripheral.hpp"
#include "bus.hpp"

using UData = Port;

namespace MemCommand {
	enum MEMORY_SETTINGS {
		read = 1,
		write = 2,
		info = 3
	};
}


/*
* Provides Volatile RAM (VRAM) to the CPU.
* 
* Inputs
* send()
* - d1 is command
* - d2 and d3 specify the parameters
* 
* Outputs
* get()
* - returns Data held by peripheral
*/
class Memory : public Peripheral {
public:
	Memory(UData size);
	~Memory();
	Data get() override;
	void send(Data d1 = 0, Data d2 = 0, Data d3 = 0) override;
private:
	UData mem_size;
	UData index;
	Data command;
	Data* mem;
};

#endif /* MEMORY_HPP */