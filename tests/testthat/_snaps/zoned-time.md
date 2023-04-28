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
      ! `x` must be a 'clock_zoned_time'.

# zoned-times don't support arithmetic

    Code
      add_years(x, 1)
    Condition
      Error in `add_years()`:
      ! Zoned-times don't support `add_years()`.

---

    Code
      add_quarters(x, 1)
    Condition
      Error in `add_quarters()`:
      ! Zoned-times don't support `add_quarters()`.

---

    Code
      add_months(x, 1)
    Condition
      Error in `add_months()`:
      ! Zoned-times don't support `add_months()`.

---

    Code
      add_weeks(x, 1)
    Condition
      Error in `add_weeks()`:
      ! Zoned-times don't support `add_weeks()`.

---

    Code
      add_days(x, 1)
    Condition
      Error in `add_days()`:
      ! Zoned-times don't support `add_days()`.

---

    Code
      add_hours(x, 1)
    Condition
      Error in `add_hours()`:
      ! Zoned-times don't support `add_hours()`.

---

    Code
      add_minutes(x, 1)
    Condition
      Error in `add_minutes()`:
      ! Zoned-times don't support `add_minutes()`.

---

    Code
      add_seconds(x, 1)
    Condition
      Error in `add_seconds()`:
      ! Zoned-times don't support `add_seconds()`.

---

    Code
      add_milliseconds(x, 1)
    Condition
      Error in `add_milliseconds()`:
      ! Zoned-times don't support `add_milliseconds()`.

---

    Code
      add_microseconds(x, 1)
    Condition
      Error in `add_microseconds()`:
      ! Zoned-times don't support `add_microseconds()`.

---

    Code
      add_nanoseconds(x, 1)
    Condition
      Error in `add_nanoseconds()`:
      ! Zoned-times don't support `add_nanoseconds()`.

# precision: can only be called on zoned-times

    Code
      zoned_time_precision(duration_days())
    Condition
      Error in `zoned_time_precision()`:
      ! `x` must be a 'clock_zoned_time'.

