#ifndef SIECI_HEADER_
#define SIECI_HEADER_

#include "types.hpp"

#include <cstdlib>
#include <iostream>
#include <set>


class Package {
public:

    Package();

    explicit Package(ElementID id): _id(id) {}

    Package(Package&& package) = default;

    Package& operator=(Package&& package) = default;

    ElementID get_id() const { return _id; }

    ~Package();

private:

    ElementID _id;

    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;

};

#endif