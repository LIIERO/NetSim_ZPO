

#include "factory.hpp"
#include "nodes.hpp"

void Factory::add_worker(Worker && worker) {
    workers.add(std::move(worker));
}
void Factory::add_ramp(Ramp && ramp) {
    ramps.add(std::move(ramp));
}
void Factory::add_storehouse(Storehouse && storehouse) {
    storehouses.add(std::move(storehouse));
}

void Factory::remove_ramp(ElementID id) {
    remove_receiver(workers,id);
    ramps.remove_by_id(id);
}
void Factory::remove_worker(ElementID id) {
    remove_receiver(workers,id);
    remove_receiver(ramps,id);
    workers.remove_by_id(id);
}
void Factory::remove_storehouse(ElementID id) {
    remove_receiver(workers,id);
    remove_receiver(ramps,id);
    storehouses.remove_by_id(id);
}

NodeCollection<Ramp>::iterator Factory::find_ramp_by_id(ElementID id) {
    return ramps.find_by_id(id);
}

NodeCollection<Worker>::iterator Factory::find_worker_by_id(ElementID id) {
    return workers.find_by_id(id);
}

NodeCollection<Storehouse>::iterator Factory::find_storehouse_by_id(ElementID id) {
    return storehouses.find_by_id(id);
}

NodeCollection<Ramp>::const_iterator Factory::find_ramp_by_id(ElementID id) const{

    return ramps.find_by_id(id);
}

NodeCollection<Worker>::const_iterator Factory::find_worker_by_id(ElementID id) const{
    return workers.find_by_id(id);
}

NodeCollection<Storehouse>::const_iterator Factory::find_storehouse_by_id(ElementID id) const{
    return storehouses.find_by_id(id);
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cbegin() {
    return ramps.cbegin();
}
NodeCollection<Worker>::const_iterator Factory::worker_cbegin() {
    return workers.cbegin();
}
NodeCollection<Storehouse>::const_iterator Factory::storehouse_cbegin() {
    return storehouses.cbegin();
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cend() {
    return ramps.cend();
}
NodeCollection<Worker>::const_iterator Factory::worker_cend() {
    return workers.cend();
}
NodeCollection<Storehouse>::const_iterator Factory::storehouse_cend() {
    return storehouses.cend();
}

bool Factory::is_consistent() {
    NodeColor color;
}
void Factory::do_deliveries(Time t) {
    for (auto &r: ramps)
        r.delivery_goods(t);
}

void Factory::do_work(Time t) {
    for (auto &w: workers)
        w.do_work(t);
}

void Factory::do_package_passing() {
    for (auto &r: ramps)
        r.send_package();
    for (auto &w: workers)
        w.send_package();

}

