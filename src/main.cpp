#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <curl/curl.h>

using namespace std;

//g++ -Wall -Wextra -o main main.cpp && ./main

class Converter{
    public:
        Converter();
        void setUrl(const string& url);
        string downloadURL();
        void parser(string parseString);

    private:
        CURL* curl;
        string url;
        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);


};

//appends libcurls recv data to resp, reccomended from docs
size_t Converter::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
//init curl
Converter::Converter(){
    curl = curl_easy_init();
    if (!curl) {
        printf("Failed to initialize cURL.\n");
        perror("Error:");
        exit(-1);
    }

}

void Converter::setUrl(const string& url) {
    this->url = url;
}

void Converter::parser(string parseString){
    cout << parseString;
}

string Converter::downloadURL(){
    printf("%s\n", url.c_str());
    CURLcode responseCode;
    string htmlResponse;

    //set options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    //write to callback funct
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlResponse);

    //get the response code
    responseCode = curl_easy_perform(curl);

    
    printf("CURL RESP CODE: %d\n",responseCode);
    printf("%s\n",htmlResponse.c_str());
    return htmlResponse;
}

int main(int argc, char *argv[]){

    Converter converter;
    
    if(argc != 2){
        printf("Error: Incorrect args!\n");
        return -1;
    }

    //set url for converter
    converter.setUrl(argv[1]);
    
    converter.parser("hi");


    
    converter.downloadURL();
    return 0;
}