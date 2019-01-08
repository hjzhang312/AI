#include "carddevices.h"

#include <iostream>
#include "type.h"

#define ERROR (-1)


using namespace std;

cardDevices::cardDevices()
{

}

int cardDevices::init(std::string name,snd_pcm_stream_t mode)
{
    int rc;
    rc = snd_pcm_open(&pcm_handle, name.c_str(), mode, 0);
    if(rc<0)
    {
        perror("\nopen PCM device failed:");
        return ERROR;
    }

    snd_pcm_hw_params_t *hw_params;
    snd_pcm_hw_params_malloc(&hw_params); //为参数变量分配空间
    //snd_pcm_hw_params_malloc (&params);
    rc = snd_pcm_hw_params_any(pcm_handle, hw_params ); //参数初始化
    if(rc<0)
    {
        fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(rc));
        return ERROR;
    }
    rc = snd_pcm_hw_params_set_access ( pcm_handle, hw_params,SND_PCM_ACCESS_RW_INTERLEAVED); //设置为交错模式
    if(rc<0)
    {
        fprintf(stderr, "cannot set access type (%s)\n", snd_strerror(rc));
        return ERROR;
    }
    rc = snd_pcm_hw_params_set_format(pcm_handle,hw_params,SND_PCM_FORMAT_S16_LE); //使用用16位样本
    if(rc<0)
    {
        fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(rc));
        return ERROR;
    }
    uint rate = 16000;
    int dir = 0;
    rc = snd_pcm_hw_params_set_rate_near(pcm_handle, hw_params, &rate, &dir); //设置采样率为44.1KHz
    if(rc < 0)
    {
        fprintf(stderr, "cannot set sample rate (%s)\n", snd_strerror(rc));
        return ERROR;
    }
    rc = snd_pcm_hw_params_set_channels( pcm_handle, hw_params,CHANNEL); //设置为立体声
    if(rc<0)
    {
        fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(rc));
        return ERROR;
    }


    unsigned long periodsize = FRAMES * 2 * CHANNEL; // 2:表示立体声 1道2byte
    rc = snd_pcm_hw_params_set_buffer_size_near(pcm_handle,hw_params, &periodsize);
    if (rc < 0)
    {
        fprintf(stderr,"Unable to set buffer size %lu : (%s)\n", periodsize, snd_strerror(rc));
        return ERROR;
    }

    cout << "set buffer size " << endl;
    unsigned long frames = FRAMES;

    rc = snd_pcm_hw_params_set_period_size_near(pcm_handle,hw_params, &frames, &dir);
    if(rc<0)
    {
        fprintf(stderr, "cannot set period size (%s)\n", snd_strerror(rc));
        return ERROR;
    }

       // printf("size %lu \n",frames);

    rc = snd_pcm_hw_params(pcm_handle,hw_params); //设置参数
    if(rc < 0)
    {
        fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(rc));
        return ERROR;
    }
    unsigned long period = 0;
    snd_pcm_hw_params_get_period_size( hw_params, &period,&dir); //获取周期长度
    printf("frames :%d \n",period);
    //params = hw_params; //保存参数，方便以后使用
    bit_per_sample=16;   //获取样本长度
    chunk_byte = frames * bit_per_sample * 2 / 8;
    //计算周期长度（字节数(bytes) = 每周期的帧数 * 样本长度(bit) * 通道数 / 8 ）

    snd_pcm_hw_params_free(hw_params); //释放参数变量空间


    hw_params = NULL;

    int err =0;
    if ((err = snd_pcm_prepare (pcm_handle)) < 0) {
                    printf ("cannot prepare audio interface for use (%s)\n",
                                     snd_strerror (err));
            }


    return 1;
}

snd_pcm_t* cardDevices::getHandle()
{
    return pcm_handle;
}

