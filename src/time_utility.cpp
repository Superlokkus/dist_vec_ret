/*! @file time_utility.cpp
 *
 */

#include "time_utility.hpp"
#include <iostream>

namespace information_retrieval {

    time_utility::time_utility(std::string name_of_computation, bool auto_start) :
            name_{std::move(name_of_computation)} {
        if (auto_start) {
            this->reset();
        }
    }


    void time_utility::reset() {
        start_tp_ = std::chrono::high_resolution_clock::now();
        started_ = true;
        std::cout << "Started operation " << this->name_ << "\n";
    }

    void time_utility::stop() {
        const std::chrono::high_resolution_clock::duration total_duration =
                std::chrono::high_resolution_clock::now() - start_tp_;
        started_ = false;
        std::cout << "Operation " << this->name_ << " took " <<
                  std::chrono::duration_cast<std::chrono::milliseconds>(total_duration).count()
                  << " ms"
                  << "\n";
    }

    void time_utility::checkpoint(const std::string &step_name) {
        this->checkpoint(step_name.c_str());
    }

    void time_utility::checkpoint(const char *const step_name) {
        if (!started_)
            this->reset();
        const std::chrono::high_resolution_clock::duration step_duration =
                std::chrono::high_resolution_clock::now() - start_tp_;
        std::cout << "Operations " << name_ << " step " << step_name << " took " <<
                  std::chrono::duration_cast<std::chrono::milliseconds>(step_duration).count()
                  << " ms"
                  << "\n";
    }
}
