

#ifndef NETSIM_ZPO_WORKERS_HPP
#define NETSIM_ZPO_WORKERS_HPP

#include "NodeCollection.hpp"

class Workers: public NodeCollection<Worker>{
public:
    Workers() =default ;

    ~Workers() = default;
};
#endif //NETSIM_ZPO_WORKERS_HPP
