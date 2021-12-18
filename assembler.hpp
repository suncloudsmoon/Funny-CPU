#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <stdexcept>

#include "cpu.hpp"

namespace fa {
	const std::unordered_map<std::string, int> basic_opcode_map{
		{"nop", Opcode::nop},
		{"add", Opcode::add},
		{"sub", Opcode::sub},
		{"div", Opcode::div},
		{"mul", Opcode::mul},
		{"cmp", Opcode::cmp},

		{"mov", Opcode::mov},
	};

	const std::unordered_map<std::string, int> jump_opcode_map{
		{"jmp", Opcode::jmp},
		{"je", Opcode::je},
		{"jl", Opcode::jl},
		{"jg", Opcode::jg},
		{"jle", Opcode::jle},
		{"jge", Opcode::jge}
	};

	const std::unordered_map<std::string, int> io_opcode_map{
		{"in", Opcode::in},
		{"out", Opcode::out}
	};

	const std::unordered_map<std::string, int> reg_map{
		{"a", Reg::a},
		{"b", Reg::b},
		{"c", Reg::c},
		{"d", Reg::d},

		{"ioa", Reg::ioa},
		{"iob", Reg::iob},
		{"ioc", Reg::ioc},
		{"iod", Reg::iod},

		{"x", Reg::x},
		{"e", Reg::e}
	};

	class Assembler {
	public:
		void assemble(const char* input_path, const char* output_path);
		void display_asm_error(const std::string& error_message);
	private:
		std::pair<std::string, Data> parse_jump_label(const std::string& line, std::size_t line_num);
		std::vector<Data> instructions_to_num(std::queue<std::string>& instructions);
		/*
		* line needs to be free if any trailing spaces, tabs, etc.
		*/
		std::queue<std::string> split_instruction(const std::string& line);

		std::unordered_map<std::string, Data> jump_table;
	};
}



#endif /* ASSEMBLER_HPP */