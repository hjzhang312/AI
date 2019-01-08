#ifndef TYPE_H
#define TYPE_H

#include <string>
#define APP_ID   "15167073"
#define API_KEY  "Y4DLHdvnUjahMAUakOLv40bR"
#define SECRET_KEY "wsoSyvXaOUHhNAsNh2VMvU8ZpzgLNPyW"
#define DEV_LANGUAGE "1536"

#define AccessToken "24.0c9e48b248900932c329eb84ebe37dce.2592000.1548384450.282335-15167073"

#define TULING_API_KEY "b44f19e7601a4c4299cf6354a55f9cf3"
#define TULING_ID "375129"

#define CHANNEL 1
#define FRAME_RATE 16000
#define FRAMES  (FRAME_RATE/100 *2)  //20ms
#define VOICEFRAMES  ((FRAME_RATE/100 * 2) * CHANNEL)

#define MUTEVOICEFRAMES  50
#define VADVOICEFRAMES  50

#define HMM  "/home/zhj/work/AI/TAR3398/tdt_sc_8k/"
#define  LM  "/home/zhj/work/AI/TAR3398/3398.lm"
#define DICT "/home/zhj/work/AI/TAR3398/3398.dic"


typedef struct
{
    int mCode;
    std::string mStt;
    std::string mIntentName;
    double mIntentConfidence;
    std::string mIntentType;
    std::string mWord;
    std::string mAction;
    std::string mSay;
}SpeakeRet;

enum IntentIndex
{
    MUSICINFO = 0x00,
    MUSICRANK,
    PAUSE,
    CONTINUE,
    CHANGE_VOL,
    TULING
};


#endif // TYPE_H
