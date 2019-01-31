/*
 * client.cpp
 *
 *  Created on: Dec 25, 2018
 *      Author: yusaku
 */



#include <iostream>
#include <thread>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

static constexpr uint PORT_NUM = 31400;

asio::io_service io_service;

tcp::socket soc(io_service);
asio::streambuf receive_buff_;

//void start_receive(void);
void on_receive(boost::system::error_code const& error, size_t bytes_transferred);

void start_receive(void)
{
    asio::async_read(soc, receive_buff_, asio::transfer_at_least(1), &on_receive);
}

void on_receive(boost::system::error_code const& error, size_t bytes_transferred)
{
    if ((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error))
    {
        // disconnected

        std::cout << "[connection lost]" << std::endl;
    }
    else
    {
        // read the data
        //std::cout << "[received new message from server]" << std::endl;

        std::string body = boost::asio::buffer_cast<const char*>(receive_buff_.data());

        auto size = receive_buff_.size();
        receive_buff_.consume(size);

        body = body.substr(0, size);


        if (body.length() > 1)
        {
            body.pop_back();
            std::cout << body << std::endl;
        }

        start_receive();
    }
}

int main(int argc, char *argv[])
{
#define BUF_LENGTH 1024
    char   buf[BUF_LENGTH];

    if (argc != 2){
        printf("Usage: ./client SERVER_NAME\n");
    //    exit(1);
    }

    std::vector<std::string> allArgs(argv + 1, argv + argc);

    //asio::io_service io_service;
    //tcp::socket socket(io_service);

    boost::system::error_code error;
    //socket.connect(tcp::endpoint(asio::ip::address::from_string(allArgs[0]), PORT_NUM), error);

    soc.connect(tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), PORT_NUM), error);

    if (error) {
        std::cout << "connect failed : " << error.message() << std::endl;
        exit(-1);
    }
    else {
        std::cout << "connected" << std::endl;
    }

    std::string name;

    std::cout << "enter your name: ";
    std::cin >> name;

    asio::write(soc, asio::buffer("JOIN+" + name + "\n"), error);

    if (error) {
        std::cout << "write failed : " << error.message() << std::endl;
        exit(-1);
    }

    asio::read_until(soc, receive_buff_, "\n", error);

    std::string body = boost::asio::buffer_cast<const char*>(receive_buff_.data());
    receive_buff_.consume(receive_buff_.size());

    if (error) {
        std::cout << "read failed : " << error.message() << std::endl;
        exit(-1);
    }
    else if(body.compare("ACK\n") != 0)
    {
        std::cout << "login failed : " << error.message() << std::endl;
        exit(-1);
    }

    start_receive();

    std::thread t1([&io_service]{io_service.run();});
    //std::thread t2([&io_service]{io_service.run();});


    std::cin.getline(buf, BUF_LENGTH);

    while (true) {

        boost::system::error_code error;
        //char buf[128];
        std::string msg;
        //std::cin >> msg;
        std::cin.getline(buf, BUF_LENGTH);
        msg = buf;
        asio::write(soc, asio::buffer("POST+" + msg + "\n"), error);


        if (error) {
            std::cout << "[failed to send message: " << error.message() << "]" << std::endl;
        }
        else {
            //std::cout << "[message sent]" << std::endl;
        }

        if(std::cin.eof())
        {
            std::cout << "[eof detected; terminating]" << std::endl;
            break;
        }
    }

    soc.cancel();
    t1.join();

    asio::write(soc, asio::buffer("QUIT+ \n"), error);

    //t2.join();

    exit(0);
}

