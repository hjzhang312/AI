
#include "musicipc.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>


MusicIPC::MusicIPC()
{

}

void MusicIPC::initialize()
{

    pid_t pid;
    unlink("./cmdfifo");
    mkfifo("./cmdfifo",O_CREAT|0744);
    if((pid =fork()) == -1)
    {
        return;
    }
    else if(0  == pid )
    {
        execlp("mplayer", "mplayer", "-slave", "-quiet","-idle", "-input","file=./cmdfifo",NULL);
    }
    else
    {
        sleep(1);
        if((fd = open("./cmdfifo",O_WRONLY)) == -1)
            cout << "open fifo error" << endl;
    }
}

MusicIPC::~MusicIPC()
{

}

void MusicIPC::WriteFifo(string cmd)
{
    const char *cmd_s = cmd.c_str();

    if((write(fd,cmd_s,cmd.size())) != cmd.size())
    {
        cout << "write error" << endl;
    }
}
