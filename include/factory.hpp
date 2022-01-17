#ifndef FABRYKA_IMPLEMENTATION_FACTORY_HPP
#define FABRYKA_IMPLEMENTATION_FACTORY_HPP

#include <types.hpp>
#include <nodes.hpp>
#include <list>
#include <map>

template<class Node>
class NodeCollection {
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node &&node) { node_collection_.emplace_back(std::move(node)); }

    iterator find_by_id(ElementID id) {
        return std::find_if(node_collection_.begin(), node_collection_.end(),
                            [id](const auto &node) { return id == node.get_id(); });
    }

    const_iterator find_by_id(ElementID id) const {
        return std::find_if(node_collection_.cbegin(), node_collection_.cend(),
                            [id](const auto &node) { return id == node.get_id(); });
    }

    void remove_by_id(ElementID id){
        auto it = find_by_id(id);
        if (it != node_collection_.cend())
            node_collection_.erase(it);
    }

    iterator begin() { return node_collection_.begin(); }
    iterator end() { return node_collection_.end(); }
    const_iterator begin() const { return node_collection_.cbegin(); }
    const_iterator end() const { return node_collection_.cend(); }
    const_iterator cbegin() const { return node_collection_.cbegin(); }
    const_iterator cend() const { return node_collection_.cend(); }

private:
    container_t node_collection_;
};

enum class NodeColor {
    UNVISITED, VISITED, VERIFIED
};

class Workers : public NodeCollection<Worker> {};
class Ramps : public NodeCollection<Ramp> {};
class Storehouses : public NodeCollection<Storehouse> {};

class Factory {
public:

    Factory() = default;


    void add_ramp(Ramp &&ramp) { ramp_collection_.add(std::move(ramp)); }
    void remove_ramp(ElementID id) {
        remove_receiver(worker_collection_, id);
        ramp_collection_.remove_by_id(id);
    }

    Ramps::iterator find_ramp_by_id(ElementID id) { return ramp_collection_.find_by_id(id); }
    Ramps::const_iterator find_ramp_by_id(ElementID id) const { return ramp_collection_.find_by_id(id); }

    Ramps::const_iterator ramp_cbegin() const { return ramp_collection_.cbegin(); }
    Ramps::const_iterator ramp_cend() const { return ramp_collection_.cend(); }


    void add_worker(Worker &&worker) { worker_collection_.add(std::move(worker)); }
    void remove_worker(ElementID id) {
        remove_receiver(ramp_collection_, id);
        remove_receiver(worker_collection_, id);
        worker_collection_.remove_by_id(id);
    }

    Workers::iterator find_worker_by_id(ElementID id) { return worker_collection_.find_by_id(id); }
    Workers::const_iterator find_worker_by_id(ElementID id) const { return worker_collection_.find_by_id(id); }

    Workers::const_iterator worker_cbegin() const { return worker_collection_.cbegin(); }
    Workers::const_iterator worker_cend() const { return worker_collection_.cend(); }

    void add_storehouse(Storehouse &&storehouse) { storehouse_collection_.add(std::move(storehouse)); }
    void remove_storehouse(ElementID id) {
        remove_receiver(worker_collection_, id);
        remove_receiver(ramp_collection_, id);
        storehouse_collection_.remove_by_id(id);
    }

    Storehouses::iterator find_storehouse_by_id(ElementID id) { return storehouse_collection_.find_by_id(id); }
    Storehouses::const_iterator find_storehouse_by_id(ElementID id) const { return storehouse_collection_.find_by_id(id); }

    Storehouses::const_iterator storehouse_cbegin() const { return storehouse_collection_.cbegin(); }
    Storehouses::const_iterator storehouse_cend() const { return storehouse_collection_.cend(); }

    void do_delivers(Time);
    void do_work(Time);
    void do_package_passing();
    bool is_consistent() const;


private:
    template<class Node>
    void remove_receiver(NodeCollection<Node> &collection, ElementID id){
        for (auto& node : collection) {
            IPackageReceiver* receiver = nullptr;
            for (const auto& preferences : node.receiver_preferences_.get_preferences()) {
                if (preferences.first->get_id() == id) {
                    receiver = preferences.first;
                    break;
                }
            }
            if (receiver != nullptr)
                ((ReceiverPreferences&) node.receiver_preferences_).remove_receiver(dynamic_cast<IPackageReceiver*>(receiver));
        }
    }

    Ramps ramp_collection_;
    Workers worker_collection_;
    Storehouses storehouse_collection_;
};

Factory load_factory_structure(std::istream &is);

void save_factory_structure(Factory& factory, std::ostream& os);

#endif //FABRYKA_IMPLEMENTATION_FACTORY_HPP
