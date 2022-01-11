#include "nodes.hpp"
#include "storage_types.hpp"



void PackageSender::send_package() {
    if (buffer_) {
        receiver_preferences_.choose_receiver()->receive_package(std::move(*buffer_));
        buffer_.reset();
    }
}



void Ramp::deliver_goods(Time t) {
    // TODO: czy typ danych się będzie zgadzał
    // Czas liczony od jedynki czyli minus jeden żeby było git
    if ((t - 1) % di_ == 0) {
        Package pack;
        push_package(std::move(pack));
        //send_package();
    }
}



void Worker::do_work(Time t) {
    if (!q_->empty() && !buffer_) {
        // Pobranie nowego produktu i rozpoczęcie przetwarzania
        buffer_.emplace(q_->pop());
        time_ = t;
    } if (buffer_ && t >= time_ + pd_) {
        send_package();

        // Jeśli coś przekazał to zaczyna pracę nad następnym? Czy czeka kolejkę? Jeśli czeka to można usunąć
        if (!q_->empty()) {
            buffer_.emplace(q_->pop());
            time_ = t;
        } else buffer_.reset();
    }
}



void Worker::receive_package(Package&& package) {
    q_->push(std::move(package));
}

void Storehouse::receive_package(Package&& other_package) {
    d_->push(std::move(other_package));
}



void ReceiverPreferences::scale() {
    auto size = double(preferences_.size());

    for(auto& it : preferences_){
        it.second = 1/size;
    }
}



void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    preferences_.insert(std::make_pair(r, 1));
    scale();
}



void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    preferences_.erase(r);
    scale();
}

IPackageReceiver *ReceiverPreferences::choose_receiver() {
    auto choice = pg_();
    double sum = 0;

    for (auto& it : preferences_) {
        if (choice >= sum && choice < sum + it.second)
            return it.first;
        sum += it.second;
    }

    return nullptr;
}

