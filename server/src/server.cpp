/*
 * server.cpp
 *
 *  Created on: Dec 25, 2018
 *      Author: yusaku
 */

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <sys/types.h>   /* socket() */
//#include <sys/socket.h>  /* socket() */
//#include <netinet/in.h>  /* #include <sys/un.h>の代わり */
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>
#include <boost/asio.hpp>

#include <thread>

#include "tcp_server.hpp"

namespace asio = boost::asio;
using asio::ip::tcp;


int main()
{
    try
    {
        boost::asio::io_service io_service;
        tcp_server server(io_service);

        //std::thread t1([&io_service]{io_service.run();});
        //std::thread t2([&io_service]{io_service.run();});

        io_service.run();

        //t1.join();
        //t2.join();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
    }

}
/*

 int main()
 {
 int i;
 int fd1, fd2;
 struct sockaddr_in saddr;
 struct sockaddr_in caddr;
 int len;
 int ret;
 char buf[1024];

 boost::system::error_code error;

 boost::asio::steady_timer timer(io_service, std::chrono::seconds(1));


 acc.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
 acc.async_accept(soc, &on_accept);
 std::cout << "[waiting for connection]" << std::endl;

 io_service.run();

 exit(0);
 }
 */
