#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include "package.hpp"

#include <cstdlib>
#include <iostream>
#include <list>



enum class PackageQueueType {
    FIFO,
    LIFO
};



class IPackageStockpile {
public:

    using const_iterator = std::list<Package>::const_iterator;

    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;


    virtual void push(Package&& package) = 0;

    virtual bool empty() const = 0;

    virtual std::size_t size() const = 0;

    virtual ~IPackageStockpile() = default;

};


class IPackageQueue : public IPackageStockpile {
public:

    virtual Package pop() = 0;

    virtual PackageQueueType get_queue_type() const = 0;

    //virtual ~IPackageQueue() = default;

};


class PackageQueue : public IPackageQueue {
public:

    PackageQueue(const PackageQueueType& pqt) : pack_queue_(std::list<Package>()), queue_type_(pqt) {}

    const_iterator begin() const override { return pack_queue_.cbegin(); }
    const_iterator end() const override { return pack_queue_.cend(); }
    const_iterator cbegin() const override { return pack_queue_.cbegin(); }
    const_iterator cend() const override { return pack_queue_.cend(); }

    void push(Package&& package) override { pack_queue_.emplace_back(std::move(package)); }

    bool empty() const override { return pack_queue_.empty(); }

    std::size_t size() const override { return pack_queue_.size(); }

    PackageQueueType get_queue_type() const override { return queue_type_; }

    Package pop() override;

private:

    std::list<Package> pack_queue_;

    PackageQueueType queue_type_;

};


#endif
