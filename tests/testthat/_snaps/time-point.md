# normal print method works

    Code
      x
    Output
      <time_point<sys><day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01" "2019-04-01" "2019-05-01"

# can limit with `max`

    Code
      print(x, max = 2)
    Output
      <time_point<sys><day>[5]>
      [1] "2019-01-01" "2019-02-01"
      Reached `max` or `getOption('max.print')`. Omitted 3 values.

---

    Code
      print(x, max = 4)
    Output
      <time_point<sys><day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01" "2019-04-01"
      Reached `max` or `getOption('max.print')`. Omitted 1 value.

---

    Code
      print(x, max = 5)
    Output
      <time_point<sys><day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01" "2019-04-01" "2019-05-01"

---

    Code
      print(x, max = 6)
    Output
      <time_point<sys><day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01" "2019-04-01" "2019-05-01"

# `max` defaults to `getOption('max.print')` but can be overridden

    Code
      x
    Output
      <time_point<naive><day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01"
      Reached `max` or `getOption('max.print')`. Omitted 2 values.

---

    Code
      print(x, max = 4)
    Output
      <time_point<naive><day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01" "2019-04-01"
      Reached `max` or `getOption('max.print')`. Omitted 1 value.

---

    Code
      print(x, max = 5)
    Output
      <time_point<naive><day>[5]>
      [1] "2019-01-01" "2019-02-01" "2019-03-01" "2019-04-01" "2019-05-01"

# cannot floor to more precise precision

    Can't floor to a more precise precision.

# rounding with `origin` requires same clock

    Can't convert `origin` <time_point<sys><day>> to <time_point<naive><day>>.

# `origin` can be cast to a more precise `precision`, but not to a less precise one

    Can't convert `origin` <time_point<naive><millisecond>> to <time_point<naive><hour>>.
    Can't cast to a less precise precision.

# `origin` must be size 1

    `origin` must have length 1.

# `origin` must not be `NA`

    `origin` must not be `NA`.

# `origin` can't be Date or POSIXt

    Can't convert `origin` <date> to <time_point<naive><day>>.

---

    Can't convert `origin` <datetime<America/New_York>> to <time_point<naive><day>>.

# `target` is recycled to size of `x`

    Can't recycle `target` (size 2) to size 1.

# `x` is validated

    `x` must be a 'clock_time_point'.

# `target` is validated

    `target` must be a 'clock_weekday'.

# `which` is validated

    `which` must be either "next" or "previous".

---

    `which` must be either "next" or "previous".

---

    `which` must be either "next" or "previous".

# `boundary` is validated

    `boundary` must be either "keep" or "advance".

---

    `boundary` must be either "keep" or "advance".

---

    `boundary` must be either "keep" or "advance".

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
      ! `end` must be a <clock_time_point>.
    Code
      (expect_error(time_point_count_between(1, sys_days(1))))
    Output
      <error/rlang_error>
      Error in `time_point_count_between()`:
      ! `start` must be a <clock_time_point>.

# both inputs must be compatible

    Code
      (expect_error(time_point_count_between(x, y)))
    Output
      <error/vctrs_error_incompatible_type>
      Error in `time_point_count_between()`:
      ! Can't combine `start` <time_point<sys><day>> and `end` <time_point<naive><day>>.

# `n` is validated

    Code
      (expect_error(time_point_count_between(x, x, "day", n = NA_integer_)))
    Output
      <error/rlang_error>
      Error in `time_point_count_between()`:
      ! `n` must be a single positive integer.
    Code
      (expect_error(time_point_count_between(x, x, "day", n = -1)))
    Output
      <error/rlang_error>
      Error in `time_point_count_between()`:
      ! `n` must be a single positive integer.
    Code
      (expect_error(time_point_count_between(x, x, "day", n = 1.5)))
    Output
      <error/vctrs_error_cast_lossy>
      Error in `time_point_count_between()`:
      ! Can't convert from `n` <double> to <integer> due to loss of precision.
      * Locations: 1
    Code
      (expect_error(time_point_count_between(x, x, "day", n = "x")))
    Output
      <error/vctrs_error_incompatible_type>
      Error in `time_point_count_between()`:
      ! Can't convert `n` <character> to <integer>.
    Code
      (expect_error(time_point_count_between(x, x, "day", n = c(1L, 2L))))
    Output
      <error/rlang_error>
      Error in `time_point_count_between()`:
      ! `n` must be a single positive integer.

# `precision` must be a time point precision

    Code
      (expect_error(time_point_count_between(x, x, "year")))
    Output
      <error/rlang_error>
      Error in `time_point_count_between()`:
      ! `precision` must be at least 'week' precision.

# can't mix chronological time points and calendrical durations

    Can't convert `by` <duration<year>> to <duration<second>>.
    Can't cast between calendrical durations and chronological durations.

# can't mix clocks in seq()

    Can't convert `to` <time_point<naive><second>> to match type of `from` <time_point<sys><second>>.

# `to` is always cast to `from`

    Can't convert `to` <time_point<naive><second>> to match type of `from` <time_point<naive><day>>.
    Can't cast to a less precise precision.

# duration to add to a time-point must have at least week precision (#120)

    Can't combine `x` <duration<second>> and `y` <duration<year>>.
    Can't combine calendrical durations with chronological durations.

# precision: can only be called on time points

    `x` must be a 'clock_time_point'.

