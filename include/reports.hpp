#ifndef NETSIM_ZPO_REPORTS_HPP
#define NETSIM_ZPO_REPORTS_HPP

# include <iostream>
# include <fstream>
# include "factory.hpp"

void generate_structure_report(const Factory& f, std::ostream& os);

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);

#endif //NETSIM_ZPO_REPORTS_HPP
