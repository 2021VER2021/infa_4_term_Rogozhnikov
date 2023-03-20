#include <chrono>


/// @brief 
/// It is the Timer class, allow's you to measure time your code runs
/// @tparam TimeType - time unit of mesurement
template<typename TimeType>
class Timer{
public:
    Timer():Timer(false){}

    Timer(bool is_starting){
        Timer::Start();
        current_condition = is_starting;
        duration = duration.zero();
    }
    ~Timer(){
        GetTime();
    }

    /// @brief use this method to Start the timer
    void Start(){
        start = std::chrono::steady_clock::now();
        current_condition = true;
        duration = duration.zero();
    }

    /// @brief use this method to show the current time on a timer, without interupting it
    TimeType GetTime(){
        if (current_condition){
            duration += std::chrono::steady_clock::now() - start;
            start = std::chrono::steady_clock::now();
        } 
        std::cout << std::chrono::duration_cast<TimeType>(duration).count() << std::endl;
        return std::chrono::duration_cast<TimeType>(duration);
    }

    /// @brief use this method to stop the timer and show the current time on a timer
    void Stop(){
        GetTime();
        current_condition = false;
        duration = duration.zero();
    }

    /// @brief use this method to pause the timer, it will remember previous duration
    void Pause(){
        duration += std::chrono::steady_clock::now() - start;
        current_condition = false;
    }

    /// @brief use this method to continue timer after pausing it, or to start the timer, if it was stopped
    void Continue(){
        start = std::chrono::steady_clock::now();
        current_condition = true;
    }
private:
    bool current_condition;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::duration duration;
};