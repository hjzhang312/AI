#include "webrtchand.h"

#include <string>
#include <iostream>

#include "webrtc/modules/interface/module_common_types.h"
WebrtcHand::WebrtcHand()
{

}

void WebrtcHand::init()
{
    int rc;
    fp = fopen("out.pcm","wb");

    apm = webrtc::AudioProcessing::Create();

    apm->high_pass_filter()->Enable(true);
    apm->echo_cancellation()->Enable(true);




    apm->noise_suppression()->set_level(webrtc::NoiseSuppression::kHigh);
    apm->noise_suppression()->Enable(true);

    apm->gain_control()->set_analog_level_limits(0, 255);
    apm->gain_control()->set_mode(webrtc::GainControl::kAdaptiveAnalog);
    apm->gain_control()->set_stream_analog_level(255);
    apm->gain_control()->Enable(true);

    apm->echo_cancellation()->set_suppression_level(webrtc::EchoCancellation::kModerateSuppression);
    apm->voice_detection()->set_likelihood( webrtc::VoiceDetection::kModerateLikelihood);
    apm->voice_detection()->Enable(true);

    frame = new webrtc::AudioFrame();

    frame->sample_rate_hz_ = 16000;
    frame->num_channels_ = 1;
    float frame_step = 10;  // ms
    frame->samples_per_channel_ = frame->sample_rate_hz_ * frame_step / 1000.0;
}

void WebrtcHand::HandAudio(void *buf)
{
    static int capture_level = 255;
    memcpy(frame->data_,(int16_t*)buf,160*1);

    apm->gain_control()->set_stream_analog_level(capture_level);
    apm->set_stream_delay_ms(10);

    //int err =  apm->AnalyzeReverseStream(frame);

    int err = apm->ProcessStream(frame);
    capture_level = apm->gain_control()->stream_analog_level();//模拟模式下，必须在ProcessStream之后调用此方法，获取新的音频HAL的推荐模拟值。

    bool stream_has_voice =apm->voice_detection()->stream_has_voice();

    cout << "err: " << err << "capture_level: " << capture_level << " stream_has_voice: " << stream_has_voice << endl;
    fwrite(frame->data_, 2, 160,fp);
    frame->Mute();
      apm->Initialize();

}
