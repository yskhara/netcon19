/*
 * tcp_server.cpp
 *
 *  Created on: Jan 30, 2019
 *      Author: yusaku
 */

#include "tcp_server.hpp"

tcp_server::tcp_server(boost::asio::io_service& io_service) :
        _acceptor(io_service, tcp::endpoint(tcp::v4(), PORT_NUM))
{
    start_accept();
}

std::vector<tcp_connection::pointer> *tcp_server::get_connections(void)
{
    return &_connections;
}

void tcp_server::start_accept()
{
    tcp_connection::pointer new_connection = tcp_connection::create(_acceptor.get_io_service());

    _acceptor.async_accept(new_connection->socket(),
            boost::bind(&tcp_server::handle_accept, this, new_connection, boost::asio::placeholders::error));

    std::cout << "[waiting for new connection]" << std::endl;
}

void tcp_server::handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error)
{
    if (!error)
    {
        //new_connection->start();
        new_connection->shake_hands();
        std::cout << "[new connection established]" << std::endl;
        new_connection->_server = this;
        _connections.push_back(new_connection);
    }

    start_accept();
}

void tcp_server::broadcast(std::string str)
{
    for (tcp_connection::pointer &con : this->_connections)
    {
        con->send(str);
    }
}

