#ifndef SPHINXHANDLE_H
#define SPHINXHANDLE_H

#include "singleton.h"

#include <vector>
#include <iostream>

#include "pocketsphinx/pocketsphinx.h"

class SphinxHandle
{
public:
    SphinxHandle();
    int initialize(string hmmName,string lmName, string dictName);

    bool getVoiceKeyword(vector<int16_t*> &voiceList);


    ~SphinxHandle();

public:

private:
    ps_decoder_t *ps;
    DECLARE_SINGLETON_CLASS(SphinxHandle)
};

 typedef Singleton<SphinxHandle> SSphinxHandle;
#endif // SPHINXHANDLE_H
