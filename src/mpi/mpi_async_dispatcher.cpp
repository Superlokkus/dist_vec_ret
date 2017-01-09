/*! @file mpi_async_dispatcher.cpp
 *
 */

#include "mpi_async_dispatcher.hpp"

#include <atomic>
#include <future>
#include <vector>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <algorithm>
#include <iterator>

struct mpi::mpi_async_dispatcher::impl {
    using state_t = std::pair<mpi::mpi_async_dispatcher::request_t, mpi::mpi_async_dispatcher::callback_t>;
    std::atomic<bool> run_{false};
    std::future<void> dispatch_future_;

    void dispatch_loop();

    std::deque<state_t> dispatch_requests_; ///@note profile with std::vector in comparesion

    std::mutex to_be_moved_into_dispatch_requests_mutex_;
    std::condition_variable to_be_moved_into_dispatch_requests_condition_variable_;
    std::vector<state_t> to_be_moved_into_dispatch_requests_;
};

void mpi::mpi_async_dispatcher::impl::dispatch_loop() {
    while (this->run_ || !dispatch_requests_.empty()) {
        for (auto current = dispatch_requests_.begin(); current != dispatch_requests_.end();) {
            if (!current->first->test()) {
                ++current;
            } else {
                current->first->wait();
                if (current->second) {
                    current->second();
                }
                current = dispatch_requests_.erase(current);
            }
        }
        std::unique_lock<std::mutex> lock{to_be_moved_into_dispatch_requests_mutex_};
        std::move(to_be_moved_into_dispatch_requests_.begin(),
                  to_be_moved_into_dispatch_requests_.end(),
                  std::back_inserter(dispatch_requests_));
        if (dispatch_requests_.empty()) {
            to_be_moved_into_dispatch_requests_condition_variable_.wait(
                    lock); // Checking for spurious wakeups not necessary
        }
    };
}

mpi::mpi_async_dispatcher::mpi_async_dispatcher(bool auto_start) : impl_(new impl()) {
    if (auto_start) {
        this->start();
    }
}

mpi::mpi_async_dispatcher::~mpi_async_dispatcher() {
    this->stop();
}

bool mpi::mpi_async_dispatcher::running() const {
    return this->impl_->run_;
}

void mpi::mpi_async_dispatcher::add_request(mpi::mpi_async_dispatcher::request_t &&request,
                                            const std::function<void()> &callback) {
    std::unique_lock<std::mutex> lock{this->impl_->to_be_moved_into_dispatch_requests_mutex_};
    this->impl_->to_be_moved_into_dispatch_requests_.push_back(std::make_pair(std::move(request), callback));
    lock.unlock();
    this->impl_->to_be_moved_into_dispatch_requests_condition_variable_.notify_one();
}

void mpi::mpi_async_dispatcher::add_request(mpi::mpi_async_dispatcher::request_t &&request,
                                            std::function<void()> &&callback) {
    std::unique_lock<std::mutex> lock{this->impl_->to_be_moved_into_dispatch_requests_mutex_};
    this->impl_->to_be_moved_into_dispatch_requests_.push_back(std::make_pair(std::move(request), std::move(callback)));
    lock.unlock();
    this->impl_->to_be_moved_into_dispatch_requests_condition_variable_.notify_one();
}

void mpi::mpi_async_dispatcher::start() {
    if (!running()) {
        this->impl_->run_ = true;
        this->impl_->dispatch_future_ = std::async(std::launch::async, &impl::dispatch_loop, impl_.get());
    }
}

void mpi::mpi_async_dispatcher::stop() noexcept {
    if (this->running()) {
        this->impl_->run_ = false;
        this->impl_->dispatch_future_.get();
    }
}

