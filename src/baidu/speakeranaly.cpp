
#include "speakeranaly.h"
#include "ttshandle.h"
#include "type.h"
#include "httplink.h"
#include "playdevices.h"
#include <iostream>
#include <fstream>

static string IntentName[] = {"MUSICINFO","MUSICRANK","PAUSE","CONTINUE","CHANGE_VOL",\
                              "CHANGE_VOL_TO","CLOSE_MUSIC","USER_WEATHER","TULING"};

SpeakerAnaly::SpeakerAnaly()
{
    mPausseEnable = false;
    mTuLingEnable = false;
}

SpeakeRet SpeakerAnaly::analyStatement(string stt,Json::Value response)
{
    SpeakeRet ret;
    ret.mCode = -1;
    ret.mStt = stt;
    if(response.isNull() || (0 != response["error_code"].asInt()))
        return ret;

    Json::Value schema = response["result"]["response_list"][0]["schema"];

    if(schema["intent_confidence"] < 100)
    {
        return ret;
    }

    ret.mIntentName = schema["intent"].asString();
    ret.mIntentConfidence = schema["intent_confidence"].asDouble();

    if(!schema["slots"].isNull())
    {
        Json::Value slot = schema["slots"][0];

        ret.mIntentType = slot["name"].asString();
        string word = slot["original_word"].asString();
        ret.mWord = trim(word);
    }

    Json::Value action = response["result"]["response_list"][0]["action_list"][0];

    string skillStr = response["result"]["response_list"][0]["qu_res"]["qu_res_chosen"].asString();

    if(!skillStr.empty())
    {
        Json::Value skillJson,keyJson;
        Json::Reader reader;
        if(reader.parse(skillStr,skillJson))
        {
            string skillAction = skillJson["match_info"].asString();
            if(reader.parse(skillAction,keyJson))
            {
                string actionStr = keyJson["match_keywords"].asString();
                string str = actionStr.substr(actionStr.rfind(':')+1);
                ret.mAction = trim(str);
            }
        }

    }

    ret.mSay = action["say"].asString();

    ret.mCode = 0;

    return ret;
}

int SpeakerAnaly::messageSend(SpeakeRet info)
{
    int ret = -1;

    if(0 != info.mCode)
        return ret;

    if(info.mIntentConfidence != 100.0)
    {
        if(mTuLingEnable)
        {
            string resp;
            if(SHttpLink::instance()->tuLingSkillUrl(info.mStt,resp))
            {
                STtsHandle::instance()->playTTs(resp);
            }
        }else
        {
            string sayStr =info.mSay;
            STtsHandle::instance()->playTTs(sayStr);
        }
        return ret;
    }

    enum IntentIndex i ;
    bool isDef = false;
    for(i = MUSICINFO; i <= TULING; i=(IntentIndex)(i+1))
    {
        if(IntentName[i] == info.mIntentName)
        {
            isDef = true;
            break;
        }

    }
    if(!isDef)
        return ret;

    cout << "iiiiii: " << i << endl;
    switch (i) {
    case MUSICINFO:
    {
        string sayStr =info.mSay ;
        if(!info.mWord.empty())
            sayStr += info.mWord;
        STtsHandle::instance()->playTTs(sayStr);

        string url = SHttpLink::instance()->getMusicUrl(info.mWord);
        string v = SHttpLink::instance()->StartPlayUrl(url);
        if(!v.empty())
        {
            string cmd = string("loadfile ") + v + string("\n");
            SPlayDevices::instance()->setNetMusicEnable(cmd);
        }
        break;
    }
    case MUSICRANK:
    {
        if(info.mWord.empty())
        {
            info.mWord = "热门歌曲";
        }
        Json::Value resp;
        if(SHttpLink::instance()->getSingerMusicUrl(info.mWord,resp))
        {
            string tts = info.mSay + info.mWord + resp["data"][0]["name"].asString();
            STtsHandle::instance()->playTTs(tts);
        }
        std::ofstream ofile;
        ofile.open("./playlist.txt",ios_base::out|ios_base::trunc);

        cout<< "data size: " << resp["data"].size() << endl;
        for(int i = 0 ; i < resp["data"].size(); i++)
        {
            string url  = resp["data"][i]["url"].asString();

            string v = SHttpLink::instance()->StartPlayUrl(url);
            cout << "v: " << v << endl;
            ofile << v <<"\n";
        }
        ofile.close();
        string cmd = string("loadlist ./playlist.txt \n");
        SPlayDevices::instance()->setNetMusicEnable(cmd);


        break;
    }
    case PAUSE:
    {
        string sayStr =info.mSay ;
        if(!info.mWord.empty())
            sayStr += info.mWord;
        STtsHandle::instance()->playTTs(sayStr);

        mPausseEnable = true;
        string cmd = string("pause\n");
        SPlayDevices::instance()->setNetMusicEnable(cmd);
        break;
    }
    case CONTINUE:
    {
        if(mPausseEnable)
        {
            string sayStr =info.mSay ;
            if(!info.mWord.empty())
                sayStr += info.mWord;
            STtsHandle::instance()->playTTs(sayStr);

            string cmd = string("pause\n");
            SPlayDevices::instance()->setNetMusicEnable(cmd);
            mPausseEnable = false;
        }
        break;
    }
    case CHANGE_VOL:
    {
        string sayStr =info.mSay ;
        STtsHandle::instance()->playTTs(sayStr);

        if(info.mWord.find("大") != string::npos)
        {
            string cmd = string("volume +0.5\n");
            SPlayDevices::instance()->setNetMusicEnable(cmd);
        }
        else if(info.mWord.find("小") != string::npos)
        {
            string cmd = string("volume -0.5\n");
            SPlayDevices::instance()->setNetMusicEnable(cmd);
        }

        break;
    }
    case CHANGE_VOL_TO:
    {
        string sayStr =info.mSay ;
        STtsHandle::instance()->playTTs(sayStr);

        string cmd = string("volume ") + info.mWord + string("\n");
        SPlayDevices::instance()->setNetMusicEnable(cmd);
        break;
    }
    case CLOSE_MUSIC:
    {
        string sayStr =info.mSay ;
        STtsHandle::instance()->playTTs(sayStr);

        string cmd = string("stop\n");
        SPlayDevices::instance()->setNetMusicEnable(cmd);
        break;
    }
    case USER_WEATHER:
    {
        string sayStr =info.mSay ;
        STtsHandle::instance()->playTTs(sayStr);

        string resp;
        if(SHttpLink::instance()->tuLingSkillUrl(info.mStt,resp))
        {
            STtsHandle::instance()->playTTs(resp);
        }
        break;
    }
    case TULING:
    {
        string sayStr =info.mSay ;
        if(!info.mAction.empty())
        {
            sayStr = sayStr + info.mAction + info.mWord;
            //sayStr.insert(sayStr.rfind("图"),info.mAction);
            cout << "sayStr: " << sayStr << endl;
            STtsHandle::instance()->playTTs(sayStr);
        }
        if(info.mAction == string("打开"))
        {
            mTuLingEnable = true;
        }
        else if(string("关闭") == info.mAction)
        {
            mTuLingEnable = false;
        }
        break;
    }
    default:
        break;
    }
}

string SpeakerAnaly::trim(string &s)
{
    /*
        if( !s.empty() )
         {
             s.erase(0,s.find_first_not_of(" "));
             s.erase(s.find_last_not_of(" ") + 1);
         }
         */
    int index = 0;
    if(!s.empty())
    {
        while( (index = s.find(' ',index)) != string::npos)
        {
            s.erase(index,1);
        }
    }
    return s;
}

bool SpeakerAnaly::getTuLingStatus()
{
    return mTuLingEnable;
}

SpeakerAnaly::~SpeakerAnaly()
{

}
