#include <iostream>
#include<array>
#include<vector>
#include<deque>
#include<list>
#include <set>
#include <random>
#include <math.h>
#include <chrono>
#include <algorithm>
#include<numeric>
#include <iterator>

std::ostream& operator << (std::ostream& os, std::vector<int> vector){
    std::copy(std::begin(vector), std::end(vector), std::ostream_iterator<int>(os, " "));
    return os;
}

std::istream& operator >> (std::istream& is, std::vector<int>& v){
    std::copy(std::istream_iterator<int>(is),
     std::istream_iterator<int>(), std::back_inserter(v));
    return is;
}

int main(){
    std::vector<int> p1(10);
    std::mt19937_64 mt(std::chrono::system_clock::to_time_t(
        std::chrono::time_point<std::chrono::system_clock>{}));
    std::uniform_int_distribution<int> dist_1(0, p1.size());
    std::uniform_int_distribution<int> dist_2(0, 20);
    std::uniform_int_distribution<int> coin_flip(0, 1);

    int counter = 0;
    std::generate(std::begin(p1), std::end(p1),[&](){return ++counter;});

    std::cout << "1. Created: " << p1 << std::endl;
    std::cout << "Add some numbers: ";
    std::cin >> p1;
    std::cout << "2. " << p1 << std::endl;
    
    std::shuffle(std::begin(p1), std::end(p1), mt);
    std::cout << "3. Shuffled: " << p1 << std::endl;

    std::set<int> s1(p1.begin(), p1.end());
    for (auto i = 0; i < p1.size(); i++){
        if (s1.find(p1[i]) == s1.end()) {
            p1.erase(std::begin(p1) + i);
            i--;
        }
        else{
            s1.erase(s1.find(p1[i]));
        }
    }
    std::cout << "4. Deleted: " << p1 << std::endl;

    std::cout << "5. Odd: " <<
    std::accumulate(std::begin(p1), std::end(p1), 0ll,
     [](auto lhs, auto rhs){return (rhs % 2) ? 1 + lhs : lhs;}) << std::endl;

    std::cout << "6. Max: " << *std::max_element(std::begin(p1), std::end(p1)) 
    << "   Min: " << *std::min_element(std::begin(p1), std::end(p1)) << std::endl;

    auto iter = std::find_if(std::begin(p1), std::end(p1), [](auto value){
    if ((value == 1) || (value == 2)){
        return true;
    }
    if (value <= 0) {
        return false;
    }
    bool result = true;
    for (auto i = 2; i < round(std::sqrt(value))+1; i++){
        if (value % i == 0){
            result = false;
            break;
        }
    }
    return result;
    });
    if (iter == std::end(p1))   std::cout << "7. No prime number" << std::endl;
    else                        std::cout << "7. Prime number: " << *iter << std::endl;

    std::transform(std::begin(p1), std::end(p1), std::begin(p1), [](auto i){return i * i;});
    std::cout << "8. Squares: " << p1 << std::endl;

    std::vector<int> p2;
    std::transform(std::begin(p1), std::end(p1), std::back_inserter(p2), [&mt, &dist_2](auto i){
        auto tmp = dist_2(mt);
        return tmp;
    });
    std::cout << "9. Sequence 2: " << p2 << std::endl;

    std::cout << "10. Sum 2: " << std::accumulate(std::begin(p2), std::end(p2), 0ll) << std::endl;

    std::transform(std::begin(p2), std::end(p2), std::begin(p2), [&mt, &coin_flip](auto i){
        return (coin_flip(mt)) ? 1 : i;});
    std::cout << "11. Modified: " << p2 << std::endl;

    std::vector<int> p3;
    std::transform(std::begin(p1), std::end(p1), std::begin(p2),
    std::back_inserter(p3), [](auto i, auto j){return i - j;});
    std::cout << "12. Diff: " << p3 << std::endl;

    std::transform(std::begin(p3), std::end(p3), std::begin(p3), [](auto i){return i < 0 ? 0 : i;});
    std::cout << "13. Replace negative: " << p3 << std::endl;

    p3.erase(std::remove_if(std::begin(p3), std::end(p3), [](auto i){return i == 0;}), std::end(p3));
    std::cout << "14. Remove zeros: " << p3 << std::endl;

    std::reverse(std::begin(p3), std::end(p3));
    std::cout << "15. Reverse: " << p3 << std::endl;

    if (p3.size() > 2){
        auto a1 = std::max_element(std::begin(p3), std::end(p3));
        auto a2 = std::max_element(std::begin(p3), std::end(p3), [&a1](auto j, auto i){
            if (i == *a1){
                return false;
            }
            if (j == *a1){
                return true;
            }
            return i > j;});
            auto a3 = std::max_element(std::begin(p3), std::end(p3), [&a1, &a2](auto j, auto i){
            if ((i == *a1) || (i == *a2)){
                return false;
            }
            if ((j == *a1)|| (j == *a2)){
                return true;
            }
            return i > j;});
            std::cout << "16. Top 3: " << *a1 << " " << *a2 << " " << *a3 << std::endl;
    }
    else std::cout << "16. Less than three elements" << std::endl;

    std::sort(std::begin(p1), std::end(p1));
    std::sort(std::begin(p2), std::end(p2));
    std::cout << "17. Sorted: p1 ->" << p1 << std::endl;
    std::cout << "            p2 ->" << p2 << std::endl;

    std::vector<int> p4;
    std::merge(std::begin(p1), std::end(p1), std::begin(p2), std::end(p2), std::back_inserter(p4));
    std::cout << "18. Merged: " << p4 << std::endl;

    auto g1 = std::lower_bound(std::begin(p4), std::end(p4), 1);
    auto g2 = std::upper_bound(std::begin(p4), std::end(p4), 1);
    std::cout << "19. Range(index): [" << (g1 - std::begin(p4)) << ", " << (g2 - std::begin(p4)) << ")" << std::endl; 

    std::cout << "20. Sequences: " << std::endl;
    std::cout << "P1 -> " <<  p1 << std::endl;
    std::cout << "P2 -> " <<  p2 << std::endl;
    std::cout << "P3 -> " <<  p3 << std::endl;
    std::cout << "P4 -> " <<  p4 << std::endl;

    
    return 0;
}