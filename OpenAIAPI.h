#include "HelperAPI.h"

class OpenAIAPI
{
private:
    CURL *curl;
    CURLcode res;
    string apiKey;

    json generateAnswer(string &url, string &postData)
    {
        get_request req = {.buffer = nullptr, .len = 0, .buflen = 0};

        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();

        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/93.0.4577.63 Safari/537.36");
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

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
                json responseData = json::parse(req.buffer);
                free(req.buffer);
                return responseData;
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

        return json();
    }

public:
    OpenAIAPI(string &apiKey)
    {
        this->curl = nullptr;
        this->apiKey = apiKey;
    }

    string getSentiment(string &phrase)
    {
        string text = "Analyze the sentiment of the text: " + phrase;
        string url = "https://api.openai.com/v1/engines/text-davinci-003/completions";
        string postData = R"({"prompt": ")" + text + R"(","max_tokens": 20,"temperature": 0.0})";
        json response = generateAnswer(url, postData);
        string sentiment = response["choices"][0]["text"];
        return sentiment;
    }

    string getImage(string &text)
    {
        string url = "https://api.openai.com/v1/images/generations";
        string postData = R"({"prompt": ")" + text + R"(","n":1,"size":"256x256"})";
        json response = generateAnswer(url, postData);
        string imageUrl = response["data"][0]["url"];
        return imageUrl;
    }

    ~OpenAIAPI()
    {
        if (curl)
        {
            curl_easy_cleanup(curl);
            curl_global_cleanup();
        }
    }
};