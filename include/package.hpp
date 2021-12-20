#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include "types.hpp"

#include <cstdlib>
#include <iostream>


class Package {
public:

    Package() = default;

    Package(ElementID id) : id_(id) {}

    Package(Package&& package);

    Package& operator=(Package&& package);

    ElementID get_id() const { return id_; }

    ~Package();

private:

    ElementID id_; //TODO: must be changed
};


#endif