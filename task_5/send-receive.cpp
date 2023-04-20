#include "boost\asio.hpp"
#include <iostream>
#include <thread>

void send(  unsigned port, std::string ip, std::string name){
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::address::from_string(ip),
        port
    );
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service, endpoint.protocol());
    socket.connect(endpoint);
    auto message = std::string("Connected");

    do {boost::asio::write(socket, boost::asio::buffer(name + " said: " + message + '\n'));
    } while (std::getline(std::cin, message));

    boost::asio::write(socket, boost::asio::buffer("EXIT\n"));
}

void receive(unsigned port, std::string ip){
    
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::address::from_string(ip),
        port
    );
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service);
    boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

    acceptor.bind(endpoint);

    acceptor.listen();

    acceptor.accept(socket);

    std::string message;
    for (;true;) {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, '\n');

        std::istream in(&buffer);
        std::getline(in, message);

        if (message == "EXIT") {
            break;
        }
        if (message != "Connected"){
            std::cout << message << std::endl;
        }
        else{
            std::cout << message;
        }
    }
}

int main(){
    std::string str;
    std::cout << std::thread::hardware_concurrency() << std::endl;
    std::cout << "Enter person number (1 or 2)" << std::endl;
    std::cin >> str;
    unsigned port;
    unsigned port_2;
    if (str == "1"){
        port = 65001u;
        port_2 = 65002u;
    }
    else{
        port_2 = 65001u;
        port = 65002u;
    }

    const std::string ip = "127.0.0.1"; // - ip of the local mashine

    std::thread th_2(receive, port, ip);
    std::cout << "receiver is ready, write your name" << std::endl;
    std::cin >> str; 
    send(port_2, ip, str);
    th_2.join();
}