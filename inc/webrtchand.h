#ifndef WEBRTCHAND_H
#define WEBRTCHAND_H

#include "singleton.h"
#include "audio_processing.h"

class WebrtcHand
{
public:
    WebrtcHand();

    void init();

    void HandAudio(void *buf);
    DECLARE_SINGLETON_CLASS(WebrtcHand)

    private:
        FILE *fp;
        webrtc::AudioProcessing* apm;
        webrtc::AudioFrame *frame;
};

 typedef Singleton<WebrtcHand> SWebrtcHand;


#endif // WEBRTCHAND_H
