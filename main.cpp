#include <iostream>

using namespace std;


#include "capdevices.h"
int main()
{
    SCapDevices::instance()->init();
    SCapDevices::instance()->readData();
    cout << "Hello World!" << endl;
    return 0;
}

