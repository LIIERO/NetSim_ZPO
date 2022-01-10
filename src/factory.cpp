#include "factory.hpp"
#include "nodes.hpp"



void Factory::add_worker(Worker&& worker) {
    workers_.add(std::move(worker));
}


void Factory::add_ramp(Ramp && ramp) {
    ramps_.add(std::move(ramp));
}


void Factory::add_storehouse(Storehouse && storehouse) {
    storehouses_.add(std::move(storehouse));
}


void Factory::remove_ramp(ElementID id) {
    remove_receiver(workers_, id);
    ramps_.remove_by_id(id);
}


void Factory::remove_worker(ElementID id) {
    remove_receiver(workers_, id);
    remove_receiver(ramps_, id);
    workers_.remove_by_id(id);
}


void Factory::remove_storehouse(ElementID id) {
    remove_receiver(workers_, id);
    remove_receiver(ramps_, id);
    storehouses_.remove_by_id(id);
}

NodeCollection<Ramp>::iterator Factory::find_ramp_by_id(ElementID id) {
    return ramps_.find_by_id(id);
}

NodeCollection<Worker>::iterator Factory::find_worker_by_id(ElementID id) {
    return workers_.find_by_id(id);
}

NodeCollection<Storehouse>::iterator Factory::find_storehouse_by_id(ElementID id) {
    return storehouses_.find_by_id(id);
}

NodeCollection<Ramp>::const_iterator Factory::find_ramp_by_id(ElementID id) const{

    return ramps_.find_by_id(id);
}

NodeCollection<Worker>::const_iterator Factory::find_worker_by_id(ElementID id) const{
    return workers_.find_by_id(id);
}

NodeCollection<Storehouse>::const_iterator Factory::find_storehouse_by_id(ElementID id) const{
    return storehouses_.find_by_id(id);
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cbegin() {
    return ramps_.cbegin();
}
NodeCollection<Worker>::const_iterator Factory::worker_cbegin() {
    return workers_.cbegin();
}
NodeCollection<Storehouse>::const_iterator Factory::storehouse_cbegin() {
    return storehouses_.cbegin();
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cend() {
    return ramps_.cend();
}
NodeCollection<Worker>::const_iterator Factory::worker_cend() {
    return workers_.cend();
}
NodeCollection<Storehouse>::const_iterator Factory::storehouse_cend() {
    return storehouses_.cend();
}

bool Factory::is_consistent() {
    NodeColor color;
}
void Factory::do_deliveries(Time t) {
    for (auto &r: ramps_)
        r.delivery_goods(t);
}

void Factory::do_work(Time t) {
    for (auto &w: workers_)
        w.do_work(t);
}

void Factory::do_package_passing() {
    for (auto &r: ramps_)
        r.send_package();
    for (auto &w: workers_)
        w.send_package();

}

