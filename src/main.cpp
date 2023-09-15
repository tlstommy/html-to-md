#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <curl/curl.h>

using namespace std;

//g++ -Wall -Wextra -o main main.cpp && ./main

class Converter{
    public:
        CURL* curl;
        string url;
        string downloadURL();
        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);


};

//appends libcurls recv data to resp, reccomended from docs
size_t Converter::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


string Converter::downloadURL(){
    printf("%s\n", url.c_str());
    CURLcode responseCode;
    string responseCurl;

    //set options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    //write to callback funct
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseCurl);

    //get the response code
    responseCode = curl_easy_perform(curl);

    
    cout <<  "CURL RESP CODE: " << responseCode << endl;
    cout << responseCurl << endl;
    return responseCurl;
}

int main(int argc, char *argv[]){

    Converter converter;
    
    if(argc != 2){
        printf("Error: Incorrect args!\n");
        return -1;
    }

    //init curl
    converter.curl = curl_easy_init();
    if (!converter.curl) {
        printf("Failed to initialize cURL.\n");
        perror("Error:");
        return -1;
    }

    converter.url = argv[1];

    
    converter.downloadURL();
    return 0;
}