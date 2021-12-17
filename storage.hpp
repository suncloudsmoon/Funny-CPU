#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <limits>
#include <string>

#include <cstdlib>
#include <cmath>

#include "peripheral.hpp"
#include "bus.hpp"

using UData = Port;

const UData HEX_SIZE = std::ceil<UData>(std::log10<UData>(std::numeric_limits<UData>::max()));
const std::string FSCANF_ARG = "%0" + std::to_string(HEX_SIZE) + "x";

namespace StorageCommand {
	enum STORAGE_COMMANDS {
		read = 1,
		write = 2,
		info = 3
	};
}

namespace StorageErr {
	enum STORAGE_ACCESS_ERRORS {
		index_out_of_bounds = -1,
		seek_err = -2,
		read_err = -3,
		write_err = -4
	};
}


class Storage : public Peripheral {
public:
	Storage(const char *storage_path);
	~Storage();
	Data get() override;
	void send(Data d1 = 0, Data d2 = 0, Data d3 = 0) override;
protected:
	Data get_storage_size();
	Data storage_size;
private:
	Data command;
	Data index;
	Data write_data;
	std::FILE* file;
};

#endif /* STORAGE_HPP */