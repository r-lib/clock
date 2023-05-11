# can't compare zoned-times with different zones

    Code
      x > y
    Condition
      Error in `vec_compare()`:
      ! Can't combine `..1` <zoned_time<second><America/New_York>> and `..2` <zoned_time<second><UTC>>.
      Zones can't differ.

# normal print method works

    Code
      x
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31T19:00:00-05:00" "2019-01-31T19:00:00-05:00"
      [3] "2019-02-28T19:00:00-05:00" "2019-03-31T20:00:00-04:00"
      [5] "2019-04-30T20:00:00-04:00"

# can limit with `max`

    Code
      print(x, max = 2)
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31T19:00:00-05:00" "2019-01-31T19:00:00-05:00"
      Reached `max` or `getOption('max.print')`. Omitted 3 values.

---

    Code
      print(x, max = 4)
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31T19:00:00-05:00" "2019-01-31T19:00:00-05:00"
      [3] "2019-02-28T19:00:00-05:00" "2019-03-31T20:00:00-04:00"
      Reached `max` or `getOption('max.print')`. Omitted 1 value.

---

    Code
      print(x, max = 5)
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31T19:00:00-05:00" "2019-01-31T19:00:00-05:00"
      [3] "2019-02-28T19:00:00-05:00" "2019-03-31T20:00:00-04:00"
      [5] "2019-04-30T20:00:00-04:00"

---

    Code
      print(x, max = 6)
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31T19:00:00-05:00" "2019-01-31T19:00:00-05:00"
      [3] "2019-02-28T19:00:00-05:00" "2019-03-31T20:00:00-04:00"
      [5] "2019-04-30T20:00:00-04:00"

# `max` defaults to `getOption('max.print')` but can be overridden

    Code
      x
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31T19:00:00-05:00" "2019-01-31T19:00:00-05:00"
      [3] "2019-02-28T19:00:00-05:00"
      Reached `max` or `getOption('max.print')`. Omitted 2 values.

---

    Code
      print(x, max = 4)
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31T19:00:00-05:00" "2019-01-31T19:00:00-05:00"
      [3] "2019-02-28T19:00:00-05:00" "2019-03-31T20:00:00-04:00"
      Reached `max` or `getOption('max.print')`. Omitted 1 value.

---

    Code
      print(x, max = 5)
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31T19:00:00-05:00" "2019-01-31T19:00:00-05:00"
      [3] "2019-02-28T19:00:00-05:00" "2019-03-31T20:00:00-04:00"
      [5] "2019-04-30T20:00:00-04:00"

# `as_zoned_time()` has good default error

    Code
      as_zoned_time(1)
    Condition
      Error in `as_zoned_time()`:
      ! Can't convert <numeric> to <clock_zoned_time>.

# can't convert zoned-time directly to calendar

    Code
      (expect_error(as_year_month_day(x), class = "clock_error_unsupported_conversion")
      )
    Output
      <error/clock_error_unsupported_conversion>
      Error in `as_year_month_day()`:
      ! Can't convert <clock_zoned_time> to <clock_year_month_day>.

# cannot parse nonexistent time

    Code
      zoned_time_parse_complete(x)
    Condition
      Warning:
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <zoned_time<second><America/New_York>[1]>
      [1] NA

# offset must align with unique offset

    Code
      zoned_time_parse_complete(x)
    Condition
      Warning:
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <zoned_time<second><America/New_York>[1]>
      [1] NA

# offset must align with one of two possible ambiguous offsets

    Code
      zoned_time_parse_complete(x)
    Condition
      Warning:
      Failed to parse 2 strings, beginning at location 1. Returning `NA` at the locations where there were parse failures.
    Output
      <zoned_time<second><America/New_York>[2]>
      [1] NA NA

# cannot have differing zone names

    Code
      zoned_time_parse_complete(x)
    Condition
      Error:
      ! All elements of `x` must have the same time zone name. Found different zone names of: 'America/New_York' and 'America/Los_Angeles'.

# zone name must be valid

    Code
      zoned_time_parse_complete(x)
    Condition
      Error:
      ! `%Z` must be used, and must result in a valid time zone name, not 'America/New_Yor'.

# abbreviation must match the one implied from naive + time zone name lookup

    Code
      zoned_time_parse_abbrev(x, "America/New_York")
    Condition
      Warning:
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <zoned_time<second><America/New_York>[1]>
      [1] NA

# %Z must be used

    Code
      zoned_time_parse_abbrev(x, "America/New_York", format = "%Y-%m-%d")
    Condition
      Error:
      ! `%Z` must be used and must result in a time zone abbreviation.

# boundaries are handled right

    Code
      x$begin
    Output
      <zoned_time<second><UTC>[1]>
      [1] "-32767-01-01T00:00:00+00:00"

---

    Code
      x$end
    Output
      <zoned_time<second><UTC>[1]>
      [1] "32767-12-31T00:00:00+00:00"

# input must be a zoned-time

    Code
      zoned_time_info(1)
    Condition
      Error in `zoned_time_info()`:
      ! `x` must be a <clock_zoned_time>, not the number 1.

# zoned-times don't support arithmetic

    Code
      add_years(x, 1)
    Condition
      Error in `add_years()`:
      ! Can't perform this operation on a <clock_zoned_time>.
      i Do you need to convert to a calendar first?
      i Use `as_naive_time()` then `as_year_month_day()` to convert to a calendar that supports `add_years()`.

---

    Code
      add_quarters(x, 1)
    Condition
      Error in `add_quarters()`:
      ! Can't perform this operation on a <clock_zoned_time>.
      i Do you need to convert to a calendar first?
      i Use `as_naive_time()` then `as_year_quarter_day()` to convert to a calendar that supports `add_quarters()`.

---

    Code
      add_months(x, 1)
    Condition
      Error in `add_months()`:
      ! Can't perform this operation on a <clock_zoned_time>.
      i Do you need to convert to a calendar first?
      i Use `as_naive_time()` then `as_year_month_day()` to convert to a calendar that supports `add_months()`.

---

    Code
      add_weeks(x, 1)
    Condition
      Error in `add_weeks()`:
      ! Can't perform this operation on a <clock_zoned_time>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_days(x, 1)
    Condition
      Error in `add_days()`:
      ! Can't perform this operation on a <clock_zoned_time>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_hours(x, 1)
    Condition
      Error in `add_hours()`:
      ! Can't perform this operation on a <clock_zoned_time>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_minutes(x, 1)
    Condition
      Error in `add_minutes()`:
      ! Can't perform this operation on a <clock_zoned_time>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_seconds(x, 1)
    Condition
      Error in `add_seconds()`:
      ! Can't perform this operation on a <clock_zoned_time>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_milliseconds(x, 1)
    Condition
      Error in `add_milliseconds()`:
      ! Can't perform this operation on a <clock_zoned_time>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_microseconds(x, 1)
    Condition
      Error in `add_microseconds()`:
      ! Can't perform this operation on a <clock_zoned_time>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_nanoseconds(x, 1)
    Condition
      Error in `add_nanoseconds()`:
      ! Can't perform this operation on a <clock_zoned_time>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

# `zoned_time_zone()` validates `x`

    Code
      zoned_time_zone(1)
    Condition
      Error in `zoned_time_zone()`:
      ! `x` must be a <clock_zoned_time>, not the number 1.

# `zoned_time_set_zone()` validates `x`

    Code
      zoned_time_set_zone(1, "UTC")
    Condition
      Error in `zoned_time_set_zone()`:
      ! `x` must be a <clock_zoned_time>, not the number 1.

# precision: can only be called on zoned-times

    Code
      zoned_time_precision(duration_days())
    Condition
      Error in `zoned_time_precision()`:
      ! `x` must be a <clock_zoned_time>, not a <clock_duration> object.

