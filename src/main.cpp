#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <functional>
#include <string>
#include <curl/curl.h>
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>


using namespace std;

//g++ -Wall -Wextra -o main main.cpp && ./main

class Converter{
    public:
        char* htmlCStr;
        int htmlCStrLen;
        Converter();
        void setUrl(const string& url);
        string downloadURL();
        void loadStr(string str);
        string parseHTML(string str, size_t& cursorPos);
        void cleanHTML(string& str,string startStr, string endStr);

        //tag parsing map tag string to function
        map<string, function<string(const string&, size_t&)>> tagMap;


        //tag parsing funcs
        string parseHeaderOne(const string& str, size_t& cursorPos,int tagLen);
        string parseHeaderTwo(const string& str, size_t& cursorPos,int tagLen);
        string parseHR(const string& str, size_t& cursorPos,int tagLen);



        void writeFile(string markdownStr);


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
    

    //map of tags
    tagMap = {
        {"<h1>", [this](const string& str, size_t& pos) { return this->parseHeaderOne(str, pos,4); }},
        {"<h2>", [this](const string& str, size_t& pos) { return this->parseHeaderTwo(str, pos,4); }},
        {"<hr>", [this](const string& str, size_t& pos) { return this->parseHR(str, pos,4); }}
    };

    if (!curl) {
        printf("Failed to initialize cURL.\n");
        perror("Error:");
        exit(-1);
    }

}


string Converter::parseHeaderOne(const string& str, size_t& cursorPos,int tagLength) {
    
    cursorPos += 4;
    return "# ";
}

string Converter::parseHeaderTwo(const string& str, size_t& cursorPos,int tagLength) {
    
    cursorPos += 4;
    return "[H2]";
}

string Converter::parseHR(const string& str, size_t& cursorPos,int tagLength) {
    cursorPos += 4;
    return "---";
}

string Converter::parseHTML(string str, size_t& cursorPos){
    string parsed;
    char currentChar;
    bool tagFound;
    while(cursorPos < htmlCStrLen){
        currentChar = str[cursorPos];
        
        //check if this could be the beginning of a html tag
        if(currentChar == '<'){

            map<string, function<string(const string&, size_t&)>>::iterator mapIter;
            tagFound = false;
            
            // for each item in the map, check to see if the possible tag is the current map tag. Then if it is run the map's mapping function
            for (mapIter = tagMap.begin(); mapIter != tagMap.end(); ++mapIter) {
                
                string tag = mapIter->first;
                function<string(const string&, size_t&)> mappingFunction = mapIter->second;

                if (str.compare(cursorPos, tag.size(), tag) == 0) {
                    parsed += mappingFunction(str, cursorPos);
                    tagFound = true;
                    break;
                }
            }

            //if its not a tag just keep going
            if (!tagFound) {
                parsed += currentChar;
                cursorPos++;
            }

        }else{
            parsed += currentChar;
            cursorPos++;
        }
    }
    return parsed;
}


void Converter::setUrl(const string& url) {
    this->url = url;
}

//load string into cstring
void Converter::loadStr(string str){
    
    cleanHTML(str,"<title>","</title>");
    cleanHTML(str,"<body","\n");

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


void Converter::writeFile(string markdownStr){
    ofstream outfile;
    outfile.open("output.md");
    outfile << markdownStr;
    outfile.close();
}

void Converter::cleanHTML(string& htmlString, string startStr, string endStr) {
    size_t first = htmlString.find(startStr);
    size_t last = htmlString.find(endStr, first);
    htmlString.erase(first,last-first+endStr.size());
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
    


    size_t cursorPos = 0;

    //start at pos 0
    string markdownString = converter.parseHTML(converter.htmlCStr,cursorPos);

    converter.writeFile(markdownString + "\n");
    cout << markdownString << endl;

    return 0;
}