# normal print method works

    Code
      x
    Output
      <sys_time<day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01" "2019-04-01" "2019-05-01"

# can limit with `max`

    Code
      print(x, max = 2)
    Output
      <sys_time<day>[5]>
      [1] "2019-01-01" "2019-02-01"
      Reached `max` or `getOption('max.print')`. Omitted 3 values.

---

    Code
      print(x, max = 4)
    Output
      <sys_time<day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01" "2019-04-01"
      Reached `max` or `getOption('max.print')`. Omitted 1 value.

---

    Code
      print(x, max = 5)
    Output
      <sys_time<day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01" "2019-04-01" "2019-05-01"

---

    Code
      print(x, max = 6)
    Output
      <sys_time<day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01" "2019-04-01" "2019-05-01"

# `max` defaults to `getOption('max.print')` but can be overridden

    Code
      x
    Output
      <naive_time<day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01"
      Reached `max` or `getOption('max.print')`. Omitted 2 values.

---

    Code
      print(x, max = 4)
    Output
      <naive_time<day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01" "2019-04-01"
      Reached `max` or `getOption('max.print')`. Omitted 1 value.

---

    Code
      print(x, max = 5)
    Output
      <naive_time<day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01" "2019-04-01" "2019-05-01"

# cannot floor to more precise precision

    Code
      time_point_floor(naive_days(), "second")
    Condition
      Error in `duration_rounder()`:
      ! Can't floor to a more precise precision.

# rounding with `origin` requires same clock

    Code
      time_point_floor(x, "day", origin = origin)
    Condition
      Error in `time_point_floor()`:
      ! Can't convert `origin` <sys_time<day>> to <naive_time<day>>.

# `origin` can be cast to a more precise `precision`, but not to a less precise one

    Code
      time_point_floor(x, "hour", origin = origin2)
    Condition
      Error in `time_point_floor()`:
      ! Can't convert `origin` <naive_time<millisecond>> to <naive_time<hour>>.
      Can't cast to a less precise precision.

# `origin` must be size 1

    Code
      time_point_floor(x, "day", origin = origin)
    Condition
      Error in `time_point_floor()`:
      ! `origin` must have size 1, not size 2.

# `origin` must not be `NA`

    Code
      time_point_floor(x, "day", origin = origin)
    Condition
      Error in `time_point_floor()`:
      ! `origin` can't contain missing values.
      i The following locations are missing: 1.

# `origin` can't be Date or POSIXt

    Code
      time_point_floor(x, "day", origin = origin1)
    Condition
      Error in `time_point_floor()`:
      ! Can't convert `origin` <date> to <naive_time<day>>.

---

    Code
      time_point_floor(x, "day", origin = origin2)
    Condition
      Error in `time_point_floor()`:
      ! Can't convert `origin` <datetime<America/New_York>> to <naive_time<day>>.

# `target` is recycled to size of `x`

    Code
      time_point_shift(sys_days(0), weekday(1:2))
    Condition
      Error in `time_point_shift()`:
      ! Can't recycle `target` (size 2) to size 1.

# `x` is validated

    Code
      time_point_shift(1)
    Condition
      Error in `time_point_shift()`:
      ! `x` must be a <clock_time_point>, not the number 1.

# `target` is validated

    Code
      time_point_shift(sys_days(0), 1)
    Condition
      Error in `time_point_shift()`:
      ! `target` must be a <clock_weekday>, not the number 1.

# `which` is validated

    Code
      time_point_shift(sys_days(), weekday(), which = 1)
    Condition
      Error in `time_point_shift()`:
      ! `which` must be a single string, not the number 1.

---

    Code
      time_point_shift(sys_days(), weekday(), which = "foo")
    Condition
      Error in `time_point_shift()`:
      ! `which` must be one of "next" or "previous", not "foo".

---

    Code
      time_point_shift(sys_days(), weekday(), which = c("next", "previous"))
    Condition
      Error in `time_point_shift()`:
      ! `which` must be a single string, not a character vector.

# `boundary` is validated

    Code
      time_point_shift(sys_days(), weekday(), boundary = 1)
    Condition
      Error in `time_point_shift()`:
      ! `boundary` must be a single string, not the number 1.

---

    Code
      time_point_shift(sys_days(), weekday(), boundary = "foo")
    Condition
      Error in `time_point_shift()`:
      ! `boundary` must be one of "keep" or "advance", not "foo".

---

    Code
      time_point_shift(sys_days(), weekday(), boundary = c("keep", "advance"))
    Condition
      Error in `time_point_shift()`:
      ! `boundary` must be a single string, not a character vector.

# OOB results return a warning and NA

    Code
      out <- time_point_count_between(sys_days(0), sys_days(1000), "nanosecond")
    Condition
      Warning:
      Conversion from duration to integer is outside the range of an integer. `NA` values have been introduced, beginning at location 1.

# both inputs must be time points

    Code
      (expect_error(time_point_count_between(sys_days(1), 1)))
    Output
      <error/rlang_error>
      Error in `time_point_count_between()`:
      ! `end` must be a <clock_time_point>, not the number 1.
    Code
      (expect_error(time_point_count_between(1, sys_days(1))))
    Output
      <error/rlang_error>
      Error in `time_point_count_between()`:
      ! `start` must be a <clock_time_point>, not the number 1.

# both inputs must be compatible

    Code
      (expect_error(time_point_count_between(x, y)))
    Output
      <error/vctrs_error_ptype2>
      Error in `time_point_count_between()`:
      ! Can't combine `start` <sys_time<day>> and `end` <naive_time<day>>.

# `n` is validated

    Code
      (expect_error(time_point_count_between(x, x, "day", n = NA_integer_)))
    Output
      <error/rlang_error>
      Error in `time_point_count_between()`:
      ! `n` must be a whole number, not an integer `NA`.
    Code
      (expect_error(time_point_count_between(x, x, "day", n = -1)))
    Output
      <error/rlang_error>
      Error in `time_point_count_between()`:
      ! `n` must be a whole number larger than or equal to 0, not the number -1.
    Code
      (expect_error(time_point_count_between(x, x, "day", n = 1.5)))
    Output
      <error/rlang_error>
      Error in `time_point_count_between()`:
      ! `n` must be a whole number, not the number 1.5.
    Code
      (expect_error(time_point_count_between(x, x, "day", n = "x")))
    Output
      <error/rlang_error>
      Error in `time_point_count_between()`:
      ! `n` must be a whole number, not the string "x".
    Code
      (expect_error(time_point_count_between(x, x, "day", n = c(1L, 2L))))
    Output
      <error/rlang_error>
      Error in `time_point_count_between()`:
      ! `n` must be a whole number, not an integer vector.

# `precision` must be a time point precision

    Code
      (expect_error(time_point_count_between(x, x, "year")))
    Output
      <error/rlang_error>
      Error in `time_point_count_between()`:
      ! `precision` must be at least "week" precision.

# can't mix chronological time points and calendrical durations

    Code
      seq(naive_seconds(0), by = duration_years(1), length.out = 2)
    Condition
      Error in `seq()`:
      ! Can't convert `by` <duration<year>> to <duration<second>>.
      Can't cast between calendrical durations and chronological durations.

# can't mix clocks in seq()

    Code
      seq(sys_seconds(0), to = naive_seconds(5), by = 1)
    Condition
      Error in `seq()`:
      ! Can't convert `to` <naive_time<second>> to match type of `from` <sys_time<second>>.

# `to` is always cast to `from`

    Code
      seq(naive_days(0), to = naive_seconds(5), by = 2)
    Condition
      Error in `seq()`:
      ! Can't convert `to` <naive_time<second>> to match type of `from` <naive_time<day>>.
      Can't cast to a less precise precision.

# validates the input

    Code
      time_point_spanning_seq(1)
    Condition
      Error in `time_point_spanning_seq()`:
      ! `x` must be a <clock_time_point>, not the number 1.

# duration to add to a time-point must have at least week precision (#120)

    Code
      naive_seconds(0) + duration_years(1)
    Condition
      Error in `duration_arith()`:
      ! Can't combine `x` <duration<second>> and `y` <duration<year>>.
      Can't combine calendrical durations with chronological durations.

# precision: can only be called on time points

    Code
      time_point_precision(duration_days())
    Condition
      Error in `time_point_precision()`:
      ! `x` must be a <clock_time_point>, not a <clock_duration> object.

# unsupported time point addition throws good error

    Code
      add_years(x, 1)
    Condition
      Error in `add_years()`:
      ! Can't perform this operation on a <clock_naive_time>.
      i Do you need to convert to a calendar first?
      i Use `as_year_month_day()` for a calendar that supports `add_years()`.

---

    Code
      add_quarters(x, 1)
    Condition
      Error in `add_quarters()`:
      ! Can't perform this operation on a <clock_naive_time>.
      i Do you need to convert to a calendar first?
      i Use `as_year_quarter_day()` for a calendar that supports `add_quarters()`.

---

    Code
      add_months(x, 1)
    Condition
      Error in `add_months()`:
      ! Can't perform this operation on a <clock_naive_time>.
      i Do you need to convert to a calendar first?
      i Use `as_year_month_day()` for a calendar that supports `add_months()`.

# minimums are right

    Code
      clock_minimum(as_sys_time(duration_nanoseconds()))
    Output
      <sys_time<nanosecond>[1]>
      [1] "1677-09-21T00:12:43.145224192"
    Code
      clock_minimum(as_naive_time(duration_nanoseconds()))
    Output
      <naive_time<nanosecond>[1]>
      [1] "1677-09-21T00:12:43.145224192"

# maximums are right

    Code
      clock_maximum(as_sys_time(duration_nanoseconds()))
    Output
      <sys_time<nanosecond>[1]>
      [1] "2262-04-11T23:47:16.854775807"
    Code
      clock_maximum(as_naive_time(duration_nanoseconds()))
    Output
      <naive_time<nanosecond>[1]>
      [1] "2262-04-11T23:47:16.854775807"

