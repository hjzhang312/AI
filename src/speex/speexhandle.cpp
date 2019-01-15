#include "speexhandle.h"
#include "type.h"
#include <list>
#include <string.h>
FILE *fp;
FILE *fp1;
FILE *fp3;
SpeexHandle::SpeexHandle()
{

}

void SpeexHandle::initialize()
{
    mMuteNum = 0;
    mVadNum = 0;
    mSpeexVadStart = false;
    st = speex_preprocess_state_init(VOICEFRAMES, FRAME_RATE);
    int denoise = 1;
    int noiseSuppress = -90;
    //打开噪声处理设置等级
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DENOISE, &denoise);
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress);

    int i = 0;
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC, &i);
    i = 16000;
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC_LEVEL, &i);

    i=0;
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB, &i);
    float f=.0;
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &f);
    f=.0;
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &f);

    //静音检测
    int vad = 1;
    int vadProbStart = 99;
    int vadProbContinue = 99;
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_VAD, &vad);
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_PROB_START, &vadProbStart);
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_PROB_CONTINUE, &vadProbContinue);
    fp = fopen("in.pcm","wb");
    fp1 = fopen("in1.pcm","wb");
    fp3 = fopen("speex.pcm","wb");

}

bool SpeexHandle::SpeexVadCheck(int16_t *buf,vector<int16_t*>&voiceList)
{

    bool retV = false;
    // fwrite(buf,2,VOICEFRAMES,fp1);
    int ret = speex_preprocess_run(st,(spx_int16_t*)buf);
    //printf("vad: %d \n",ret);

#if 1
    if(ret)
    {
        if(!mSpeexVadStart)
        {
            mVadNum++;
            if(mVadNum == 2)
            {
                mVadNum = 0;
                mSpeexVadStart = true;
            }
        }
        if(mSpeexVadStart)
        {
            mMuteNum = 0;
            int16_t *speexBuf = (int16_t*)malloc(sizeof(int16_t) * VOICEFRAMES);
            memcpy(speexBuf,buf,sizeof(int16_t) * VOICEFRAMES);
            voiceList.push_back(speexBuf);
            //fwrite(voiceList.back(),2,VOICEFRAMES,fp);
        }
    }else
    {
        if(mSpeexVadStart)
        {
            mMuteNum++;
            if(mMuteNum > MUTEVOICEFRAMES)
            {
                mMuteNum = 0;
                mSpeexVadStart = false;
                if(voiceList.size() > VADVOICEFRAMES)
                retV = true;
                else
                {
                    clearVectorList(voiceList);
                }

            }
        }
        else
        {
            mVadNum = 0;
        }
    }
#endif
    return retV;
}

void SpeexHandle::clearVectorList(vector<int16_t *> &voiceList)
{
    printf("clear vector list \n");
    for (vector<int16_t *>::iterator it = voiceList.begin(); it != voiceList.end(); it ++)
    {
        if (NULL != *it)
        {
            //printf("::::\n");
            delete (*it);
            *it = NULL;
        }
    }
    voiceList.clear();
}

SpeexHandle::~SpeexHandle()
{
    speex_preprocess_state_destroy(st);
}
