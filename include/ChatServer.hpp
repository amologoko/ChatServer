#ifndef __CHAT_SERVER_HPP__
#define __CHAT_SERVER_HPP__

#include "ChatSession.hpp"
#include <atomic>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <stdint.h>

using asio::ip::tcp;

class ChatServer
{
public:
    ChatServer(asio::io_service& io_service, uint16_t port);
    void publish(const asio::mutable_buffer& buf);
    bool register_user(const std::string& nick, const std::string& password);
    
    uint16_t counter() {
        return _counter++;
    }

private:
    void start_accept();
    void handle_accept(ChatSessionPtr new_session, const std::error_code& error);

    std::mutex _lock;
    std::atomic_uint16_t _counter;
    std::vector<ChatSessionPtr> _sessions;
    std::unordered_map<std::string, std::string> _registry;
    asio::io_service& _io_service;
    tcp::acceptor _acceptor;
};

#endif

