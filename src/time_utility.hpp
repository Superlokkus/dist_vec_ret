/*! @file time_utility.hpp
 *
 */

#ifndef DIST_VEC_RET_TIME_UTILITY_HPP
#define DIST_VEC_RET_TIME_UTILITY_HPP

#include <string>
#include <chrono>

namespace information_retrieval {

    /*! @brief Convenience utility for time profiling code, this base class will log to std::cout
     *
     */
    class time_utility final {
    public:

        /*!
         * @param name_of_computation For logging and organizational purposes
         * @param auto_start Start the timer with contruction
         */
        explicit time_utility(std::string name_of_computation, bool auto_start = true);


        /*! Take the for the step and write it to std::cout
         * @attention If the timer wasn't started before or had been stopped, then this function will start it
         *
         * @param step_name Name of the step
         */
        void checkpoint(const std::string &step_name);

        void checkpoint(const char *const step_name = "");

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


    private:
        bool started_{false};
        std::string name_;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_tp_;
    };

}
#endif //DIST_VEC_RET_TIME_UTILITY_HPP
