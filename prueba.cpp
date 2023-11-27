#include "OpenAIAPI.h"

int main(void)
{
       string apiKey = "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";
       OpenAIAPI *openai = new OpenAIAPI(apiKey);
       string text = "horror monster dark mistery";

       string feeling = openai->getSentiment(text);
       cout << feeling << endl;

       string image = openai->getImage(text);
       cout << image << endl;

       return 0;
}