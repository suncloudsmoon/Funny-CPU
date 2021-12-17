#ifndef CPU_HPP
#define CPU_HPP

#include "bus.hpp"

constexpr Port MEMORY_PORT{ 1 };
constexpr Port BIOS_PORT{ 2 };

namespace Opcode {
	enum OPCODES {
		nop = 0,
		add = 1,
		sub = 2,
		div = 3,
		mul = 4,

		jmp = 5,
		cmp = 6,
		je = 7,
		jl = 8,
		jg = 9,
		jle = 10,
		jge = 11
	};
};

namespace Reg {
	enum REGISTERS {
		nop = 0,
		a = 1,
		b = 2,
		c = 3,
		d = 4,

		x = 5,
		e = 6
	};
};

namespace RegType {
	enum REG_TYPES {
		num = 0,
		cpu = 1
	};
};

namespace ErrorType {
	enum ERROR_TYPE {
		opcode_not_found = -1,
		stack_pointer_out_of_bounds = -2
	};
};

class CPU {
public:
	CPU(const Bus &bus);
	void start();
protected:
	// BIOS Functions
	Data get_bios_size();
	Data get_bios_data(Data index);

	// Memory Functions
	Data get_mem_size();
	Data read_mem_at(Data index);
	void write_mem_to(Data index, Data d);

	Data& get_reg(Data reg2, Data reg3);
	Data& get_cpu_reg(Data cpu_reg);
	/*
	* Returns true if the execute_instruction wants to jmp
	*/
	bool execute_instruction(Data opcode, Data reg1, Data reg_type, Data reg2);

	Bus data_bus;
	Data stackptr;
	Data nop, a, b, c, d;
	Data x, e; // error reg
private:
	Data treat_num_as_reg;
};

#endif /* CPU_HPP */