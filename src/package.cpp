#include "storage_types.hpp"
#include "types.hpp"
#include "package.hpp"

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

Package::Package() {
    ElementID new_id;
    if (!freed_IDs.empty()) {
        auto it = freed_IDs.begin();
        new_id = *it;
        freed_IDs.erase(it);

    } else {
        if (assigned_IDs.empty()) {
            new_id = 1;
        } else {
            new_id = *(assigned_IDs.rbegin());
            ++new_id;
        }
    }

    _id = new_id;
    assigned_IDs.insert(new_id);
}


Package::~Package() {
    assigned_IDs.erase(_id);
    freed_IDs.insert(_id);
    _id.~ElementID();
}


//Package& Package::operator=(Package&& package) {
//    _id = package._id;
//    return (*this);
//}
