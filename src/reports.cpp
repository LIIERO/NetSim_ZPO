#include "reports.hpp"



void generate_structure_report(const Factory& f, std::ostream& os){
    os<<"\n";

    os<<"== LOADING RAMPS ==\n\n";

    auto ramp_it = f.ramp_cbegin();
    if (ramp_it == f.ramp_cend()){
        os<<"(empty)\n\n";
    }
    else{
        std::vector<ElementID> rampsID;
        while (ramp_it != f.ramp_cend()){
            rampsID.push_back(ramp_it->get_id());
            ramp_it++;
        }
        std::sort(rampsID.begin(), rampsID.end());
        for (auto id:rampsID){
            ramp_it = f.find_ramp_by_id(id);
            os<<"LOADING RAMP #"<<ramp_it->get_id()<<'\n';
            os<<"  Delivery interval: "<<ramp_it->get_delivery_interval();
            os<<"\n  Receivers:\n";
            std::vector<ElementID> workersID;
            std::vector<ElementID> storehausesID;
            for(auto receiver : ramp_it->receiver_preferences_ ){

                switch(receiver.first->get_receiver_type()){
                    case ReceiverType::WORKER:
                        workersID.push_back(receiver.first->get_id());
                        break;
                    case ReceiverType::STOREHOUSE:
                        storehausesID.push_back(receiver.first->get_id());
                        break;
                };
            }

            std::sort(storehausesID.begin(), storehausesID.end());
            std::sort(workersID.begin(),workersID.end());
            for(auto storehauseID: storehausesID){
                os<<"    ";
                os<<"storehouse #"<<storehauseID<<"\n";
            }
            for(auto workID: workersID){
                os<<"    ";
                os<<"worker #"<<workID<<"\n";
            }


            os<<"\n";
        }
    }

    os<<"\n";

    os<<"== WORKERS ==\n\n";

    auto worker_it = f.worker_cbegin();
    if (worker_it == f.worker_cend()){
        os<<"(empty)\n\n";
    }
    else{
        std::vector<ElementID> workerID;
        while (worker_it != f.worker_cend()){
            workerID.push_back(worker_it->get_id());
            worker_it++;
        }
        std::sort(workerID.begin(), workerID.end());
        for (auto id:workerID){
            worker_it = f.find_worker_by_id(id);
            os<<"WORKER #"<<worker_it->get_id()<<'\n';
            os<<"  Processing time: "<<worker_it->get_processing_duration();
            os<<"\n  Queue type: ";
            switch(worker_it->get_queue()->get_queue_type()){
                case PackageQueueType::LIFO:
                    os<<"LIFO\n";
                    break;
                case PackageQueueType::FIFO:
                    os<<"FIFO\n";
                    break;
            }
            os<<"  Receivers:\n";
            std::vector<ElementID> workersID;
            std::vector<ElementID> storehausesID;
            for(auto receiver : worker_it->receiver_preferences_ ){

                switch(receiver.first->get_receiver_type()){
                    case ReceiverType::WORKER:
                        workersID.push_back(receiver.first->get_id());
                        break;
                    case ReceiverType::STOREHOUSE:
                        storehausesID.push_back(receiver.first->get_id());
                        break;
                };
            }
            std::sort(storehausesID.begin(), storehausesID.end());
            std::sort(workersID.begin(),workersID.end());
            for(auto storehauseID: storehausesID){
                os<<"    ";
                os<<"storehouse #"<<storehauseID<<"\n";
            }
            for(auto workID: workersID){
                os<<"    ";
                os<<"worker #"<<workID<<"\n";
            }
            os<<'\n';
        }
    }

    os<<"\n";

    os<<"== STOREHOUSES ==\n";

    auto storehouse_it = f.storehouse_cbegin();
    if (storehouse_it == f.storehouse_cend()){
        os<<"(empty)\n";
    }
    else{
        std::vector<ElementID> storehausesID;
        while (storehouse_it != f.storehouse_cend()){
            storehausesID.push_back(storehouse_it->get_id());
            storehouse_it ++;
        }
        std::sort(storehausesID.begin(), storehausesID.end());
        for (auto id:storehausesID){
            storehouse_it  = f.find_storehouse_by_id(id);
            os<<'\n';
            os<<"STOREHOUSE #"<<storehouse_it->get_id();
            os<<'\n';
        }
    }
    os<<"\n";
    return;

}




void generate_simulation_turn_report(const Factory &  f, std::ostream& os, Time t){
    os<<"=== [ Turn: "<<t<<" ] ===\n\n";

    os<<"== WORKERS ==\n\n";

    auto worker_it = f.worker_cbegin();
    if (worker_it != f.worker_cend()){
        while (worker_it != f.worker_cend()){
            os<<"WORKER #"<<worker_it->get_id()<<'\n';

            os<<"  PBuffer: ";
            if((worker_it->get_processing_buffer()) != std::nullopt) {
                os<<'#'<<(worker_it->get_processing_buffer())->get_id() <<" (pt = "<<(t -worker_it->get_package_processing_start_time() + 1)<<")\n";
            }
            else{
                os<<"(empty)\n";
            }

            os<<"  Queue: ";
            if(worker_it->get_queue()->empty()){
                os<<"(empty)\n";
            }
            else {
                auto it = worker_it->get_queue()->cbegin();
                os<<"#"<<it->get_id();
                it++;
                for (;it != worker_it->get_queue()->cend();it++) {
                    os<<", #"<<it->get_id();
                }
                os<<'\n';
            }
            os<<"  SBuffer: ";
            if((worker_it->get_sending_buffer()) != std::nullopt) {
                os<<'#'<<(worker_it->get_sending_buffer())->get_id() << '\n';
            }
            else{
                os<<"(empty)\n";
            }
            os<<'\n';
            worker_it++;
        }
    }

    os<<"\n";

    os<<"== STOREHOUSES ==\n";

    auto storehouse_it = f.storehouse_cbegin();
    if (storehouse_it != f.storehouse_cend()){
        while (storehouse_it != f.storehouse_cend()){
            os<<'\n';
            os<<"STOREHOUSE #"<<storehouse_it->get_id()<<'\n';
            os<<"  Stock: ";
            if (storehouse_it->cbegin() == storehouse_it->cend()){
                os<<"(empty)\n";
            }
            else{
                auto queue_it = storehouse_it->cbegin();
                os<<"#"<<queue_it->get_id()<<'\n';
                queue_it++;
                while (queue_it != storehouse_it->cend()){
                    os<<", #"<<queue_it->get_id()<<'\n';
                    queue_it++;
                }
            }
            storehouse_it++;
        }
    }
    os<<'\n';
    return;

}