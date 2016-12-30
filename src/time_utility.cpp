/*! @file time_utility.cpp
 *
 */

#include "time_utility.hpp"

namespace information_retrieval {

    time_utility::time_utility(std::string name_of_computation, bool auto_start) :
            name_{std::move(name_of_computation)} {
        event_times_.reserve(32);
        if (auto_start) {
            this->reset();
        }
    }


    void time_utility::reset() {
        started_ = true;
        event_times_.clear();
        start_tp_ = std::chrono::high_resolution_clock::now();
        last_step_tp_ = start_tp_;
    }

    void time_utility::stop() {
        const std::chrono::high_resolution_clock::duration total_duration =
                std::chrono::high_resolution_clock::now() - start_tp_;
        this->event_times_.emplace_back(name_, std::move(total_duration));
        started_ = false;
    }

    void time_utility::checkpoint(const std::string &step_name) {
        if (!started_)
            this->reset();
        const auto now = clock::now();
        const clock::duration step_duration =
                now - last_step_tp_;
        last_step_tp_ = std::move(now);
        this->event_times_.emplace_back(step_name, std::move(step_duration));
    }

    void time_utility::checkpoint(std::string &&step_name) {
        if (!started_)
            this->reset();
        const std::chrono::high_resolution_clock::duration step_duration =
                std::chrono::high_resolution_clock::now() - start_tp_;
        this->event_times_.emplace_back(std::move(step_name), std::move(step_duration));
    }

    std::ostream &operator<<(std::ostream &os, const time_utility &util) {
        os << "Started operation " << util.name_ << "\n";

        for (auto i = util.event_times_.cbegin(), end = util.event_times_.cend(); i != end; ++i) {
            if (!util.started_ && i == end - 1) {
                os << "Operation " << util.name_ << " took " <<
                   std::chrono::duration_cast<std::chrono::milliseconds>(i->second).count()
                   << " ms"
                   << "\n";
            } else {
                os << "Operations " << util.name_ << " step " << i->first << " took " <<
                   std::chrono::duration_cast<std::chrono::milliseconds>(i->second).count()
                   << " ms"
                   << "\n";
            }
        }

        return os;
    }
}
