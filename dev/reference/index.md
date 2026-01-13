# Package index

## High Level API

This section contains the high level API for R’s native date and
date-time types (Date, POSIXct, and POSIXlt). For most clock users,
these are the only functions you’ll need.

### Construction

- [`date_build()`](https://clock.r-lib.org/dev/reference/date_build.md)
  : Building: date
- [`date_time_build()`](https://clock.r-lib.org/dev/reference/date_time_build.md)
  : Building: date-time
- [`date_today()`](https://clock.r-lib.org/dev/reference/date-today.md)
  [`date_now()`](https://clock.r-lib.org/dev/reference/date-today.md) :
  Current date and date-time

### Manipulation

- [`date_group()`](https://clock.r-lib.org/dev/reference/date_group.md)
  : Group date and date-time components
- [`date_count_between()`](https://clock.r-lib.org/dev/reference/date_count_between.md)
  : Counting: date and date-time
- [`date_start()`](https://clock.r-lib.org/dev/reference/date-and-date-time-boundary.md)
  [`date_end()`](https://clock.r-lib.org/dev/reference/date-and-date-time-boundary.md)
  : Boundaries: date and date-time
- [`date_floor()`](https://clock.r-lib.org/dev/reference/date-and-date-time-rounding.md)
  [`date_ceiling()`](https://clock.r-lib.org/dev/reference/date-and-date-time-rounding.md)
  [`date_round()`](https://clock.r-lib.org/dev/reference/date-and-date-time-rounding.md)
  : Date and date-time rounding
- [`date_shift()`](https://clock.r-lib.org/dev/reference/date-and-date-time-shifting.md)
  : Shifting: date and date-time
- [`date_seq()`](https://clock.r-lib.org/dev/reference/date_seq.md) :
  Sequences: date and date-time
- [`date_spanning_seq()`](https://clock.r-lib.org/dev/reference/date_spanning_seq.md)
  : Spanning sequence: date and date-time
- [`date_weekday_factor()`](https://clock.r-lib.org/dev/reference/date_weekday_factor.md)
  : Convert a date or date-time to a weekday factor
- [`date_month_factor()`](https://clock.r-lib.org/dev/reference/date_month_factor.md)
  : Convert a date or date-time to an ordered factor of month names
- [`date_leap_year()`](https://clock.r-lib.org/dev/reference/date_leap_year.md)
  : Is the year a leap year?
- [`date_time_zone()`](https://clock.r-lib.org/dev/reference/date-time-zone.md)
  [`date_time_set_zone()`](https://clock.r-lib.org/dev/reference/date-time-zone.md)
  : Get or set the time zone
- [`date_time_info()`](https://clock.r-lib.org/dev/reference/date_time_info.md)
  : Info: date-time

### Getters and setters

- [`get_year(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/Date-getters.md)
  [`get_month(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/Date-getters.md)
  [`get_day(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/Date-getters.md)
  : Getters: date
- [`get_year(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-getters.md)
  [`get_month(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-getters.md)
  [`get_day(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-getters.md)
  [`get_hour(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-getters.md)
  [`get_minute(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-getters.md)
  [`get_second(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-getters.md)
  : Getters: date-time
- [`set_year(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/Date-setters.md)
  [`set_month(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/Date-setters.md)
  [`set_day(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/Date-setters.md)
  : Setters: date
- [`set_year(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-setters.md)
  [`set_month(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-setters.md)
  [`set_day(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-setters.md)
  [`set_hour(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-setters.md)
  [`set_minute(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-setters.md)
  [`set_second(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-setters.md)
  : Setters: date-time

### Parsing and formatting

- [`date_format()`](https://clock.r-lib.org/dev/reference/date_format.md)
  : Formatting: date and date-time
- [`date_parse()`](https://clock.r-lib.org/dev/reference/date_parse.md)
  : Parsing: date
- [`date_time_parse()`](https://clock.r-lib.org/dev/reference/date-time-parse.md)
  [`date_time_parse_complete()`](https://clock.r-lib.org/dev/reference/date-time-parse.md)
  [`date_time_parse_abbrev()`](https://clock.r-lib.org/dev/reference/date-time-parse.md)
  [`date_time_parse_RFC_3339()`](https://clock.r-lib.org/dev/reference/date-time-parse.md)
  : Parsing: date-time

### Arithmetic

- [`add_years(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/Date-arithmetic.md)
  [`add_quarters(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/Date-arithmetic.md)
  [`add_months(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/Date-arithmetic.md)
  [`add_weeks(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/Date-arithmetic.md)
  [`add_days(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/Date-arithmetic.md)
  : Arithmetic: date
- [`add_years(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-arithmetic.md)
  [`add_quarters(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-arithmetic.md)
  [`add_months(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-arithmetic.md)
  [`add_weeks(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-arithmetic.md)
  [`add_days(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-arithmetic.md)
  [`add_hours(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-arithmetic.md)
  [`add_minutes(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-arithmetic.md)
  [`add_seconds(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-arithmetic.md)
  : Arithmetic: date-time

### Conversion

- [`as_date()`](https://clock.r-lib.org/dev/reference/as_date.md) :
  Convert to a date
- [`as_date_time()`](https://clock.r-lib.org/dev/reference/as_date_time.md)
  : Convert to a date-time

## Calendars

This section contains the overarching `calendar_*()` API that applies to
all calendars. See the subsections for details about a specific
calendar.

- [`calendar_group()`](https://clock.r-lib.org/dev/reference/calendar_group.md)
  : Group calendar components
- [`calendar_count_between()`](https://clock.r-lib.org/dev/reference/calendar-count-between.md)
  : Counting: calendars
- [`calendar_start()`](https://clock.r-lib.org/dev/reference/calendar-boundary.md)
  [`calendar_end()`](https://clock.r-lib.org/dev/reference/calendar-boundary.md)
  : Boundaries: calendars
- [`calendar_narrow()`](https://clock.r-lib.org/dev/reference/calendar_narrow.md)
  : Narrow a calendar to a less precise precision
- [`calendar_widen()`](https://clock.r-lib.org/dev/reference/calendar_widen.md)
  : Widen a calendar to a more precise precision
- [`calendar_spanning_seq()`](https://clock.r-lib.org/dev/reference/calendar_spanning_seq.md)
  : Spanning sequence: calendars
- [`calendar_precision()`](https://clock.r-lib.org/dev/reference/calendar_precision.md)
  : Precision: calendar
- [`calendar_leap_year()`](https://clock.r-lib.org/dev/reference/calendar_leap_year.md)
  : Is the calendar year a leap year?
- [`calendar_month_factor()`](https://clock.r-lib.org/dev/reference/calendar_month_factor.md)
  : Convert a calendar to an ordered factor of month names

### Year-month-day

- [`year_month_day()`](https://clock.r-lib.org/dev/reference/year_month_day.md)
  : Calendar: year-month-day

- [`as_year_month_day()`](https://clock.r-lib.org/dev/reference/as_year_month_day.md)
  : Convert to year-month-day

- [`is_year_month_day()`](https://clock.r-lib.org/dev/reference/is_year_month_day.md)
  :

  Is `x` a year-month-day?

- [`year_month_day_parse()`](https://clock.r-lib.org/dev/reference/year_month_day_parse.md)
  : Parsing: year-month-day

- [`calendar_group(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-group.md)
  : Grouping: year-month-day

- [`calendar_count_between(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-count-between.md)
  : Counting: year-month-day

- [`calendar_start(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-boundary.md)
  [`calendar_end(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-boundary.md)
  : Boundaries: year-month-day

- [`calendar_narrow(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-narrow.md)
  : Narrow: year-month-day

- [`calendar_widen(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-widen.md)
  : Widen: year-month-day

- [`seq(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/seq.clock_year_month_day.md)
  : Sequences: year-month-day

- [`add_years(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-arithmetic.md)
  [`add_quarters(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-arithmetic.md)
  [`add_months(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-arithmetic.md)
  : Arithmetic: year-month-day

- [`get_year(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-getters.md)
  [`get_month(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-getters.md)
  [`get_day(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-getters.md)
  [`get_hour(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-getters.md)
  [`get_minute(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-getters.md)
  [`get_second(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-getters.md)
  [`get_millisecond(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-getters.md)
  [`get_microsecond(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-getters.md)
  [`get_nanosecond(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-getters.md)
  : Getters: year-month-day

- [`set_year(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-setters.md)
  [`set_month(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-setters.md)
  [`set_day(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-setters.md)
  [`set_hour(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-setters.md)
  [`set_minute(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-setters.md)
  [`set_second(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-setters.md)
  [`set_millisecond(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-setters.md)
  [`set_microsecond(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-setters.md)
  [`set_nanosecond(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/year-month-day-setters.md)
  : Setters: year-month-day

### Year-month-weekday

- [`year_month_weekday()`](https://clock.r-lib.org/dev/reference/year_month_weekday.md)
  : Calendar: year-month-weekday

- [`as_year_month_weekday()`](https://clock.r-lib.org/dev/reference/as_year_month_weekday.md)
  : Convert to year-month-weekday

- [`is_year_month_weekday()`](https://clock.r-lib.org/dev/reference/is_year_month_weekday.md)
  :

  Is `x` a year-month-weekday?

- [`calendar_group(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-group.md)
  : Grouping: year-month-weekday

- [`calendar_count_between(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-count-between.md)
  : Counting: year-month-weekday

- [`calendar_start(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-boundary.md)
  [`calendar_end(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-boundary.md)
  : Boundaries: year-month-weekday

- [`calendar_narrow(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-narrow.md)
  : Narrow: year-month-weekday

- [`calendar_widen(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-widen.md)
  : Widen: year-month-weekday

- [`seq(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/seq.clock_year_month_weekday.md)
  : Sequences: year-month-weekday

- [`add_years(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-arithmetic.md)
  [`add_quarters(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-arithmetic.md)
  [`add_months(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-arithmetic.md)
  : Arithmetic: year-month-weekday

- [`get_year(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-getters.md)
  [`get_month(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-getters.md)
  [`get_day(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-getters.md)
  [`get_index(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-getters.md)
  [`get_hour(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-getters.md)
  [`get_minute(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-getters.md)
  [`get_second(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-getters.md)
  [`get_millisecond(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-getters.md)
  [`get_microsecond(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-getters.md)
  [`get_nanosecond(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-getters.md)
  : Getters: year-month-weekday

- [`set_year(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-setters.md)
  [`set_month(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-setters.md)
  [`set_day(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-setters.md)
  [`set_index(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-setters.md)
  [`set_hour(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-setters.md)
  [`set_minute(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-setters.md)
  [`set_second(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-setters.md)
  [`set_millisecond(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-setters.md)
  [`set_microsecond(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-setters.md)
  [`set_nanosecond(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/year-month-weekday-setters.md)
  : Setters: year-month-weekday

### Year-week-day

- [`year_week_day()`](https://clock.r-lib.org/dev/reference/year_week_day.md)
  : Calendar: year-week-day

- [`as_year_week_day()`](https://clock.r-lib.org/dev/reference/as_year_week_day.md)
  : Convert to year-week-day

- [`is_year_week_day()`](https://clock.r-lib.org/dev/reference/is_year_week_day.md)
  :

  Is `x` a year-week-day?

- [`calendar_group(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-group.md)
  : Grouping: year-week-day

- [`calendar_count_between(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-count-between.md)
  : Counting: year-week-day

- [`calendar_start(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-boundary.md)
  [`calendar_end(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-boundary.md)
  : Boundaries: year-week-day

- [`calendar_narrow(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-narrow.md)
  : Narrow: year-week-day

- [`calendar_widen(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-widen.md)
  : Widen: year-week-day

- [`seq(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/seq.clock_year_week_day.md)
  : Sequences: year-week-day

- [`add_years(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-arithmetic.md)
  : Arithmetic: year-week-day

- [`get_year(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-getters.md)
  [`get_week(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-getters.md)
  [`get_day(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-getters.md)
  [`get_hour(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-getters.md)
  [`get_minute(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-getters.md)
  [`get_second(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-getters.md)
  [`get_millisecond(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-getters.md)
  [`get_microsecond(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-getters.md)
  [`get_nanosecond(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-getters.md)
  : Getters: year-week-day

- [`set_year(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-setters.md)
  [`set_week(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-setters.md)
  [`set_day(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-setters.md)
  [`set_hour(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-setters.md)
  [`set_minute(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-setters.md)
  [`set_second(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-setters.md)
  [`set_millisecond(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-setters.md)
  [`set_microsecond(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-setters.md)
  [`set_nanosecond(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/year-week-day-setters.md)
  : Setters: year-week-day

### ISO-year-week-day

- [`iso_year_week_day()`](https://clock.r-lib.org/dev/reference/iso_year_week_day.md)
  : Calendar: iso-year-week-day

- [`as_iso_year_week_day()`](https://clock.r-lib.org/dev/reference/as_iso_year_week_day.md)
  : Convert to iso-year-week-day

- [`is_iso_year_week_day()`](https://clock.r-lib.org/dev/reference/is_iso_year_week_day.md)
  :

  Is `x` a iso-year-week-day?

- [`calendar_group(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-group.md)
  : Grouping: iso-year-week-day

- [`calendar_count_between(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-count-between.md)
  : Counting: iso-year-week-day

- [`calendar_start(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-boundary.md)
  [`calendar_end(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-boundary.md)
  : Boundaries: iso-year-week-day

- [`calendar_narrow(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-narrow.md)
  : Narrow: iso-year-week-day

- [`calendar_widen(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-widen.md)
  : Widen: iso-year-week-day

- [`seq(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/seq.clock_iso_year_week_day.md)
  : Sequences: iso-year-week-day

- [`add_years(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-arithmetic.md)
  : Arithmetic: iso-year-week-day

- [`get_year(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-getters.md)
  [`get_week(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-getters.md)
  [`get_day(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-getters.md)
  [`get_hour(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-getters.md)
  [`get_minute(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-getters.md)
  [`get_second(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-getters.md)
  [`get_millisecond(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-getters.md)
  [`get_microsecond(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-getters.md)
  [`get_nanosecond(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-getters.md)
  : Getters: iso-year-week-day

- [`set_year(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-setters.md)
  [`set_week(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-setters.md)
  [`set_day(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-setters.md)
  [`set_hour(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-setters.md)
  [`set_minute(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-setters.md)
  [`set_second(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-setters.md)
  [`set_millisecond(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-setters.md)
  [`set_microsecond(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-setters.md)
  [`set_nanosecond(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/iso-year-week-day-setters.md)
  : Setters: iso-year-week-day

### Year-quarter-day

- [`year_quarter_day()`](https://clock.r-lib.org/dev/reference/year_quarter_day.md)
  : Calendar: year-quarter-day

- [`as_year_quarter_day()`](https://clock.r-lib.org/dev/reference/as_year_quarter_day.md)
  : Convert to year-quarter-day

- [`is_year_quarter_day()`](https://clock.r-lib.org/dev/reference/is_year_quarter_day.md)
  :

  Is `x` a year-quarter-day?

- [`calendar_group(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-group.md)
  : Grouping: year-quarter-day

- [`calendar_count_between(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-count-between.md)
  : Counting: year-quarter-day

- [`calendar_start(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-boundary.md)
  [`calendar_end(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-boundary.md)
  : Boundaries: year-quarter-day

- [`calendar_narrow(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-narrow.md)
  : Narrow: year-quarter-day

- [`calendar_widen(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-widen.md)
  : Widen: year-quarter-day

- [`seq(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/seq.clock_year_quarter_day.md)
  : Sequences: year-quarter-day

- [`add_years(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-arithmetic.md)
  [`add_quarters(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-arithmetic.md)
  : Arithmetic: year-quarter-day

- [`get_year(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-getters.md)
  [`get_quarter(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-getters.md)
  [`get_day(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-getters.md)
  [`get_hour(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-getters.md)
  [`get_minute(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-getters.md)
  [`get_second(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-getters.md)
  [`get_millisecond(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-getters.md)
  [`get_microsecond(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-getters.md)
  [`get_nanosecond(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-getters.md)
  : Getters: year-quarter-day

- [`set_year(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-setters.md)
  [`set_quarter(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-setters.md)
  [`set_day(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-setters.md)
  [`set_hour(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-setters.md)
  [`set_minute(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-setters.md)
  [`set_second(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-setters.md)
  [`set_millisecond(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-setters.md)
  [`set_microsecond(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-setters.md)
  [`set_nanosecond(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/year-quarter-day-setters.md)
  : Setters: year-quarter-day

### Year-day

- [`year_day()`](https://clock.r-lib.org/dev/reference/year_day.md) :
  Calendar: year-day

- [`as_year_day()`](https://clock.r-lib.org/dev/reference/as_year_day.md)
  : Convert to year-day

- [`is_year_day()`](https://clock.r-lib.org/dev/reference/is_year_day.md)
  :

  Is `x` a year-day?

- [`calendar_group(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-group.md)
  : Grouping: year-day

- [`calendar_count_between(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-count-between.md)
  : Counting: year-day

- [`calendar_start(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-boundary.md)
  [`calendar_end(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-boundary.md)
  : Boundaries: year-day

- [`calendar_narrow(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-narrow.md)
  : Narrow: year-day

- [`calendar_widen(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-widen.md)
  : Widen: year-day

- [`seq(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/seq.clock_year_day.md)
  : Sequences: year-day

- [`add_years(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-arithmetic.md)
  : Arithmetic: year-day

- [`get_year(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-getters.md)
  [`get_day(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-getters.md)
  [`get_hour(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-getters.md)
  [`get_minute(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-getters.md)
  [`get_second(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-getters.md)
  [`get_millisecond(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-getters.md)
  [`get_microsecond(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-getters.md)
  [`get_nanosecond(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-getters.md)
  : Getters: year-day

- [`set_year(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-setters.md)
  [`set_day(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-setters.md)
  [`set_hour(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-setters.md)
  [`set_minute(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-setters.md)
  [`set_second(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-setters.md)
  [`set_millisecond(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-setters.md)
  [`set_microsecond(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-setters.md)
  [`set_nanosecond(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/year-day-setters.md)
  : Setters: year-day

## Invalid dates

- [`invalid_detect()`](https://clock.r-lib.org/dev/reference/clock-invalid.md)
  [`invalid_any()`](https://clock.r-lib.org/dev/reference/clock-invalid.md)
  [`invalid_count()`](https://clock.r-lib.org/dev/reference/clock-invalid.md)
  [`invalid_remove()`](https://clock.r-lib.org/dev/reference/clock-invalid.md)
  [`invalid_resolve()`](https://clock.r-lib.org/dev/reference/clock-invalid.md)
  : Invalid calendar dates

## Durations

- [`duration_years()`](https://clock.r-lib.org/dev/reference/duration-helper.md)
  [`duration_quarters()`](https://clock.r-lib.org/dev/reference/duration-helper.md)
  [`duration_months()`](https://clock.r-lib.org/dev/reference/duration-helper.md)
  [`duration_weeks()`](https://clock.r-lib.org/dev/reference/duration-helper.md)
  [`duration_days()`](https://clock.r-lib.org/dev/reference/duration-helper.md)
  [`duration_hours()`](https://clock.r-lib.org/dev/reference/duration-helper.md)
  [`duration_minutes()`](https://clock.r-lib.org/dev/reference/duration-helper.md)
  [`duration_seconds()`](https://clock.r-lib.org/dev/reference/duration-helper.md)
  [`duration_milliseconds()`](https://clock.r-lib.org/dev/reference/duration-helper.md)
  [`duration_microseconds()`](https://clock.r-lib.org/dev/reference/duration-helper.md)
  [`duration_nanoseconds()`](https://clock.r-lib.org/dev/reference/duration-helper.md)
  : Construct a duration

- [`as_duration()`](https://clock.r-lib.org/dev/reference/as_duration.md)
  : Convert to a duration

- [`is_duration()`](https://clock.r-lib.org/dev/reference/is_duration.md)
  :

  Is `x` a duration?

- [`duration_cast()`](https://clock.r-lib.org/dev/reference/duration_cast.md)
  : Cast a duration between precisions

- [`duration_floor()`](https://clock.r-lib.org/dev/reference/duration-rounding.md)
  [`duration_ceiling()`](https://clock.r-lib.org/dev/reference/duration-rounding.md)
  [`duration_round()`](https://clock.r-lib.org/dev/reference/duration-rounding.md)
  : Duration rounding

- [`duration_precision()`](https://clock.r-lib.org/dev/reference/duration_precision.md)
  : Precision: duration

- [`duration_spanning_seq()`](https://clock.r-lib.org/dev/reference/duration_spanning_seq.md)
  : Spanning sequence: duration

- [`seq(`*`<clock_duration>`*`)`](https://clock.r-lib.org/dev/reference/seq.clock_duration.md)
  : Sequences: duration

- [`add_years(`*`<clock_duration>`*`)`](https://clock.r-lib.org/dev/reference/duration-arithmetic.md)
  [`add_quarters(`*`<clock_duration>`*`)`](https://clock.r-lib.org/dev/reference/duration-arithmetic.md)
  [`add_months(`*`<clock_duration>`*`)`](https://clock.r-lib.org/dev/reference/duration-arithmetic.md)
  [`add_weeks(`*`<clock_duration>`*`)`](https://clock.r-lib.org/dev/reference/duration-arithmetic.md)
  [`add_days(`*`<clock_duration>`*`)`](https://clock.r-lib.org/dev/reference/duration-arithmetic.md)
  [`add_hours(`*`<clock_duration>`*`)`](https://clock.r-lib.org/dev/reference/duration-arithmetic.md)
  [`add_minutes(`*`<clock_duration>`*`)`](https://clock.r-lib.org/dev/reference/duration-arithmetic.md)
  [`add_seconds(`*`<clock_duration>`*`)`](https://clock.r-lib.org/dev/reference/duration-arithmetic.md)
  [`add_milliseconds(`*`<clock_duration>`*`)`](https://clock.r-lib.org/dev/reference/duration-arithmetic.md)
  [`add_microseconds(`*`<clock_duration>`*`)`](https://clock.r-lib.org/dev/reference/duration-arithmetic.md)
  [`add_nanoseconds(`*`<clock_duration>`*`)`](https://clock.r-lib.org/dev/reference/duration-arithmetic.md)
  : Arithmetic: duration

## Time points

### Manipulation

- [`time_point_cast()`](https://clock.r-lib.org/dev/reference/time_point_cast.md)
  : Cast a time point between precisions
- [`time_point_count_between()`](https://clock.r-lib.org/dev/reference/time_point_count_between.md)
  : Counting: time point
- [`time_point_floor()`](https://clock.r-lib.org/dev/reference/time-point-rounding.md)
  [`time_point_ceiling()`](https://clock.r-lib.org/dev/reference/time-point-rounding.md)
  [`time_point_round()`](https://clock.r-lib.org/dev/reference/time-point-rounding.md)
  : Time point rounding
- [`time_point_shift()`](https://clock.r-lib.org/dev/reference/time_point_shift.md)
  : Shifting: time point
- [`time_point_precision()`](https://clock.r-lib.org/dev/reference/time_point_precision.md)
  : Precision: time point
- [`time_point_spanning_seq()`](https://clock.r-lib.org/dev/reference/time_point_spanning_seq.md)
  : Spanning sequence: time points
- [`seq(`*`<clock_time_point>`*`)`](https://clock.r-lib.org/dev/reference/seq.clock_time_point.md)
  : Sequences: time points
- [`add_weeks(`*`<clock_time_point>`*`)`](https://clock.r-lib.org/dev/reference/time-point-arithmetic.md)
  [`add_days(`*`<clock_time_point>`*`)`](https://clock.r-lib.org/dev/reference/time-point-arithmetic.md)
  [`add_hours(`*`<clock_time_point>`*`)`](https://clock.r-lib.org/dev/reference/time-point-arithmetic.md)
  [`add_minutes(`*`<clock_time_point>`*`)`](https://clock.r-lib.org/dev/reference/time-point-arithmetic.md)
  [`add_seconds(`*`<clock_time_point>`*`)`](https://clock.r-lib.org/dev/reference/time-point-arithmetic.md)
  [`add_milliseconds(`*`<clock_time_point>`*`)`](https://clock.r-lib.org/dev/reference/time-point-arithmetic.md)
  [`add_microseconds(`*`<clock_time_point>`*`)`](https://clock.r-lib.org/dev/reference/time-point-arithmetic.md)
  [`add_nanoseconds(`*`<clock_time_point>`*`)`](https://clock.r-lib.org/dev/reference/time-point-arithmetic.md)
  : Arithmetic: Time points

### Sys-time

- [`as_sys_time()`](https://clock.r-lib.org/dev/reference/as_sys_time.md)
  : Convert to a sys-time

- [`is_sys_time()`](https://clock.r-lib.org/dev/reference/is_sys_time.md)
  :

  Is `x` a sys-time?

- [`sys_time_parse()`](https://clock.r-lib.org/dev/reference/sys-parsing.md)
  [`sys_time_parse_RFC_3339()`](https://clock.r-lib.org/dev/reference/sys-parsing.md)
  : Parsing: sys-time

- [`sys_time_info()`](https://clock.r-lib.org/dev/reference/sys_time_info.md)
  : Info: sys-time

- [`sys_time_now()`](https://clock.r-lib.org/dev/reference/sys_time_now.md)
  : What is the current sys-time?

- [`as_zoned_time(`*`<clock_sys_time>`*`)`](https://clock.r-lib.org/dev/reference/as-zoned-time-sys-time.md)
  : Convert to a zoned-time from a sys-time

### Naive-time

- [`as_naive_time()`](https://clock.r-lib.org/dev/reference/as_naive_time.md)
  : Convert to a naive-time

- [`is_naive_time()`](https://clock.r-lib.org/dev/reference/is_naive_time.md)
  :

  Is `x` a naive-time?

- [`naive_time_parse()`](https://clock.r-lib.org/dev/reference/naive_time_parse.md)
  : Parsing: naive-time

- [`naive_time_info()`](https://clock.r-lib.org/dev/reference/naive_time_info.md)
  : Info: naive-time

- [`as_zoned_time(`*`<clock_naive_time>`*`)`](https://clock.r-lib.org/dev/reference/as-zoned-time-naive-time.md)
  : Convert to a zoned-time from a naive-time

## Zoned-time

- [`as_zoned_time()`](https://clock.r-lib.org/dev/reference/as_zoned_time.md)
  : Convert to a zoned-time

- [`is_zoned_time()`](https://clock.r-lib.org/dev/reference/is_zoned_time.md)
  :

  Is `x` a zoned-time?

- [`zoned_time_parse_complete()`](https://clock.r-lib.org/dev/reference/zoned-parsing.md)
  [`zoned_time_parse_abbrev()`](https://clock.r-lib.org/dev/reference/zoned-parsing.md)
  : Parsing: zoned-time

- [`zoned_time_now()`](https://clock.r-lib.org/dev/reference/zoned_time_now.md)
  : What is the current zoned-time?

- [`zoned_time_zone()`](https://clock.r-lib.org/dev/reference/zoned-zone.md)
  [`zoned_time_set_zone()`](https://clock.r-lib.org/dev/reference/zoned-zone.md)
  : Get or set the time zone

- [`zoned_time_precision()`](https://clock.r-lib.org/dev/reference/zoned_time_precision.md)
  : Precision: zoned-time

- [`zoned_time_info()`](https://clock.r-lib.org/dev/reference/zoned_time_info.md)
  : Info: zoned-time

- [`format(`*`<clock_zoned_time>`*`)`](https://clock.r-lib.org/dev/reference/format.clock_zoned_time.md)
  : Formatting: zoned-time

## Weekdays

- [`weekday()`](https://clock.r-lib.org/dev/reference/weekday.md) :
  Construct a weekday vector

- [`as_weekday()`](https://clock.r-lib.org/dev/reference/as_weekday.md)
  : Convert to a weekday

- [`is_weekday()`](https://clock.r-lib.org/dev/reference/is_weekday.md)
  :

  Is `x` a weekday?

- [`weekday_code()`](https://clock.r-lib.org/dev/reference/weekday_code.md)
  : Extract underlying weekday codes

- [`weekday_factor()`](https://clock.r-lib.org/dev/reference/weekday_factor.md)
  : Convert a weekday to an ordered factor

- [`add_days(`*`<clock_weekday>`*`)`](https://clock.r-lib.org/dev/reference/weekday-arithmetic.md)
  : Arithmetic: weekday

## Locale

- [`clock_locale()`](https://clock.r-lib.org/dev/reference/clock_locale.md)
  : Create a clock locale
- [`clock_labels()`](https://clock.r-lib.org/dev/reference/clock_labels.md)
  [`clock_labels_lookup()`](https://clock.r-lib.org/dev/reference/clock_labels.md)
  [`clock_labels_languages()`](https://clock.r-lib.org/dev/reference/clock_labels.md)
  : Create or retrieve date related labels

## Codes

- [`clock_months`](https://clock.r-lib.org/dev/reference/clock-codes.md)
  [`clock_weekdays`](https://clock.r-lib.org/dev/reference/clock-codes.md)
  [`clock_iso_weekdays`](https://clock.r-lib.org/dev/reference/clock-codes.md)
  : Integer codes

## Developer

- [`vec_arith(`*`<clock_year_day>`*`)`](https://clock.r-lib.org/dev/reference/clock-arith.md)
  [`vec_arith(`*`<clock_year_month_day>`*`)`](https://clock.r-lib.org/dev/reference/clock-arith.md)
  [`vec_arith(`*`<clock_year_month_weekday>`*`)`](https://clock.r-lib.org/dev/reference/clock-arith.md)
  [`vec_arith(`*`<clock_iso_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/clock-arith.md)
  [`vec_arith(`*`<clock_naive_time>`*`)`](https://clock.r-lib.org/dev/reference/clock-arith.md)
  [`vec_arith(`*`<clock_year_quarter_day>`*`)`](https://clock.r-lib.org/dev/reference/clock-arith.md)
  [`vec_arith(`*`<clock_sys_time>`*`)`](https://clock.r-lib.org/dev/reference/clock-arith.md)
  [`vec_arith(`*`<clock_year_week_day>`*`)`](https://clock.r-lib.org/dev/reference/clock-arith.md)
  [`vec_arith(`*`<clock_weekday>`*`)`](https://clock.r-lib.org/dev/reference/clock-arith.md)
  : Support for vctrs arithmetic

## Generics and methods

This section contains documentation about generics and methods that is
already exposed elsewhere on this reference page.

- [`add_years()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)
  [`add_quarters()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)
  [`add_months()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)
  [`add_weeks()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)
  [`add_days()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)
  [`add_hours()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)
  [`add_minutes()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)
  [`add_seconds()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)
  [`add_milliseconds()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)
  [`add_microseconds()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)
  [`add_nanoseconds()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)
  : Clock arithmetic
- [`get_year()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  [`get_quarter()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  [`get_month()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  [`get_week()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  [`get_day()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  [`get_hour()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  [`get_minute()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  [`get_second()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  [`get_millisecond()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  [`get_microsecond()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  [`get_nanosecond()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  [`get_index()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  : Calendar getters
- [`set_year()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  [`set_quarter()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  [`set_month()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  [`set_week()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  [`set_day()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  [`set_hour()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  [`set_minute()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  [`set_second()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  [`set_millisecond()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  [`set_microsecond()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  [`set_nanosecond()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  [`set_index()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  : Calendar setters
- [`date_group(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/date-group.md)
  : Group date components
- [`date_group(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-group.md)
  : Group date-time components
- [`date_count_between(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/date-count-between.md)
  : Counting: date
- [`date_count_between(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-count-between.md)
  : Counting: date-times
- [`date_start(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/date-boundary.md)
  [`date_end(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/date-boundary.md)
  : Boundaries: date
- [`date_start(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-boundary.md)
  [`date_end(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-boundary.md)
  : Boundaries: date-time
- [`date_format(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/date-formatting.md)
  : Formatting: date
- [`date_format(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-formatting.md)
  : Formatting: date-time
- [`date_floor(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/date-rounding.md)
  [`date_ceiling(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/date-rounding.md)
  [`date_round(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/date-rounding.md)
  : Rounding: date
- [`date_floor(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-rounding.md)
  [`date_ceiling(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-rounding.md)
  [`date_round(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-rounding.md)
  : Rounding: date-time
- [`date_shift(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/date-shifting.md)
  : Shifting: date
- [`date_shift(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-shifting.md)
  : Shifting: date and date-time
- [`date_seq(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/date-sequence.md)
  : Sequences: date
- [`date_seq(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/posixt-sequence.md)
  : Sequences: date-time
- [`as_zoned_time(`*`<Date>`*`)`](https://clock.r-lib.org/dev/reference/as-zoned-time-Date.md)
  : Convert to a zoned-time from a date
- [`as_zoned_time(`*`<POSIXt>`*`)`](https://clock.r-lib.org/dev/reference/as-zoned-time-posixt.md)
  : Convert to a zoned-time from a date-time

## Reexports

- [`reexports`](https://clock.r-lib.org/dev/reference/reexports.md)
  [`tzdb_names`](https://clock.r-lib.org/dev/reference/reexports.md)
  [`tzdb_version`](https://clock.r-lib.org/dev/reference/reexports.md) :
  Objects exported from other packages
