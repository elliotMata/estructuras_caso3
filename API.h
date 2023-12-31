#ifndef API_H_
#define API_H_

#include <chrono>
#include <thread>

#include "HelperAPI.h"

using json = nlohmann::json;

class API
{
private:
    CURL *curl;
    CURLcode res;

    // Helper method for performing the request
    json performRequest(string url)
    {
        get_request req = {.buffer = NULL, .len = 0, .buflen = 0};

        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/93.0.4577.63 Safari/537.36");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

        req.buffer = (unsigned char *)malloc(CHUNK_SIZE);
        req.buflen = CHUNK_SIZE;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&req);

        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);

        json responseData;

        if (res == CURLE_OK)
        {
            responseData = json::parse(req.buffer);
        }
        else
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        free(req.buffer);

        // Clean the curl objects
        curl_easy_cleanup(curl);
        curl_global_cleanup();

        return responseData;
    }

public:
    json getRecordsWithRetry(string url, int maxRetries = 3)
    {
        int retries = 0;
        json responseData;

        while (retries < maxRetries)
        {
            responseData = performRequest(url);

            // If the request is successful, exit the loop
            if (!responseData.is_null())
            {
                break;
            }

            // Sleep for a short duration before retrying
            std::this_thread::sleep_for(std::chrono::seconds(1));
            ++retries;
        }

        return responseData;
    }

    // Existing getRecords method without retry
    json getRecords(string url)
    {
        return performRequest(url);
    }
};

/*int main(void)
{

    API API;

    json allrecords = API.getRecords("https://api.datamuse.com/words?rel_syn=horror");
    for (auto &pair : allrecords)
    {
        cout << pair["word"].get<string>();
    }

    return 0;
}*/

#endif