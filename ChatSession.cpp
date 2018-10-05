#include "ChatSession.hpp"
#include "ChatServer.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using asio::ip::tcp;

ChatSession::ChatSession(ChatServer* server, asio::io_service& io_service)
    : _server(server), _socket(io_service), _active(true)
{
    std::stringstream user;
    user << "Guest" << std::setfill('0') << std::setw(3) << server->counter();
    _user = user.str();
}

void ChatSession::write(const asio::mutable_buffer& buf)
{
    if(_active) {
        asio::async_write(_socket, buf, 
            std::bind(&ChatSession::handle_write, this, asio::placeholders::error, asio::placeholders::bytes_transferred)
        );
    }
}

void ChatSession::write(const asio::const_buffer& buf)
{
    if(_active) {
        asio::async_write(_socket, buf, 
            std::bind(&ChatSession::handle_write, this, asio::placeholders::error, asio::placeholders::bytes_transferred)
        );
    }
}

void ChatSession::start(const std::vector<ChatSessionPtr> sessions)
{
    std::for_each(sessions.cbegin(), sessions.cend(), [this](ChatSessionPtr p){
        if(p->is_active()) {
            std::string s = p->user() + ", ";    
            s.copy(_data, s.size());
            this->write(asio::buffer(_data, s.size()));
        }
    });
    _data[0] = '\n';
    this->write(asio::buffer(_data, 1));
}

void ChatSession::handle_read(const std::error_code& error, size_t bytes_transferred)
{
    static const std::string UNKNOWN_COMMAND {"Unknown command!\n"};
    static const std::string NICK_TAKEN {"Nick taken!\n"};
    static const std::string REGISTRATION_FAILED {"Registration failed!\n"};

    if (!error) {
        std::cout << "ChatSession::handle_read " << (int) bytes_transferred << " bytes\n";
        if(_data[0] == '/') {
            if(std::string(_data + 1, 5) == "nick ") {
                std::string s(_data + 6, bytes_transferred - 6);
                trim(s);
                if(_server->register_user(s, _password)) {
                    _user = s;
                } else {
                    this->write(asio::buffer(NICK_TAKEN.c_str(), NICK_TAKEN.size()));
                }
            } else if (std::string(_data + 1, 9) == "register ") {
                std::string s(_data + 10, bytes_transferred - 10);
                trim(s);
                if(_server->register_user(_user, s)) {
                    _password = s;
                } else {
                    this->write(asio::buffer(REGISTRATION_FAILED.c_str(), REGISTRATION_FAILED.size()));
                }
            } else {
                this->write(asio::buffer(UNKNOWN_COMMAND.c_str(), UNKNOWN_COMMAND.size()));
            }
        } else {
            _server->publish(asio::buffer(_data, bytes_transferred));
        }
    } else {
        std::cout << "ChatSession::handle_read error: " << error << std::endl;
        _active = false;
    }
}

void ChatSession::handle_write(const std::error_code& error, size_t bytes_transferred)
{
    if (!error) {
        std::cout << "ChatSession::handle_write " << (int) bytes_transferred << " bytes\n";
        _socket.async_read_some(
            asio::buffer(_data, MAX_SIZE),
            std::bind(&ChatSession::handle_read, this, asio::placeholders::error, asio::placeholders::bytes_transferred)
        );
    } else {
        std::cout << "ChatSession::handle_write error: " << error << std::endl;
        _active = false;
    }
}

