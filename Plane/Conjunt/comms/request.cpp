#include <iostream>
#include <unistd.h>
#include <string>
#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

int main(void)
{
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init();
  if (curl)
  {
    while (true)
    {
      curl_easy_setopt(curl, CURLOPT_URL, "192.168.1.200:8080");
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
      res = curl_easy_perform(curl);

      std::cout << readBuffer << std::endl;
      usleep(100000);
    }
  }
  return 0;
}