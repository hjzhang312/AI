#ifndef SPEAKERANALY_H
#define SPEAKERANALY_H


#include "singleton.h"

#include <vector>
#include <json/json.h>
#include <type.h>

class SpeakerAnaly
{

public:
    SpeakerAnaly();

    SpeakeRet analyStatement(string stt,Json::Value response);
    int messageSend(SpeakeRet value);
    string trim(string &s);
    bool getTuLingStatus();

    ~SpeakerAnaly();

public:

private:
    bool mPausseEnable;
    bool mTuLingEnable;
    DECLARE_SINGLETON_CLASS(SpeakerAnaly)
};

 typedef Singleton<SpeakerAnaly> SSpeakerAnaly;
#endif // SPEAKERANALY_H
