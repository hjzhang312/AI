TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    src/thread.cpp \
    src/capdevices.cpp \
    src/playdevices.cpp \
    src/carddevices.cpp \
    src/speex/speexhandle.cpp \
    src/sphinx/sphinxhandle.cpp \
    src/baidu/asrhandle.cpp \
    src/http/httplink.cpp \
    src/baidu/speakeranaly.cpp \
    src/baidu/musicdown.cpp \
    src/musicipc.cpp \
    src/baidu/ttshandle.cpp



HEADERS += \
    inc/capdevices.h \
    inc/singleton.h \
    inc/thread.h \
    inc/playdevices.h \
    inc/carddevices.h \
    inc/type.h \
    inc/speex/speexhandle.h \
    inc/sphinx/sphinxhandle.h \
    inc/baidu/asrhandle.h \
    inc/baidu/base/utils.h \
    inc/baidu/base/http.h \
    inc/baidu/base/base64.h \
    inc/baidu/base/base.h \
    inc/baidu/speech.h \
    inc/http/httplink.h \
    inc/baidu/speakeranaly.h \
    inc/baidu/musicdown.h \
    inc/musicipc.h \
    inc/baidu/ttshandle.h




INCLUDEPATH += inc inc/speex inc/sphinx inc/baidu inc/baidu/base inc/http /usr/local/include/ /usr/include/jsoncpp/ /usr/local/include/pocketsphinx/ /usr/local/include/sphinxbase/
DEPENDPATH += inc src src/speex src/sphinx src/baidu src/http

LIBS += -lasound -lpthread -lspeexdsp -lpocketsphinx -lsphinxbase -lsphinxad -lcurl -lcrypto -ljsoncpp
#-L/home/zhj/work/AI/QtAI/Ai/lib -lwebrtc_audio_processing

