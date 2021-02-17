# normal print method works

    Code
      x
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31 19:00:00-05:00" "2019-01-31 19:00:00-05:00"
      [3] "2019-02-28 19:00:00-05:00" "2019-03-31 20:00:00-04:00"
      [5] "2019-04-30 20:00:00-04:00"

# can limit with `max`

    Code
      print(x, max = 2)
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31 19:00:00-05:00" "2019-01-31 19:00:00-05:00"
      Reached `max` or `getOption('max.print')`. Omitted 3 values.

---

    Code
      print(x, max = 4)
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31 19:00:00-05:00" "2019-01-31 19:00:00-05:00"
      [3] "2019-02-28 19:00:00-05:00" "2019-03-31 20:00:00-04:00"
      Reached `max` or `getOption('max.print')`. Omitted 1 value.

---

    Code
      print(x, max = 5)
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31 19:00:00-05:00" "2019-01-31 19:00:00-05:00"
      [3] "2019-02-28 19:00:00-05:00" "2019-03-31 20:00:00-04:00"
      [5] "2019-04-30 20:00:00-04:00"

---

    Code
      print(x, max = 6)
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31 19:00:00-05:00" "2019-01-31 19:00:00-05:00"
      [3] "2019-02-28 19:00:00-05:00" "2019-03-31 20:00:00-04:00"
      [5] "2019-04-30 20:00:00-04:00"

# `max` defaults to `getOption('max.print')` but can be overridden

    Code
      x
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31 19:00:00-05:00" "2019-01-31 19:00:00-05:00"
      [3] "2019-02-28 19:00:00-05:00"
      Reached `max` or `getOption('max.print')`. Omitted 2 values.

---

    Code
      print(x, max = 4)
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31 19:00:00-05:00" "2019-01-31 19:00:00-05:00"
      [3] "2019-02-28 19:00:00-05:00" "2019-03-31 20:00:00-04:00"
      Reached `max` or `getOption('max.print')`. Omitted 1 value.

---

    Code
      print(x, max = 5)
    Output
      <zoned_time<second><America/New_York>[5]>
      [1] "2018-12-31 19:00:00-05:00" "2019-01-31 19:00:00-05:00"
      [3] "2019-02-28 19:00:00-05:00" "2019-03-31 20:00:00-04:00"
      [5] "2019-04-30 20:00:00-04:00"

# cannot parse nonexistent time

    Code
      zoned_parse(x)
    Warning <clock_warning_parse_failures>
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <zoned_time<second><America/New_York>[1]>
      [1] NA

# offset must align with unique offset

    Code
      zoned_parse(x)
    Warning <clock_warning_parse_failures>
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <zoned_time<second><America/New_York>[1]>
      [1] NA

# offset must align with one of two possible ambiguous offsets

    Code
      zoned_parse(x)
    Warning <clock_warning_parse_failures>
      Failed to parse 2 strings, beginning at location 1. Returning `NA` at the locations where there were parse failures.
    Output
      <zoned_time<second><America/New_York>[2]>
      [1] NA NA

# cannot have differing zone names

    All elements of `x` must have the same time zone name. Found different zone names of: 'America/New_York' and 'America/Los_Angeles'.

# zone name must be valid

    `%Z` must be used, and must result in a valid time zone name, not 'America/New_Yor'.

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

