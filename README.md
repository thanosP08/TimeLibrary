# TimeLibrary (C)

A small collection of **24 time and Gregorian-calendar utility functions**, implemented in standard C without third-party date/time libraries. The project covers time formatting, date arithmetic, weekdays, working-day counts, ISO week numbers, and age calculation.

## Contents

| Category | Functions | Purpose |
| --- | --- | --- |
| Time conversion | `get_sec`, `get_fullTime` | Convert `hours:minutes:seconds` to seconds or format seconds as `HH:MM:SS`. |
| Time validation and arithmetic | `isValidTime`, `addSeconds`, `secondsBetween`, `compareTimes`, `to12HourFormat` | Validate 24-hour times, add seconds across midnight, measure or compare times, and format AM/PM time. |
| Calendar fundamentals | `isLeapYear`, `daysInMonth`, `isValidDate` | Check leap years, count days in a month, and validate dates. |
| Date arithmetic | `nextDate`, `addDate`, `dateToDays`, `daysBetween` | Find the next date, add years/months/days, encode dates as day counts, or find elapsed days. |
| Calendar position | `dayOfWeek`, `dayOfYear`, `dateFromDayOfYear` | Find weekdays and day-of-year numbers, or convert a day-of-year number back to a date. |
| Weekday scheduling | `nextWeekday`, `nthWeekdayOfMonth` | Find the next occurrence of a weekday or its nth occurrence in a month. |
| Working days | `countWorkingDays`, `workingDaysBetween` | Count Monday–Friday dates, excluding weekends but not public holidays. |
| ISO weeks | `isoWeeksInYear`, `isoWeekNumber` | Find the number of ISO weeks in a year and the ISO week number of a date. |
| Age | `calculateAge` | Calculate age in completed years at a given reference date. |

## Conventions

- Dates use the **proleptic Gregorian calendar**, with years starting at `1`; leap years follow the 4/100/400 rule.
- `dateToDays(1, 1, 1)` is `1`, and that reference day is treated as Monday. `dayOfWeek` returns `1` for Monday through `7` for Sunday.
- `addDate` applies **years first, then months, then days**. If a year/month addition produces a nonexistent day (such as February 30), it clamps the day to the last day of that month. For any of the three added components, `-1` means **skip this component**, and values less than `-1` are invalid.
- `nextWeekday` always finds a **later** occurrence; when the starting day already matches, it advances seven days.
- `daysBetween` and `workingDaysBetween` exclude the start date and include the end date. `workingDaysBetween` counts Monday–Friday only and does not account for holidays.
- `isoWeekNumber` returns the **week number only**, not the ISO week-numbering year (which can differ near New Year).
- For February 29 birthdays, `calculateAge` treats **March 1** as the birthday in non-leap years.
- `dateToDays` and `countWorkingDays` are numeric helpers and assume valid, nonnegative inputs consistent with the library's conventions.

## Return values and memory management

- Functions returning `char *` allocate their result with `malloc()`. **The caller owns the returned string and must call `free()`** after use.
- String-returning functions return `NULL` for invalid inputs or allocation failures when applicable.
- Most integer-valued date functions return `-1` for invalid dates or a reversed interval. `compareTimes` returns `-2` for invalid times, `-1` for an earlier first time, `0` for equality, and `1` for a later first time.
- `get_sec` and `get_fullTime` are low-level conversion/formatting helpers; callers should provide meaningful numeric inputs.

## Example

```c
char *result = nextWeekday(2026, 11, 29, 3);
if (result != NULL) {
    printf("%s\n", result); // 2026-12-02
    free(result);
}
```

## Build

The current `time_utils.c` contains an intentionally empty `main()`:

```sh
gcc -std=c11 -Wall -Wextra -pedantic time_utils.c -o time_utils
```

Running the executable currently produces no output. You can add test calls to `main()` when needed. In a later step, the utility functions can be separated into a source file and a public `.h` header for reuse in other programs.
