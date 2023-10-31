#include "httplib.h"

void handle_hello_request(const httplib::Request &req, httplib::Response &res)
{
    res.set_content("Hello, world", "text/plain");
}

int main()
{
    httplib::Server server;

    server.Get("/hello", handle_hello_request);

    try
    {
        if (server.listen("localhost", 65000))
        {
            std::cout << "Server is running. Press Enter to stop it." << std::endl;
            std::string line;
            std::getline(std::cin, line);
            server.stop();
        }
        else
        {
            std::cerr << "Error: Server failed to start" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}