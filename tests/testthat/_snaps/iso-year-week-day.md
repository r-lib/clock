# validates value ranges

    Code
      iso_year_week_day(50000)
    Condition
      Error in `iso_year_week_day()`:
      ! `year` must be between [-32767, 32767].
      i Invalid results at locations: 1.

---

    Code
      iso_year_week_day(2020, 54)
    Condition
      Error in `iso_year_week_day()`:
      ! `week` must be between [1, 53].
      i Invalid results at locations: 1.

---

    Code
      iso_year_week_day(2020, 1, 8)
    Condition
      Error in `iso_year_week_day()`:
      ! `day` must be between [1, 7].
      i Invalid results at locations: 1.

---

    Code
      iso_year_week_day(2020, 1, 1, 24)
    Condition
      Error in `iso_year_week_day()`:
      ! `hour` must be between [0, 23].
      i Invalid results at locations: 1.

---

    Code
      iso_year_week_day(2020, 1, 1, 1, 60)
    Condition
      Error in `iso_year_week_day()`:
      ! `minute` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      iso_year_week_day(2020, 1, 1, 1, 1, 60)
    Condition
      Error in `iso_year_week_day()`:
      ! `second` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      iso_year_week_day(2020, 1, 1, 1, 1, 1, 1000, subsecond_precision = "millisecond")
    Condition
      Error in `iso_year_week_day()`:
      ! `subsecond` must be between [0, 999].
      i Invalid results at locations: 1.

---

    Code
      iso_year_week_day(2020, 1, 1, 1, 1, 1, 1e+06, subsecond_precision = "microsecond")
    Condition
      Error in `iso_year_week_day()`:
      ! `subsecond` must be between [0, 999999].
      i Invalid results at locations: 1.

---

    Code
      iso_year_week_day(2020, 1, 1, 1, 1, 1, 1e+09, subsecond_precision = "nanosecond")
    Condition
      Error in `iso_year_week_day()`:
      ! `subsecond` must be between [0, 999999999].
      i Invalid results at locations: 1.

# full ptype is correct

    [1] "iso_year_week_day<year>"

---

    [1] "iso_year_week_day<day>"

---

    [1] "iso_year_week_day<nanosecond>"

---

    [1] "iso_year_week_day<week>"

# abbreviated ptype is correct

    [1] "iso_ywd<year>"

---

    [1] "iso_ywd<day>"

---

    [1] "iso_ywd<nano>"

---

    [1] "iso_ywd<week>"

# setters recycling works both ways

    Code
      x <- iso_year_week_day(1:2)
      y <- 1:3
      set_week(x, y)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! Can't recycle `x` (size 2) to match `value` (size 3).

# setters require integer `value`

    Code
      set_year(x, 1.5)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_week(x, 1.5)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_day(x, 1.5)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_hour(x, 1.5)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_minute(x, 1.5)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_second(x, 1.5)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_millisecond(x, 1.5)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_microsecond(x, 1.5)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_nanosecond(x, 1.5)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

# setters check `value` range

    Code
      set_year(x, 1e+05)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! `value` must be between [-32767, 32767].
      i Invalid results at locations: 1.

---

    Code
      set_week(x, 54)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! `value` must be between [1, 53].
      i Invalid results at locations: 1.

---

    Code
      set_day(x, 8)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! `value` must be between [1, 7].
      i Invalid results at locations: 1.

---

    Code
      set_hour(x, 24)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! `value` must be between [0, 23].
      i Invalid results at locations: 1.

---

    Code
      set_minute(x, 60)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! `value` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      set_second(x, 60)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! `value` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      set_millisecond(x, -1)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! `value` must be between [0, 999].
      i Invalid results at locations: 1.

---

    Code
      set_microsecond(x, -1)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! `value` must be between [0, 999999].
      i Invalid results at locations: 1.

---

    Code
      set_nanosecond(x, -1)
    Condition
      Error in `set_field_iso_year_week_day()`:
      ! `value` must be between [0, 999999999].
      i Invalid results at locations: 1.

# setters require minimum precision

    Code
      set_day(iso_year_week_day(year = 1), 1)
    Condition
      Error in `set_day()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "week".
      i `x` has a precision of "year".

---

    Code
      set_hour(iso_year_week_day(year = 1, week = 2), 1)
    Condition
      Error in `set_hour()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "day".
      i `x` has a precision of "week".

---

    Code
      set_minute(iso_year_week_day(year = 1, week = 2, day = 3), 1)
    Condition
      Error in `set_minute()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "hour".
      i `x` has a precision of "day".

---

    Code
      set_second(iso_year_week_day(year = 1, week = 2, day = 3, hour = 4), 1)
    Condition
      Error in `set_second()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "minute".
      i `x` has a precision of "hour".

---

    Code
      set_millisecond(iso_year_week_day(year = 1, week = 2, day = 3, hour = 4,
        minute = 5), 1)
    Condition
      Error in `set_millisecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "millisecond".
      i `x` has a precision of "minute".

---

    Code
      set_microsecond(iso_year_week_day(year = 1, week = 2, day = 3, hour = 4,
        minute = 5), 1)
    Condition
      Error in `set_microsecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "microsecond".
      i `x` has a precision of "minute".

---

    Code
      set_nanosecond(iso_year_week_day(year = 1, week = 2, day = 3, hour = 4, minute = 5),
      1)
    Condition
      Error in `set_nanosecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "nanosecond".
      i `x` has a precision of "minute".

# setters require correct subsecond precision

    Code
      set_millisecond(iso_year_week_day(year = 1, week = 2, day = 3, hour = 4,
        minute = 5, second = 6, subsecond = 7, subsecond_precision = "microsecond"),
      1)
    Condition
      Error in `set_millisecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "millisecond".
      i `x` has a precision of "microsecond".

---

    Code
      set_millisecond(iso_year_week_day(year = 1, week = 2, day = 3, hour = 4,
        minute = 5, second = 6, subsecond = 7, subsecond_precision = "nanosecond"), 1)
    Condition
      Error in `set_millisecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "millisecond".
      i `x` has a precision of "nanosecond".

---

    Code
      set_microsecond(iso_year_week_day(year = 1, week = 2, day = 3, hour = 4,
        minute = 5, second = 6, subsecond = 7, subsecond_precision = "millisecond"),
      1)
    Condition
      Error in `set_microsecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "microsecond".
      i `x` has a precision of "millisecond".

---

    Code
      set_microsecond(iso_year_week_day(year = 1, week = 2, day = 3, hour = 4,
        minute = 5, second = 6, subsecond = 7, subsecond_precision = "nanosecond"), 1)
    Condition
      Error in `set_microsecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "microsecond".
      i `x` has a precision of "nanosecond".

---

    Code
      set_nanosecond(iso_year_week_day(year = 1, week = 2, day = 3, hour = 4, minute = 5,
        second = 6, subsecond = 7, subsecond_precision = "millisecond"), 1)
    Condition
      Error in `set_nanosecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "nanosecond".
      i `x` has a precision of "millisecond".

---

    Code
      set_nanosecond(iso_year_week_day(year = 1, week = 2, day = 3, hour = 4, minute = 5,
        second = 6, subsecond = 7, subsecond_precision = "microsecond"), 1)
    Condition
      Error in `set_nanosecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "nanosecond".
      i `x` has a precision of "microsecond".

# default formats are correct

    Code
      format(iso_year_week_day(2019))
    Output
      [1] "2019"

---

    Code
      format(iso_year_week_day(2019, 1))
    Output
      [1] "2019-W01"

---

    Code
      format(iso_year_week_day(2019, 1, 1, 1))
    Output
      [1] "2019-W01-1T01"

---

    Code
      format(iso_year_week_day(2019, 1, 1, 1, 2, 3, 50, subsecond_precision = "microsecond"))
    Output
      [1] "2019-W01-1T01:02:03.000050"

# can't compute a unsupported count precision

    Code
      (expect_error(calendar_count_between(x, x, "week")))
    Output
      <error/rlang_error>
      Error in `calendar_count_between_standardize_precision_n()`:
      ! `precision` must be one of: 'year'.

# only year precision is allowed

    Code
      seq(iso_year_week_day(2019, 1), by = 1, length.out = 2)
    Condition
      Error in `seq()`:
      ! `from` must be 'year' precision.

# strict mode can be activated

    Code
      invalid_resolve(iso_year_week_day(2019, 1))
    Condition
      Error in `strict_validate_invalid()`:
      ! The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

# throws known classed error

    Code
      invalid_resolve(iso_year_week_day(2019, 53))
    Condition
      Error in `invalid_resolve()`:
      ! Invalid date found at location 1.
      i Resolve invalid date issues by specifying the `invalid` argument.

# errors on invalid precisions

    Code
      diff(iso_year_week_day(2019, 1))
    Condition
      Error in `diff()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at most "year".
      i `x` has a precision of "week".

# minimums are right

    Code
      clock_minimum(clock_empty_iso_year_week_day_year)
    Output
      <iso_year_week_day<year>[1]>
      [1] "-32767"
    Code
      clock_minimum(clock_empty_iso_year_week_day_week)
    Output
      <iso_year_week_day<week>[1]>
      [1] "-32767-W01"
    Code
      clock_minimum(clock_empty_iso_year_week_day_day)
    Output
      <iso_year_week_day<day>[1]>
      [1] "-32767-W01-1"
    Code
      clock_minimum(clock_empty_iso_year_week_day_hour)
    Output
      <iso_year_week_day<hour>[1]>
      [1] "-32767-W01-1T00"
    Code
      clock_minimum(clock_empty_iso_year_week_day_minute)
    Output
      <iso_year_week_day<minute>[1]>
      [1] "-32767-W01-1T00:00"
    Code
      clock_minimum(clock_empty_iso_year_week_day_second)
    Output
      <iso_year_week_day<second>[1]>
      [1] "-32767-W01-1T00:00:00"
    Code
      clock_minimum(clock_empty_iso_year_week_day_millisecond)
    Output
      <iso_year_week_day<millisecond>[1]>
      [1] "-32767-W01-1T00:00:00.000"
    Code
      clock_minimum(clock_empty_iso_year_week_day_microsecond)
    Output
      <iso_year_week_day<microsecond>[1]>
      [1] "-32767-W01-1T00:00:00.000000"
    Code
      clock_minimum(clock_empty_iso_year_week_day_nanosecond)
    Output
      <iso_year_week_day<nanosecond>[1]>
      [1] "-32767-W01-1T00:00:00.000000000"

# maximums are right

    Code
      clock_maximum(clock_empty_iso_year_week_day_year)
    Output
      <iso_year_week_day<year>[1]>
      [1] "32767"
    Code
      clock_maximum(clock_empty_iso_year_week_day_week)
    Output
      <iso_year_week_day<week>[1]>
      [1] "32767-W52"
    Code
      clock_maximum(clock_empty_iso_year_week_day_day)
    Output
      <iso_year_week_day<day>[1]>
      [1] "32767-W52-7"
    Code
      clock_maximum(clock_empty_iso_year_week_day_hour)
    Output
      <iso_year_week_day<hour>[1]>
      [1] "32767-W52-7T23"
    Code
      clock_maximum(clock_empty_iso_year_week_day_minute)
    Output
      <iso_year_week_day<minute>[1]>
      [1] "32767-W52-7T23:59"
    Code
      clock_maximum(clock_empty_iso_year_week_day_second)
    Output
      <iso_year_week_day<second>[1]>
      [1] "32767-W52-7T23:59:59"
    Code
      clock_maximum(clock_empty_iso_year_week_day_millisecond)
    Output
      <iso_year_week_day<millisecond>[1]>
      [1] "32767-W52-7T23:59:59.999"
    Code
      clock_maximum(clock_empty_iso_year_week_day_microsecond)
    Output
      <iso_year_week_day<microsecond>[1]>
      [1] "32767-W52-7T23:59:59.999999"
    Code
      clock_maximum(clock_empty_iso_year_week_day_nanosecond)
    Output
      <iso_year_week_day<nanosecond>[1]>
      [1] "32767-W52-7T23:59:59.999999999"

# add_*() respect recycling rules

    Code
      add_years(iso_year_week_day(1:2), 1:3)
    Condition
      Error in `add_years()`:
      ! Can't recycle `x` (size 2) to match `n` (size 3).

