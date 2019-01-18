#include "news.h"
#include "ttshandle.h"
#include <unistd.h>

News::News()
{

}

void News::Run()
{
    while(1)
    {
        usleep(10);
        playNewtts();
    }
}

void News::playNewtts()
{
     //printf("platNewtts.....\n");
    if(mResp.empty())
        return;

    printf("platNewtts.....%d ..\n",mResp["list"].size());
   for(int i = 0; i < mResp["list"].size(); i++)
   {
       if(mIsNewsEnable)
       {
           string str = mResp["list"][i]["article"].asString();
           STtsHandle::instance()->playTTs(str);
           sleep(1);
       }
       else
       {
           break;
       }
   }
   mIsNewsEnable = false;
   mResp.clear();
}

void News::stopNews(bool v)
{
    printf("aaaaaaa\n");
    mIsNewsEnable = v;
}

void News::setNewValue(bool enable, Json::Value resp)
{

    mIsNewsEnable = enable;
    mResp = resp;
}

