#include <iostream>
#include <thread>
#include <future>
#include <algorithm>
#include <iterator>
#include <numeric>

class Parallel_for_bodge{
public:
    template <typename Iter, typename T>
    void for_each(Iter start, Iter end, void (*f)(T&)){
    std::for_each(start, end, f);
}
};

template <typename Iter, typename T>
void parallel_for_each(Iter begin, Iter end, void (*f)(int&)){
    std::vector<std::future<void>> futures;
    auto num_threads = std::thread::hardware_concurrency();
    Parallel_for_bodge F;
    auto delta = std::distance(begin, end) / num_threads;
    for (auto i = 0u; i+1 <= num_threads; i++){
        futures.push_back(std::async(std::launch::async,
         &Parallel_for_bodge::for_each<Iter, T>,
          &F, begin + delta * i, begin + delta * (i + 1), f));
    } 
    for (auto i = 0; i+1 < num_threads; i++){
        futures[i].wait();
    }
}

void d(int &a){
    a+=2;
}


int main(){
    std::vector<int> v(10);
    std::iota(std::begin(v), std::end(v), 1);
    parallel_for_each<std::vector<int>::iterator, int>(std::begin(v), std::end(v), d);

    for (auto i : v){
        std::cout << i << ' ';
    }
    std::cout << std::endl;

    return 0;
}