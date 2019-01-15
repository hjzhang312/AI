#include <iostream>

using namespace std;


#include "capdevices.h"
#include "playdevices.h"
#include "speexhandle.h"
#include "sphinxhandle.h"
#include "asrhandle.h"
#include "httplink.h"
#include "musicdown.h"
#include "musicipc.h"

#include "type.h"
#include "ttshandle.h"
#include <sstream>


int main()
{
   // printf("start %ld \n",time((time_t*)NULL));
   // sleep(3);
   // printf("end %ld \n",time((time_t*)NULL));
   // SHttpLink::instance()->tuLingPostUrl("附近的酒店");


#if 0
    string k ;
    SPlayDevices::instance()->init();
    STtsHandle::instance()->playTTs("欢迎使用百度语音");
    SPlayDevices::instance()->Start();
    SPlayDevices::instance()->Join();


    //printf("%d %d\n",sizeof(int16_t),sizeof(int16));
    //cout << SHttpLink::instance()->UrlEncode("来?@") <<endl;
    cout << "k: " << k.size() << endl;




    #endif
 #if 1
    SMusicIPC::instance()->initialize();


   SSpeexHandle::instance()->initialize();
    SSphinxHandle::instance()->initialize(HMM,LM,DICT);
    SHttpLink::instance()->initialize();


    SCapDevices::instance()->init();
    SPlayDevices::instance()->init();

   // SCapDevices::instance()->testPipe();
    //SMusicDown::instance()->Start();
    SCapDevices::instance()->Start();
    SPlayDevices::instance()->Start();

    //SAsrHandle::instance()->init();

    //SMusicDown::instance()->Join();
    SCapDevices::instance()->Join();
    SPlayDevices::instance()->Join();
#endif

    cout << "Hello World!" << endl;
    return 0;
}

