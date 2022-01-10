#include "factory.hpp"
#include "nodes.hpp"

void Factory::add_worker(Worker && worker) {
    workers->add(std::move(worker));
}



void Factory::add_ramp(Ramp && ramp) {
    ramps->add(std::move(ramp));
}



void Factory::add_storehouse(Storehouse && storehouse) {
    storehouses->add(std::move(storehouse));
}



void Factory::remove_ramp(ElementID id) {
    ramps->remove_by_id(id);
}



void Factory::remove_worker(ElementID id) {
    workers->remove_by_id(id);
}



void Factory::remove_storehouse(ElementID id) {
    storehouses->remove_by_id(id);
}



NodeCollection<Ramp>::iterator Factory::find_ramp_by_id(ElementID id) {

    return ramps->find_by_id(id);
}



NodeCollection<Worker>::iterator Factory::find_worker_by_id(ElementID id) {
    return workers->find_by_id(id);
}



NodeCollection<Storehouse>::iterator Factory::find_storehouse_by_id(ElementID id) {
    return storehouses->find_by_id(id);
}
NodeCollection<Ramp>::const_iterator Factory::find_ramp_by_id(ElementID id) const{

    return ramps->find_by_id(id);
}

NodeCollection<Worker>::const_iterator Factory::find_worker_by_id(ElementID id) const{
    return workers->find_by_id(id);
}

NodeCollection<Storehouse>::const_iterator Factory::find_storehouse_by_id(ElementID id) const{
    return storehouses->find_by_id(id);
}



NodeCollection<Ramp>::const_iterator Factory::ramp_cbegin() {
    return ramps->cbegin();
}



NodeCollection<Worker>::const_iterator Factory::worker_cbegin() {
    return workers->cbegin();
}



NodeCollection<Storehouse>::const_iterator Factory::storehouse_cbegin() {
    return storehouses->cbegin();
}



NodeCollection<Ramp>::const_iterator Factory::ramp_cend() {
    return ramps->cend();
}



NodeCollection<Worker>::const_iterator Factory::worker_cend() {
    return workers->cend();
}



NodeCollection<Storehouse>::const_iterator Factory::storehouse_cend() {
    return storehouses->cend();
}



void Factory::remove_receiver(NodeCollection<IPackageReceiver> &collection, ElementID id) {

}



bool Factory::is_consistent() {
    NodeColor color;
}
void Factory::do_deliveries(Time T) {
    ramps->do_ramps(T);
}

void Factory::do_work(Time T) {
    workers->do_workers(T);
}

void Factory::do_package_passing() {

}

void Workers::do_workers(Time T) {
    for(size_t i = 0; i < nodes.size(); i++){
        nodes.at(i).do_work(T);
    }
}

void Ramps::do_ramps(Time T) {
    for(size_t i = 0; i < nodes.size(); i++){
        nodes.at(i).delivery_goods(T);
    }
}