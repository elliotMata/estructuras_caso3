#ifndef API_H
#define API_H

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <curl/curl.h>
#include <iomanip>
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
            // Trim non-JSON content
            const char *jsonStart = reinterpret_cast<const char *>(buffer);
            while (*jsonStart && !(*jsonStart == '{' || *jsonStart == '['))
            {
                ++jsonStart;
            }

            try
            {
                return json::parse(jsonStart);
            }
            catch (const exception &e)
            {
                cerr << "Error parsing JSON response: " << e.what() << endl;
                // Print or log the raw response for further inspection
                cerr << "Raw Response: " << buffer << endl;
            }
        }
        return json(); // You might want to return a specific value or handle this case differently
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

    // Check if this is the first chunk, where headers might be present
    if (req->len == 0)
    {
        // Check for the Content-Type header
        const char *contentTypeHeader = "Content-Type: ";
        const char *contentStart = strstr(ptr, contentTypeHeader);

        if (contentStart)
        {
            contentStart += strlen(contentTypeHeader);
            const char *contentEnd = strchr(contentStart, '\r');

            if (contentEnd)
            {
                // Print or log the Content-Type header
                cout << "Content-Type: " << string(contentStart, contentEnd - contentStart) << endl;
            }
        }
    }

    // Allocate enough space for the new data
    req->buffer = (unsigned char *)realloc(req->buffer, req->buflen + realsize);
    memcpy(&req->buffer[req->len], ptr, realsize);
    req->len += realsize;

    cout << "Raw Response: ";
    for (size_t i = 0; i < req->len; ++i)
    {
        if (isprint(req->buffer[i]) || isspace(req->buffer[i]))
        {
            cout << req->buffer[i];
        }
        else
        {
            cout << "\\x" << hex << setw(2) << setfill('0') << (int)req->buffer[i];
        }
    }

    return realsize;
}

class ApiRequest
{
private:
    CURL *curl;
    string apiKey;

public:
    ApiRequest(const string &apiKey) : curl(nullptr), apiKey(apiKey) {}

    ApiResponse makePostRequest(const string &url, const string &data)
    {
        get_request req = {.buffer = nullptr, .len = 0, .buflen = 0};

        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();

        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Your User Agent Here");
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

            req.buffer = (unsigned char *)malloc(CHUNK_SIZE);
            req.buflen = CHUNK_SIZE;

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&req);

            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

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

            curl_slist_free_all(headers);
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

#endif