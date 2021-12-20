#include "storage_types.hpp"
#include "types.hpp"
#include "package.hpp"


Package::~Package() {

}

Package& Package::operator=(Package&& package) {
    id_ = package.id_;

    return (*this);
}

Package::Package(Package&& package) {

}
