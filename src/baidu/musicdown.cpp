
#include "musicdown.h"
#include "playdevices.h"
#include <curl/curl.h>
#include <sstream>

MusicDown::MusicDown()
{
       mIsDown = false;
}

void MusicDown::Run()
{

    while(1)
    {
        if(mIsDown)
        {
            SPlayDevices::instance()->setTTSPlay();
            GetUrl();
        }
    }
}

size_t MusicDown::HeadData(void *buffer, size_t size, size_t nmemb, void *lpVoid)
{

    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
    if( NULL == str || NULL == buffer )
    {
        return -1;
    }

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
    cout << "HeadData..." <<str<<endl;
    return nmemb;
}
stringstream a;
size_t MusicDown::downData(void *buffer, size_t size, size_t nmemb, void* lpVoid)
{
#if 0
    cout << "downData..." <<"size: " << size << "nmemb: " << nmemb<<endl;
    /*
    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
    if( NULL == str || NULL == buffer )
    {
        return -1;
    }
*/
    char* pData = (char*)buffer;
    //str->append(pData, size * nmemb);
    SPlayDevices::instance()->stream.write(pData,size*nmemb);
    a.write(pData,size*nmemb);

    char kk[size*nmemb] ={0};
    char kkk[size*nmemb] ={0};

    a.read(kk,2);
    SPlayDevices::instance()->stream.read(kkk,2);
    cout << "kk: " <<kk << endl;
    cout << "kkk: " <<kkk << endl;
#endif
    return nmemb;
}

int MusicDown::GetUrl()
{
    cout << "MusicDown GetUrl: " << mUrl << endl;
   // string strResponse;
    CURLcode res;
    mCurl = curl_easy_init();
    if(NULL == mCurl)
    {
        return CURLE_FAILED_INIT;
    }
    string headUrl;
    curl_easy_setopt(mCurl, CURLOPT_URL, mUrl.c_str());
    //curl_easy_setopt(mCurl, CURLOPT_READFUNCTION, NULL);
    //curl_easy_setopt(mCurl, CURLOPT_HEADERFUNCTION, HeadData );
    //curl_easy_setopt(mCurl, CURLOPT_HEADERDATA, (void*)&headUrl );
    //curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, downData);
    //curl_easy_setopt(mCurl, CURLOPT_FOLLOWLOCATION, 1L);
    //curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, (stringstream&)SPlayDevices::instance()->stream);

    res = curl_easy_perform(mCurl);

    char *url =NULL;
    curl_easy_setopt(mCurl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_getinfo(mCurl,CURLINFO_REDIRECT_URL,&url);

    cout << "MP3 address: " << url << endl;

    cout <<"Get recv:" <<res << curl_easy_strerror(res)<< endl;
    curl_easy_cleanup(mCurl);
    return res;
}

void MusicDown::setUrl(string url)
{
    mIsDown = true;
    mUrl = url;
}

void MusicDown::pauseDown()
{
    mIsDown = false;
    curl_easy_pause(mCurl,CURLPAUSE_RECV);
}

void MusicDown::keepDown()
{
    mIsDown = true;
    curl_easy_pause(mCurl,CURLPAUSE_CONT);
}

MusicDown::~MusicDown()
{

}
