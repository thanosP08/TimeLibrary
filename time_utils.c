#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Converts a time into total seconds.
int get_sec(int sec, int min, int hours) {
    return 3600 * hours + 60 * min + sec;
}

// Formats a number of seconds as HH:MM:SS.
char *get_fullTime(int sec) {
    char *time = malloc(32 * sizeof(char));
    if (time == NULL) return NULL;
    snprintf(time, 32, "%02d:%02d:%02d", sec / 3600, (sec % 3600) / 60, sec % 60);
    return time;
}

// Checks whether a time is valid in the 24-hour format.
int isValidTime(int hours, int minutes, int seconds) {
    return (hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60 && seconds >= 0 && seconds < 60);
}

// Adds seconds to a time and wraps around midnight.
char *addSeconds(int hours, int minutes, int seconds, int extraSeconds) {
    if (!isValidTime(hours, minutes, seconds)) return NULL;
    int total_seconds = get_sec(seconds, minutes, hours);
    total_seconds = (total_seconds + extraSeconds % 86400) % 86400;
    if (total_seconds < 0) total_seconds += 86400;
    return get_fullTime(total_seconds);
}

// Returns the elapsed seconds from the first time to the second.
int secondsBetween(int h1, int m1, int s1, int h2, int m2, int s2) {
    if (!isValidTime(h1, m1, s1) || !isValidTime(h2, m2, s2)) return -1;
    int time1 = get_sec(s1, m1, h1);
    int time2 = get_sec(s2, m2, h2);
    if (time1 > time2) return -1;
    return time2 - time1;
}

// Compares two times: 1 if first is later, -1 if earlier, 0 if equal, -2 if invalid.
int compareTimes(int h1, int m1, int s1, int h2, int m2, int s2) {
    if (!(isValidTime(h1, m1, s1) && isValidTime(h2, m2, s2))) return -2;
    int time1 = get_sec(s1, m1, h1);
    int time2 = get_sec(s2, m2, h2);
    if (time1 > time2) return 1;
    if (time1 < time2) return -1;
    return 0;
}

// Converts a 24-hour time to a 12-hour AM/PM string.
char *to12HourFormat(int hours, int min, int sec) {
    if (!isValidTime(hours, min, sec)) return NULL;
    char *time = malloc(32 * sizeof(char));
    if (time == NULL) return NULL;
    const char *period = (hours >= 12) ? "PM" : "AM";
    int h = hours % 12;
    if (h == 0) h = 12;
    snprintf(time, 32, "%02d:%02d:%02d %s", h, min, sec, period);
    return time;
}

// Checks whether a year is a leap year in the Gregorian calendar.
int isLeapYear(int year) {
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

// Returns the number of days in a given month, or -1 for invalid inputs.
int daysInMonth(int year, int month) {
    if (year <= 0 || month < 1 || month > 12) return -1;
    switch (month) {
        case 2:
            return isLeapYear(year) ? 29 : 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 31;
    }
}

// Checks whether a year-month-day combination is a valid date.
int isValidDate(int year, int month, int day) {
    return (year > 0 && month >= 1 && month <= 12 && day >= 1 && day <= daysInMonth(year, month));
}

// Returns the date immediately after the given date.
char *nextDate(int year, int month, int day) {
    if (!isValidDate(year, month, day)) return NULL;
    int month_days = daysInMonth(year, month);
    if (year == INT_MAX && month == 12 && day == month_days) return NULL;
    char *date = malloc(32 * sizeof(char));
    if (date == NULL) return NULL;
    if (month_days == day) {
        if (month == 12) {
            snprintf(date, 32, "%04d-%02d-%02d", year + 1, 1, 1);
        } else {
            snprintf(date, 32, "%04d-%02d-%02d", year, month + 1, 1);
        }
    } else {
        snprintf(date, 32, "%04d-%02d-%02d", year, month, day + 1);
    }
    return date;
}

// Adds calendar years, months, and days; -1 means to skip that component.
char *addDate(int year, int month, int day, int extraYears, int extraMonths, int extraDays) {
    if (!isValidDate(year, month, day) || extraYears < -1 || extraMonths < -1 || extraDays < -1) return NULL;
    if (extraYears == -1) extraYears = 0;
    if (extraMonths == -1) extraMonths = 0;
    if (extraDays == -1) extraDays = 0;
    long long newYear = (long long)year + extraYears;
    if (newYear > INT_MAX) return NULL;
    year = (int)newYear;
    if (day > daysInMonth(year, month)) day = daysInMonth(year, month);
    long long totalMonths = (long long)(month - 1) + extraMonths;
    newYear = (long long)year + totalMonths / 12;
    if (newYear > INT_MAX) return NULL;
    year = (int)newYear;
    month = (int)(totalMonths % 12) + 1;
    if (day > daysInMonth(year, month)) day = daysInMonth(year, month);
    while (extraDays > 0) {
        int remaining = daysInMonth(year, month) - day;
        if (extraDays <= remaining) {
            day += extraDays;
            break;
        }
        extraDays -= remaining + 1;
        day = 1;
        if (month == 12) {
            if (year == INT_MAX) return NULL;
            month = 1;
            year++;
        } else {
            month++;
        }
    }
    char *date = malloc(32);
    if (date == NULL) return NULL;
    snprintf(date, 32, "%04d-%02d-%02d", year, month, day);
    return date;
}

// Converts a valid date into a day count starting at 1 on 0001-01-01.
long long dateToDays(int year, int month, int day) {
    long long y = year - 1LL;
    long long total = 365 * y + y / 4 - y / 100 + y / 400;
    for (int m = 1; m < month; m++) total += daysInMonth(year, m);
    total += day;
    return total;
}

// Returns the number of days from the first date to the second.
int daysBetween(int y1, int m1, int d1, int y2, int m2, int d2) {
    if (!isValidDate(y1, m1, d1) || !isValidDate(y2, m2, d2)) return -1;
    long long first = dateToDays(y1, m1, d1);
    long long second = dateToDays(y2, m2, d2);
    long long diff = second - first;
    if (diff < 0 || diff > INT_MAX) return -1;
    return (int)diff;
}

// Returns the weekday: Monday = 1 through Sunday = 7.
int dayOfWeek(int year, int month, int day) {
    if (!isValidDate(year, month, day)) return -1;
    long long totalDays = dateToDays(year, month, day);
    return (int)((totalDays - 1) % 7) + 1;
}

// Returns the ordinal day within a year (1 through 365 or 366).
int dayOfYear(int year, int month, int day) {
    if (!isValidDate(year, month, day)) return -1;
    int days = 0, cnt = 1;
    while (cnt < month) {
        days += daysInMonth(year, cnt);
        cnt++;
    }
    return days + day;
}

// Converts an ordinal day of a year to a YYYY-MM-DD string.
char *dateFromDayOfYear(int year, int dayOfYear) {
    if (year <= 0 || dayOfYear < 1 || dayOfYear > (isLeapYear(year) ? 366 : 365)) return NULL;
    int month = 1;
    while (dayOfYear > daysInMonth(year, month)) {
        dayOfYear -= daysInMonth(year, month);
        month++;
    }
    char *date = malloc(32);
    if (date == NULL) return NULL;
    snprintf(date, 32, "%04d-%02d-%02d", year, month, dayOfYear);
    return date;
}

// Finds the next occurrence of a weekday strictly after the given date.
char *nextWeekday(int year, int month, int day, int targetWeekday) {
    if (!isValidDate(year, month, day) || targetWeekday < 1 || targetWeekday > 7) return NULL;
    int currentWeekday = dayOfWeek(year, month, day);
    int extraDays = (targetWeekday - currentWeekday + 7) % 7;
    if (extraDays == 0) extraDays = 7;
    return addDate(year, month, day, -1, -1, extraDays);
}

// Finds the nth occurrence of a weekday in a given month.
char *nthWeekdayOfMonth(int year, int month, int targetWeekday, int n) {
    if (year <= 0 || month < 1 || month > 12 || targetWeekday < 1 || targetWeekday > 7 || n < 1 || n > 5) return NULL;
    int firstWeekday = dayOfWeek(year, month, 1);
    int offset = (targetWeekday - firstWeekday + 7) % 7;
    int resultDay = 1 + offset + (n - 1) * 7;
    if (resultDay > daysInMonth(year, month)) return NULL;
    char *date = malloc(32);
    if (date == NULL) return NULL;
    snprintf(date, 32, "%04d-%02d-%02d", year, month, resultDay);
    return date;
}

// Counts weekdays from 0001-01-01 through a given day count.
long long countWorkingDays(long long totalDays) {
    long long weeks = totalDays / 7;
    long long remaining = totalDays % 7;
    if (remaining > 5) remaining = 5;
    return weeks * 5 + remaining;
}

// Counts Monday-Friday dates after the start date and through the end date.
int workingDaysBetween(int y1, int m1, int d1, int y2, int m2, int d2) {
    if (!isValidDate(y1, m1, d1) || !isValidDate(y2, m2, d2)) return -1;
    long long start = dateToDays(y1, m1, d1);
    long long end = dateToDays(y2, m2, d2);
    if (start > end) return -1;
    long long result = countWorkingDays(end) - countWorkingDays(start);
    if (result > INT_MAX) return -1;
    return (int)result;
}

// Returns whether an ISO week-numbering year contains 52 or 53 weeks.
int isoWeeksInYear(int year) {
    int firstDay = dayOfWeek(year, 1, 1);
    if (firstDay == 4 || (firstDay == 3 && isLeapYear(year))) return 53;
    return 52;
}

// Returns the ISO 8601 week number (1-53) for a valid date.
int isoWeekNumber(int year, int month, int day) {
    if (!isValidDate(year, month, day)) return -1;
    int ordinal = dayOfYear(year, month, day);
    int weekday = dayOfWeek(year, month, day);
    int week = (ordinal - weekday + 10) / 7;
    if (week < 1) return isoWeeksInYear(year - 1);
    if (week > isoWeeksInYear(year)) return 1;
    return week;
}

// Calculates completed years between a birth date and a reference date.
int calculateAge(int birthYear, int birthMonth, int birthDay, int currentYear, int currentMonth, int currentDay) {
    if (!isValidDate(birthYear, birthMonth, birthDay) || !isValidDate(currentYear, currentMonth, currentDay)) return -1;
    if (dateToDays(birthYear, birthMonth, birthDay) > dateToDays(currentYear, currentMonth, currentDay)) return -1;
    int age = currentYear - birthYear;
    if (currentMonth < birthMonth || (currentMonth == birthMonth && currentDay < birthDay)) age--;
    return age;
}

int main(void) {
}
