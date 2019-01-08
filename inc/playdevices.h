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

    snd_pcm_t                 *pcm_handle;        //PCI�豸���
    snd_pcm_uframes_t         frames;
    snd_pcm_hw_params_t       *hw_params;//Ӳ����Ϣ��PCM������

    unsigned long bit_per_sample; //��������(bit)
    unsigned long period_size; //���ڳ���(֡��)
    unsigned long chunk_byte; //���ڳ���(�ֽ���)

    DECLARE_SINGLETON_CLASS(PlayDevices)
};

 typedef Singleton<PlayDevices> SPlayDevices;

#endif // PLAYDEVICES_H
