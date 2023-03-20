#include <iostream>
#include <tuple>
#include <vector>
#include "include/nlohmann/json.hpp"
#include "include/Timer.h"
#include <fstream>
#include <thread>
#include <algorithm>
#include <numeric>
#include <filesystem>
#include <sstream>

/// @brief support class
class Data{
public:
    nlohmann::json to_json(std::tuple<int, double> t) const{
        return {
            {"NUM ", std::get<0>(t)},
            {"VAL ", std::get<1>(t)},
        };
    }
    void set_data(int n, double v){
        data.push_back(std::tuple<int, double>(n, v));
    }
    void set_data(std::tuple<int, double> t){
        data.push_back(t);
    }
    std::vector<std::tuple<int, double>> get_data() const{
        return data;
    }
private:
     std::vector<std::tuple<int, double>> data;
};

template <typename Iter, typename T>
void accumulate_block(Iter begin, Iter end, T init, T& result){
    result = std::accumulate(begin, end, init);
}
template <typename Iter, typename T>
T parallel_accumulate(Iter begin, Iter end, T init, int num_threads){
    ///auto num_threads = std::thread::hardware_concurrency();
    auto block_size = std::distance(begin, end) / num_threads;
    std::vector<std::thread> threads;
    std::vector<T> results(num_threads - 1);
    for (auto i = 0u; i+1 < num_threads; i++){
        threads.push_back
        (
        std::thread
        (
        accumulate_block<Iter, T>
        ,std::next(begin, i * block_size),
        std::next(begin, (i + 1) * block_size), 
        0,
         std::ref(results[i])
        )
        );
    }
    T last_result;
    accumulate_block<Iter, T>(std::next(begin, (num_threads - 1)*block_size), end, 0, last_result);
    for (auto& thread: threads){
        thread.join();
    }
    return std::accumulate(std::begin(results), std::end(results), last_result);
}

int main(){
    Data my_data;
    auto number = 20;
    auto N = 1 << 25;

    auto num_threads = std::thread::hardware_concurrency();
    std::cout << "threads available: " << num_threads << std::endl;

    std::vector<int> numbers(N);
    std::iota(std::begin(numbers), std::end(numbers), 1);
    Timer<std::chrono::microseconds> T(true);
    for (auto i = 1; i < number+1; i++){
        T.Start();
        for (auto j = 0; j < 10; j++){
            std::cout << parallel_accumulate(std::begin(numbers), std::end(numbers), 0, i) << std::endl;
        }
        my_data.set_data(i, T.GetTime().count()/10);
    }
    nlohmann::json j(my_data.get_data());

    std::string file_name = "data.json";
    std::ofstream file(file_name);

    file << j.dump();
    file.close();

    return 0;
}