#ifndef PERIPHERAL_HPP
#define PERIPHERAL_HPP

using Data = int;

class Peripheral {
public:
	Peripheral() = default;
	virtual ~Peripheral() = default;
	virtual Data get() = 0;
	virtual void send(Data d1 = 0, Data d2 = 0, Data d3 = 0) = 0;
};

#endif /* PERIPHERAL_HPP */