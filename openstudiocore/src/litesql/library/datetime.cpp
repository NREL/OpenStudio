/* LiteSQL - Date and time classes
 *
 * The list of contributors at http://litesql.sf.net/
 *
 * See LICENSE for copyright information. */
#include "litesql/datetime.hpp"
#include "litesql/split.hpp"
#include "compatibility.hpp"
#include <time.h>
#include <cstdio>

using namespace std;

namespace litesql {

TimeStruct::TimeStruct(time_t t) {
    if (!t) 
        t = time(NULL);
    localtime_r(&t, &mytm);
}
int TimeStruct::day() const {
    return mytm.tm_mday;
}
int TimeStruct::dayOfWeek() const {
    return mytm.tm_wday;
}
int TimeStruct::dayOfYear() const {
    return mytm.tm_yday;
}
int TimeStruct::month() const {
    return mytm.tm_mon + 1;
}
int TimeStruct::year() const {
    return mytm.tm_year + 1900;
}
int TimeStruct::hour() const {
    return mytm.tm_hour;
}
int TimeStruct::min() const {
    return mytm.tm_min;
}
int TimeStruct::sec() const {
    return mytm.tm_sec;
}
time_t TimeStruct::timeStamp() const {
    return mktime(const_cast<struct tm*>(&mytm));
}
TimeStruct&  TimeStruct::setDay(int day) {
    mytm.tm_mday = day;
    return *this;
}
TimeStruct& TimeStruct::setMonth(int month) {
    mytm.tm_mon = month - 1;
    return *this;
}
TimeStruct& TimeStruct::setYear(int year) {
    mytm.tm_year = year - 1900;
    return *this;
}
TimeStruct& TimeStruct::setHour(int hour) {
    mytm.tm_hour = hour;
    return *this;
}
TimeStruct& TimeStruct::setMin(int min) {
    mytm.tm_min = min;
    return *this;
}
TimeStruct& TimeStruct::setSec(int sec) {
    mytm.tm_sec = sec;
    return *this;
}
TimeStruct& TimeStruct::setTimeStamp(time_t t) {
    localtime_r(&t, &mytm);
    return *this;
}
                  
Date::Date(time_t t) : value(t) {
    if (value == 0)
        value = time(NULL);
}
Date::Date(int day, int month, int year) {
    value = TimeStruct().setDay(day).setMonth(month).setYear(year)
        .timeStamp();
}
int Date::day() const {
    return TimeStruct(value).day();
}
int Date::dayOfWeek() const {
    return TimeStruct(value).dayOfWeek();
}
int Date::month() const {
    return TimeStruct(value).month();
}
int Date::year() const {
    return TimeStruct(value).year();
}
time_t Date::timeStamp() const {
    return value;
}
TimeStruct Date::timeStruct() const {
    return TimeStruct(value);
}
Date& Date::setDay(int d) {
    value = TimeStruct(value).setDay(d).timeStamp();
    return *this;
}
Date& Date::setMonth(int m) {
    value = TimeStruct(value).setMonth(m).timeStamp();
    return *this;
}
Date& Date::setYear(int y) {
    value = TimeStruct(value).setYear(y).timeStamp();
    return *this;
}
Date& Date::setTimeStamp(time_t t) {
    value = t;
    return *this;
}
string Date::asString(string format) const {
    if (format == "%u") {
        char buf[32];
        snprintf(buf, 32, "%lu", value);
        return buf;
    }
    Split data(format, "%");
    TimeStruct ts(value);
    string res = data[0];
    for (size_t i = 1; i < data.size(); i++) {
        string rest = data[i].substr(1, data[i].size());
        switch(data[i][0]) {
        case 'd':
            res += toString(ts.day()) + rest;
            break;
        case 'm':
            res += toString(ts.month()) + rest;
            break;
        case 'y':
            res += toString(ts.year()) + rest;
        }
    }
    return res;
}
Time::Time(int secs) : value(secs) {
}
Time::Time(int hour, int min, int sec) {
    value = hour * 3600 + min * 60 + sec;
}
int Time::hour() const {
    return value / 3600;
}
int Time::min() const {
    return (value / 60) % 60;
}
int Time::sec() const {
    return value % 60;
}
int Time::secs() const {
    return value;
}
Time& Time::setHour(int h) {
    value = h * 3600 + min() * 60 + sec();
    return *this;
}
Time& Time::setMin(int m) {
    value = hour() * 3600 + m * 60 + sec();
    return *this;
}
Time& Time::setSec(int s) {
    value = hour() * 3600 + min() * 60 + s;
    return *this;
}
Time& Time::setSecs(int secs) {
    value = secs;
    return *this;
}
string Time::asString(string format) const {
    if (format == "%u") {
        char buf[32];
        snprintf(buf, 32, "%d", value);
        return buf;
    }
    Split data(format, "%");
    string res = data[0];
    for (size_t i = 1; i < data.size(); i++) {
        string rest = data[i].substr(1, data[i].size());
        switch(data[i][0]) {
        case 'h':
            res += toString(hour()) + rest;
            break;
        case 'M':
            if (min() < 10)
                res += "0";
            res += toString(min()) + rest;
            break;
        case 's':
            if (sec() < 10)
                res += "0";
            res += toString(sec()) + rest;
        }
    }
    return res;
}
DateTime::DateTime(time_t t) {
    value =  (t==0) ? time(NULL) : t;
}
int DateTime::hour() const {
    return TimeStruct(value).hour();
}
int DateTime::min() const {
    return TimeStruct(value).min();
}
int DateTime::sec() const {
    return TimeStruct(value).sec();
}
time_t DateTime::timeStamp() const {
    return TimeStruct(value).timeStamp();
}
TimeStruct DateTime::timeStruct() const {
    return TimeStruct(value);
}
DateTime& DateTime::setDay(int d) {
    value = TimeStruct(value).setDay(d).timeStamp();
    return *this;
}
DateTime& DateTime::setMonth(int m) {
    value = TimeStruct(value).setMonth(m).timeStamp();
    return *this;
}
DateTime& DateTime::setYear(int y) {
    value = TimeStruct(value).setYear(y).timeStamp();
    return *this;
}
DateTime& DateTime::setHour(int h) {
    value = TimeStruct(value).setHour(h).timeStamp();
    return *this;
}
DateTime& DateTime::setMin(int m) {
    value = TimeStruct(value).setMin(m).timeStamp();
    return *this;
}
DateTime& DateTime::setSec(int s) {
    value = TimeStruct(value).setSec(s).timeStamp();
    return *this;
}
string DateTime::asString(string format) const {
    if (format == "%u") {
        char buf[32];
        snprintf(buf, 32, "%lu", value);
        return buf;
    }
    Split data(format, "%");
    TimeStruct ts(value);
    string res = data[0];
    for (size_t i = 1; i < data.size(); i++) {
        string rest = data[i].substr(1, data[i].size());
        switch(data[i][0]) {
        case 'd':
            res += toString(ts.day()) + rest;
            break;
        case 'm':
            res += toString(ts.month()) + rest;
            break;
        case 'y':
            res += toString(ts.year()) + rest;
            break;
        case 'h':
            res += toString(ts.hour()) + rest;
            break;
        case 'M':
            if (ts.min() < 10)
                res += "0";
            res += toString(ts.min()) + rest;
            break;
        case 's':
            if (ts.sec() < 10)
                res += "0";
            res += toString(ts.sec()) + rest;
            break;
        }
    }
    return res;
}
template <>
Date convert<const string&, Date>(const string& value) {
    return Date(atoi(value));
}
template <>
Time convert<const string&, Time>(const string& value) {
    return Time(atoi(value));
}
template <>
DateTime convert<const string&, DateTime>(const string& value) {
    return DateTime(atoi(value));
}
template <>
Date convert<int, Date>(int value) {
    return Date(value);
}
template <>
Time convert<int, Time>(int value) {
    return Time(value);
}
template <>
DateTime convert<int, DateTime>(int value) {
    return DateTime(value);
}

template <>
std::string convert<const Date&, std::string>(const Date& value) {
    return toString(value.timeStamp());
}
template <>
std::string convert<const Time&, std::string>(const Time& value) {
    return toString(value.secs());
}
template <>
std::string convert<const DateTime&, std::string>(const DateTime& value) {
    return toString(value.timeStamp());
}

ostream& operator << (ostream& os, const Date& d) {
    return os << d.asString();
}
ostream& operator << (ostream& os, const Time& d) {
    return os << d.asString();
}
ostream& operator << (ostream& os, const DateTime& d) {
    return os << d.asString();
}
}
