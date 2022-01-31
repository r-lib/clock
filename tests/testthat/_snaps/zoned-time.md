# can't compare zoned-times with different zones

    Can't combine `..1` <zoned_time<second><America/New_York>> and `..2` <zoned_time<second><UTC>>.
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

    All elements of `x` must have the same time zone name. Found different zone names of: 'America/New_York' and 'America/Los_Angeles'.

# zone name must be valid

    `%Z` must be used, and must result in a valid time zone name, not 'America/New_Yor'.

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

    `%Z` must be used and must result in a time zone abbreviation.

# zoned-times don't support arithmetic

    Zoned-times don't support `add_years()`.

---

    Zoned-times don't support `add_quarters()`.

---

    Zoned-times don't support `add_months()`.

---

    Zoned-times don't support `add_weeks()`.

---

    Zoned-times don't support `add_days()`.

---

    Zoned-times don't support `add_hours()`.

---

    Zoned-times don't support `add_minutes()`.

---

    Zoned-times don't support `add_seconds()`.

---

    Zoned-times don't support `add_milliseconds()`.

---

    Zoned-times don't support `add_microseconds()`.

---

    Zoned-times don't support `add_nanoseconds()`.

# precision: can only be called on zoned-times

    `x` must be a 'clock_zoned_time'.

