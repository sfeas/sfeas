//Author		: Xiao Tuzi
//Date(Create)	: 01/21/2023
//Description	: this program is to get cpu time


#include "Timer/Tools/getCPUTime.h"
#include <Windows.h>

double getCPUTime() {
    FILETIME a, b, c, d;
    if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
        //  Returns total user time.
        //  Can be tweaked to include kernel times as well.
        return
            (double)(d.dwLowDateTime | ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
    }
    else {
        //  Handle error
        return 0;
    }
}
