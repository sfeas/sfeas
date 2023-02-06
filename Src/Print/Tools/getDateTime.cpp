//Author		: Xiao Tuzi
//Date(Create)	: 09/28/2022
//Description	: this program is getting data and time

#include "Print/Tools/getDataTime.h"

void getDataTime(int& year, int& month, int& day, int& hour, int& minute, int& second) {
	struct tm buf;
	time_t ts = time(NULL);
	year = 1900;
	month = 1;
	day = 0;
	hour = 0;
	minute = 0;
	second = 0;

	localtime_s(&buf, &ts);
	year += buf.tm_year;
	month += buf.tm_mon;
	day += buf.tm_mday;
	hour += buf.tm_hour;
	minute += buf.tm_min;
	second += buf.tm_sec;

	return;
}
