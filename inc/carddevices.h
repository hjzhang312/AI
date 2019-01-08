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
    snd_pcm_t                 *pcm_handle;        //PCI设备句柄
    snd_pcm_uframes_t         frames;
   // snd_pcm_hw_params_t       *hw_params;//硬件信息和PCM流配置
//snd_pcm_hw_params_t *hw_params;
    unsigned long bit_per_sample; //样本长度(bit)
    unsigned long period_size; //周期长度(帧数)
    unsigned long chunk_byte; //周期长度(字节数)
};

#endif // CARDDEVICES_H
