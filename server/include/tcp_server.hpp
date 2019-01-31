/*
 * server.hpp
 *
 *  Created on: Jan 30, 2019
 *      Author: yusaku
 */

#ifndef TCP_SERVER_HPP_
#define TCP_SERVER_HPP_

#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>


class tcp_server;

#include "tcp_connection.hpp"

namespace asio = boost::asio;
using asio::ip::tcp;
static constexpr uint PORT_NUM = 31400;

class tcp_server
{
public:
    tcp_server(boost::asio::io_service& io_service);
    std::vector<tcp_connection::pointer> *get_connections(void);

    void broadcast(std::string str);

private:
    void start_accept();
    void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error);

    tcp::acceptor _acceptor;
    std::vector<tcp_connection::pointer> _connections;
};

#endif /* TCP_SERVER_HPP_ */
