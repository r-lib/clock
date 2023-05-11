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
      Error in `print()`:
      ! `max` must be a whole number larger than or equal to 0, not the number -1.

---

    Code
      print(x, max = c(1, 2))
    Condition
      Error in `print()`:
      ! `max` must be a whole number, not a double vector.

---

    Code
      print(x, max = NA_integer_)
    Condition
      Error in `print()`:
      ! `max` must be a whole number, not an integer `NA`.

---

    Code
      print(x, max = "foo")
    Condition
      Error in `print()`:
      ! `max` must be a whole number, not the string "foo".

# group: `precision` is validated

    Code
      calendar_group(year_month_day(2019), "foo")
    Condition
      Error in `calendar_group()`:
      ! `precision` must be one of "year", "quarter", "month", "week", "day", "hour", "minute", "second", "millisecond", "microsecond", or "nanosecond", not "foo".

# group: `precision` must be calendar specific

    Code
      calendar_group(year_month_day(2019), "quarter")
    Condition
      Error in `calendar_group()`:
      ! `precision` must be a valid precision for a <year_month_day>, not "quarter".

# group: `precision` can't be wider than `x`

    Code
      calendar_group(year_month_day(2019, 1, 1), "second")
    Condition
      Error in `calendar_group()`:
      ! Can't group at a precision ("second") that is more precise than `x` ("day").

# group: can't group a subsecond precision `x` at another subsecond precision

    Code
      calendar_group(x, "microsecond")
    Condition
      Error in `calendar_group()`:
      ! Can't group a subsecond precision `x` ("nanosecond") by another subsecond precision ("microsecond").

# narrow: `precision` is validated

    Code
      calendar_narrow(year_month_day(2019), "foo")
    Condition
      Error in `calendar_narrow()`:
      ! `precision` must be one of "year", "quarter", "month", "week", "day", "hour", "minute", "second", "millisecond", "microsecond", or "nanosecond", not "foo".

# narrow: `precision` must be calendar specific

    Code
      calendar_narrow(year_month_day(2019), "quarter")
    Condition
      Error in `calendar_narrow()`:
      ! `precision` must be a valid precision for a <year_month_day>, not "quarter".

# narrow: `precision` can't be wider than `x`

    Code
      calendar_narrow(year_month_day(2019, 1, 1), "second")
    Condition
      Error in `calendar_narrow()`:
      ! Can't narrow to a precision ("second") that is wider than `x` ("day").

# narrow: can't narrow a subsecond precision `x` to another subsecond precision

    Code
      calendar_narrow(x, "microsecond")
    Condition
      Error in `calendar_narrow()`:
      ! Can't narrow a subsecond precision `x` ("nanosecond") to another subsecond precision ("microsecond").

# widen: `precision` is validated

    Code
      calendar_widen(year_month_day(2019), "foo")
    Condition
      Error in `calendar_widen()`:
      ! `precision` must be one of "year", "quarter", "month", "week", "day", "hour", "minute", "second", "millisecond", "microsecond", or "nanosecond", not "foo".

# widen: `precision` must be calendar specific

    Code
      calendar_widen(year_month_day(2019), "quarter")
    Condition
      Error in `calendar_widen()`:
      ! `precision` must be a valid precision for a <year_month_day>, not "quarter".

# widen: `precision` can't be narrower than `x`

    Code
      calendar_widen(year_month_day(2019, 1, 1), "month")
    Condition
      Error in `calendar_widen()`:
      ! Can't widen to a precision ("month") that is narrower than `x` ("day").

# widen: can't widen a subsecond precision `x` to another subsecond precision

    Code
      calendar_widen(x, "microsecond")
    Condition
      Error in `calendar_widen()`:
      ! Can't widen a subsecond precision `x` ("millisecond") to another subsecond precision ("microsecond").

# start: `x` is validated

    Code
      calendar_start(1)
    Condition
      Error in `calendar_start()`:
      ! `x` must be a <clock_calendar>, not the number 1.

# start: `precision` is validated

    Code
      calendar_start(year_month_day(2019), "foo")
    Condition
      Error in `calendar_start()`:
      ! `precision` must be one of "year", "quarter", "month", "week", "day", "hour", "minute", "second", "millisecond", "microsecond", or "nanosecond", not "foo".

---

    Code
      calendar_start(year_month_day(2019), 1)
    Condition
      Error in `calendar_start()`:
      ! `precision` must be a single string, not the number 1.

# start: errors on unsupported precision

    Code
      calendar_start(year_month_day(2019, 1), "quarter")
    Condition
      Error in `calendar_start_end_checks()`:
      ! `precision` must be a valid precision for a <year_month_day>, not "quarter".

# start: `precision` can't be more precise than `x`

    Code
      calendar_start(year_month_day(2019), "month")
    Condition
      Error in `calendar_start_end_checks()`:
      ! Can't compute the start of `x` ("year") at a more precise precision ("month").

# start: can't mix different subsecond precisions

    Code
      calendar_start(x, "millisecond")
    Condition
      Error in `calendar_start_end_checks()`:
      ! Can't compute the start of a subsecond precision `x` ("microsecond") at another subsecond precision ("millisecond").

# end: `x` is validated

    Code
      calendar_end(1)
    Condition
      Error in `calendar_end()`:
      ! `x` must be a <clock_calendar>, not the number 1.

# end: `precision` is validated

    Code
      calendar_end(year_month_day(2019), "foo")
    Condition
      Error in `calendar_end()`:
      ! `precision` must be one of "year", "quarter", "month", "week", "day", "hour", "minute", "second", "millisecond", "microsecond", or "nanosecond", not "foo".

---

    Code
      calendar_end(year_month_day(2019), 1)
    Condition
      Error in `calendar_end()`:
      ! `precision` must be a single string, not the number 1.

# end: errors on unsupported precision

    Code
      calendar_end(year_month_day(2019, 1), "quarter")
    Condition
      Error in `calendar_start_end_checks()`:
      ! `precision` must be a valid precision for a <year_month_day>, not "quarter".

# end: `precision` can't be more precise than `x`

    Code
      calendar_end(year_month_day(2019), "month")
    Condition
      Error in `calendar_start_end_checks()`:
      ! Can't compute the end of `x` ("year") at a more precise precision ("month").

# end: can't mix different subsecond precisions

    Code
      calendar_end(x, "millisecond")
    Condition
      Error in `calendar_start_end_checks()`:
      ! Can't compute the end of a subsecond precision `x` ("microsecond") at another subsecond precision ("millisecond").

# `end` must be a calendar

    Code
      (expect_error(calendar_count_between(x, 1, "year")))
    Output
      <error/rlang_error>
      Error in `calendar_count_between()`:
      ! `end` must be a <clock_calendar>, not the number 1.

# can't count with a precision finer than the calendar precision

    Code
      (expect_error(calendar_count_between(x, x, "month")))
    Output
      <error/rlang_error>
      Error in `calendar_count_between()`:
      ! Precision of inputs ("year") must be at least as precise as `precision` ("month").

# `n` is validated

    Code
      (expect_error(calendar_count_between(x, x, "year", n = NA_integer_)))
    Output
      <error/rlang_error>
      Error in `calendar_count_between()`:
      ! `n` must be a whole number, not an integer `NA`.
    Code
      (expect_error(calendar_count_between(x, x, "year", n = -1)))
    Output
      <error/rlang_error>
      Error in `calendar_count_between()`:
      ! `n` must be a whole number larger than or equal to 0, not the number -1.
    Code
      (expect_error(calendar_count_between(x, x, "year", n = 1.5)))
    Output
      <error/rlang_error>
      Error in `calendar_count_between()`:
      ! `n` must be a whole number, not the number 1.5.
    Code
      (expect_error(calendar_count_between(x, x, "year", n = "x")))
    Output
      <error/rlang_error>
      Error in `calendar_count_between()`:
      ! `n` must be a whole number, not the string "x".
    Code
      (expect_error(calendar_count_between(x, x, "year", n = c(1L, 2L))))
    Output
      <error/rlang_error>
      Error in `calendar_count_between()`:
      ! `n` must be a whole number, not an integer vector.

# validates the input

    Code
      calendar_spanning_seq(1)
    Condition
      Error in `calendar_spanning_seq()`:
      ! `x` must be a <clock_calendar>, not the number 1.

# the input must be at a precision allowed by `seq()`

    Code
      calendar_spanning_seq(year_month_day(2019, 1, 2))
    Condition
      Error in `seq()`:
      ! `from` must be 'year' or 'month' precision.

# errors on types that don't support min/max calls

    Code
      calendar_spanning_seq(x)
    Condition
      Error in `vec_proxy_compare()`:
      ! 'year_month_weekday' types with a precision of >= 'day' cannot be trivially compared or ordered. Convert to 'year_month_day' to compare using day-of-month values.

# precision: can only be called on calendars

    Code
      calendar_precision(sys_days(0))
    Condition
      Error in `calendar_precision()`:
      ! `x` must be a <clock_calendar>, not a <clock_sys_time> object.

# addition helpers throw error with advice

    Code
      add_weeks(x)
    Condition
      Error in `add_weeks()`:
      ! Can't perform this operation on a <clock_year_month_day>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_days(x)
    Condition
      Error in `add_days()`:
      ! Can't perform this operation on a <clock_year_month_day>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_hours(x)
    Condition
      Error in `add_hours()`:
      ! Can't perform this operation on a <clock_year_month_day>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_minutes(x)
    Condition
      Error in `add_minutes()`:
      ! Can't perform this operation on a <clock_year_month_day>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_seconds(x)
    Condition
      Error in `add_seconds()`:
      ! Can't perform this operation on a <clock_year_month_day>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_milliseconds(x)
    Condition
      Error in `add_milliseconds()`:
      ! Can't perform this operation on a <clock_year_month_day>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_microseconds(x)
    Condition
      Error in `add_microseconds()`:
      ! Can't perform this operation on a <clock_year_month_day>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

---

    Code
      add_nanoseconds(x)
    Condition
      Error in `add_nanoseconds()`:
      ! Can't perform this operation on a <clock_year_month_day>.
      i Do you need to convert to a time point first?
      i Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

