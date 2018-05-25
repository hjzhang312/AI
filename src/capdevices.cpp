#include "capdevices.h"
#include <iostream>

#include <time.h>
#include <sys/time.h>

#define ERROR (-1)


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
    int rc;
    fp = fopen("x.pcm","ab+");
    rc = snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_CAPTURE, 0);
    if(rc<0)
    {
        perror("\nopen PCM device failed:");
        return ERROR;
    }

    snd_pcm_hw_params_t *hw_params;
    snd_pcm_hw_params_malloc (&hw_params); //Ϊ������������ռ�
    //snd_pcm_hw_params_malloc (&params);
    rc = snd_pcm_hw_params_any( pcm_handle, hw_params ); //������ʼ��
    if(rc<0)
    {
        fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(rc));
        return ERROR;
    }
    rc = snd_pcm_hw_params_set_access ( pcm_handle, hw_params,SND_PCM_ACCESS_RW_INTERLEAVED); //����Ϊ����ģʽ
    if(rc<0)
    {
        fprintf(stderr, "cannot set access type (%s)\n", snd_strerror(rc));
        return ERROR;
    }
    rc = snd_pcm_hw_params_set_format(pcm_handle,hw_params,SND_PCM_FORMAT_S16_LE); //ʹ����16λ����
    if(rc<0)
    {
        fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(rc));
        return ERROR;
    }
    uint rate = 44100;
    int dir = 0;
    rc = snd_pcm_hw_params_set_rate_near( pcm_handle, hw_params, &rate, &dir); //���ò�����Ϊ44.1KHz
    if(rc < 0)
    {
        fprintf(stderr, "cannot set sample rate (%s)\n", snd_strerror(rc));
        return ERROR;
    }
    rc = snd_pcm_hw_params_set_channels( pcm_handle, hw_params,2); //����Ϊ������
    if(rc<0)
    {
        fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(rc));
        return ERROR;
    }
    snd_pcm_hw_params_get_period_size( hw_params, &period_size,0); //��ȡ���ڳ���

    bit_per_sample = 16;//��ȡ��������
    chunk_byte = period_size * bit_per_sample * 2 / 8;
    //�������ڳ��ȣ��ֽ���(bytes) = ÿ���ڵ�֡�� * ��������(bit) * ͨ���� / 8 ��
    rc = snd_pcm_hw_params( pcm_handle, hw_params); //���ò���
    if(rc < 0)
    {
        fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(rc));
        return ERROR;
    }
    //params = hw_params; //��������������Ժ�ʹ��
    snd_pcm_hw_params_free( hw_params); //�ͷŲ��������ռ�
    return true;

    return 1;

}

void CapDevices::readData()
{
    int err = 0;


    while(1) {

        CurrTime();

        if(( err = snd_pcm_readi(pcm_handle, buf, 128)) == 128) {
             fwrite(buf, 4, 128,fp);
             printf("hello1 %lu %lu\n",period_size,chunk_byte);
        }
        else if(err == -EPIPE){
            {
                if ((err = snd_pcm_prepare (pcm_handle)) < 0) {
                    fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
                             snd_strerror (err));

                }
                printf("hello2\n");
            }
        }
        else
        {
            printf("hello3 +++%d \n",err);
        }
    CurrTime();

    }
}

