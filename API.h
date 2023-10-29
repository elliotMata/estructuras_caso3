#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <curl/curl.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

#define CHUNK_SIZE 2048

typedef struct
{
    unsigned char *buffer;
    size_t len;
    size_t buflen;
} get_request;

class ApiResponse
{
public:
    ApiResponse(unsigned char *data, size_t length) : buffer(data), len(length) {}

    json parseJsonResponse()
    {
        if (buffer)
        {
            try
            {
                return json::parse(reinterpret_cast<const char *>(buffer));
            }
            catch (const exception &e)
            {
                cerr << "Error parsing JSON response: " << e.what() << endl;
            }
        }
        return json();
    }

private:
    unsigned char *buffer;
    size_t len;
};

// static method to act as a callback for curl
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t realsize = size * nmemb;
    get_request *req = (get_request *)userdata;

    while (req->buflen < req->len + realsize + 1)
    {
        req->buffer = (unsigned char *)realloc(req->buffer, req->buflen + CHUNK_SIZE);
        req->buflen += CHUNK_SIZE;
    }
    memcpy(&req->buffer[req->len], ptr, realsize);
    req->len += realsize;
    req->buffer[req->len] = 0;

    return realsize;
}

class ApiRequest
{
private:
    CURL *curl;

public:
    ApiRequest() : curl(nullptr) {}

    ApiResponse makeGetRequest(const string &url)
    {
        get_request req = {.buffer = nullptr, .len = 0, .buflen = 0};

        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();

        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Your User Agent Here");
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

            req.buffer = (unsigned char *)malloc(CHUNK_SIZE);
            req.buflen = CHUNK_SIZE;

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&req);

            CURLcode res = curl_easy_perform(curl);

            if (res == CURLE_OK)
            {
                ApiResponse response(req.buffer, req.len);
                free(req.buffer);
                return response;
            }
            else
            {
                cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            }
        }
        else
        {
            cerr << "Failed to initialize CURL." << endl;
        }

        if (req.buffer)
        {
            free(req.buffer);
        }

        if (curl)
        {
            curl_easy_cleanup(curl);
            curl_global_cleanup();
        }

        return ApiResponse(nullptr, 0);
    }

    ~ApiRequest()
    {
        if (curl)
        {
            curl_easy_cleanup(curl);
            curl_global_cleanup();
        }
    }
};

int main()
{
    ApiRequest apiRequest;
    string apiUrl = "https://your-api-endpoint-here";
    ApiResponse response = apiRequest.makeGetRequest(apiUrl);

    if (!response.parseJsonResponse().empty())
    {
        // Parse the JSON response and work with the data
        json responseData = response.parseJsonResponse();

        // Now, you have the JSON response data to work with.
        cout << responseData.dump(4) << endl;
    }

    return 0;
}