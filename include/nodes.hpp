#ifndef NETSIM_ZPO_NODES_HPP
#define NETSIM_ZPO_NODES_HPP

#include "storage_types.hpp"
#include "helpers.hpp"

#include <cstdlib>
#include <iostream>
#include <list>
#include <memory>
#include <map>


class ReceiverPreferences {
public:

    using preferences_t = std::map<IPackageReceiver*, double>;

    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg);

    void add_receiver(IPackageReceiver* r);

    void remove_receiver(IPackageReceiver* r);

    IPackageReceiver* choose_receiver();

    preferences_t& get_preferences();

};


class PackageSender {
public:

    PackageSender();

    void send_package();

    std::optional<Package>& get_sending_buffer() const;

    void push_package(Package&& package);

private:

    ReceiverPreferences receiver_preferences_;

};



class IPackageReceiver {
public:

    virtual void receive_package(Package&& package) = 0;

    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;


    virtual ElementID get_id() const = 0;

};



class Storehouse : public IPackageStockpile, public IPackageReceiver {
public:

    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) : id_(id), package_queue_ptr_(std::move(d)) {}

private:

    ElementID id_;

    std::unique_ptr<IPackageStockpile> package_queue_ptr_;

};



class Ramp : public PackageSender {
public:

    Ramp(ElementID id, TimeOffset di);

    void delivery_goods(Time t);

    TimeOffset get_delivery_interval() const;

    ElementID get_id() const;

};



class Worker : public IPackageQueue, public IPackageReceiver, public PackageSender {
public:

    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);

    void do_work(Time t);

    TimeOffset get_processing_duration() const;

    Time get_processing_start_time() const;

};



#endif
