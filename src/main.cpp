#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <curl/curl.h>
#include <bits/stdc++.h>


using namespace std;

//g++ -Wall -Wextra -o main main.cpp && ./main

class Converter{
    public:
        Converter();
        void setUrl(const string& url);
        string downloadURL();
        void loadStr(string str);
        string parseHTML(string str, int& cursor_pos);
        char* htmlCStr;
        int htmlCStrLen;

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

string Converter::parseHTML(string str, int& cursor_pos){
    while(cursor_pos < htmlCStrLen){
        printf("%c",str[cursor_pos]);
        cursor_pos++;
    }
    return str;
}

void Converter::setUrl(const string& url) {
    this->url = url;
}

//load string into cstring
void Converter::loadStr(string str){

    int stringLen = str.length() + 1;
    htmlCStrLen = stringLen;
    htmlCStr = new char[stringLen];
    strcpy(htmlCStr, str.c_str());
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
    
    //convert to cstr and load into mem
    converter.loadStr(converter.downloadURL());

    int cursor_pos = 0;

    //start at pos 0
    string markdownString = converter.parseHTML(converter.htmlCStr,cursor_pos);

    //cout << markdownString << endl;

    return 0;
}