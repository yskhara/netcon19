/*
 * tcp_connection.cpp
 *
 *  Created on: Jan 30, 2019
 *      Author: yusaku
 */

#include "tcp_connection.hpp"

tcp_connection::pointer tcp_connection::create(boost::asio::io_service& io_service)
{
    return pointer(new tcp_connection(io_service));
}

tcp::socket& tcp_connection::socket()
{
    return _socket;
}

void tcp_connection::send(std::string str)
{
    asio::async_write(_socket, asio::buffer(str + "\n"),
            boost::bind(&tcp_connection::handle_write, this->shared_from_this(), boost::asio::placeholders::error));
}

void tcp_connection::shake_hands()
{
    boost::asio::async_read_until(_socket, _message, "\n",
            boost::bind(&tcp_connection::handle_handshake, this->shared_from_this(), boost::asio::placeholders::error));
}

void tcp_connection::start()
{
    boost::asio::async_read_until(_socket, _message, "\n",
            boost::bind(&tcp_connection::handle_read, this->shared_from_this(), boost::asio::placeholders::error));
    //boost::asio::async_read(_socket, _message, asio::transfer_at_least(1),
    //        boost::bind(&tcp_connection::handle_read, this->shared_from_this(), boost::asio::placeholders::error));

    //asio::async_read(soc, receive_buff_, asio::transfer_at_least(1), &on_receive);
}

tcp_connection::tcp_connection(boost::asio::io_service& io_service) :
        _socket(io_service)
{
}

void tcp_connection::handle_write(const boost::system::error_code& error)
{
    // method stub
}

void tcp_connection::send_nak(void)
{
    asio::async_write(_socket, asio::buffer("NAK\n"),
            boost::bind(&tcp_connection::handle_write, this->shared_from_this(), boost::asio::placeholders::error));
}

void tcp_connection::handle_handshake(const boost::system::error_code& error)
{
    if (!error)
    {
        //std::cout << "[new message received]" << std::endl;

        std::string body = boost::asio::buffer_cast<const char*>(_message.data());

        auto size = _message.size();
        _message.consume(size);

        body = body.substr(0, size);

        if (body.empty())
        {
            send_nak();
            return;
        }

        body.pop_back();

        if (body.substr(0, 5).compare("JOIN+") != 0)
        {
            send_nak();
            return;
        }

        this->_name = body.substr(5, body.length() - 5);

        std::stringstream msg_ss;

        msg_ss << "[new client " << "\"" << this->_name << "\" has joined] ";

        std::cout << msg_ss.str() << std::endl;

        asio::async_write(_socket, asio::buffer("ACK\n"),
                boost::bind(&tcp_connection::handle_write, this->shared_from_this(), boost::asio::placeholders::error));

        _server->broadcast(msg_ss.str());

        this->start();
    }
    else
    {
        std::cout << "[an error occurred] " << std::endl;
    }
}

void tcp_connection::handle_read(const boost::system::error_code& error)
{
    if (error)
    {
        std::cout << "[an error occurred] " << std::endl;
        return;
    }

    //std::cout << "[new message received]" << std::endl;

    std::string frame = boost::asio::buffer_cast<const char*>(_message.data());

    auto size = _message.size();
    _message.consume(size);

    frame = frame.substr(0, size);

    if (frame.length() < 7)
    {
        send_nak();
        return;
    }

    frame.pop_back();

    std::string command = frame.substr(0, 5);
    std::string body = frame.substr(5, frame.length() - 5);

    if (command.compare("POST+") == 0)
    {
        Message *p_msg = new Message(std::chrono::system_clock::now(), body);
        Message::messages_.push_back(std::unique_ptr<Message>(p_msg));

        std::stringstream msg_ss;

        msg_ss << "[" << std::chrono::duration_cast<std::chrono::seconds>(p_msg->get_time().time_since_epoch()).count()
                << "] " << this->_name << " : " << p_msg->get_body();

        std::cout << msg_ss.str() << std::endl;

        _server->broadcast(msg_ss.str());

        this->start();
    }
    else if (command.compare("QUIT+") == 0)
    {
        std::cout << "[client " << "\"" << this->_name << "\" has quit] " << std::endl;
        this->_socket.close();
    }
}

