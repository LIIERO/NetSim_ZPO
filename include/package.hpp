#ifndef SIECI_HEADER_
#define SIECI_HEADER_

#include "types.hpp"

#include <cstdlib>
#include <iostream>


class Package {
    public:

    Package() = default;

    Package(ElementID id);

    Package(Package&& package);

    Package& operator=(Package&& package);

    ElementID get_id() const { return id_; }

    ~Package() = default;

    private:

    ElementID id_; //TODO: must be changed

};


#endif