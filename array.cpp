#include <iostream>
#include <curl/curl.h>
#include <string>
#include <json/json.h> // You need a JSON library like JsonCpp

// Callback function to handle data received from the API
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t totalSize = size * nmemb;
    buffer->append((char*)contents, totalSize);
    return totalSize;
}

void fetchWeather(const std::string& apiKey, const std::string& city) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    std::string url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Parse JSON response
            Json::CharReaderBuilder readerBuilder;
            Json::Value jsonData;
            std::string errs;

            std::istringstream stream(readBuffer);
            if (Json::parseFromStream(readerBuilder, stream, &jsonData, &errs)) {
                std::cout << "City: " << jsonData["name"].asString() << std::endl;
                std::cout << "Temperature: " << jsonData["main"]["temp"].asFloat() << "°C" << std::endl;
                std::cout << "Weather: " << jsonData["weather"][0]["description"].asString() << std::endl;
            } else {
                std::cerr << "Error parsing JSON: " << errs << std::endl;
            }
        }

        curl_easy_cleanup(curl);
    }
}

int main() {
    std::string apiKey = "your_api_key"; // Replace with your OpenWeatherMap API key
    std::string city;

    std::cout << "Enter city name: ";
    std::getline(std::cin, city);

    fetchWeather(apiKey, city);

    return 0;
}




// #include<iostream>
// using namespace std;

// int main() {
//     int num[5];
//     cout << "Enter 5 numbers: ";
//     for (int i = 0; i < 5; i++) {
//         cin >> num[i];
//     }

//     cout << "The numbers you entered are: ";
//     for (int i = 0; i < 5; i++) {  // Reuse `i` in a separate loop
//         cout << num[i] << " ";
//     }

//     return 0;
// }
