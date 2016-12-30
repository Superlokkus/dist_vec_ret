/*! @file time_utility.hpp
 *
 */

#ifndef DIST_VEC_RET_TIME_UTILITY_HPP
#define DIST_VEC_RET_TIME_UTILITY_HPP

#include <string>
#include <chrono>
#include <vector>
#include <ostream>

namespace information_retrieval {

    /*! @brief Convenience utility for time profiling code
     *
     */
    class time_utility final {
    public:
        using clock = std::chrono::high_resolution_clock;
        /*!
         * @param name_of_computation For logging and organizational purposes
         * @param auto_start Start the timer with contruction
         */
        explicit time_utility(std::string name_of_computation, bool auto_start = true);


        /*! Take the time for the step
         * @attention If the timer wasn't started before or had been stopped, then this function will start it
         *
         * @param step_name Name of the step
         */
        void checkpoint(const std::string &step_name = std::string{});

        void checkpoint(std::string &&step_name);

        /*! To reset i.e. start the timer,
         *
         */
        void reset();

        /*! To stop for the step and total time of the computation
         *
         */
        void stop();

        bool is_started() noexcept {
            return started_;
        }

        std::string name() {
            return name_;
        }

        /*!
         *
         * @return The events sorted from excluding start to last checkpoint/stop
         */
        std::vector<std::pair<std::string, clock::duration>> event_names_with_times() const {
            return this->event_times_;
        };

        friend std::ostream &operator<<(std::ostream &os, const time_utility &util);
    private:
        bool started_{false};
        std::string name_;
        std::chrono::time_point<clock> start_tp_;
        std::chrono::time_point<clock> last_step_tp_;
        std::vector<std::pair<std::string, clock::duration>> event_times_;
    };

    std::ostream &operator<<(std::ostream &os, const time_utility &util);

}
#endif //DIST_VEC_RET_TIME_UTILITY_HPP
