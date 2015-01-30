// Dummy Project

#include <conio.h>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <iomanip>
#include <vector>
#include <fstream>
#include <direct.h>
#include <string>
#include <math.h>
#include <WinBase.h>
#include <stdio.h>
#include <thread>

using namespace std;

typedef void _Thrd_imp_t;

int main()
{
    _Thrd_abort();
    _Thrd_create();
    _Thrd_current();
    _Thrd_detach();
    _Thrd_equal();
    _Thrd_exit();
    _Thrd_join();
    _Thrd_lt();
    _Thrd_sleep();
    _Thrd_start();
    _Thrd_yield();

    printf("Hello World!\n");

    return 0;
}
