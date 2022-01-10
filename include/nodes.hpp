#ifndef NETSIM_ZPO_NODES_HPP
#define NETSIM_ZPO_NODES_HPP

#include "storage_types.hpp"
#include "helpers.hpp"


#include <cstdlib>
#include <iostream>
#include <list>
#include <memory>
#include <map>



enum class ReceiverType {
    WORKER,
    STOREHOUSE
};



class IPackageReceiver {
public:

    virtual void receive_package(Package&& package) = 0;

    virtual ReceiverType get_receiver_type() const = 0;

    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;

    virtual ElementID get_id() const = 0;

};



class ReceiverPreferences {
public:

    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences() = default;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(pg) {};

    void add_receiver(IPackageReceiver* r);

    void remove_receiver(IPackageReceiver* r);

    IPackageReceiver* choose_receiver();

    preferences_t& get_preferences();

private:

    ProbabilityGenerator pg_;

};



class PackageSender {
public:

    PackageSender() = default;

    PackageSender(PackageSender&&) = default;

    void send_package();

    std::optional<Package>& get_sending_buffer() { return buffer_; }

    ReceiverPreferences receiver_preferences_; // Pole publiczne

protected:

    std::optional<Package> buffer_ = std::nullopt;

    void push_package(Package&& package) { buffer_.emplace(std::move(package)); }

};



class Storehouse : public IPackageReceiver {
public:

    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d =
            std::make_unique<PackageQueue>(PackageQueueType::FIFO)) : id_(id), d_(std::move(d)) {};

    ElementID get_id() const override { return id_; }

    void receive_package(Package&& other_package) override;

    IPackageStockpile::const_iterator begin() const override { return d_->begin(); };
    IPackageStockpile::const_iterator end() const override { return d_->end();};
    IPackageStockpile::const_iterator cbegin() const override { return d_->cbegin(); };
    IPackageStockpile::const_iterator cend() const override { return d_->cend(); };

    ReceiverType get_receiver_type() const override { return node_type_; }

private:

    ElementID id_;

    std::unique_ptr<IPackageStockpile> d_;

    ReceiverType node_type_ = ReceiverType::STOREHOUSE;

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

    IPackageStockpile::const_iterator begin() const override { return q_->begin(); };
    IPackageStockpile::const_iterator end() const override { return q_->end();};
    IPackageStockpile::const_iterator cbegin() const override { return q_->cbegin(); };
    IPackageStockpile::const_iterator cend() const override { return q_->cend(); };

    ReceiverType get_receiver_type() const override { return node_type_; }

private:

    ElementID id_;

    TimeOffset pd_;

    std::unique_ptr<IPackageQueue> q_;

    Time time_ = 0;

    ReceiverType node_type_ = ReceiverType::WORKER;

};

#endif
