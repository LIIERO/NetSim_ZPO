#ifndef NETSIM_ZPO_SIMULATION_HPP
#define NETSIM_ZPO_SIMULATION_HPP

#include "factory.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void (Factory&, Time)> rf);

#endif
