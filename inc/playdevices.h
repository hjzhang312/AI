#ifndef PLAYDEVICES_H
#define PLAYDEVICES_H

#include "singleton.h"
#include "thread.h"
#include "carddevices.h"


#include <list>
#include <alsa/asoundlib.h>
#include <semaphore.h>
#include <sstream>

class PlayDevices :public Thread
{
public:
    PlayDevices();
    int init();

    void Run();

    bool writeData();

    bool writeNetTTSData();
    bool writeNetMusicData();

    void addListSem();
    void setTTSPlay(string str ="");

    void setNetMusicEnable(string cmd);
public:
    cardDevices card;
    pthread_mutex_t  mutex;
    std::stringstream stream;
    list<string> mNetTTSData;
    list<int16_t*> playBufData;
private:
    FILE *fp;
    string mNetMusicUrl;
    sem_t mStartNetTTSPlayEnable;
    sem_t mNetMusicPlayEnable;
    sem_t mListSem; /*wait data*/
    int16_t buf[1024];

    snd_pcm_t                 *pcm_handle;        //PCI设备句柄
    snd_pcm_uframes_t         frames;
    snd_pcm_hw_params_t       *hw_params;//硬件信息和PCM流配置

    unsigned long bit_per_sample; //样本长度(bit)
    unsigned long period_size; //周期长度(帧数)
    unsigned long chunk_byte; //周期长度(字节数)

    DECLARE_SINGLETON_CLASS(PlayDevices)
};

 typedef Singleton<PlayDevices> SPlayDevices;

#endif // PLAYDEVICES_H
