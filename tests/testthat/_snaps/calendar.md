# normal print method works

    Code
      x
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03" "2019-04" "2019-05"

# can limit with `max`

    Code
      print(x, max = 2)
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02"
      Reached `max` or `getOption('max.print')`. Omitted 3 values.

---

    Code
      print(x, max = 4)
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03" "2019-04"
      Reached `max` or `getOption('max.print')`. Omitted 1 value.

---

    Code
      print(x, max = 5)
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03" "2019-04" "2019-05"

---

    Code
      print(x, max = 6)
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03" "2019-04" "2019-05"

# `max` defaults to `getOption('max.print')` but can be overridden

    Code
      x
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03"
      Reached `max` or `getOption('max.print')`. Omitted 2 values.

---

    Code
      print(x, max = 4)
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03" "2019-04"
      Reached `max` or `getOption('max.print')`. Omitted 1 value.

---

    Code
      print(x, max = 5)
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03" "2019-04" "2019-05"

# `max` is validated

    `max` must be a positive number.

---

    `max` must be a single number, or `NULL`.

---

    `max` must be a single number, or `NULL`.

---

    Can't convert `max` <character> to <integer>.

# group: `precision` is validated

    `precision` not recognized.

# group: `precision` must be calendar specific

    `precision` must be a valid precision for a 'year_month_day'.

# group: `precision` can't be wider than `x`

    Can't group at a precision (second) that is more precise than `x` (day).

# group: can't group a subsecond precision `x` at another subsecond precision

    Can't group a subsecond precision `x` (nanosecond) by another subsecond precision (microsecond).

# narrow: `precision` is validated

    `precision` not recognized.

# narrow: `precision` must be calendar specific

    `precision` must be a valid precision for a 'year_month_day'.

# narrow: `precision` can't be wider than `x`

    Can't narrow to a precision (second) that is wider than `x` (day).

# narrow: can't narrow a subsecond precision `x` to another subsecond precision

    Can't narrow a subsecond precision `x` (nanosecond) to another subsecond precision (microsecond).

# widen: `precision` is validated

    `precision` not recognized.

# widen: `precision` must be calendar specific

    `precision` must be a valid precision for a 'year_month_day'.

# widen: `precision` can't be narrower than `x`

    Can't widen to a precision (month) that is narrower than `x` (day).

# widen: can't widen a subsecond precision `x` to another subsecond precision

    Can't widen a subsecond precision `x` (millisecond) to another subsecond precision (microsecond).

# start: `x` is validated

    no applicable method for 'calendar_start' applied to an object of class "c('double', 'numeric')"

# start: `precision` is validated

    `precision` not recognized.

---

    `precision` must be a string.

# start: errors on unsupported precision

    `precision` must be a valid precision for a 'year_month_day'.

# start: `precision` can't be more precise than `x`

    Can't compute the start of `x` (year) at a more precise precision (month).

# start: can't mix different subsecond precisions

    Can't compute the start of a subsecond precision `x` (microsecond) at another subsecond precision (millisecond).

# end: `x` is validated

    no applicable method for 'calendar_end' applied to an object of class "c('double', 'numeric')"

# end: `precision` is validated

    `precision` not recognized.

---

    `precision` must be a string.

# end: errors on unsupported precision

    `precision` must be a valid precision for a 'year_month_day'.

# end: `precision` can't be more precise than `x`

    Can't compute the end of `x` (year) at a more precise precision (month).

# end: can't mix different subsecond precisions

    Can't compute the end of a subsecond precision `x` (microsecond) at another subsecond precision (millisecond).

# `end` must be a calendar

    Code
      (expect_error(calendar_count_between(x, 1, "year")))
    Output
      <error/rlang_error>
      `end` must be a <clock_calendar>.

# can't count with a precision finer than the calendar precision

    Code
      (expect_error(calendar_count_between(x, x, "month")))
    Output
      <error/rlang_error>
      Precision of inputs must be at least as precise as `precision`.

# `n` is validated

    Code
      (expect_error(calendar_count_between(x, x, "year", n = NA_integer_)))
    Output
      <error/rlang_error>
      `n` must be a single positive integer.
    Code
      (expect_error(calendar_count_between(x, x, "year", n = -1)))
    Output
      <error/rlang_error>
      `n` must be a single positive integer.
    Code
      (expect_error(calendar_count_between(x, x, "year", n = 1.5)))
    Output
      <error/vctrs_error_cast_lossy>
      Can't convert from `n` <double> to <integer> due to loss of precision.
      * Locations: 1
    Code
      (expect_error(calendar_count_between(x, x, "year", n = "x")))
    Output
      <error/vctrs_error_incompatible_type>
      Can't convert `n` <character> to <integer>.
    Code
      (expect_error(calendar_count_between(x, x, "year", n = c(1L, 2L))))
    Output
      <error/rlang_error>
      `n` must be a single positive integer.

# precision: can only be called on calendars

    no applicable method for 'calendar_precision' applied to an object of class "c('clock_sys_time', 'clock_time_point', 'clock_rcrd', 'vctrs_rcrd', 'vctrs_vctr')"

