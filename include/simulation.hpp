#ifndef FABRYKA_IMPLEMENTATION_SIMULATION_HPP
#define FABRYKA_IMPLEMENTATION_SIMULATION_HPP

#include "factory.hpp"


void simulate(Factory&, TimeOffset, std::function<void (Factory&, Time)>);


#endif
