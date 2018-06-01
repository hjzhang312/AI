TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    src/thread.cpp \
    src/capdevices.cpp \
    src/webrtchand.cpp

HEADERS += \
    src/singleton.h \
    src/thread.h \
    src/capdevices.h \
    inc/capdevices.h \
    inc/singleton.h \
    inc/thread.h \
    inc/webrtc/common.h \
    inc/webrtc/common_types.h \
    inc/webrtc/typedefs.h \
    inc/webrtc/base/arraysize.h \
    inc/webrtc/base/basictypes.h \
    inc/webrtc/base/checks.h \
    inc/webrtc/base/constructormagic.h \
    inc/webrtc/base/maybe.h \
    inc/webrtc/system_wrappers/include/trace.h \
    inc/webrtc/modules/audio_processing/beamformer/array_util.h \
    inc/webrtc/modules/audio_processing/include/audio_processing.h \
    inc/webrtc/modules/interface/module_common_types.h \
    src/webrtchand.h \
    inc/webrtchand.h \
    inc/webrtc/base/platform_file.h


INCLUDEPATH += inc inc/webrtc/modules/audio_processing/include/
DEPENDPATH += inc src lib

LIBS += -lasound -lpthread -L/home/zhj/extend/AI/QtAI/Ai/lib -lwebrtc_audio_processing

