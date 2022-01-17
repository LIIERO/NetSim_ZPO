
# include <iostream>
# include "factory.hpp"

std::string rt_to_str(ReceiverType typ) {
    switch(typ) {
        case ReceiverType::WORKER:
            return "worker";
        case ReceiverType::STOREHOUSE:
            return "storehouse";
    }
    throw std::invalid_argument("Taki odbiorca nie istnieje!");
}

void generate_structure_report(const Factory& f, std::ostream& os) {
    // Ramps
    os << "\n== LOADING RAMPS ==\n\n";
    for (auto ramp_it = f.ramp_cbegin(); ramp_it != f.ramp_cend(); ++ramp_it) {
        os << "LOADING RAMP #" << ramp_it->get_id() << "\n  Delivery interval: " << ramp_it->get_delivery_interval() << std::endl;
        os << "  Receivers:\n" << std::endl;
        for (auto elements : ramp_it->receiver_preferences_.get_preferences()) {
            os << "    " << rt_to_str(elements.first->get_receiver_type()) << " #" << elements.first->get_id() << std::endl;
        }
    }

    // Workers
    os << "\n== WORKERS ==\n\n";
    for (auto worker_it = f.worker_cbegin(); worker_it != f.worker_cend(); ++worker_it) {
        os << "WORKER #" << worker_it->get_id() << "\n  Processing time: " << worker_it->get_processing_duration() << std::endl;
        if(worker_it->get_queue_type() == PackageQueueType::FIFO) { os << "  Queue type: FIFO"; } else { os << "  Queue type: LIFO"; }
        os << "\n  Receivers:\n" << std::endl;
        for (auto elements : worker_it->receiver_preferences_.get_preferences()) {
            os << "    " << rt_to_str(elements.first->get_receiver_type()) << " #" << elements.first->get_id() << std::endl;
        }
    }

    // Storehouses
    os << "\n== STOREHOUSES ==\n\n";
    for (auto storehouse_it = f.storehouse_cbegin(); storehouse_it != f.storehouse_cend(); ++storehouse_it) {
        os << "STOREHOUSE #" << storehouse_it->get_id() << std::endl;
    }
}


void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t) { // TODO: Skończyć
    os << "=== [ Turn: " << t << " ] ===" << std::endl;

    // Workers
    os << "\n== WORKERS ==\n\n";
    for (auto worker_it = f.worker_cbegin(); worker_it != f.worker_cend(); ++worker_it) {
        os << "WORKER #" << worker_it->get_id() << std::endl;

        os << "  PBuffer: " << std::endl;
        os << "  Queue: " << std::endl;
        os << "  SBuffer: " << std::endl;
    }

    // Storehouses
    os << "\n== STOREHOUSES ==\n\n";
    for (auto storehouse_it = f.storehouse_cbegin(); storehouse_it != f.storehouse_cend(); ++storehouse_it) {
        os << "STOREHOUSE #" << storehouse_it->get_id() << std::endl;
        os << "  Stock: ";
    }
}
