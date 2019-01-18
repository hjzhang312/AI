#ifndef HTTPLINK_H
#define HTTPLINK_H
#include "singleton.h"

#include <vector>
#include <json/json.h>

class HttpLink
{
public:
    HttpLink();
    void initialize();
    unsigned char ToHex(unsigned char x);
    string UrlEncode(const std::string& str);
    static size_t WriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid);

    string code_convert_utf8(char *source_charset, char *to_charset, const std::string &sourceStr);

    int PostUrl(const string strUrl,const string strPost,string &strResponse);
    int GetUrl(const string strUrl, string &strResponse);

    int unitSpeaker(string speaker,Json::Value &response);

    string getMusicUrl(string musicName);
    bool getSingerMusicUrl(string singerName,Json::Value &rootRes);

    string StartPlayUrl(string url);


    /*************************tuling*********************/
    int tuLingPostUrl(const string text);
    bool tuLingSkillUrl(const string text,string &str);
    bool newsSkillUrl(const string text, Json::Value &resp);

    ~HttpLink();

public:

private:

    DECLARE_SINGLETON_CLASS(HttpLink)
};

 typedef Singleton<HttpLink> SHttpLink;
#endif // HTTPLINK_H
