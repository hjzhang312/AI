
#include "ttshandle.h"

#if 1
#include "type.h"
#include "speech.h"
#include <stdio.h>
#include <streambuf>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "playdevices.h"


TtsHandle::TtsHandle()
{
    mClient =  new aip::Speech(APP_ID,API_KEY,SECRET_KEY);
}

void TtsHandle::playTTs(string text,string per,bool ret)
{
//    std::ofstream ofile;
   // ofile.open("./test.pcm",ios_base::out|ios_base::binary);
   // aip::Speech mClient;
    string tts_ret;
    map<string,string> options;
    options["vol"] = "12";
    options["aue"] = "4";
    options["per"] = per;
    if("4" == per)
    {
        options["spd"] = "3";
        options["pit"] = "12";
    }

    Json::Value result = mClient->text2audio(text, options,tts_ret);


    cout << "tts result: " << result.toStyledString() << tts_ret.size()<<endl;
    if(!tts_ret.empty())
    {
        SPlayDevices::instance()->setTTSPlay(tts_ret,ret);
       // ofile << tts_ret;
      //  ofile.close();

    }


}

TtsHandle::~TtsHandle()
{

}


#endif
