/*
 * message.cpp
 *
 *  Created on: Jan 30, 2019
 *      Author: yusaku
 */

#include "message.hpp"

Message::Message(const std::chrono::system_clock::time_point & time__, const std::string & body__) :
        time_(new std::chrono::system_clock::time_point(time__)), body_(new std::string(body__))
{
}

std::chrono::system_clock::time_point Message::get_time(void)
{
    return *(this->time_);
}

std::string Message::get_body(void)
{
    return *(this->body_);
}

std::vector<std::unique_ptr<Message>> Message::messages_;

