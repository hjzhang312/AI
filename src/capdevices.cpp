#include "capdevices.h"
#include <iostream>

#include <time.h>
#include <sys/time.h>
#include <stdio.h>

#include "ttshandle.h"
#include "playdevices.h"
#include "speexhandle.h"
#include "sphinxhandle.h"
#include "asrhandle.h"
#include "speakeranaly.h"
#include "type.h"
#define ERROR (-1)

static string mKeyResp[] = {" 我在，你说"," 哎，在呢"," 主人，你说"," 来啦",\
                            " 肿么啦"," 你好主人"};

void CurrTime()
{
    struct timeval	tv;
    struct tm tim;
    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &tim);

    printf("%04d/%02d/%02d %02d:%02d:%02d.%06d\n",
           tim.tm_year + 1900,
           tim.tm_mon + 1,
           tim.tm_mday,
           tim.tm_hour,
           tim.tm_min,
           tim.tm_sec,
           (int)(tv.tv_usec));
}

using namespace std;
CapDevices::CapDevices()
{

}

int CapDevices::init()
{
    pthread_mutex_init(&mutex,NULL);
    mVoiceList.clear();
    fp = fopen("cap.pcm","wb");
    card.init("hw:0,0",SND_PCM_STREAM_CAPTURE);
    srand((unsigned int )time(NULL));
    mAwake = false;
    // pcm_handle = card.getHandle();
    return 1;

}

void CapDevices::readData()
{
    int err = 0;
    bufData.clear();

    while(1)
    {
        //   CurrTime();
        usleep(1);
        memset(buf,0,sizeof(buf));

        //sleep(5);
        //   CurrTime();
        if(( err = snd_pcm_readi(card.getHandle(), buf, FRAMES)) > 0) {
            //for(int i = 320;i < 325; i++)
            // printf(" buf : %d hh \n",buf[i]);
           // fwrite(buf,2,VOICEFRAMES,fp);

            if(SSpeexHandle::instance()->SpeexVadCheck(buf,mVoiceList))
            {
                printf("mVoiceList: %d \n", mVoiceList.size());

               //SAsrHandle::instance()->getAsrSTT(mVoiceList);
                if(SSpeakerAnaly::instance()->getTuLingStatus())
                {
                    SAsrHandle::instance()->getAsrSTT(mVoiceList);
                }
                else if(mAwake)
                {
                    SAsrHandle::instance()->getAsrSTT(mVoiceList);
                    mAwake = false;
                }
                else
                {
                    mAwake = SSphinxHandle::instance()->getVoiceKeyword(mVoiceList);

                    if(mAwake)
                    {
                        mTime = time((time_t*)NULL);
                        printf("mKeyResp:::::%s ,time: %ld\n",mKeyResp[rand()%3],mTime);
                        STtsHandle::instance()->playTTs(mKeyResp[rand()%6],string("4"));
                    }

                }



#if 0
                for (vector<int16_t *>::iterator it = mVoiceList.begin(); it != mVoiceList.end(); it ++)
                {
                    if (NULL != *it)
                    {
                        delete *it;
                        *it = NULL;
                    }
                }
                mVoiceList.clear();
#endif
                printf("mVoiceList: %d \n", mVoiceList.size());
            }
            else
            {
                if(mAwake)
                {
                    time_t t = time((time_t*)NULL);
                    printf("time ::: %ld \n",t - mTime);
                    if(t - mTime > 20)
                    {
                        mAwake = false;
                    }
                }

            }

            //addData(buf);

            //printf("readData: ...%lu cap size %lu \n",err,bufData.size());


        }
        else if(err == -EPIPE){
            {
                if ((err = snd_pcm_prepare (card.getHandle())) < 0) {
                    printf ("cannot prepare audio interface for use (%s)\n",
                            snd_strerror (err));
                }
            }
        }
        else
        {
            printf("cap under error +++%d \n",err);
        }

#if 0
        if( (err= snd_pcm_writei(SPlayDevices::instance()->card.getHandle(),buf, FRAMES)) == FRAMES)
        {
            // playBufData.push_back(SCapDevices::instance()->bufData.front());
            rmData();
            printf("playAudioSize  %lu play size: %lu \n",err,bufData.size());


        }else if (err == -EPIPE) {    /* under-run */
            int ret;
            if((ret = snd_pcm_prepare (SPlayDevices::instance()->card.getHandle())) < 0)
            {
                printf ("cannot prepare audio interface for use (%s)\n",
                        snd_strerror (ret));
            }

        }
        else
        {
            printf("play error\n");
        }
#endif
        //CurrTime();

    }
}

void CapDevices::addData(int16_t buf[])
{
    pthread_mutex_lock(&mutex);
    bufData.push_back(buf);
    SPlayDevices::instance()->addListSem();
    pthread_mutex_unlock(&mutex);

}

void CapDevices::rmData()
{
    pthread_mutex_lock(&mutex);
    bufData.pop_front();
    pthread_mutex_unlock(&mutex);
}

void CapDevices::testPipe()
{
    int pid;
    int fd[2];
    if(pipe(fd) < 0)
    {
        printf("create fail \n");
    }

    if((pid = fork()) < 0)
    {
        printf("fork fail \n");
    }

    if(pid > 0)
    {
        close(fd[0]);
        cardDevices card;
        card.init("hw:0,0",SND_PCM_STREAM_CAPTURE);
        while(1)
        {
            int err = 0;
            if(( err = snd_pcm_readi(card.getHandle(), buf, FRAMES)) > 0) {
                //for(int i = 320;i < 325; i++)
                // printf(" buf : %d hh \n",buf[i]);

                write(fd[1],buf,FRAMES*4);
                printf("pipe write ok\n");
                //printf("write: ...%lu cap size %lu \n",err,bufData.size());


            }
            else if(err == -EPIPE){
                {
                    if ((err = snd_pcm_prepare (card.getHandle())) < 0) {
                        printf ("cannot prepare audio interface for use (%s)\n",
                                snd_strerror (err));
                    }
                }
            }
            else
            {
                printf("hello3 +++%d \n",err);
            }
        }
    }
    else
    {
        close(fd[1]);
        SPlayDevices::instance()->init();
        while(1)
        {
            int err;
            int16_t buf[320] = {0};
            err = read(fd[0],buf,FRAMES*4);
            printf("read pipe %d \n",err);
            if( (err= snd_pcm_writei(SPlayDevices::instance()->card.getHandle(),buf, FRAMES)) == FRAMES)
            {



            }else if (err == -EPIPE) {    /* under-run */
                int ret;
                if((ret = snd_pcm_prepare (SPlayDevices::instance()->card.getHandle())) < 0)
                {
                    printf ("cannot prepare audio interface for use (%s)\n",
                            snd_strerror (ret));
                }

            }
            else
            {
                printf("play error\n");
            }
        }
    }
}


void CapDevices::Run()
{
    while(1)
    {

        readData();

    }
}
