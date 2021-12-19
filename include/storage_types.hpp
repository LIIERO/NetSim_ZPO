#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include <cstdlib>
#include <iostream>



enum class PackageQueueType {
    LIFO,
    FIFO
};



class IPackageStockpile {
public:
    void push(Package&& package);

    bool empty() const;

    std::size_t size() const;

    //TODO: Implement iterators and fix size() (look into size_type) !!!

    ~IPackageStockpile() = default;
private:
};


class IPackageQueue : public IPackageStockpile {
public:
    Package pop();

    PackageQueueType get_queue_type() const;
private:

};


class PackageQueue : IPackageQueue {
public:
    PackageQueue(PackageQueueType pqt);//Remember to
private:

};


#endif
