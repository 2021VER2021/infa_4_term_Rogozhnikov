#include <iostream>
#include "include/nlohmann/json.hpp"
#include <filesystem>
#include <fstream>
#include <regex>
#include <vector>
#include <algorithm>
#include <sstream>

class Person{
public:
    Person(std::string n, int a, std::string i){
        name = n;
        age = a;
        info = i;
    }

    friend std::ostream& operator << (std::ostream& os, const Person& p){
        os << p.name << " " << p.age << " " << p.info;
        return os;
    } 

    friend std::istream& operator >> (std::istream& is, Person& p){
        is >> p.name >> p.age >> p.info;
        return is;
    }

    nlohmann::json to_json() const {
        return {
            {"name", name},
            {"age", age},
            {"info", info}
        };
    }

    static Person from_json(const nlohmann::json& j){
        return Person(j["name"], j["age"], j["info"]);
    }

private:
    int age;
    std::string name;
    std::string info;
};

std::vector<std::string> find_matches(const std::string& text, std:: regex pattern){
    std::vector<std::string> matches;
    std::transform(std::sregex_iterator(std::begin(text),
    std::end(text), pattern), std::sregex_iterator(),
     std::back_inserter(matches), [](auto match){return match.str();});
     return matches;
}   

int main(){
    Person p1("",0,"");
    Person p2("",0,"");
    std::cout << "Enter two persons: " << std::endl;
    std::cin >> p1;
    std::cin >> p2;

    std::cout << "Person 1: "<< p1 << std::endl;
    std::cout << "Person 2: "<< p2 << std::endl;

    nlohmann::json j_1 = p1.to_json();
    nlohmann::json j_2 = p2.to_json();
    std::cout << "Converted to JSON: " << j_1.dump() << std::endl;

    Person p3 = Person::from_json(j_1);
    std::cout << "Converted from JSON: " << p3 << std::endl; 

    std::string file_name = "Json_1.txt";
    std::string dir_name = "JsonFiles";
    std::filesystem::create_directory("JsonFiles");

    std::filesystem::path dir_path = std::filesystem::current_path() / dir_name;
    std::filesystem::path file_path = dir_path / file_name;
    std::ofstream file(file_path);
    if (file.is_open()){
        file << j_1;
    }
    file.close();
    file_name = "Json_2.txt";
    file_path = dir_path / file_name;
    std::ofstream file_2(file_path);
    if (file_2.is_open()){
        file_2 << j_2;
    }
    file_2.close();

    /// regex

    std::regex pattern(R"(\w+([-+.]\w+)*@\w+\.(com|ru|edu|ua|uk|by))");
    std::fstream fin("text.txt", std::ios::in);
    std::stringstream ss;
    for (std::string tmp = ""; std::getline(fin, tmp); ss << tmp);
    auto text = ss.str();
    std::cout << text << std::endl;

    std::vector<std::string> mat = find_matches(text, pattern);
    std::cout << "Found emails: " << std::endl;
    for (auto s: mat){
        std::cout << s << std::endl;
    }
    fin.close();
    return 0;
}