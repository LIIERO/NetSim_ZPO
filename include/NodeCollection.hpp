#ifndef NETSIM_ZPO_NODECOLLECTION_HPP
#define NETSIM_ZPO_NODECOLLECTION_HPP

#include "types.hpp"
#include "nodes.hpp"



template <class Node> class NodeCollection {

public:
    using container_t = typename std::vector<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node){
        nodes_.push_back(node);
    }

    void remove_by_id(ElementID id){
        nodes_.erase(std::remove_if(nodes_.begin(), nodes_.end(), [id](ElementID x){return (x == id);}));
    }

    NodeCollection<Node>::iterator find_by_id(ElementID id){
        return std::find_if(nodes_.begin(), nodes_.end(), [id](ElementID x){return (x == id);});
    };

    NodeCollection<Node>::const_iterator find_by_id (ElementID id) const{ //NIE WIEM CO TU DAĆ ELO NAURU bool pies = kot;
        return std::find_if(nodes_.begin(), nodes_.end(), [id](ElementID x){return (x == id);});
    };

    const_iterator  cbegin(){
        return nodes_.front();
    };

    const_iterator  cend(){
        return nodes_.back();
    };

protected:

    container_t nodes_;

};


#endif //NETSIM_ZPO_NODECOLLECTION_HPP
