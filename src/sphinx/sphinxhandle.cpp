
#include "sphinxhandle.h"
#include "type.h"
#include <unistd.h>
#include "speexhandle.h"

FILE *fp2;

SphinxHandle::SphinxHandle()
{

}

int SphinxHandle::initialize(string hmmName, string lmName, string dictName)
{
    fp2 = fopen("sphinx.pcm","wb");
    cmd_ln_t *config;
    config = cmd_ln_init(NULL, ps_args(),TRUE,"-hmm",hmmName.c_str(),\
                         "-lm",lmName.c_str(),"-dict",dictName.c_str(),NULL);
    if (config == NULL)
    {
        printf("cmd_in_init error\n");
        return 0;
    }

    //2、初始化解码器（语言识别就是一个解码过程，通俗的将就是将你说的话解码成对应的文字串）
    ps = ps_init(config);
    if (ps == NULL)
        return 0;

    sleep(1);
}

bool SphinxHandle::getVoiceKeyword(vector<int16_t *> &voiceList)
{
    printf("getVoiceKeyword.........\n");

    bool ret = false;
    string keyword ="";
    if(voiceList.empty())
        return ret;
    char const *hyp, *uttid;
    int rv = ps_start_utt(ps,NULL);
    if (rv < 0)
    {
        printf("ps start utt error \n");
        SSpeexHandle::instance()->clearVectorList(voiceList);
        return ret;
    }
    for(vector<int16_t*>::iterator iter= voiceList.begin();iter != voiceList.end();iter++)
    {
       /* rv = ps_start_utt(ps,NULL);
           if(rv < 0)
           {
               printf("ps start utt error \n");
               return keyword;
           }*/
        //fwrite(*iter,2,VOICEFRAMES,fp2);
        size_t nsamp = VOICEFRAMES ;
       // printf("nsamp:  %d \n",nsamp);

        //我们将每次从文件中读取160大小的样本，使用ps_process_raw()把它们放到解码器中:
        rv = ps_process_raw(ps,*iter, nsamp, FALSE, FALSE);
        //rv = ps_get_in_speech(ps);
       // printf("sphinx: %d \n",rv);

    }
    SSpeexHandle::instance()->clearVectorList(voiceList);
    fflush(stdout);
    rv = ps_end_utt(ps);
    if (rv < 0)
    {
        return ret;
    }

    //以相同精确的方式运行来检索假设的字符串：
    hyp = ps_get_hyp(ps, NULL, &uttid);
    keyword = hyp;
    if (hyp == NULL)
    {
        return ret;
    }
    //keyword = hyp;
    fflush(stdout);
    printf("Recognized: %s voiceList: %d \n", hyp,voiceList.size());

    if("天猫精灵" == keyword)
    {
        ret = true;
    }


    return ret;

}

SphinxHandle::~SphinxHandle()
{
    ps_free(ps);
}
