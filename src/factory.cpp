#include "factory.hpp"
#include "nodes.hpp"
#include <memory>
#include <map>
#include <sstream>


void Factory::do_delivers(Time t) {
    for (auto &ramp: ramp_collection_)
        ramp.deliver_goods(t);
}


void Factory::do_work(Time t) {
    for (auto &worker: worker_collection_)
        worker.do_work(t);
}


void Factory::do_package_passing() {
    for (auto &worker: worker_collection_)
        worker.send_package();
    for (auto &ramp: ramp_collection_)
        ramp.send_package();
}


bool has_reachable_storehouse(const PackageSender *sender, std::map<const PackageSender *, NodeColor> &node_colors) {

    if (node_colors[sender] == NodeColor::VERIFIED)
        return true;
    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().size() == 0)
        throw (std::runtime_error("RECEIVER NOT DEFINED"));

    bool is_reachable_storehouse = false;

    for (auto &receiver : sender->receiver_preferences_.get_preferences()) {

        auto receiver_type = receiver.first->get_receiver_type();
        if (receiver_type == ReceiverType::STOREHOUSE)
            is_reachable_storehouse = true;

        else if (receiver_type == ReceiverType::WORKER) {
            IPackageReceiver *receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<Worker *>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender *>(worker_ptr);
            if (sender == sendrecv_ptr)
                continue;
            is_reachable_storehouse = true;
            if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED)
                has_reachable_storehouse(sendrecv_ptr, node_colors);
        }
    }
    node_colors[sender] = NodeColor::VERIFIED;
    if (is_reachable_storehouse)
        return true;
    else {
        throw (std::runtime_error("STOREHOUSE NOT REACHABLE"));
    }
}

bool Factory::is_consistent() const {
    std::map<const PackageSender *, NodeColor> colors;
    for (const auto &worker : worker_collection_)
        colors[(PackageSender *) &worker] = NodeColor::UNVISITED;
    for (const auto &ramp : ramp_collection_)
        colors[(PackageSender *) &ramp] = NodeColor::UNVISITED;

    try {
        for (const auto &ramp : ramp_collection_) {
            const PackageSender *ptr = &ramp;
            has_reachable_storehouse(ptr, colors);
        }
    }
    catch (...) {
        return false;
    }
    return true;
}


enum class ElementType {
    RAMP, WORKER, STOREHOUSE, LINK
};


struct ParsedLineData {
    ElementType element_type;
    std::map<std::string, std::string> parameters;

    ParsedLineData() = default;
};

// helpers start

ElementType str2enum_et(std::string word) {
    if (word == "LOADING_RAMP") return ElementType::RAMP;
    if (word == "WORKER") return ElementType::WORKER;
    if (word == "STOREHOUSE") return ElementType::STOREHOUSE;
    if (word == "LINK") return ElementType::LINK;
    throw std::invalid_argument("Nie ma takiego typu elementu!");
}


PackageQueueType str2enum_pqt(std::string word) {
    if (word == "FIFO") return PackageQueueType::FIFO;
    if (word == "LIFO") return PackageQueueType::LIFO;
    throw std::invalid_argument("Nie ma takiej kolejki!");
}


std::string enum2str_pqt(PackageQueueType type_) {
    switch (type_) {
        case PackageQueueType::LIFO:
            return "LIFO";
        case PackageQueueType::FIFO:
            return "FIFO";
    }
    throw std::invalid_argument("Nie ma takiego typu kolejki!");
}

std::string enum2str_rt(ReceiverType type_) {
    switch (type_) {
        case ReceiverType::WORKER:
            return "worker";
        case ReceiverType::STOREHOUSE:
            return "store";
    }
    throw std::invalid_argument("Nie ma takiego odbiorcy!");
}

std::vector<std::string> split(std::string &words, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(words);

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}


IPackageReceiver* find_dest(Factory &factory_, std::vector<std::string> &dest) {
    if (dest[0] == "worker") {
        auto worker_it = factory_.find_worker_by_id(std::stoi(dest[1]));
        return &(*worker_it);
    }
    else if (dest[0] == "store") {
        auto store_it = factory_.find_storehouse_by_id(std::stoi(dest[1]));
        return &(*store_it);
    }
    else
        throw std::invalid_argument("Nie ma takiego typu odbiorcy!");
}


void load_link(Factory &factory_, std::vector<std::string> &src, std::vector<std::string> &dest) {
    if (src[0] == "ramp") {
        auto ramp_it = factory_.find_ramp_by_id(std::stoi(src[1]));
        auto receiver_ptr = find_dest(factory_, dest);
        ramp_it->receiver_preferences_.add_receiver(receiver_ptr);
    }
    else if (src[0] == "worker") {
        auto worker_it = factory_.find_worker_by_id(std::stoi(src[1]));
        auto receiver_ptr = find_dest(factory_, dest);
        worker_it->receiver_preferences_.add_receiver(receiver_ptr);
    }
    else
        throw std::invalid_argument("Nie ma takiego typu nadawcy!");
}

//helpers end

ParsedLineData parse_line(std::string line) {
    auto tmp = split(line, ' ');
    std::list<std::string> tokens(std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
    ParsedLineData node;
    node.element_type = str2enum_et(std::move(tokens.front()));
    tokens.pop_front();

    for (auto &elem : tokens) {
        auto key_and_value = split(elem, '=');
        node.parameters.insert({key_and_value[0], key_and_value[1]});
    }

    return node;
}


Factory load_factory_structure(std::istream &is) {
    Factory factory_;
    std::string line;

    while (std::getline(is, line)) {
        bool is_blank_line = line.empty() || isblank(line[0]);
        bool is_comment_line = !line.empty() && line[0] == ';';
        if (is_blank_line || is_comment_line) continue;
        auto parsed = parse_line(line);
        switch (parsed.element_type) {
            case ElementType::RAMP: {
                Ramp ramp_(std::stoi(parsed.parameters["id"]), std::stoi(parsed.parameters["delivery-interval"]));
                factory_.add_ramp(std::move(ramp_));
                break;
            }
            case ElementType::WORKER: {
                Worker worker_(std::stoi(parsed.parameters["id"]), std::stoi(parsed.parameters["processing-time"]),
                               std::make_unique<PackageQueue>(str2enum_pqt(parsed.parameters["queue-type"])));
                factory_.add_worker(std::move(worker_));
                break;
            }
            case ElementType::STOREHOUSE: {
                Storehouse storehouse_(std::stoi(parsed.parameters["id"]));
                factory_.add_storehouse(std::move(storehouse_));
                break;
            }
            case ElementType::LINK: {
                auto src_value = split(parsed.parameters["src"], '-');
                auto dest_value = split(parsed.parameters["dest"], '-');
                load_link(factory_, src_value, dest_value);
                break;
            }
        }
    }

    return factory_;
}


void save_factory_structure(Factory& factory, std::ostream& os) {
    std::ostringstream tmp;
    os << "; == LOADING RAMPS ==" << std::endl << std::endl;
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        os << "LOADING_RAMP id=" << it->get_id() << " delivery-interval=" << it->get_delivery_interval() << std::endl;
        for (auto elem : it->receiver_preferences_.get_preferences()) {
            tmp << "LINK src=ramp-" << it->get_id() << " dest=" << enum2str_rt(elem.first->get_receiver_type())
                << "-" << elem.first->get_id() << std::endl;
        }
        tmp << std::endl;
    }
    os << std::endl << "; == WORKERS ==" << std::endl << std::endl;
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        os << "WORKER id=" << it->get_id() << " processing-time=" << it->get_processing_duration() << " queue-type="
           << enum2str_pqt(it->get_queue()->get_queue_type()) << std::endl;
        for (auto elem : it->receiver_preferences_.get_preferences()) {
            tmp << "LINK src=worker-" << it->get_id() << " dest=" << enum2str_rt(elem.first->get_receiver_type())
                << "-" << elem.first->get_id() << std::endl;
        }
        tmp << std::endl;
    }
    os << std::endl << "; == STOREHOUSES ==" << std::endl << std::endl;
    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) {
        os << "STOREHOUSE id=" << it->get_id() << std::endl;
    }
    os << std::endl << "; == LINKS ==" << std::endl << std::endl;
    os << tmp.str();
}