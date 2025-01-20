# requires `subsecond_precision` as needed

    Code
      year_month_day(2019, 1, 1, 0, 0, 0, 1)
    Condition
      Error in `year_month_day()`:
      ! When `subsecond` is provided, `subsecond_precision` must also be specified.

# validates `subsecond_precision`

    Code
      year_month_day(2019, 1, 1, 0, 0, 0, 1, subsecond_precision = "second")
    Condition
      Error in `year_month_day()`:
      ! `subsecond_precision` must be one of "millisecond", "microsecond", or "nanosecond", not "second".
      i Did you mean "nanosecond"?

# validates value ranges

    Code
      year_month_day(50000)
    Condition
      Error in `year_month_day()`:
      ! `year` must be between [-32767, 32767].
      i Invalid results at locations: 1.

---

    Code
      year_month_day(2020, 13)
    Condition
      Error in `year_month_day()`:
      ! `month` must be between [1, 12].
      i Invalid results at locations: 1.

---

    Code
      year_month_day(2020, 1, 32)
    Condition
      Error in `year_month_day()`:
      ! `day` must be between [1, 31].
      i Invalid results at locations: 1.

---

    Code
      year_month_day(2020, 1, 1, 24)
    Condition
      Error in `year_month_day()`:
      ! `hour` must be between [0, 23].
      i Invalid results at locations: 1.

---

    Code
      year_month_day(2020, 1, 1, 1, 60)
    Condition
      Error in `year_month_day()`:
      ! `minute` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      year_month_day(2020, 1, 1, 1, 1, 60)
    Condition
      Error in `year_month_day()`:
      ! `second` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      year_month_day(2020, 1, 1, 1, 1, 1, 1000, subsecond_precision = "millisecond")
    Condition
      Error in `year_month_day()`:
      ! `subsecond` must be between [0, 999].
      i Invalid results at locations: 1.

---

    Code
      year_month_day(2020, 1, 1, 1, 1, 1, 1e+06, subsecond_precision = "microsecond")
    Condition
      Error in `year_month_day()`:
      ! `subsecond` must be between [0, 999999].
      i Invalid results at locations: 1.

---

    Code
      year_month_day(2020, 1, 1, 1, 1, 1, 1e+09, subsecond_precision = "nanosecond")
    Condition
      Error in `year_month_day()`:
      ! `subsecond` must be between [0, 999999999].
      i Invalid results at locations: 1.

# full ptype is correct

    [1] "year_month_day<year>"

---

    [1] "year_month_day<day>"

---

    [1] "year_month_day<nanosecond>"

---

    [1] "year_month_day<day>"

# abbreviated ptype is correct

    [1] "ymd<year>"

---

    [1] "ymd<day>"

---

    [1] "ymd<nano>"

---

    [1] "ymd<day>"

# subsecond precision getters require exact precisions

    Code
      get_millisecond(micro)
    Condition
      Error in `get_millisecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "millisecond".
      i `x` has a precision of "microsecond".

---

    Code
      get_microsecond(milli)
    Condition
      Error in `get_microsecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "microsecond".
      i `x` has a precision of "millisecond".

---

    Code
      get_nanosecond(micro)
    Condition
      Error in `get_nanosecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "nanosecond".
      i `x` has a precision of "microsecond".

# setters recycling works both ways

    Code
      x <- year_month_day(1:2)
      y <- 1:3
      set_month(x, y)
    Condition
      Error in `set_field_year_month_day()`:
      ! Can't recycle `x` (size 2) to match `value` (size 3).

# setters require integer `value`

    Code
      set_year(x, 1.5)
    Condition
      Error in `set_field_year_month_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_month(x, 1.5)
    Condition
      Error in `set_field_year_month_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_day(x, 1.5)
    Condition
      Error in `set_field_year_month_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_hour(x, 1.5)
    Condition
      Error in `set_field_year_month_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_minute(x, 1.5)
    Condition
      Error in `set_field_year_month_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_second(x, 1.5)
    Condition
      Error in `set_field_year_month_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_millisecond(x, 1.5)
    Condition
      Error in `set_field_year_month_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_microsecond(x, 1.5)
    Condition
      Error in `set_field_year_month_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_nanosecond(x, 1.5)
    Condition
      Error in `set_field_year_month_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

# setters check `value` range

    Code
      set_year(x, 1e+05)
    Condition
      Error in `set_field_year_month_day()`:
      ! `value` must be between [-32767, 32767].
      i Invalid results at locations: 1.

---

    Code
      set_month(x, 13)
    Condition
      Error in `set_field_year_month_day()`:
      ! `value` must be between [1, 12].
      i Invalid results at locations: 1.

---

    Code
      set_day(x, 32)
    Condition
      Error in `set_field_year_month_day()`:
      ! `value` must be between [1, 31].
      i Invalid results at locations: 1.

---

    Code
      set_hour(x, 24)
    Condition
      Error in `set_field_year_month_day()`:
      ! `value` must be between [0, 23].
      i Invalid results at locations: 1.

---

    Code
      set_minute(x, 60)
    Condition
      Error in `set_field_year_month_day()`:
      ! `value` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      set_second(x, 60)
    Condition
      Error in `set_field_year_month_day()`:
      ! `value` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      set_millisecond(x, -1)
    Condition
      Error in `set_field_year_month_day()`:
      ! `value` must be between [0, 999].
      i Invalid results at locations: 1.

---

    Code
      set_microsecond(x, -1)
    Condition
      Error in `set_field_year_month_day()`:
      ! `value` must be between [0, 999999].
      i Invalid results at locations: 1.

---

    Code
      set_nanosecond(x, -1)
    Condition
      Error in `set_field_year_month_day()`:
      ! `value` must be between [0, 999999999].
      i Invalid results at locations: 1.

# setters require minimum precision

    Code
      set_day(year_month_day(year = 1), 1)
    Condition
      Error in `set_day()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "month".
      i `x` has a precision of "year".

---

    Code
      set_hour(year_month_day(year = 1, month = 2), 1)
    Condition
      Error in `set_hour()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "day".
      i `x` has a precision of "month".

---

    Code
      set_minute(year_month_day(year = 1, month = 2, day = 3), 1)
    Condition
      Error in `set_minute()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "hour".
      i `x` has a precision of "day".

---

    Code
      set_second(year_month_day(year = 1, month = 2, day = 3, hour = 4), 1)
    Condition
      Error in `set_second()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "minute".
      i `x` has a precision of "hour".

---

    Code
      set_millisecond(year_month_day(year = 1, month = 2, day = 3, hour = 4, minute = 5),
      1)
    Condition
      Error in `set_millisecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "millisecond".
      i `x` has a precision of "minute".

---

    Code
      set_microsecond(year_month_day(year = 1, month = 2, day = 3, hour = 4, minute = 5),
      1)
    Condition
      Error in `set_microsecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "microsecond".
      i `x` has a precision of "minute".

---

    Code
      set_nanosecond(year_month_day(year = 1, month = 2, day = 3, hour = 4, minute = 5),
      1)
    Condition
      Error in `set_nanosecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "nanosecond".
      i `x` has a precision of "minute".

# setters require correct subsecond precision

    Code
      set_millisecond(year_month_day(year = 1, month = 2, day = 3, hour = 4, minute = 5,
        second = 6, subsecond = 7, subsecond_precision = "microsecond"), 1)
    Condition
      Error in `set_millisecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "millisecond".
      i `x` has a precision of "microsecond".

---

    Code
      set_millisecond(year_month_day(year = 1, month = 2, day = 3, hour = 4, minute = 5,
        second = 6, subsecond = 7, subsecond_precision = "nanosecond"), 1)
    Condition
      Error in `set_millisecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "millisecond".
      i `x` has a precision of "nanosecond".

---

    Code
      set_microsecond(year_month_day(year = 1, month = 2, day = 3, hour = 4, minute = 5,
        second = 6, subsecond = 7, subsecond_precision = "millisecond"), 1)
    Condition
      Error in `set_microsecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "microsecond".
      i `x` has a precision of "millisecond".

---

    Code
      set_microsecond(year_month_day(year = 1, month = 2, day = 3, hour = 4, minute = 5,
        second = 6, subsecond = 7, subsecond_precision = "nanosecond"), 1)
    Condition
      Error in `set_microsecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "microsecond".
      i `x` has a precision of "nanosecond".

---

    Code
      set_nanosecond(year_month_day(year = 1, month = 2, day = 3, hour = 4, minute = 5,
        second = 6, subsecond = 7, subsecond_precision = "millisecond"), 1)
    Condition
      Error in `set_nanosecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "nanosecond".
      i `x` has a precision of "millisecond".

---

    Code
      set_nanosecond(year_month_day(year = 1, month = 2, day = 3, hour = 4, minute = 5,
        second = 6, subsecond = 7, subsecond_precision = "microsecond"), 1)
    Condition
      Error in `set_nanosecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "nanosecond".
      i `x` has a precision of "microsecond".

# invalid dates must be resolved when converting to another calendar

    Code
      as_year_quarter_day(year_month_day(2019, 2, 31))
    Condition
      Error in `as_sys_time()`:
      ! Can't convert `x` to another type because some dates are invalid.
      i The following locations are invalid: 1.
      i Resolve invalid dates with `invalid_resolve()`.

# invalid dates must be resolved when converting to a sys-time

    Code
      as_sys_time(year_month_day(2019, 2, 31))
    Condition
      Error in `as_sys_time()`:
      ! Can't convert `x` to another type because some dates are invalid.
      i The following locations are invalid: 1.
      i Resolve invalid dates with `invalid_resolve()`.

# invalid dates must be resolved when converting to a naive-time

    Code
      as_naive_time(year_month_day(2019, 2, 31))
    Condition
      Error in `as_sys_time()`:
      ! Can't convert `x` to another type because some dates are invalid.
      i The following locations are invalid: 1.
      i Resolve invalid dates with `invalid_resolve()`.

# default formats are correct

    Code
      format(year_month_day(2019))
    Output
      [1] "2019"

---

    Code
      format(year_month_day(2019, 1))
    Output
      [1] "2019-01"

---

    Code
      format(year_month_day(2019, 1, 1, 1))
    Output
      [1] "2019-01-01T01"

---

    Code
      format(year_month_day(2019, 1, 1, 1, 2, 3, 50, subsecond_precision = "microsecond"))
    Output
      [1] "2019-01-01T01:02:03.000050"

# failure to parse results in a warning

    Code
      year_month_day_parse("foo")
    Condition
      Warning:
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <year_month_day<day>[1]>
      [1] NA

# requires month precision

    Code
      calendar_month_factor(year_month_day(2019))
    Condition
      Error in `calendar_month_factor()`:
      ! `x` must have at least "month" precision.

# `labels` is validated

    Code
      calendar_month_factor(year_month_day(2019, 1), labels = 1)
    Condition
      Error in `calendar_month_factor()`:
      ! `labels` must be a <clock_labels>, not the number 1.

# `abbreviate` is validated

    Code
      calendar_month_factor(year_month_day(2019, 1), abbreviate = "foo")
    Condition
      Error in `calendar_month_factor()`:
      ! `abbreviate` must be `TRUE` or `FALSE`, not the string "foo".

---

    Code
      calendar_month_factor(year_month_day(2019, 1), abbreviate = 1)
    Condition
      Error in `calendar_month_factor()`:
      ! `abbreviate` must be `TRUE` or `FALSE`, not the number 1.

---

    Code
      calendar_month_factor(year_month_day(2019, 1), abbreviate = c(TRUE, FALSE))
    Condition
      Error in `calendar_month_factor()`:
      ! `abbreviate` must be `TRUE` or `FALSE`, not a logical vector.

# can't compute a unsupported count precision

    Code
      (expect_error(calendar_count_between(x, x, "day")))
    Output
      <error/rlang_error>
      Error in `calendar_count_between_standardize_precision_n()`:
      ! `precision` must be one of: 'year', 'quarter', 'month'.

# only granular precisions are allowed

    Code
      seq(year_month_day(2019, 1, 1), by = 1, length.out = 2)
    Condition
      Error in `seq()`:
      ! `from` must be 'year' or 'month' precision.

# strict mode can be activated

    Code
      invalid_resolve(year_month_day(2019, 1, 1))
    Condition
      Error in `strict_validate_invalid()`:
      ! The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

# throws known classed error

    Code
      invalid_resolve(year_month_day(2019, 2, 31))
    Condition
      Error in `invalid_resolve()`:
      ! Invalid date found at location 1.
      i Resolve invalid date issues by specifying the `invalid` argument.

# errors on invalid precisions

    Code
      diff(year_month_day(2019, 1, 2))
    Condition
      Error in `diff()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at most "month".
      i `x` has a precision of "day".

# minimums are right

    Code
      clock_minimum(clock_empty_year_month_day_year)
    Output
      <year_month_day<year>[1]>
      [1] "-32767"
    Code
      clock_minimum(clock_empty_year_month_day_month)
    Output
      <year_month_day<month>[1]>
      [1] "-32767-01"
    Code
      clock_minimum(clock_empty_year_month_day_day)
    Output
      <year_month_day<day>[1]>
      [1] "-32767-01-01"
    Code
      clock_minimum(clock_empty_year_month_day_hour)
    Output
      <year_month_day<hour>[1]>
      [1] "-32767-01-01T00"
    Code
      clock_minimum(clock_empty_year_month_day_minute)
    Output
      <year_month_day<minute>[1]>
      [1] "-32767-01-01T00:00"
    Code
      clock_minimum(clock_empty_year_month_day_second)
    Output
      <year_month_day<second>[1]>
      [1] "-32767-01-01T00:00:00"
    Code
      clock_minimum(clock_empty_year_month_day_millisecond)
    Output
      <year_month_day<millisecond>[1]>
      [1] "-32767-01-01T00:00:00.000"
    Code
      clock_minimum(clock_empty_year_month_day_microsecond)
    Output
      <year_month_day<microsecond>[1]>
      [1] "-32767-01-01T00:00:00.000000"
    Code
      clock_minimum(clock_empty_year_month_day_nanosecond)
    Output
      <year_month_day<nanosecond>[1]>
      [1] "-32767-01-01T00:00:00.000000000"

# maximums are right

    Code
      clock_maximum(clock_empty_year_month_day_year)
    Output
      <year_month_day<year>[1]>
      [1] "32767"
    Code
      clock_maximum(clock_empty_year_month_day_month)
    Output
      <year_month_day<month>[1]>
      [1] "32767-12"
    Code
      clock_maximum(clock_empty_year_month_day_day)
    Output
      <year_month_day<day>[1]>
      [1] "32767-12-31"
    Code
      clock_maximum(clock_empty_year_month_day_hour)
    Output
      <year_month_day<hour>[1]>
      [1] "32767-12-31T23"
    Code
      clock_maximum(clock_empty_year_month_day_minute)
    Output
      <year_month_day<minute>[1]>
      [1] "32767-12-31T23:59"
    Code
      clock_maximum(clock_empty_year_month_day_second)
    Output
      <year_month_day<second>[1]>
      [1] "32767-12-31T23:59:59"
    Code
      clock_maximum(clock_empty_year_month_day_millisecond)
    Output
      <year_month_day<millisecond>[1]>
      [1] "32767-12-31T23:59:59.999"
    Code
      clock_maximum(clock_empty_year_month_day_microsecond)
    Output
      <year_month_day<microsecond>[1]>
      [1] "32767-12-31T23:59:59.999999"
    Code
      clock_maximum(clock_empty_year_month_day_nanosecond)
    Output
      <year_month_day<nanosecond>[1]>
      [1] "32767-12-31T23:59:59.999999999"

# add_*() respect recycling rules

    Code
      add_years(year_month_day(1:2), 1:3)
    Condition
      Error in `add_years()`:
      ! Can't recycle `x` (size 2) to match `n` (size 3).

