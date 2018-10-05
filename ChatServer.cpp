#include "ChatServer.hpp"
#include <iostream>

using asio::ip::tcp;

ChatServer::ChatServer(asio::io_service& io_service, uint16_t port)
    : _io_service(io_service),
      _acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
      _counter(0)
{
    start_accept();
}

void ChatServer::start_accept()
{
    std::cout << "ChatServer::start_accept\n";
    ChatSessionPtr new_session(new ChatSession(this, _io_service));
    _acceptor.async_accept(new_session->socket(), 
        std::bind(&ChatServer::handle_accept, 
            this, new_session, asio::placeholders::error));
}

void ChatServer::handle_accept(ChatSessionPtr new_session, const std::error_code& error)
{
    std::lock_guard<std::mutex> guard(_lock);
    if (!error)
    {
        std::cout << "ChatServer::handle_accept starting session\n";
        _sessions.push_back(new_session);
        new_session->start(_sessions);
    }
    else
    {
        std::cout << "ChatServer::handle_accept error: " << error << std::endl;
        new_session.reset();
    }

    start_accept();
}

void ChatServer::publish(const asio::mutable_buffer& buf)
{
    std::lock_guard<std::mutex> guard(_lock);
    std::cout << "ChatServer::publish" << std::endl;
    for_each(_sessions.begin(), _sessions.end(), [buf](ChatSessionPtr p){ p->write(buf); });
}

bool ChatServer::register_user(const std::string& nick, const std::string& password) {
        std::lock_guard<std::mutex> guard(_lock);
        std::cout << "ChatServer::register_user " << nick << " " << password << std::endl; 
        auto it = _registry.find(nick);
        if(it!=_registry.end()) {
            if(it->second.empty()) {
                if(password.empty()) {
                    return true;
                } else {
                    _registry[nick] = password;
                    return true;
                }
            } else {
                return (it->second == password);
            }               
        }
        _registry.insert({nick, password});
        return true;
}