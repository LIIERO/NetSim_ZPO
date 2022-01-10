
#ifndef NETSIM_ZPO_FACTORY1_HPP
#define NETSIM_ZPO_FACTORY1_HPP

#include "types.hpp"
#include "nodes.hpp"

template <class Node> class NodeCollection{

public:
    using container_t = typename std::vector<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node){
        nodes.push_back(node);
    }
    //TODO Możliwe że poniżej jest źle
    void remove_by_id(ElementID id){
        nodes.erase(std::remove_if(nodes.begin(),nodes.end(), [id](const auto &node){return (node.get_id() == id);}));
    }
    NodeCollection<Node>::iterator find_by_id(ElementID id){
        return std::find_if(nodes.begin(),nodes.end(), [id](const auto &node){return (node.get_id() == id);});
    };
    NodeCollection<Node>::const_iterator find_by_id (ElementID id) const{
        return std::find_if(nodes.cbegin(),nodes.cend(), [id](const auto &node){return (node.get_id() == id);});
    };
    /*const_iterator  cbegin(){
        return nodes.front();
    };
    const_iterator  cend(){
        return nodes.back();
    };*/
    iterator begin() { return nodes.begin(); }
    iterator end() { return nodes.end(); }
    const_iterator begin() const { return nodes.cbegin(); }
    const_iterator end() const { return nodes.cend(); }
    const_iterator cbegin() const { return nodes.cbegin(); }
    const_iterator cend() const { return nodes.cend(); }


protected:
    container_t nodes;
};

class Ramps: public NodeCollection<Ramp>{
public:
    Ramps() = default;

    ~Ramps() = default;
};

class Storehouses: public NodeCollection<Storehouse>{
public:
    Storehouses() = default;

    ~Storehouses() = default;
};

class Workers: public NodeCollection<Worker>{
public:
    Workers() =default ;

    ~Workers() = default;
};

class Factory {
private:
    Ramps ramps;
    Workers workers;
    Storehouses storehouses;

    template<class Node> void remove_receiver(NodeCollection<Node>& collection, ElementID id){
        for(auto& node : collection){
            IPackageReceiver* receiver = nullptr;
            for (const auto& preferences : node.receiver_preferences_.get_preferences()){
                if(preferences.first->get_id() == id){
                    receiver =preferences.first;
                    break;
                }

            }
            if(receiver != nullptr)
                ((ReceiverPreferences&) node.receiver_preferences_).remove_receiver(dynamic_cast<IPackageReceiver*>(receiver));
        }
    }

public:

    Factory() = default;

    void add_ramp(Ramp&& ramp);
    void add_worker(Worker&& worker);
    void add_storehouse(Storehouse&& storehouse);

    void remove_ramp(ElementID id);
    void remove_worker(ElementID id);
    void remove_storehouse(ElementID id);

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id);

    NodeCollection<Ramp>::const_iterator find_ramp_by_id (ElementID id) const;
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id)  const;
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id)  const;

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
    ~Factory() = default;

};

enum class NodeColor { UNVISITED, VISITED, VERIFIED };




#endif //NETSIM_ZPO_FACTORY1_HPP
