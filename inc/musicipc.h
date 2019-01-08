#ifndef MUSICIPC_H
#define MUSICIPC_H


#include "singleton.h"

#include <vector>
#include <json/json.h>
#include <type.h>

class MusicIPC
{
public:
    MusicIPC();

    void initialize();

    ~MusicIPC();

public:
    void WriteFifo(string cmd);
private:
    int fd;
    DECLARE_SINGLETON_CLASS(MusicIPC)
};

 typedef Singleton<MusicIPC> SMusicIPC;

#endif // MUSICIPC_H
