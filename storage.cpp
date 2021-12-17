#include <cstdio>

#include "storage.hpp"

Storage::Storage(const char* storage_path) : command(0), index(-1), storage_size(0) {
	file = std::fopen(storage_path, "r+");
	if (!file) {
		file = std::fopen(storage_path, "w+");
	}
	storage_size = get_storage_size();
}

Storage::~Storage() {
	std::fclose(file);
}

Data Storage::get_storage_size() {
	send(StorageCommand::read, -1);
	return get();
}

Data Storage::get() {
	switch (command) {
	case StorageCommand::read: {
		if (index < -1 || index >= storage_size) {
			return StorageErr::index_out_of_bounds;
		}
		if (std::fseek(file, (index + 1) * (HEX_SIZE + 1), SEEK_SET)) {
			return StorageErr::seek_err;
		}
		Data num = 0;
		if (std::fscanf(file, FSCANF_ARG.c_str(), &num) == EOF) {
			return StorageErr::read_err;
		}
		return num;
	}
	case StorageCommand::write: {
		if (index < -1 || index >= storage_size) {
			return StorageErr::index_out_of_bounds;
		}
		if (std::fseek(file, (index + 1) * (HEX_SIZE + 1), SEEK_SET)) {
			return StorageErr::seek_err;
		}
		Data num = 0;
		if (std::fprintf(file, FSCANF_ARG.c_str(), write_data) < 0) {
			return StorageErr::write_err;
		}
		return EXIT_SUCCESS;
	}
	case StorageCommand::info:
		return storage_size;
	default:
		return StorageErr::read_err;
	}
}

void Storage::send(Data d1, Data d2, Data d3) {
	command = d1;
	index = d2;
	write_data = d3;
}