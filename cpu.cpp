#include "cpu.hpp"
#include "memory.hpp"
#include "storage.hpp"

#ifndef NDEBUG
#include <iostream>
#endif

CPU::CPU(const Bus& bus) : data_bus(bus), a(0), b(0), c(0), d(0), e(0), treat_num_as_reg(0) {}

void CPU::start() {
	Data mem_size = get_mem_size();
	Data bios_size = get_bios_size();
	
	// Get BIOS Data
	for (Data i = 0; (i < bios_size) && (i < mem_size); i++) {
		write_mem_to(i, get_bios_data(i));
	}

	// Run BIOS
	Data stackptr = 0;
	while (stackptr < mem_size) {
		Data opcode = read_mem_at(stackptr);
		Data reg1 = read_mem_at(stackptr + 1);
		Data reg_type = read_mem_at(stackptr + 2);
		Data reg2 = read_mem_at(stackptr + 3);

		execute_instruction(opcode, reg1, reg_type, reg2);

		stackptr += 4;
	}

#ifndef NDEBUG
	std::cout << "Registers [a,b,c,d,e]: " << a << ", " << b << ", " << c << ", " << d << ", " << e << std::endl;
#endif
}

// BIOS Functions

Data CPU::get_bios_size() {
	data_bus.send(BIOS_PORT, StorageCommand::info);
	return data_bus.get(BIOS_PORT);
}

Data CPU::get_bios_data(Data index) {
	data_bus.send(BIOS_PORT, StorageCommand::read, index);
	 return data_bus.get(BIOS_PORT);
}

// Memory Functions

Data CPU::get_mem_size() {
	data_bus.send(MEMORY_PORT, MemCommand::info);
	return data_bus.get(MEMORY_PORT);
}

Data CPU::read_mem_at(Data index) {
	data_bus.send(MEMORY_PORT, MemCommand::read, index);
	return data_bus.get(MEMORY_PORT);
}

void CPU::write_mem_to(Data index, Data d) {
	data_bus.send(MEMORY_PORT, MemCommand::write, index, d);
}

Data& CPU::get_reg(Data reg_type, Data d) {
	switch (reg_type) {
	case RegType::num:
		treat_num_as_reg = d;
		return treat_num_as_reg;
	case RegType::cpu:
		return get_cpu_reg(d);
	default:
		return treat_num_as_reg;
	}
}

Data& CPU::get_cpu_reg(Data cpu_reg) {
	switch (cpu_reg) {
	case Reg::a:
		return a;
	case Reg::b:
		return b;
	case Reg::c:
		return c;
	case Reg::d:
		return d;
	case Reg::e:
		return e;
	default:
		return treat_num_as_reg;
	}
}

void CPU::execute_instruction(Data opcode, Data reg1, Data reg_type, Data reg2) {
	Data& first = get_cpu_reg(reg1);
	Data& second = get_reg(reg_type, reg2);
	switch (opcode) {
	case Opcode::nop:
		break;
	case Opcode::add:
		first += second;
		break;
	case Opcode::sub:
		first -= second;
		break;
	case Opcode::div:
		first /= second;
		break;
	case Opcode::mul:
		first *= second;
		break;
	default:
		e = ErrorType::opcode_not_found;
		break;
	}
}
