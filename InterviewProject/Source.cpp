//#include <chrono>
//using namespace std::chrono;
#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

typedef long long int EpochTime;
typedef vector<EpochTime> Sessions;
typedef pair<EpochTime, EpochTime> SessionInfo;

const EpochTime EpochMinute = 60 * 1000;
const EpochTime EpochHour = 60 * EpochMinute;
const EpochTime EpochDay = 24 * EpochHour;
const EpochTime EpochWeek = 7 * EpochDay;


void printDate(EpochTime launchTime)
{
	time_t launchTimeS = launchTime / 1000;
	struct tm ptm;
	//localtime_s(&ptm, &launchTimeS);
	gmtime_s(&ptm, &launchTimeS);
	char date[60] = { 0 };
	sprintf_s(date, "%d-%02d-%02d      %02d:%02d:%02d",
		(int)ptm.tm_year + 1900, (int)ptm.tm_mon + 1, (int)ptm.tm_mday,
		(int)ptm.tm_hour, (int)ptm.tm_min, (int)ptm.tm_sec);
	cout << date << "," << launchTime<<endl;
}
void printSessions(Sessions sessions)
{
	for (auto &it : sessions)
	{
		printDate(it);
	}
}

void printSessionInfo(vector<SessionInfo> &res)
{
	for (auto &it : res)
	{
		cout << "Start Time:" << endl;
		printDate(it.first);
		cout << "End Time:" << endl;
		printDate(it.second);
	}
}




EpochTime getSessionBeginTime(EpochTime launchTime)
{
	time_t launchTimeS = launchTime / 1000;
	struct tm ptm;
	gmtime_s(&ptm, &launchTimeS);
	int weekDay = ptm.tm_wday;
	int shiftDays = 0;
	if (weekDay == 0) shiftDays = 1;
	else if (weekDay > 1) shiftDays = 8 - weekDay;
	EpochTime res = launchTime + shiftDays * EpochDay;
	return res;
}

bool isLate(EpochTime startTime, EpochTime enrollTime)
{
	time_t startTimeS = startTime / 1000, enrollTimeS = enrollTime / 1000;
	struct tm tmStart, tmEnroll;
	gmtime_s(&tmStart, &startTimeS);
	gmtime_s(&tmEnroll, &enrollTimeS);
	if ((tmStart.tm_year < tmEnroll.tm_year) ||
		(tmStart.tm_year == tmEnroll.tm_year) && (tmStart.tm_mon < tmEnroll.tm_mon) ||
		(tmStart.tm_year == tmEnroll.tm_year) && (tmStart.tm_mon == tmEnroll.tm_mon) && (tmStart.tm_mday < tmEnroll.tm_mday))
		return true;
	else return false;
}


void getThreeSessionsFromDate(EpochTime time, int repeatFreq, Sessions &sessions)
{
	EpochTime startTime = getSessionBeginTime(time);
	printDate(startTime);
	for (int i = 0; i < 3; ++i)
	{
		sessions.push_back(startTime);
		startTime += repeatFreq * EpochWeek;
	}
}

//the earlest startTime is the first Monday following launchTime (startTime >= launchTime)
//Sessions are repeated with given frequency.
//user enrollTime should be earlier than the startTime;
Sessions getThreeSelectiveSessions(EpochTime launchTime, int freq, EpochTime enrollTime)
{
	Sessions sessions;
	EpochTime startTime = getSessionBeginTime(launchTime);
	while (isLate(startTime, enrollTime))
	{
		startTime += freq * EpochWeek;
	}
	getThreeSessionsFromDate(startTime, freq, sessions);
	return sessions;
}


struct Course
{
	EpochTime launch_date;
	int repeatFreq;
	int duration;
};

//taken course in order. no overlapping time interval.
void getSpecializationSchedule(vector<Course> &courses, vector<SessionInfo> &res, EpochTime enrollTime)
{
	if (courses.empty()) return;
	EpochTime sessionStartTime, sessionEndTime, userStartTime;
	for (int i = 0; i < courses.size(); ++i)
	{
		sessionStartTime = getSessionBeginTime(courses[i].launch_date);
		if (i == 0) userStartTime = enrollTime;
		else userStartTime = res[i - 1].second;
		while (isLate(sessionStartTime, userStartTime))
		{
			sessionStartTime += courses[i].repeatFreq * EpochWeek;
		}
		sessionEndTime = sessionStartTime + courses[i].duration * EpochWeek;
		res.push_back(make_pair(sessionStartTime, sessionEndTime));
	}
}


int main()
{

	//printDate(getSessionBeginTime(1470096000000));
	//printDate(getSessionBeginTime(1470268800000));
	//printDate(1470096000000);
	//printDate(1470268800000);

	//printDate(getSessionBeginTime(1451419052000));
	
	printSessions(getThreeSelectiveSessions(1470096000000, 2, 1470787200000));
	//printSessions(getThreeSelectiveSessions(1470096000000, 1, 1470096000000));
	printSessions(getThreeSelectiveSessions(1470700800000, 1,1470096000000));


	//vector<Course> courseList = { {1469923200000, 2, 4},{ 1469923200000, 2, 4 } };
	//vector<SessionInfo> res;
	//getSpecializationSchedule(courseList, res, 1470268800000);

	//printSessionInfo(res);

	//auto tt1 = std::chrono::duration_cast<std::chrono::milliseconds>
	//	(std::chrono::system_clock::now().time_since_epoch()).count();
	//auto tt2 = std::chrono::system_clock::to_time_t
	//(std::chrono::system_clock::now());
	//long long int time = 1470096000000;
	//auto tt = time;
	//tt /= 1000;
	//int delta = 24 * 60 * 60 * 14;
	//tt += delta;
	////cout << tt1 << endl;
	//struct tm ptm;
	//localtime_s(&ptm,&tt);
	//char date[60] = { 0 };
	//sprintf_s(date, "%d-%02d-%02d      %02d:%02d:%02d",
	//	(int)ptm.tm_year + 1900, (int)ptm.tm_mon + 1, (int)ptm.tm_mday,
	//	(int)ptm.tm_hour, (int)ptm.tm_min, (int)ptm.tm_sec);
	//cout << date << endl;
	//high_resolution_clock::time_point p = high_resolution_clock::now();

	//milliseconds ms = duration_cast<milliseconds>(p.time_since_epoch());

	//seconds s = duration_cast<seconds>(ms);
	//std::time_t t = s.count();
	//cout << t << "  t"<<endl;
	//std::size_t fractional_seconds = ms.count() % 1000;

	//char str[26];
	//std::cout << ctime_s(str, sizeof str, &t) << std::endl;
	//std::cout << fractional_seconds << std::endl;
	//cout << str << endl;

	return 0;
}