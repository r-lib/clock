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

    Code
      print(x, max = -1)
    Condition
      Error in `max_collect()`:
      ! `max` must be a positive number.

---

    Code
      print(x, max = c(1, 2))
    Condition
      Error in `max_collect()`:
      ! `max` must be a single number, or `NULL`.

---

    Code
      print(x, max = NA_integer_)
    Condition
      Error in `max_collect()`:
      ! `max` must be a single number, or `NULL`.

---

    Code
      print(x, max = "foo")
    Condition
      Error in `max_collect()`:
      ! Can't convert `max` <character> to <integer>.

# group: `precision` is validated

    Code
      calendar_group(year_month_day(2019), "foo")
    Condition
      Error in `validate_precision_string()`:
      ! `precision` not recognized.

# group: `precision` must be calendar specific

    Code
      calendar_group(year_month_day(2019), "quarter")
    Condition
      Error in `calendar_group()`:
      ! `precision` must be a valid precision for a 'year_month_day'.

# group: `precision` can't be wider than `x`

    Code
      calendar_group(year_month_day(2019, 1, 1), "second")
    Condition
      Error in `calendar_group()`:
      ! Can't group at a precision (second) that is more precise than `x` (day).

# group: can't group a subsecond precision `x` at another subsecond precision

    Code
      calendar_group(x, "microsecond")
    Condition
      Error in `calendar_group()`:
      ! Can't group a subsecond precision `x` (nanosecond) by another subsecond precision (microsecond).

# narrow: `precision` is validated

    Code
      calendar_narrow(year_month_day(2019), "foo")
    Condition
      Error in `validate_precision_string()`:
      ! `precision` not recognized.

# narrow: `precision` must be calendar specific

    Code
      calendar_narrow(year_month_day(2019), "quarter")
    Condition
      Error in `calendar_narrow()`:
      ! `precision` must be a valid precision for a 'year_month_day'.

# narrow: `precision` can't be wider than `x`

    Code
      calendar_narrow(year_month_day(2019, 1, 1), "second")
    Condition
      Error in `calendar_narrow()`:
      ! Can't narrow to a precision (second) that is wider than `x` (day).

# narrow: can't narrow a subsecond precision `x` to another subsecond precision

    Code
      calendar_narrow(x, "microsecond")
    Condition
      Error in `calendar_narrow()`:
      ! Can't narrow a subsecond precision `x` (nanosecond) to another subsecond precision (microsecond).

# widen: `precision` is validated

    Code
      calendar_widen(year_month_day(2019), "foo")
    Condition
      Error in `validate_precision_string()`:
      ! `precision` not recognized.

# widen: `precision` must be calendar specific

    Code
      calendar_widen(year_month_day(2019), "quarter")
    Condition
      Error in `calendar_widen()`:
      ! `precision` must be a valid precision for a 'year_month_day'.

# widen: `precision` can't be narrower than `x`

    Code
      calendar_widen(year_month_day(2019, 1, 1), "month")
    Condition
      Error in `calendar_widen()`:
      ! Can't widen to a precision (month) that is narrower than `x` (day).

# widen: can't widen a subsecond precision `x` to another subsecond precision

    Code
      calendar_widen(x, "microsecond")
    Condition
      Error in `calendar_widen()`:
      ! Can't widen a subsecond precision `x` (millisecond) to another subsecond precision (microsecond).

# start: `x` is validated

    Code
      calendar_start(1)
    Condition
      Error in `UseMethod()`:
      ! no applicable method for 'calendar_start' applied to an object of class "c('double', 'numeric')"

# start: `precision` is validated

    Code
      calendar_start(year_month_day(2019), "foo")
    Condition
      Error in `validate_precision_string()`:
      ! `precision` not recognized.

---

    Code
      calendar_start(year_month_day(2019), 1)
    Condition
      Error in `validate_precision_string()`:
      ! `precision` must be a string.

# start: errors on unsupported precision

    Code
      calendar_start(year_month_day(2019, 1), "quarter")
    Condition
      Error in `calendar_start_end_checks()`:
      ! `precision` must be a valid precision for a 'year_month_day'.

# start: `precision` can't be more precise than `x`

    Code
      calendar_start(year_month_day(2019), "month")
    Condition
      Error in `calendar_start_end_checks()`:
      ! Can't compute the start of `x` (year) at a more precise precision (month).

# start: can't mix different subsecond precisions

    Code
      calendar_start(x, "millisecond")
    Condition
      Error in `calendar_start_end_checks()`:
      ! Can't compute the start of a subsecond precision `x` (microsecond) at another subsecond precision (millisecond).

# end: `x` is validated

    Code
      calendar_end(1)
    Condition
      Error in `UseMethod()`:
      ! no applicable method for 'calendar_end' applied to an object of class "c('double', 'numeric')"

# end: `precision` is validated

    Code
      calendar_end(year_month_day(2019), "foo")
    Condition
      Error in `validate_precision_string()`:
      ! `precision` not recognized.

---

    Code
      calendar_end(year_month_day(2019), 1)
    Condition
      Error in `validate_precision_string()`:
      ! `precision` must be a string.

# end: errors on unsupported precision

    Code
      calendar_end(year_month_day(2019, 1), "quarter")
    Condition
      Error in `calendar_start_end_checks()`:
      ! `precision` must be a valid precision for a 'year_month_day'.

# end: `precision` can't be more precise than `x`

    Code
      calendar_end(year_month_day(2019), "month")
    Condition
      Error in `calendar_start_end_checks()`:
      ! Can't compute the end of `x` (year) at a more precise precision (month).

# end: can't mix different subsecond precisions

    Code
      calendar_end(x, "millisecond")
    Condition
      Error in `calendar_start_end_checks()`:
      ! Can't compute the end of a subsecond precision `x` (microsecond) at another subsecond precision (millisecond).

# `end` must be a calendar

    Code
      (expect_error(calendar_count_between(x, 1, "year")))
    Output
      <error/rlang_error>
      Error in `calendar_count_between()`:
      ! `end` must be a <clock_calendar>.

# can't count with a precision finer than the calendar precision

    Code
      (expect_error(calendar_count_between(x, x, "month")))
    Output
      <error/rlang_error>
      Error in `calendar_count_between()`:
      ! Precision of inputs must be at least as precise as `precision`.

# `n` is validated

    Code
      (expect_error(calendar_count_between(x, x, "year", n = NA_integer_)))
    Output
      <error/rlang_error>
      Error in `calendar_count_between()`:
      ! `n` must be a single positive integer.
    Code
      (expect_error(calendar_count_between(x, x, "year", n = -1)))
    Output
      <error/rlang_error>
      Error in `calendar_count_between()`:
      ! `n` must be a single positive integer.
    Code
      (expect_error(calendar_count_between(x, x, "year", n = 1.5)))
    Output
      <error/vctrs_error_cast_lossy>
      Error in `calendar_count_between()`:
      ! Can't convert from `n` <double> to <integer> due to loss of precision.
      * Locations: 1
    Code
      (expect_error(calendar_count_between(x, x, "year", n = "x")))
    Output
      <error/vctrs_error_cast>
      Error in `calendar_count_between()`:
      ! Can't convert `n` <character> to <integer>.
    Code
      (expect_error(calendar_count_between(x, x, "year", n = c(1L, 2L))))
    Output
      <error/rlang_error>
      Error in `calendar_count_between()`:
      ! `n` must be a single positive integer.

# precision: can only be called on calendars

    Code
      calendar_precision(sys_days(0))
    Condition
      Error in `UseMethod()`:
      ! no applicable method for 'calendar_precision' applied to an object of class "c('clock_sys_time', 'clock_time_point', 'clock_rcrd', 'vctrs_rcrd', 'vctrs_vctr')"

