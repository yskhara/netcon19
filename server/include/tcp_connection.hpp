/*
 * tcp_connection.hpp
 *
 *  Created on: Jan 30, 2019
 *      Author: yusaku
 */

#ifndef TCP_CONNECTION_HPP_
#define TCP_CONNECTION_HPP_

#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

class tcp_server;

#include "message.hpp"

namespace asio = boost::asio;
using asio::ip::tcp;

class tcp_connection: public boost::enable_shared_from_this<tcp_connection>
{
public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_service& io_service);
    tcp::socket& socket();
    void send(std::string str);
    void shake_hands();
    void start();

    tcp_server *_server;

private:
    tcp_connection(boost::asio::io_service& io_service);

    void handle_write(const boost::system::error_code& error);
    void handle_handshake(const boost::system::error_code& error);
    void handle_read(const boost::system::error_code& error);

    void send_nak(void);


    tcp::socket _socket;
    boost::asio::streambuf _message;

    std::string _name;
};

#include "tcp_server.hpp"


#endif /* TCP_CONNECTION_HPP_ */
