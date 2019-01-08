#ifndef SPEEXHANDLE_H
#define SPEEXHANDLE_H

#include "singleton.h"

#include <vector>
#include <speex/speex.h>
#include <speex/speex_preprocess.h>
class SpeexHandle
{
public:
    SpeexHandle();
    void initialize();

    bool SpeexVadCheck(int16_t *buf,vector<int16_t*> &voiceList);
    void clearVectorList(vector<int16_t*> &voiceList);

    ~SpeexHandle();

public:

private:
    int32_t mVadNum;
    int32_t mMuteNum;
    bool mSpeexVadStart;
    SpeexPreprocessState *st;
    DECLARE_SINGLETON_CLASS(SpeexHandle)
};

 typedef Singleton<SpeexHandle> SSpeexHandle;
#endif // SPEEXHANDLE_H
