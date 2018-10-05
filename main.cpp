#include "ProcessOptions.hpp"
#include "PersistentStore.hpp"
#include "ChatServer.hpp"
#include <iostream>
#include <asio.hpp>

int main(int argc, char* argv[])
{
    asio::io_service io_service;

    try
    {
        ProcessOptions po(argc, argv);
        uint16_t port = std::stoi(po.get("port")); 
        ChatServer ts(io_service, port);
        std::cout << "Listening to port " << port << std::endl;
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

    return 0;
}
