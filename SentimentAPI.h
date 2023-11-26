#ifndef SENTIMENT_API_H_
#define SENTIMENT_API_H_

#include <chrono>
#include <thread>

#include "HelperAPI.h"

using json = nlohmann::json;

class SentimentAPI
{
private:
    CURL *curl;
    CURLcode res;

    // Helper method for performing the request
    json performRequest(string url, const string &postData = "")
    {
        get_request req = {.buffer = NULL, .len = 0, .buflen = 0};

        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/93.0.4577.63 Safari/537.36");

        // Set the POST data
        if (!postData.empty())
        {
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        }

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

    json getRecords(string url, const string &postData = "")
    {
        return performRequest(url, postData);
    }

    CURL *getCurlHandle() const
    {
        return curl;
    }

public:
    string getSentiment(string textToAnalyze)
    {
        string sentiment = "";
        string sentimentApiUrl = "http://text-processing.com/api/sentiment/";
        string postData = "text=" + string(curl_easy_escape(getCurlHandle(), textToAnalyze.c_str(), textToAnalyze.length()));
        json response = getRecords(sentimentApiUrl, postData);

        if (!response.is_null())
        {
            // Extract sentiment information from the response
            string label = response["label"];
            double probability = response["probability"][label];
            if (label.compare("pos") == 0)
                label = "positive";
            if (label.compare("neg") == 0)
                label = "negative";
            sentiment = "The sentiment is " + label + " with a percentage of " + to_string(probability);
        }
        return sentiment;
    }
};

#endif