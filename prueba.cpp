#include "API.h"

int main()
{
       ApiRequest apiRequest("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"); // Add your OpenAI API key here
       string apiUrl = "https://api.openai.com/v1/engines/babbage/completions";      // Update the URL

       // Construct a JSON string with your prompt and other parameters
       string jsonData = R"({"prompt": "Feeling of: A horror tale with monsters and blood; full of danger."})"; // Add your prompt

       ApiResponse response = apiRequest.makePostRequest(apiUrl, jsonData);

       if (response.parseJsonResponse().contains("error"))
       {
              // Handle error response
              cout << "API Error: " << response.parseJsonResponse()["error"]["message"] << endl;
       }
       else
       {
              // Parse the JSON response and work with the data
              json responseData = response.parseJsonResponse();
              cout << responseData.dump(4) << endl;
       }

       return 0;
}
