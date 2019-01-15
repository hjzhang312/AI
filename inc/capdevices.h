#ifndef CAPDEVICES_H
#define CAPDEVICES_H


#include "singleton.h"
#include "thread.h"
#include <alsa/asoundlib.h>
/*��������(sample)�������Ǽ�¼��Ƶ����������ĵ�λ����������8λ��16λ��
ͨ����(channel)���ò���Ϊ1��ʾ��������2������������
֡(frame)��֡��¼��һ��������Ԫ���䳤��Ϊ����������ͨ�����ĳ˻���
������(rate)��ÿ���Ӳ����������ô��������֡���ԡ�
����(period)����Ƶ�豸һ�δ�������Ҫ��֡����������Ƶ�豸�����ݷ����Լ���Ƶ���ݵĴ洢�������Դ�Ϊ��λ��
����ģʽ(interleaved)����һ����Ƶ���ݵļ�¼��ʽ���ڽ���ģʽ�£�����������֡����ʽ��ţ�
�����ȼ�¼��֡1������������������������������Ϊ��������ʽ�����ٿ�ʼ֡2�ļ�¼��
���ڷǽ���ģʽ�£����ȼ�¼����һ������������֡���������������ټ�¼������������������������ͨ���ķ�ʽ�洢��
*/

#include "carddevices.h"
#include <list>
#include <vector>
#include <time.h>

class CapDevices : public Thread
{
public:
    CapDevices();

    int init();

    void readData();
    void addData(int16_t buf[]);
    void rmData();

    void testPipe();

    void Run();

public:
    list<int16_t*> bufData;

private:
    FILE *fp;
    bool mAwake;
    time_t mTime;
    pthread_mutex_t  mutex;
    int16_t buf[1024];
    vector<int16_t*> mVoiceList;
    cardDevices card;
    unsigned long bit_per_sample; //��������(bit)
    unsigned long period_size; //���ڳ���(֡��)
    unsigned long chunk_byte; //���ڳ���(�ֽ���)
    snd_pcm_t                 *pcm_handle;        //PCI�豸���
    snd_pcm_uframes_t         frames;
    snd_pcm_hw_params_t       *hw_params;//Ӳ����Ϣ��PCM������

     DECLARE_SINGLETON_CLASS(CapDevices)
};

  typedef Singleton<CapDevices> SCapDevices;

#endif // CAPDEVICES_H
