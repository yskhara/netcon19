/*
 * message.hpp
 *
 *  Created on: Jan 30, 2019
 *      Author: yusaku
 */

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

class Message
{
private:
    std::unique_ptr<std::chrono::system_clock::time_point> time_;
    std::unique_ptr<std::string> body_;

public:
    Message(const std::chrono::system_clock::time_point & time__, const std::string & body__);

    std::chrono::system_clock::time_point get_time(void);

    std::string get_body(void);

    static std::vector<std::unique_ptr<Message>> messages_;
};




#endif /* MESSAGE_HPP_ */
