#include "httplink.h"

#include "type.h"
#include <curl/curl.h>
#include <iostream>
#include <iconv.h>
#include <string.h>

unsigned char HttpLink::ToHex(unsigned char x)
{
    return  x > 9 ? x + 55 : x + 48;
}


std::string HttpLink::UrlEncode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();

    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '*'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}

HttpLink::HttpLink()
{

}

void HttpLink::initialize()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

size_t HttpLink::WriteData(void *buffer, size_t size, size_t nmemb, void *lpVoid)
{
    cout << "writeData..." <<"size: " << size << "nmemb: " << nmemb<<endl;
    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
    if( NULL == str || NULL == buffer )
    {
        return -1;
    }

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
    return nmemb;
}

string HttpLink::code_convert_utf8(char *source_charset, char *to_charset, const string &sourceStr)
{
    iconv_t cd = iconv_open(to_charset, source_charset);//»ñÈ¡×ª»»¾ä±ú£¬void*ÀàÐÍ
    if (cd == 0)
        return "";

    size_t inlen = sourceStr.size();
    size_t outlen = 255;
    char* inbuf = (char*)sourceStr.c_str();
    char outbuf[255];

    memset(outbuf, 0, outlen);

    char *poutbuf = outbuf; //¶à¼ÓÕâ¸ö×ª»»ÊÇÎªÁË±ÜÃâiconvÕâ¸öº¯Êý³öÏÖchar(*)[255]ÀàÐÍµÄÊµ²ÎÓëchar**ÀàÐÍµÄÐÎ²Î²»¼æÈÝ
    if (iconv(cd, &inbuf, &inlen, &poutbuf,&outlen) == -1)
        return "";

    std::string strTemp(outbuf);//´ËÊ±µÄstrTempÎª×ª»»±àÂëÖ®ºóµÄ×Ö·û´®

    iconv_close(cd);
    return strTemp;
}

int HttpLink::PostUrl(const string strUrl, const string strPost, string &strResponse)
{
    CURLcode res;
    CURL* curl = curl_easy_init();
    cout << "PostUrl: " << strUrl + strPost << endl;
    if(NULL == curl)
    {
        return CURLE_FAILED_INIT;
    }
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: */*;");
    headers = curl_slist_append(headers,"Content-Type:application/json;charset=UTF-8");

    //headers = curl_slist_append(headers,"Content-Type:application/x-www-form-urlencoded");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    res = curl_easy_perform(curl);
    cout <<"Post recv:" <<res << curl_easy_strerror(res)<< endl;

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return res;
}

int HttpLink::GetUrl(const string strUrl, string &strResponse)
{
    cout << "GetUrl: " << strUrl << endl;
    CURLcode res;
    CURL* curl = curl_easy_init();
    if(NULL == curl)
    {
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);


    res = curl_easy_perform(curl);

    cout <<"Get recv:" <<res << curl_easy_strerror(res)<< endl;
    curl_easy_cleanup(curl);
    return res;
}

int HttpLink::unitSpeaker(string speaker,Json::Value &response)
{
    int ret = -1;
    string url,strPost,strRequest;
    Json::Value root,requestV,asrInfo;
    Json::FastWriter fast_write;
    root["log_id"] = Json::Value("UNITTEST_10000");
    root["version"] = Json::Value("2.0");
    root["service_id"] = Json::Value("S11391");
    root["session_id"] = Json::Value("");
    requestV["user_id"] = Json::Value("8888");
    requestV["query"] = Json::Value(speaker.c_str());
    //asrInfo["source"] = Json::Value("ASR");
    //requestV["query_info"] = asrInfo;

    root["request"] = requestV;
    strPost = fast_write.write(root);
    url = string("https://aip.baidubce.com/rpc/2.0/unit/service/chat?access_token=") + AccessToken;

    ret = PostUrl(url,strPost,strRequest);
    cout << "strRequest: " << strRequest << endl;

    if(CURLE_OK == ret)
    {
        Json::Reader reader;
        if(!reader.parse(strRequest,response))
        {
            ret = -1;
        }
    }
    return ret;
}

string HttpLink::getMusicUrl(string musicName)
{
    int ret = -1;
    string musicUrl;
    string url,strPost,strRequest;
    Json::Value root,response;
    Json::FastWriter fast_write;

    string name = UrlEncode(musicName);
    cout << "UrlEncode name: " << name << endl;
   // root["key"] = Json::Value("579621905");
    root["s"] = Json::Value(name.c_str());
    root["type"] = Json::Value(1);
    root["limit"] = Json::Value(1);
    root["offset"] = Json::Value(0);
    strPost = fast_write.write(root);
    //url = string("https://api.bzqll.com/music/netease/search?");

    url = string("https://api.bzqll.com/music/netease/search?key=579621905&s=")+name+string("&type=song&limit=1&offset=0");;
     //url = string("https://music.163.com/api/search/pc?s=")+name+string("&type=1&limit=1&offset=0");
    //ret = PostUrl(url,strPost,strRequest);

    ret = GetUrl(url,strRequest);
    cout << "strRequest: " << strRequest << endl;

    if(CURLE_OK == ret)
    {
        Json::Reader reader;
        if(!reader.parse(strRequest,response))
        {
            return musicUrl;
        }

        if(!response["data"].isNull())
        {
            musicUrl = response["data"][0]["url"].asString();
        }
    }
    return musicUrl;
}

bool HttpLink::getSingerMusicUrl(string singerName,Json::Value &rootRes)
{
    int ret = -1;
    bool res = false;
    string url,strPost,strRequest;
    Json::Value root;
    Json::FastWriter fast_write;

    string name = UrlEncode(singerName);
    cout << "UrlEncode name: " << name << endl;
   // root["key"] = Json::Value("579621905");
    root["s"] = Json::Value(name.c_str());
    root["type"] = Json::Value(2);
    root["limit"] = Json::Value(3);
    root["offset"] = Json::Value(0);
    strPost = fast_write.write(root);
    //url = string("https://api.bzqll.com/music/netease/search?");

    url = string("https://api.bzqll.com/music/netease/search?key=579621905&s=")+name+string("&type=2&limit=3&offset=0");
    ret = GetUrl(url,strRequest);
    cout << "strRequest: " << strRequest << endl;

    if(CURLE_OK == ret)
    {
        Json::Reader reader;
        if(!reader.parse(strRequest,rootRes))
        {
            return res;
        }

        if(rootRes["data"].isNull())
        {
            return res;
        }
        res = true;
        return res;

    }
    return res;
}

string HttpLink::StartPlayUrl(string mUrl)
{
    if(mUrl.empty())
        return "";

    CURLcode res;
    CURL* mCurl = curl_easy_init();
    if(NULL == mCurl)
    {
        return "";
    }
    curl_easy_setopt(mCurl, CURLOPT_URL, mUrl.c_str());

    res = curl_easy_perform(mCurl);
    if(res != CURLE_OK)
    {
        curl_easy_cleanup(mCurl);
        return "";
    }
    long response = 0;
    curl_easy_getinfo(mCurl,CURLINFO_RESPONSE_CODE,&response);
    cout << "StartPlayUrl response: " << response << endl;

    if((response != 302) && (response != 301))
    {
        curl_easy_cleanup(mCurl);
        return "";
    }
     cout << "StartPlayUrl response: " << response << endl;
    char *url =NULL;
    curl_easy_setopt(mCurl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_getinfo(mCurl,CURLINFO_REDIRECT_URL,&url);

    string v(url);
    cout << "MP3 address: " << v << endl;
    curl_easy_cleanup(mCurl);

    return v;
}

/**************************tuling***********************/
int HttpLink::tuLingPostUrl(const string text)
{
    int ret = -1;

    string text_t = UrlEncode(text);
    string url,strPost,strRequest;
    Json::Value root,per,Jtext,userinfo;
    Json::FastWriter fast_write;
    root["reqtype"] = Json::Value(1);
    Jtext["text"] = Json::Value(text_t.c_str());
    per["inputText"] = Jtext;
    userinfo["apiKey"] = Json::Value(TULING_API_KEY);
    userinfo["userId"] = Json::Value(TULING_ID);

    root["perception"] = per;
    root["userInfo"] = userinfo;
    strPost = fast_write.write(root);
    url = string("http://openapi.tuling123.com/openapi/api/v2");

    ret = PostUrl(url,strPost,strRequest);
    cout << "strRequest: " << strRequest << endl;

    if(CURLE_OK == ret)
    {
        /*
        Json::Reader reader;
        if(!reader.parse(strRequest,response))
        {
            ret = -1;
        }
        */
    }
    return ret;
}

bool HttpLink::tuLingSkillUrl(const string text, string &str)
{
    int ret = -1;
    bool res = false;

    //string text_t = UrlEncode(text);
    string url,strPost,strRequest;
    Json::Value root;
    Json::FastWriter fast_write;

    root["key"] = Json::Value(TULING_API_KEY);
    root["info"] = Json::Value(text.c_str());


    strPost = fast_write.write(root);
    url = string("http://openapi.tuling123.com/openapi/api");

    ret = PostUrl(url,strPost,strRequest);
    cout << "strRequest: " << strRequest << endl;
    Json::Value resp;
    if(CURLE_OK == ret)
    {

        Json::Reader reader;
        if(!reader.parse(strRequest,resp))
        {
            return res;
        }

        if(100000 == resp["code"].asInt())
        {
            str = resp["text"].asString();
            res = true;
        }
        else if(40004 == resp["code"].asInt())
        {
            str = string("当天请求次数已使用完");
            res = true;
        }

    }
    else
    {
        return res;
    }


    return res;
}



HttpLink::~HttpLink()
{

}
