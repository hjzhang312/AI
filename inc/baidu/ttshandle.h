#ifndef TTSHANDLE_H
#define TTSHANDLE_H

#if 1
#include "singleton.h"
#include "type.h"
#include <iostream>
#include <vector>
#include <json/json.h>

namespace aip {
 class Speech;
}
class TtsHandle
{
public:
    TtsHandle();

    void playTTs(string text,string per = "0",bool ret = false);

    ~TtsHandle();

public:

private:
     aip::Speech *mClient;

    DECLARE_SINGLETON_CLASS(TtsHandle)
};

 typedef Singleton<TtsHandle> STtsHandle;
#endif
#endif // TTSHANDLE_H
