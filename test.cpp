#include "API.h"

int main() 
{
    std::string apiKey = "98edf27a887a4afc9bb222de157d4f61";
    ApiRequest apiRequest(apiKey);

    std::string apiUrl = "https://api.thenounproject.com/icons/search";
    std::string requestData = "{\"query\": \"horror\"}"; // Replace with your search query

    ApiResponse response = apiRequest.makePostRequest(apiUrl, requestData);

    json jsonResponse = response.parseJsonResponse();
    cout << jsonResponse.dump(4) << endl;
}