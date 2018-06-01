#ifndef CAPDEVICES_H
#define CAPDEVICES_H

#include "singleton.h"

#include <alsa/asoundlib.h>
/*样本长度(sample)：样本是记录音频数据最基本的单位，常见的有8位和16位。
通道数(channel)：该参数为1表示单声道，2则是立体声。
帧(frame)：帧记录了一个声音单元，其长度为样本长度与通道数的乘积。
采样率(rate)：每秒钟采样次数，该次数是针对帧而言。
周期(period)：音频设备一次处理所需要的帧数，对于音频设备的数据访问以及音频数据的存储，都是以此为单位。
交错模式(interleaved)：是一种音频数据的记录方式，在交错模式下，数据以连续帧的形式存放，
即首先记录完帧1的左声道样本和右声道样本（假设为立体声格式），再开始帧2的记录。
而在非交错模式下，首先记录的是一个周期内所有帧的左声道样本，再记录右声道样本，数据是以连续通道的方式存储。
*/

class CapDevices
{
public:
    CapDevices();

    int init();

    void readData();

private:
    FILE *fp;
    int16_t buf[1024];
    unsigned long bit_per_sample; //样本长度(bit)
    unsigned long period_size; //周期长度(帧数)
    unsigned long chunk_byte; //周期长度(字节数)
    snd_pcm_t                 *pcm_handle;        //PCI设备句柄
    snd_pcm_uframes_t         frames;
    snd_pcm_hw_params_t       *hw_params;//硬件信息和PCM流配置

     DECLARE_SINGLETON_CLASS(CapDevices)
};

  typedef Singleton<CapDevices> SCapDevices;

#endif // CAPDEVICES_H
