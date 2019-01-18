#ifndef NEWS_H
#define NEWS_H

#include "singleton.h"
#include "thread.h"
#include <json/json.h>

class News :public Thread
{
public:
    News();
    int init();

    void Run();


public:
    void playNewtts();
    void stopNews(bool v);
    void setNewValue(bool enable, Json::Value resp = "");
private:
    bool mIsNewsEnable;
    Json::Value mResp;
    DECLARE_SINGLETON_CLASS(News)
};

 typedef Singleton<News> SNews;
#endif // NEWS_H
