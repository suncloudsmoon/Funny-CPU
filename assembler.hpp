#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include <cstddef>

#include "cpu.hpp"

const std::unordered_map<std::string, int> basic_opcode_map{
	{"nop", Opcode::nop},
	{"add", Opcode::add},
	{"sub", Opcode::sub},
	{"div", Opcode::div},
	{"mul", Opcode::mul},
	{"cmp", Opcode::cmp},
};

const std::unordered_map<std::string, int> jump_opcode_map{
	{"jmp", Opcode::jmp},
	{"je", Opcode::je},
	{"jl", Opcode::jl},
	{"jg", Opcode::jg},
	{"jle", Opcode::jle},
	{"jge", Opcode::jge}
};

const std::unordered_map<std::string, int> reg_map{
	{"a", Reg::a},
	{"b", Reg::b},
	{"c", Reg::c},
	{"d", Reg::d},

	{"x", Reg::x},
	{"e", Reg::e}
};

class Assembler {
public:
	void assemble(const char *input_path, const char* output_path);
private:
	std::vector<Data> instructions_to_num(std::queue<std::string> &instructions);
	std::queue<std::string> split_instruction(const std::string& line);
};

#endif /* ASSEMBLER_HPP */