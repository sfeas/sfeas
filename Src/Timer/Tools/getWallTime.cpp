//Author		: Xiao Tuzi
//Date(Create)	: 01/21/2023
//Description	: this program is to get wall time


#include "Timer/Tools/getWallTime.h"
#include <Windows.h>

double getWallTime() {
    LARGE_INTEGER time, freq;
    if (!QueryPerformanceFrequency(&freq)) {
        //  Handle error
        return 0;
    }
    if (!QueryPerformanceCounter(&time)) {
        //  Handle error
        return 0;
    }
    return (double)time.QuadPart / freq.QuadPart;
}
