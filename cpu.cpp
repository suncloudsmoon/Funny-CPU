#include "cpu.hpp"
#include "memory.hpp"
#include "storage.hpp"

#ifndef NDEBUG
#include <iostream>
#endif

CPU::CPU(const Bus& bus) : data_bus(bus), stackptr(0),  
							nop(0), a(0), b(0), c(0), d(0),
							ioa(0), iob(0), ioc(0), iod(0),
							x(0), e(0), treat_num_as_reg(0) {}

void CPU::start() {
	Data mem_size = get_mem_size();
	Data bios_size = get_bios_size();
	
	// Get BIOS Data
	for (Data i = 0; (i < bios_size) && (i < mem_size); i++) {
		write_mem_to(i, get_bios_data(i));
	}

	// Run BIOS
	while (stackptr < mem_size) {
		if (stackptr < 0) {
			e = ErrorType::stack_pointer_out_of_bounds;
			stackptr = 0;
		}
		Data opcode = read_mem_at(stackptr);
		Data reg1 = read_mem_at(stackptr + 1);
		Data reg_type = read_mem_at(stackptr + 2);
		Data reg2 = read_mem_at(stackptr + 3);

		if (!execute_instruction(opcode, reg1, reg_type, reg2))
			stackptr += 4;
	}

#ifndef NDEBUG
	std::cout << "---------------CPU Profile-----------------" << std::endl;
	std::cout << "General-Purpose Registers [a,b,c,d]: " << a << ", " << b << ", " << c << ", " << d << std::endl;
	std::cout << "IO Registers [ioa, iob, ioc, iod]: " << ioa << ", " << iob << ", " << ioc << ", " << iod << std::endl;
	std::cout << "Special Registers [x, e]: " << x << ", " << e << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
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
	case Reg::nop:
		return nop;
	case Reg::a:
		return a;
	case Reg::b:
		return b;
	case Reg::c:
		return c;
	case Reg::d:
		return d;
	case Reg::ioa:
		return ioa;
	case Reg::iob:
		return iob;
	case Reg::ioc:
		return ioc;
	case Reg::iod:
		return iod;
	case Reg::x:
		return x;
	case Reg::e:
		return e;
	default:
		return treat_num_as_reg;
	}
}

bool CPU::execute_instruction(Data opcode, Data reg1, Data reg_type, Data reg2) {
	bool isjmp = false;
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
	case Opcode::jmp:
		stackptr = second;
		isjmp = true;
		break;
	case Opcode::cmp:
		x = first - second;
		break;
	case Opcode::je:
		if (x == 0) {
			stackptr = second;
			isjmp = true;
		}
		break;
	case Opcode::jl:
		if (x < 0) {
			stackptr = second;
			isjmp = true;
		}
		break;
	case Opcode::jg:
		if (x > 0) {
			stackptr = second;
			isjmp = true;
		}	
		break;
	case Opcode::jle:
		if (x <= 0) {
			stackptr = second;
			isjmp = true;
		}
		break;
	case Opcode::jge:
		if (x >= 0) {
			stackptr = second;
			isjmp = true;
		}
		break;
	case Opcode::mov:
		first = second;
		break;
	case Opcode::in:
		iod = data_bus.get(first);
		break;
	case Opcode::out:
		data_bus.send(first, ioa, iob, ioc);
		break;
	default:
		e = ErrorType::opcode_not_found;
		break;
	}
	return isjmp;
}
