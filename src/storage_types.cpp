#include "storage_types.hpp"
#include "types.hpp"
#include "package.hpp"

Package PackageQueue::pop() {
    if(queue_type_ == PackageQueueType::LIFO) {
        auto back = std::move(pack_queue_.back());
        pack_queue_.pop_back();
        return back;
    } else if(queue_type_ == PackageQueueType::FIFO) {
        auto front = std::move(pack_queue_.front());
        pack_queue_.pop_front();

        return front;
    } else {
        return Package();
    }
}