#include <fstream>
#include <cstddef>
#include <cstdlib>

#include "assembler.hpp"
#include "storage.hpp"
#include "storageutil.hpp"

#ifndef NDEBUG
#include <iostream>
#endif

namespace fa {
	// Logs errors to console by using try catch()
	void Assembler::assemble(const char* input_path, const char* output_path) {
		std::vector<Data> all_data;
		std::size_t line_index = 0;

		std::ifstream input(input_path);
		std::string line;
		// std::getline() returns number of characters read
		while (std::getline(input, line)) {
			if (line.empty() || line.starts_with(R"(//)"))
				continue;
			if (line.ends_with(":")) {
				jump_table.insert(parse_jump_label(line, line_index));
				continue; // jumps don't count as a line number
			}
			auto str_list = split_instruction(line);
			auto num_list = instructions_to_num(str_list);

			all_data.insert(all_data.end(), num_list.begin(), num_list.end());
			line_index++;
		}
		// Create output asm file
		storeutil::create_empty_storage_device(output_path, HEX_SIZE, all_data.size());
		Storage s(output_path);

		Data index = 0;
		for (const auto& d : all_data) {
			s.send(StorageCommand::write, index, d);
			s.get();
			index++;
		}
	}

	void Assembler::display_asm_error(const std::string &error_message) {
		std::cerr << error_message << std::endl;
		std::exit(-1);
	}

	std::pair<std::string, Data> Assembler::parse_jump_label(const std::string& line, std::size_t line_num) {
		std::size_t colon_index = line.find(":");

		if (colon_index == std::string::npos)
			display_asm_error("Unable to find \':\' in the string \"" + line + "\"!");

		std::string case_label = line.substr(0, colon_index);
		return std::pair<std::string, Data>(case_label, line_num);
	}

	std::vector<Data> Assembler::instructions_to_num(std::queue<std::string>& instructions) {
		std::vector<Data> num_list;
		Data opcode = 0, reg1 = 0, reg_type = 0, reg2 = 0;

		if (basic_opcode_map.find(instructions.front()) != basic_opcode_map.end()) {
			opcode = basic_opcode_map.at(instructions.front());
			instructions.pop();

			if (reg_map.find(instructions.front()) != reg_map.end()) {
				reg1 = reg_map.at(instructions.front());
			}
			else {
				reg1 = std::stoi(instructions.front());
			}
			instructions.pop();

			if (reg_map.find(instructions.front()) == reg_map.end()) {
				reg_type = RegType::num;
				reg2 = std::stoi(instructions.front());
			}
			else {
				reg_type = RegType::cpu;
				reg2 = reg_map.at(instructions.front());
			}
		}
		else if (jump_opcode_map.find(instructions.front()) != jump_opcode_map.end()) {
			Data line_num = 0;
			
			opcode = jump_opcode_map.at(instructions.front());
			instructions.pop();

			auto case_label = instructions.front();
			try {
				line_num = jump_table.at(case_label);
			}
			catch (std::out_of_range& e) {
				display_asm_error("Unable to find case_label - " + case_label + "(" + e.what() + ")!");
			}
		
			reg1 = Reg::nop;
			reg_type = RegType::num;
			reg2 = line_num * 4;
		}
		// Adding stuff to num_list
		num_list.push_back(opcode);
		num_list.push_back(reg1);
		num_list.push_back(reg_type);
		num_list.push_back(reg2);

		return num_list;
	}

	std::queue<std::string> Assembler::split_instruction(const std::string& line) {
		std::queue<std::string> split_list;
		std::size_t pos = 0, new_pos = 0;

		// First String
		if ((new_pos = line.find(" ")) == std::string::npos) {
			display_asm_error("Unable to parse assembly string: " + line);
		}
		split_list.push(line.substr(pos, new_pos - pos));
		pos = new_pos + 1;

		if ((new_pos = line.find(",")) == std::string::npos) {
			split_list.push(line.substr(pos));
			split_list.push(reg_map.begin()->first);
		}
		else {
			// Second String
			split_list.push(line.substr(pos, new_pos - pos));

			// Third String
			pos = new_pos + 1;
			split_list.push(line.substr(pos));
		}
		return split_list;
	}
}
