
#include "asrhandle.h"
#if 1
#include "type.h"
#include "speech.h"
#include <stdio.h>
#include <streambuf>
#include <iostream>

#include "speexhandle.h"
#include "httplink.h"
#include "speakeranaly.h"
#include "musicdown.h"
#include "musicipc.h"
#include "ttshandle.h"


FILE *asr;

AsrHandle::AsrHandle()
{
    std::string scret;
    mClient =  new aip::Speech(APP_ID,API_KEY,SECRET_KEY);
    printf("hhhh\n");
    /*
    asr = fopen("16k_test.pcm","rb");
    fseek(asr,0L,SEEK_END);
    int size = ftell(asr);
    printf("szie: %d \n",size);
    fseek(asr,0L,SEEK_SET);
    */
#if 0
    std::ifstream f;
    f.open("./16k_test.pcm");
    f.seekg(0,std::ios_base::end);
    long l = f.tellg();
    char *buffer;
    buffer = new char[l]; //129600
    f.seekg((0,std::ios_base::beg));
    f.read(buffer,l);
    mVoiceStr = string(buffer,l);
    printf("l:%d  %d \n",l,mVoiceStr.size());
#endif

/*
    std::ifstream f("16k_test.pcm");
    std::stringstream buffer;
    buffer << f.rdbuf();
    mVoiceStr(buffer);
    */
   // std::string str(std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>());

   // printf ("str: %s \n",str);
   // string a = str;
    //mVoiceStr = str;
}

void AsrHandle::init()
{
    int i =0;
    int j =0;
    vector<int16_t *> a;
    getAsrSTT(a);
    while(0)
    {
        j++;
        char *t = (char*)malloc(FRAMES+1);
        if((i = fread(t,1,FRAMES,asr)) !=FRAMES)
        {
             mVoiceStr = mVoiceStr + string(t);
            vector<int16_t *> a;
            getAsrSTT(a);
            printf("j: %d eee\n",j);

            return;
        }
        else
        {
           // mVoiceStr.push_back(t);
            mVoiceStr = mVoiceStr + string(t);
        }

    }

}

void AsrHandle::getAsrSTT(vector<int16_t *> &voiceLsit)
{
    if(voiceLsit.empty())
        return;
    mVoiceStr.clear();
    printf(" getAsrSTT....\n");
    //mVoiceStr.clear();
#if 1
    for(int i = 0; i < voiceLsit.size(); i++)
    {
       // char tmp[1024] = {0};
       // char *tmp = (char*)malloc(sizeof(char) * VOICEFRAMES * 2);
       // memcpy(tmp,(char*)voiceLsit.at(i),sizeof(int16_t) * VOICEFRAMES);
        //snprintf(tmp,320,"%s",(char*)voiceLsit.at(i));
        mVoiceStr += string((char*)voiceLsit.at(i),2*FRAMES);
    }
    SSpeexHandle::instance()->clearVectorList(voiceLsit);
#endif


#if 1

   // aip::get_file_content("./16k_test.pcm",&mVoiceStr);
    std::map<std::string, std::string> options;
    options["dev_pid"] = DEV_LANGUAGE;
    Json::Value result = mClient->recognize(mVoiceStr, "pcm", FRAME_RATE, options);
    Json::FastWriter fast_write;
    printf("... %d \n",mVoiceStr.size());
    string strRes = fast_write.write(result);
    cout << "result: " << strRes <<endl;
    sendAsrMessage(result);


#endif

#if 0
    //string strRes = "播放成都。";
    Json::Value response;
    SHttpLink::instance()->unitSpeaker(strRes,response);

    SpeakeRet ret =  SSpeakerAnaly::instance()->analyStatement(response);

    cout << " mCode: " << ret.mCode << "mIntentName:" << ret.mIntentName <<"mIntenrType: " << ret.mIntentType << "Say: "<< ret.mSay <<"word: " << ret.mWord << endl;
    STtsHandle::instance()->playTTs(ret.mSay);

    string url = SHttpLink::instance()->getMusicUrl(ret.mWord);

    string v = SHttpLink::instance()->StartPlayUrl(url);
    //SMusicDown::instance()->setUrl(url);

    string cmd = string("loadfile ") + v + string("\n");
    SMusicIPC::instance()->WriteFifo(cmd);


#endif
    // printf("result: %s \n",result.asCString());
}

bool AsrHandle::sendAsrMessage(Json::Value root)
{
#if 1
    bool retV = false;
    if(0 != root["err_no"].asInt())
        return retV;
    if(root["result"].isNull())
        return retV;
    string res = root["result"][0].asString();
#endif

    Json::Value response;

    //string res = "关闭图灵";//"南京今天的天气";//"关闭图灵";
    SHttpLink::instance()->unitSpeaker(res,response);

    SpeakeRet ret =  SSpeakerAnaly::instance()->analyStatement(res,response);


    cout << "STT: " << ret.mStt << " mCode: " << ret.mCode <<" mIntentConfidence: " << ret.mIntentConfidence << " mIntentName:" << ret.mIntentName <<" mIntenrType: " << ret.mIntentType << " Say: "<< ret.mSay <<" word: " << ret.mWord  <<" Action: " <<ret.mAction << endl;
    SSpeakerAnaly::instance()->messageSend(ret);
    /*
    string url = SHttpLink::instance()->getMusicUrl(ret.mWord);

    string v = SHttpLink::instance()->StartPlayUrl(url);
    //SMusicDown::instance()->setUrl(url);

    string cmd = string("loadfile ") + v + string("\n");
    SMusicIPC::instance()->WriteFifo(cmd);
    */
}

AsrHandle::~AsrHandle()
{

}

#endif
