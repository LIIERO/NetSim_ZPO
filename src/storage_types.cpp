#include "storage_types.hpp"
#include "types.hpp"
#include "package.hpp"

Package PackageQueue::pop() {
    if(queue_type_ == PackageQueueType::LIFO) {
        auto back = std::move(package_queue_.back());
        package_queue_.pop_back();
        return back;
    } else if(queue_type_ == PackageQueueType::FIFO) {
        auto front = std::move(package_queue_.front());
        package_queue_.pop_front();

        return front;
    } else {
        return Package();
    }
}