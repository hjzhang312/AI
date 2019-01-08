#ifndef ASRHANDLE_H
#define ASRHANDLE_H
#if 1

#include "singleton.h"
#include <iostream>
#include <vector>
#include <json/json.h>

namespace aip
{
class  Speech;
}

class AsrHandle
{

public:
    AsrHandle();
    void init();
    void getAsrSTT(vector<int16_t*> &voiceLsit);

    bool sendAsrMessage(Json::Value root);
    ~AsrHandle();

public:

private:
    std::string mVoiceStr;
    aip::Speech *mClient;

    DECLARE_SINGLETON_CLASS(AsrHandle)
};

 typedef Singleton<AsrHandle> SAsrHandle;


#endif

#endif // ASRHANDLE_H
