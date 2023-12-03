#include <chrono>
#include <thread>
#include <iostream>

class Timer {
  public:
    Timer() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();

        int hours = duration / 3600;
        int minutes = (duration % 3600) / 60;
        int seconds = duration % 60;

        std::cout << "\033[1;34m\nProgram started at: " << getFormattedTime(start_time)
                  << "\nProgram ended at: " << getFormattedTime(end_time)
                  << "\nProgram duration: " << hours << " hours, " << minutes << " minutes, " << seconds << " seconds\n\033[0m\n";
    }

    std::chrono::high_resolution_clock::time_point getStartTime() const {
        return start_time;
    }

private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;

    std::string getFormattedTime(const std::chrono::high_resolution_clock::time_point& time) {
        auto time_t = std::chrono::system_clock::to_time_t(time);
        std::string time_str = std::ctime(&time_t);
        time_str.pop_back();  // Remove the trailing newline character
        return time_str;
    }
};