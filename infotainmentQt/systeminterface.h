#ifndef SYSTEMINTERFACE_H
#define SYSTEMINTERFACE_H

/*
 * Source: https://www.jeremymorgan.com/tutorials/c-programming/how-to-capture-the-output-of-a-linux-command-in-c/
*/

#include <iostream>
using namespace std;
#include <algorithm>

string GetStdoutFromCommand(string cmd);

#endif // SYSTEMINTERFACE_H
