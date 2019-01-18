#include "playdevices.h"


#include "capdevices.h"
#include "musicipc.h"

#include "type.h"


#define ERROR (-1)

PlayDevices::PlayDevices()
{

}


int PlayDevices::init()
{
    pthread_mutex_init(&mutex,NULL);
    mIsContinue = true;
    card.init("hw:0,1",SND_PCM_STREAM_PLAYBACK);
    pcm_handle = card.getHandle();
    sem_init(&mListSem,0,0);
    sem_init(&mStartNetTTSPlayEnable,0,0);
    sem_init(&mNetMusicPlayEnable,0,0);
    return 0;
}

void PlayDevices::Run()
{
    while(1)
    {
        // writeData();
        usleep(1);
        if(0 == sem_trywait(&mStartNetTTSPlayEnable))
        {
            writeNetTTSData();
        }

        if(0 == sem_trywait(&mNetMusicPlayEnable))
        {
            writeNetMusicData();
        }
    }
}

bool PlayDevices::writeData()
{
    int err;

    /*
    err = fread(buf,2,160,fp);
    if(err == 0)
        return false;
        */
    if(0 == SCapDevices::instance()->bufData.size())
        return false;
    printf("play thread coming.... size: %d\n",SCapDevices::instance()->bufData.size());
    sem_wait(&mListSem);



    if( (err= snd_pcm_writei(card.getHandle(),SCapDevices::instance()->bufData.front(), FRAMES)) == FRAMES)
    {
        // playBufData.push_back(SCapDevices::instance()->bufData.front());
        // SCapDevices::instance()->rmData();
        // printf("playAudioSize  %lu play size: %lu \n",err,SCapDevices::instance()->bufData.size());

    }else if (err == -EPIPE) {    /* under-run */
        int ret;
        if((ret = snd_pcm_prepare (card.getHandle())) < 0)
        {
            printf ("cannot prepare audio interface for use (%s)\n",
                    snd_strerror (ret));
        }
    }
    else
    {
        printf("play error\n");
    }
    SCapDevices::instance()->rmData();
    printf("playAudioSize write %d play size: %lu \n",err,SCapDevices::instance()->bufData.size());
    return true;
}

bool PlayDevices::writeNetTTSData()
{
    if(mNetTTSData.empty())
        return false;

    mIsContinue  = true;
    int err;
    size_t index = 0;
    string netTTSDataStr = mNetTTSData.front();
    mNetTTSData.pop_front();
    char netData[2*FRAMES+1] ={0};
    while(netTTSDataStr.size() - index > 0)
    {
        memset(netData,0,sizeof(netData));
        if(!mIsContinue)
            break;


        size_t v = netTTSDataStr.copy(netData,2*FRAMES,index);

        if( (err= snd_pcm_writei(card.getHandle(),netData, v/2)) == (v/2))
        {
            index += v;
           // cout << "index: " << index << endl;
            // playBufData.push_back(SCapDevices::instance()->bufData.front());
            // SCapDevices::instance()->rmData();
            // printf("playAudioSize  %lu play size: %lu \n",err,SCapDevices::instance()->bufData.size());

        }else if (err == -EPIPE) {    /* under-run */
            int ret;
            if((ret = snd_pcm_prepare(card.getHandle())) < 0)
            {
                printf ("cannot prepare audio interface for use (%s)\n",
                        snd_strerror (ret));
            }
        }
        else
        {
            printf("play data size %d\n",err);
        }
    }


}

bool PlayDevices::writeNetMusicData()
{
    SMusicIPC::instance()->WriteFifo(mNetMusicUrl);
}

void PlayDevices::addListSem()
{
    sem_post(&mListSem);
}

void PlayDevices::setTTSPlay(string str,bool ret)
{
    if(ret)
    {
        mNetTTSData.clear();
        mIsContinue = false;
    }

    mNetTTSData.push_back(str);
    sem_post(&mStartNetTTSPlayEnable);
}

void PlayDevices::setNetMusicEnable(string cmd)
{
    mNetMusicUrl = cmd;
    sem_post(&mNetMusicPlayEnable);
}
