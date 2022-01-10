

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
    ramps.remove_by_id(id);
}
void Factory::remove_worker(ElementID id) {
    workers.remove_by_id(id);
}
void Factory::remove_storehouse(ElementID id) {
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

bool reaches_storehouse(const PackageSender *p_sen_ptr, std::map<const PackageSender*, NodeColor> &color) {

    if (color[p_sen_ptr] == NodeColor::VERIFIED) { return true; }
    color[p_sen_ptr] = NodeColor::VISITED;
    if (p_sen_ptr->receiver_preferences_.get_preferences().empty()) { throw (std::runtime_error("No defined receiver")); }

    bool reaches = false;
    for (auto &r : p_sen_ptr->receiver_preferences_.get_preferences()) {
        auto receiver_type = r.first->get_receiver_type();
        if (receiver_type == ReceiverType::STOREHOUSE) { reaches = true; }
        else if (receiver_type == ReceiverType::WORKER) {
            IPackageReceiver* receiver_ptr = r.first;
            auto send_rec_ptr = dynamic_cast<PackageSender*>(receiver_ptr);
            if (p_sen_ptr == send_rec_ptr) {
                continue; }
            else {
                reaches = true;
                if (color[send_rec_ptr] == NodeColor::UNVISITED) { reaches_storehouse(send_rec_ptr, color); }
            }
        }
    }
    color[p_sen_ptr] = NodeColor::VERIFIED;
    if (reaches) { return true; }

    throw (std::runtime_error("Storehouse cannot be reached"));
}

bool Factory::is_consistent() const {
    std::map<const PackageSender*, NodeColor> sender_colors;
    for (const auto &r : ramps)
        sender_colors[&r] = NodeColor::UNVISITED;
    for (const auto &w : workers)
        sender_colors[&w] = NodeColor::UNVISITED;

    try {
        for (const auto &r : ramps) { reaches_storehouse(&r, sender_colors); }
    }
    catch (...) {
        return false;
    }

    return true;
}

