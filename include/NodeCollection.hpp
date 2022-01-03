

#ifndef NETSIM_ZPO_NODECOLLECTION_HPP
#define NETSIM_ZPO_NODECOLLECTION_HPP

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
    void remove_by_id(ElementID id){
        nodes.erase(std::remove_if(nodes.begin(),nodes.end(), [id](ElementID x){return (x == id);}));
    }
    NodeCollection<Node>::iterator find_by_id(ElementID id){
        return std::find_if(nodes.begin(),nodes.end(), [id](ElementID x){return (x == id);});
    };
    NodeCollection<Node>::const_iterator find_by_id (ElementID id) const{ //NIE WIEM CO TU DAĆ ELO NAURU bool pies = kot;
        return std::find_if(nodes.begin(),nodes.end(), [id](ElementID x){return (x == id);});
    };
    const_iterator  cbegin(){
        return nodes.front();
    };
    const_iterator  cend(){
        return nodes.back();
    };

protected:
    container_t nodes;
};
#endif //NETSIM_ZPO_NODECOLLECTION_HPP
