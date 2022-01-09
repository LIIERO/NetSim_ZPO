#ifndef NETSIM_ZPO_NODES_HPP
#define NETSIM_ZPO_NODES_HPP

#include "storage_types.hpp"
#include "helpers.hpp"


#include <cstdlib>
#include <iostream>
#include <list>
#include <memory>
#include <map>



enum class ReceiverType{
    WORKER,
    STOREHOUSE
};



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

    virtual ReceiverType get_receiver_type() = 0;

    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;

    virtual ElementID get_id() const = 0;

};



class Storehouse : public IPackageReceiver {
public:
//    PackageSender() = default;
//
//    PackageSender(PackageSender&& other) = default; //to jest nie potrzebne!!!

    ElementID get_id() const override { return id_; }

    void receive_package(Package&& other_package) override;

private:

    ElementID id_;

    //TODO sprawdzić to
    //std::optional<Package>& get_sending_buffer() { return buffer_; }

    std::unique_ptr<IPackageStockpile> package_queue_ptr_;  // = std::make_unique<PackageQueue> (PackageQueueType::FIFO);
                                                                // nie wiem czy to nam potrzebne ogólnie
    //TODO to chyba  jest nie potrzebne
//protected:
//
//    std::optional<Package> buffer_ = std::nullopt;
//
//    void push_package(Package&& package) { buffer_.emplace(std::move(package)); }

};



class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di): id_(id), di_(di) {}

    void delivery_goods(Time t);

    TimeOffset get_delivery_interval() const { return di_; };

    ElementID get_id() const { return id_; };

private:

    ElementID id_;

    TimeOffset di_;
};



class Worker : public IPackageReceiver, public PackageSender {
public:

    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q): id_(id), pd_(pd), q_(std::move(q)) {}

    void do_work(Time t);

    TimeOffset get_processing_duration() const { return pd_; }

    Time get_processing_start_time() const { return time_; }

    //implementacja IPackageReceiver
    ElementID get_id() const override { return id_; }

    //TODO: sprawdzić
    void receive_package(Package&& package) override;

private:

    ElementID id_;

    TimeOffset pd_;

    std::unique_ptr<IPackageQueue> q_;

    Time time_ = 0;

};



#endif
