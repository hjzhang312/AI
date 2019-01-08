#ifndef CARDDEVICES_H
#define CARDDEVICES_H
#include <string.h>
#include <iostream>
#include <alsa/asoundlib.h>

class cardDevices
{
public:
    cardDevices();

    int init(std::string name,snd_pcm_stream_t mode);
    snd_pcm_t* getHandle();

private:
    snd_pcm_t                 *pcm_handle;        //PCI�豸���
    snd_pcm_uframes_t         frames;
   // snd_pcm_hw_params_t       *hw_params;//Ӳ����Ϣ��PCM������
//snd_pcm_hw_params_t *hw_params;
    unsigned long bit_per_sample; //��������(bit)
    unsigned long period_size; //���ڳ���(֡��)
    unsigned long chunk_byte; //���ڳ���(�ֽ���)
};

#endif // CARDDEVICES_H
