#ifndef __CHAT_SESSION_HPP__
#define __CHAT_SESSION_HPP__

#include <system_error>
#include <functional>
#include <memory>
#include <atomic>
#include <vector>
#include <asio.hpp>

using asio::ip::tcp;

namespace asio {
namespace placeholders {
const decltype(std::placeholders::_1) error{};
const decltype(std::placeholders::_2) bytes_transferred{};
const decltype(std::placeholders::_2) iterator{};
const decltype(std::placeholders::_2) results{};
const decltype(std::placeholders::_2) endpoint{};
const decltype(std::placeholders::_2) signal_number{};
}
};

class ChatServer;
class ChatSession;
typedef std::shared_ptr<ChatSession> ChatSessionPtr;

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

class ChatSession
{
public:
    enum { MAX_SIZE = 140 };

    ChatSession(ChatServer* server, asio::io_service& io_service);
    void write(const asio::mutable_buffer& buf);
    void write(const asio::const_buffer& buf);
    void start(const std::vector<ChatSessionPtr> sessions);
    tcp::socket& socket() {
        return _socket;
    }
    bool is_active() {
        return _active;
    }
    std::string user() {
        return _user;
    }

private:

    void handle_read(const std::error_code& error, size_t bytes_transferred);
    void handle_write(const std::error_code& error, size_t bytes_transferred);

    ChatServer* _server;
    tcp::socket _socket;
    char _data[MAX_SIZE];
    std::string _user;
    std::string _password;
    std::atomic<bool> _active;
};

#endif

