#include <fstream>

#include "assembler.hpp"
#include "storage.hpp"
#include "storageutil.hpp"

#ifndef NDEBUG
#include <iostream>
#endif

// Logs errors to console by using try catch()
void Assembler::assemble(const char* input_path, const char *output_path) {
	std::vector<Data> all_data;
	std::ifstream input(input_path);

	std::string line;
	// std::getline() returns number of characters read
	while (std::getline(input, line)) {
		if (line.empty() || line.starts_with(R"(//)"))
			continue;
		auto str_list = split_instruction(line);
		auto num_list = instructions_to_num(str_list);
		
		all_data.insert(all_data.end(), num_list.begin(), num_list.end());
	}
	storeutil::create_empty_storage_device(output_path, HEX_SIZE, all_data.size());
	
	Storage s(output_path);
	Data index = 0;
	for (const auto& d : all_data) {
		s.send(StorageCommand::write, index, d);
		s.get();
		index++;
	}
}

std::vector<Data> Assembler::instructions_to_num(std::queue<std::string>& instructions) {
	std::vector<Data> num_list;
	
	Data opcode = opcode_map.at(instructions.front());
	num_list.push_back(opcode);
	
	instructions.pop();
	Data reg1 = reg_map.at(instructions.front());
	num_list.push_back(reg1);

	instructions.pop();
	if (reg_map.find(instructions.front()) == reg_map.end()) {
		num_list.push_back(RegType::num);
		num_list.push_back(std::stoi(instructions.front()));
	}
	else {
		num_list.push_back(RegType::cpu);
		num_list.push_back(reg_map.at(instructions.front()));
	}
	return num_list;
}

std::queue<std::string> Assembler::split_instruction(const std::string& line) {
	std::queue<std::string> split_list;
	
	// First String
	std::size_t pos = 0;
	std::size_t new_pos = line.find(" ", pos);
	if (new_pos == std::string::npos) {
		throw std::runtime_error("Unable to parse assembly string: " + line);
	}
	split_list.push(line.substr(pos, new_pos - pos));
	
	// Second String
	pos = new_pos + 1;
	new_pos = line.find(",", pos);
	if (new_pos == std::string::npos) {
		throw std::runtime_error("Unable to parse assembly string: " + line);
	}
	split_list.push(line.substr(pos, new_pos - pos));

	// Third String
	pos = new_pos + 1;
	new_pos = line.size();
	split_list.push(line.substr(pos, new_pos - pos));

	return split_list;
}
