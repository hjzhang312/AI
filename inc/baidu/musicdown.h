#ifndef MUSICDOWN_H
#define MUSICDOWN_H


#include "singleton.h"
#include "thread.h"
#include <curl/curl.h>
#include <iostream>
#include <vector>

class MusicDown : public Thread
{
public:
    MusicDown();

    void Run();

    static size_t HeadData(void* buffer, size_t size, size_t nmemb, void* lpVoid);
    static size_t downData(void* buffer, size_t size, size_t nmemb, void* lpVoid);
    int GetUrl();
    void setUrl(string url);

    void pauseDown();
    void keepDown();

    ~MusicDown();

public:
    bool mIsDown;
private:
    CURL* mCurl;
    string mUrl;


    DECLARE_SINGLETON_CLASS(MusicDown)
};

 typedef Singleton<MusicDown> SMusicDown;


#endif // MUSICDOWN_H
