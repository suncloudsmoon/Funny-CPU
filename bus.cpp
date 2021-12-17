#include "bus.hpp"

void Bus::connect(Port p, Peripheral &item) {
	locations.insert({ p, item });
}

Data Bus::get(Port p) {
	return locations.at(p).get();
}

void Bus::send(Port p, Data d1, Data d2, Data d3) {
	locations.at(p).send(d1, d2, d3);
}
