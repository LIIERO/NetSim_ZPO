
#ifndef NETSIM_ZPO_FACTORY1_HPP
#define NETSIM_ZPO_FACTORY1_HPP

#include "types.hpp"
#include "NodeCollection.hpp"
#include "Ramps.hpp"
#include "Workers.hpp"
#include "Storehouses.hpp"

class Factory {
private:
    Ramps* ramps = new Ramps();
    Workers* workers = new Workers();
    Storehouses* storehouses = new Storehouses();
    void remove_receiver(NodeCollection<IPackageReceiver>& collection, ElementID id);

public:

    Factory() = default;

    void add_ramp(Ramp&&);
    void add_worker(Worker&&);
    void add_storehouse(Storehouse&&);

    void remove_ramp(ElementID id);
    void remove_worker(ElementID id);
    void remove_storehouse(ElementID id);

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id);

    NodeCollection<Ramp>::const_iterator find_ramp_by_cid(ElementID id);
    NodeCollection<Worker>::const_iterator find_worker_by_cid(ElementID id);
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_cid(ElementID id);

    NodeCollection<Ramp>::const_iterator  ramp_cbegin();
    NodeCollection<Worker>::const_iterator  worker_cbegin();
    NodeCollection<Storehouse>::const_iterator  storehouse_cbegin();

    NodeCollection<Ramp>::const_iterator  ramp_cend();
    NodeCollection<Worker>::const_iterator  worker_cend();
    NodeCollection<Storehouse>::const_iterator  storehouse_cend();

    bool is_consistent(void);
    void do_deliveries(Time);
    void do_package_passing(void);
    void do_work(Time);
    ~Factory();

};

enum class NodeColor { UNVISITED, VISITED, VERIFIED };
#endif //NETSIM_ZPO_FACTORY1_HPP
