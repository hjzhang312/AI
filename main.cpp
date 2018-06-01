#include <iostream>

using namespace std;


#include "capdevices.h"
#include "webrtchand.h"
int main()
{
    SCapDevices::instance()->init();
    SWebrtcHand::instance()->init();
    SCapDevices::instance()->readData();
    cout << "Hello World!" << endl;
    return 0;
}

