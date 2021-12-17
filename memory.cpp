#include <cassert>

#include "memory.hpp"

Memory::Memory(UData size) : mem_size(size), index(0), command(0) {
	mem = new Data[static_cast<unsigned int>(size)]();
}

Memory::~Memory() {
	delete[] mem;
}

Data Memory::get() {
	switch (command) {
	case MemCommand::read:
		if (index >= mem_size)
			return -1;
		return mem[index];
	case MemCommand::info:
		return mem_size;
	default:
		return -1;
	}
}

void Memory::send(Data d1, Data d2, Data d3) {
	command = d1;
	switch (d1) {
	case MemCommand::read:
		index = static_cast<UData>(d2);
		break;
	case MemCommand::write:
		if (static_cast<UData>(d2) >= mem_size)
			return;
		mem[d2] = d3;
		break;
	default:
		break;
	}
}
