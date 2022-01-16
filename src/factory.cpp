#include "factory.hpp"
#include "nodes.hpp"
#include <map>
#include <sstream>


void Factory::add_worker(Worker && worker) {
    workers_.add(std::move(worker));
}



void Factory::add_ramp(Ramp && ramp) {
    ramps_.add(std::move(ramp));
}



void Factory::add_storehouse(Storehouse && storehouse) {
    storehouses_.add(std::move(storehouse));
}



void Factory::remove_ramp(ElementID id) {
    remove_receiver(workers_,id);
    ramps_.remove_by_id(id);
}


void Factory::remove_worker(ElementID id) {
    remove_receiver(workers_,id);
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



void Factory::do_deliveries(Time t) {
    for (auto &r: ramps_)
        r.deliver_goods(t);
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



bool has_reachable_storehouse(const PackageSender *p_sen_ptr, std::map<const PackageSender*, NodeColor> &color) {

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
                if (color[send_rec_ptr] == NodeColor::UNVISITED) { has_reachable_storehouse(send_rec_ptr, color); }
            }
        }
    }
    color[p_sen_ptr] = NodeColor::VERIFIED;
    if (reaches) { return true; }

    throw (std::runtime_error("Storehouse cannot be reached"));
}



bool Factory::is_consistent() const {
    std::map<const PackageSender*, NodeColor> sender_colors;
    for (const auto &r : ramps_)
        sender_colors[&r] = NodeColor::UNVISITED;
    for (const auto &w : workers_)
        sender_colors[&w] = NodeColor::UNVISITED;

    try {
        for (const auto &r : ramps_) { has_reachable_storehouse(&r, sender_colors); }
    }
    catch (...) {
        return false;
    }

    return true;
}

struct ParsedLineData {
    ParsedLineData() = default;
    ElementType elemnt_type;
    std::map<std::string, std::string> parametrs; // mapowanie "ID -> wartość"

    ~ParsedLineData() = default;
};

ElementType eltype (std::string line){
    if(line == "LINK") {return ElementType::LINK;}
    if(line == "STOREHOUSE") {return ElementType::STOREHOUSE;}
    if(line == "WORKER") {return ElementType::WORKER;}
    if(line == "LOADING_RAMP") {return ElementType::RAMP;}
    throw std::invalid_argument ("Taki typ elementu nie istnieje!");
}

PackageQueueType pqttype(std::string line){
    if(line == "FIFO") {return PackageQueueType::FIFO;}
    if(line == "LIFO") {return PackageQueueType::LIFO;}
    throw std::invalid_argument ("Taki typ kolejki nie istnieje!");
}

std::string str_pqt (PackageQueueType typ){
    switch (typ) {
        case PackageQueueType::LIFO:
            return "LIFO";
        case PackageQueueType::FIFO:
            return "FIFO";
    }
    throw std::invalid_argument("Taki typ kolejki nie istnieje!");
}

std::string str_rt (ReceiverType typ){
    switch(typ) {
        case ReceiverType::WORKER:
            return "worker";
        case ReceiverType::STOREHOUSE:
            return "store";
    }
    throw std::invalid_argument("Taki odbiorca nie istnieje!");
}

std::vector<std::string> split_line(std::string &line, char delimiter){
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(line);
    while(std::getline(token_stream, token, delimiter)){
         tokens.push_back(token);
    }
    return tokens;
}


void load(Factory &factory_, std::vector<std::string> &sender, std::vector<std::string> &rec){
    if (sender[0] == "worker"){
        auto iterator_worker = factory_.find_worker_by_id(static_cast<ElementID>(std::stoi(sender[1])));

        if (rec[0] == "store") {
            auto it_receiver_store = factory_.find_storehouse_by_id(static_cast<ElementID>(std::stoi(rec[1])));
            auto receiver = &(*it_receiver_store);
            iterator_worker->receiver_preferences_.add_receiver(receiver);
        }
        else if (rec[0] == "worker") {
            auto it_receiver_worker = factory_.find_worker_by_id(static_cast<ElementID>(std::stoi(rec[1])));
            auto receiver = &(*it_receiver_worker);
            iterator_worker->receiver_preferences_.add_receiver(receiver);
        }
        else {
            throw std::invalid_argument("Taki typ odbiorcy nie istnieje");
        }
    }
    if (sender[0] == "ramp"){
        auto iterator_ramp = factory_.find_ramp_by_id(static_cast<ElementID>(std::stoi(sender[1])));

        if (rec[0] == "store") {
            auto it_receiver_store = factory_.find_storehouse_by_id(static_cast<ElementID>(std::stoi(rec[1])));
            auto receiver = &(*it_receiver_store);
            iterator_ramp->receiver_preferences_.add_receiver(receiver);
        }
        else if (rec[0] == "worker") {
            auto it_receiver_worker = factory_.find_worker_by_id(static_cast<ElementID>(std::stoi(rec[1])));
            auto receiver = &(*it_receiver_worker);
            iterator_ramp->receiver_preferences_.add_receiver(receiver);
        }
        else {
            throw std::invalid_argument("Taki typ odbiorcy nie istnieje");
        }
    }
    else{
        throw std::invalid_argument("Taki typ nadawcy nie istnieje!");
    }
}

ParsedLineData parse_line (std::string line){
    auto words = split_line(line, ' ');
    std::list<std::string> tokens (std::make_move_iterator(words.begin()),std::make_move_iterator(words.end()));
    ParsedLineData node;
    node.elemnt_type = eltype(std::move((tokens.front())));
    tokens.pop_front();
    for (auto &el :tokens){
        auto key_value = split_line(el, '=');
        node.parametrs.insert({key_value[0], key_value[1]});
    }
    return node;
}

Factory load_factory_structure(std::istream& is){
    Factory factory_;
    std::string line;
    while(std::getline(is, line)){
        if (line.empty() || isblank(line[0])) continue;
        else if (!line.empty() && line[0] == ';') continue;
        else{
            auto parsed = parse_line(line);
            switch (parsed.elemnt_type){
                case ElementType::RAMP: {
                    Ramp ramp_(static_cast<ElementID>(std::stoi(parsed.parametrs["id"])), std::stoi(parsed.parametrs["delivery-interval"]));
                    factory_.add_ramp(std::move(ramp_));
                    break;
                }
                case ElementType::STOREHOUSE: {
                            Storehouse storehouse_(static_cast<ElementID>(std::stoi(parsed.parametrs["id"])));
                            factory_.add_storehouse(std::move(storehouse_));
                            break;
                    }
                    case ElementType::WORKER: {
                            Worker worker_(static_cast<ElementID>(std::stoi(parsed.parametrs["id"])), std::stoi(parsed.parametrs["processing-time"]), std::make_unique<PackageQueue>(pqttype(parsed.parametrs["queue-type"])));
                            factory_.add_worker(std::move(worker_));
                            break;
                    }
                    case ElementType::LINK: {
                            auto sender_val = split_line(parsed.parametrs["src"], '-');
                            auto receiver_val = split_line(parsed.parametrs["dest"], '-');
                            load(factory_, sender_val, receiver_val);
                            break;
                    }
            }
        }
    }
    return factory_;
}

void save_factory_structure(Factory& factory, std::ostream& os){
    std::ostringstream temp;
    os << "; == LOADING RAMPS ==" << std::endl << std::endl;
    for(auto iterator = factory.ramp_cbegin(); iterator != factory.ramp_cend(); ++iterator){
        os << "LOADING_RAMP id=" << iterator->get_id() << " delivery-interval="<< iterator->get_delivery_interval() << std::endl;
        for (auto elements : iterator->receiver_preferences_.get_preferences()){
            temp << "LINK src=ramp-" << iterator->get_id() << " dest=" << str_rt(elements.first->get_receiver_type()) << "-" << elements.first->get_id() << std::endl;
        }
        temp << std::endl;
    }
    os << "; == WORKERS ==" << std::endl << std::endl;
    for(auto iterator = factory.worker_cbegin(); iterator != factory.worker_cend(); ++iterator){
        os << "WORKER id=" << iterator->get_id() << " processing-time="<< iterator->get_processing_duration() << " queue-type=" << str_pqt(iterator->get_queue()->get_queue_type()) << std::endl;
        for (auto elements : iterator->receiver_preferences_.get_preferences()){
            temp << "LINK src=worker-" << iterator->get_id() << " dest=" << str_rt(elements.first->get_receiver_type()) << "-" << elements.first->get_id() << std::endl;
        }
        temp << std::endl;
    }
    os << "; == STOREHOUSES ==" << std::endl << std::endl;
    for(auto iterator = factory.storehouse_cbegin(); iterator != factory.storehouse_cend(); ++iterator){
        os << "STOREHOUSE id=" << iterator->get_id() <<std::endl;
    }
    os << std::endl << "; == LINKS ==" <<std::endl << std::endl;
    os << temp.str();
}