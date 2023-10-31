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
        server.listen("0.0.0.0", 65000);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}