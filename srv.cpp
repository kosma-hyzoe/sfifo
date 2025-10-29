#include "common.h"

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

int main()
{
    pid_t pid;
    string path;

    path = "/tmp/sfifo/srv";
    cout << path << endl;
    // mkfifo();
    return 0;
}

