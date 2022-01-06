#include "nodes.hpp"
#include "storage_types.hpp"

void PackageSender::send_package() {
    if (buffer_) {
        receiver_preferences_.choose_receiver()->receive_package(std::move(*buffer_));
        buffer_.reset();
    }
}

void Ramp::delivery_goods(Time t) {
    // Czas liczony od jedynki czyli minus jeden żeby było git
    if ((t - 1) % di_ == 0) {
        Package pack;
        push_package(std::move(pack));
        send_package();
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
