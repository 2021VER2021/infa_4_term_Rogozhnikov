#include <iostream>
#include <thread>
#include <future>
#include <algorithm>
#include <iterator>
#include <numeric>

class Parallel_for{
public:
    template <typename Iter>
    void parallel_for_each_block(Iter start, Iter end, void (*f)(int&)){
    std::for_each(start, end, f);
}
};


void d(int &a){
    a+=2;
}


int main(){
    std::vector<int> v(10);
    std::iota(std::begin(v), std::end(v), 1);
    std::vector<std::future<void>> futures;
    auto num_threads = std::thread::hardware_concurrency();
    std::cout << num_threads << std::endl;
    Parallel_for F;

    auto delta = std::distance(std::begin(v), std::end(v))/num_threads;
    for (auto i = 0u; i+1 <= num_threads; i++){
        futures.push_back(std::async(std::launch::async, &Parallel_for::parallel_for_each_block<std::vector<int>::iterator>, &F, std::begin(v) + delta*i, std::begin(v) + delta*(i+1),d));
    } 
    for (auto i = 0; i+1 < num_threads; i++){
        futures[i].wait();
    }
    for (auto i = 0; i+1 < num_threads; i++){
        futures[i].get();
    }
    for (auto i:v){
        std::cout << i << ' ';
    }
    std::cout << std::endl;

    return 0;
}