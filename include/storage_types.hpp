#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include "package.hpp"

#include <cstdlib>
#include <iostream>
#include <list>



enum class PackageQueueType {
    LIFO,
    FIFO
};



class IPackageStockpile {
public:

    virtual void push(Package&& package) = 0;

    virtual bool empty() const = 0;

    virtual std::size_t size() const = 0;

    //TODO: Implement iterators and fix ->size()<- (look into size_type) !!!

    virtual ~IPackageStockpile() = default;

private:
    using const_iterator = std::list<Package>::const_iterator;
};


class IPackageQueue : public IPackageStockpile {
public:
    virtual Package pop() = 0;

    virtual PackageQueueType get_queue_type() const = 0;
};


class PackageQueue : IPackageQueue {
public:
    PackageQueue(PackageQueueType pqt);//Remember to
};


#endif
