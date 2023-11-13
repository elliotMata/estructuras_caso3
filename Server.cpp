#include "httplib.h"
#include "MatchMaker.h"
#include "json.hpp"

void handle_hello_request(const httplib::Request &req, httplib::Response &res)
{
    res.set_content("Hello, world", "text/plain");
}

void handle_match_making(const httplib::Request &req, httplib::Response &res)
{
    string req_phrase = req.get_param_value("req_phrase");

    if (req_phrase != "")
    {
        MatchMaker *matchMaker = new MatchMaker(req_phrase);
        vector<string> top;

        matchMaker->findSimilarities();
        top = matchMaker->getTop();

        nlohmann::json response_json = nlohmann::json::array();
        for (const string &item : top)
        {
            response_json.push_back(item);
        }

        res.set_content(response_json.dump(), "application/json");
    }
    else
    {
        res.status = 400;
        res.set_content("Missing req_phrase parameter", "text/plain");
    }
}

int main()
{
    httplib::Server server;

    server.Get("/match", handle_match_making);

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