/*
 * server.cpp
 *
 *  Created on: Dec 25, 2018
 *      Author: yusaku
 */

#include <iostream>
#include <boost/asio.hpp>

#include "tcp_server.hpp"

int main()
{
    try
    {
        boost::asio::io_service io_service;
        tcp_server server(io_service);
        io_service.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
    }
}

