#ifndef BUS_HPP
#define BUS_HPP

#include <unordered_map>

#include "peripheral.hpp"

using Port = unsigned int;

class Bus {
public:
	void connect(Port p, Peripheral &item);
	Data get(Port p);
	void send(Port p, Data d1 = 0, Data d2 = 0, Data d3 = 0);
private:
	std::unordered_map<Port, Peripheral&> locations;
};

#endif /* BUS_HPP */