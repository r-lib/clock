# validates value ranges

    Code
      year_day(50000)
    Condition
      Error in `year_day()`:
      ! `year` must be between [-32767, 32767].
      i Invalid results at locations: 1.

---

    Code
      year_day(2020, 367)
    Condition
      Error in `year_day()`:
      ! `day` must be between [1, 366].
      i Invalid results at locations: 1.

---

    Code
      year_day(2020, 1, 24)
    Condition
      Error in `year_day()`:
      ! `hour` must be between [0, 23].
      i Invalid results at locations: 1.

---

    Code
      year_day(2020, 1, 1, 60)
    Condition
      Error in `year_day()`:
      ! `minute` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      year_day(2020, 1, 1, 1, 60)
    Condition
      Error in `year_day()`:
      ! `second` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      year_day(2020, 1, 1, 1, 1, 1000, subsecond_precision = "millisecond")
    Condition
      Error in `year_day()`:
      ! `subsecond` must be between [0, 999].
      i Invalid results at locations: 1.

---

    Code
      year_day(2020, 1, 1, 1, 1, 1e+06, subsecond_precision = "microsecond")
    Condition
      Error in `year_day()`:
      ! `subsecond` must be between [0, 999999].
      i Invalid results at locations: 1.

---

    Code
      year_day(2020, 1, 1, 1, 1, 1e+09, subsecond_precision = "nanosecond")
    Condition
      Error in `year_day()`:
      ! `subsecond` must be between [0, 999999999].
      i Invalid results at locations: 1.

# full ptype is correct

    [1] "year_day<year>"

---

    [1] "year_day<hour>"

---

    [1] "year_day<nanosecond>"

---

    [1] "year_day<day>"

# abbreviated ptype is correct

    [1] "yd<year>"

---

    [1] "yd<hour>"

---

    [1] "yd<nano>"

---

    [1] "yd<day>"

# setters recycling works both ways

    Code
      x <- year_day(1:2)
      y <- 1:3
      set_day(x, y)
    Condition
      Error in `set_field_year_day()`:
      ! Can't recycle `x` (size 2) to match `value` (size 3).

# setters require integer `value`

    Code
      set_year(x, 1.5)
    Condition
      Error in `set_field_year_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_day(x, 1.5)
    Condition
      Error in `set_field_year_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_hour(x, 1.5)
    Condition
      Error in `set_field_year_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_minute(x, 1.5)
    Condition
      Error in `set_field_year_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_second(x, 1.5)
    Condition
      Error in `set_field_year_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_millisecond(x, 1.5)
    Condition
      Error in `set_field_year_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_microsecond(x, 1.5)
    Condition
      Error in `set_field_year_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_nanosecond(x, 1.5)
    Condition
      Error in `set_field_year_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

# setters check `value` range

    Code
      set_year(x, 1e+05)
    Condition
      Error in `set_field_year_day()`:
      ! `value` must be between [-32767, 32767].
      i Invalid results at locations: 1.

---

    Code
      set_day(x, 367)
    Condition
      Error in `set_field_year_day()`:
      ! `value` must be between [1, 366].
      i Invalid results at locations: 1.

---

    Code
      set_hour(x, 24)
    Condition
      Error in `set_field_year_day()`:
      ! `value` must be between [0, 23].
      i Invalid results at locations: 1.

---

    Code
      set_minute(x, 60)
    Condition
      Error in `set_field_year_day()`:
      ! `value` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      set_second(x, 60)
    Condition
      Error in `set_field_year_day()`:
      ! `value` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      set_millisecond(x, -1)
    Condition
      Error in `set_field_year_day()`:
      ! `value` must be between [0, 999].
      i Invalid results at locations: 1.

---

    Code
      set_microsecond(x, -1)
    Condition
      Error in `set_field_year_day()`:
      ! `value` must be between [0, 999999].
      i Invalid results at locations: 1.

---

    Code
      set_nanosecond(x, -1)
    Condition
      Error in `set_field_year_day()`:
      ! `value` must be between [0, 999999999].
      i Invalid results at locations: 1.

# setters require minimum precision

    Code
      set_hour(year_day(year = 1), 1)
    Condition
      Error in `calendar_require_minimum_precision()`:
      ! `set_hour()` requires a minimum precision of 'day'.

---

    Code
      set_minute(year_day(year = 1, day = 1), 1)
    Condition
      Error in `calendar_require_minimum_precision()`:
      ! `set_minute()` requires a minimum precision of 'hour'.

---

    Code
      set_second(year_day(year = 1, day = 1, hour = 1), 1)
    Condition
      Error in `calendar_require_minimum_precision()`:
      ! `set_second()` requires a minimum precision of 'minute'.

---

    Code
      set_millisecond(year_day(year = 1, day = 1, hour = 1, minute = 1), 1)
    Condition
      Error in `calendar_require_any_of_precisions()`:
      ! `set_millisecond()` does not support a precision of 'minute'.

---

    Code
      set_microsecond(year_day(year = 1, day = 1, hour = 1, minute = 1), 1)
    Condition
      Error in `calendar_require_any_of_precisions()`:
      ! `set_microsecond()` does not support a precision of 'minute'.

---

    Code
      set_nanosecond(year_day(year = 1, day = 1, hour = 1, minute = 1), 1)
    Condition
      Error in `calendar_require_any_of_precisions()`:
      ! `set_nanosecond()` does not support a precision of 'minute'.

# setters require correct subsecond precision

    Code
      set_millisecond(year_day(year = 1, day = 3, hour = 4, minute = 5, second = 6,
        subsecond = 7, subsecond_precision = "microsecond"), 1)
    Condition
      Error in `calendar_require_any_of_precisions()`:
      ! `set_millisecond()` does not support a precision of 'microsecond'.

---

    Code
      set_millisecond(year_day(year = 1, day = 3, hour = 4, minute = 5, second = 6,
        subsecond = 7, subsecond_precision = "nanosecond"), 1)
    Condition
      Error in `calendar_require_any_of_precisions()`:
      ! `set_millisecond()` does not support a precision of 'nanosecond'.

---

    Code
      set_microsecond(year_day(year = 1, day = 3, hour = 4, minute = 5, second = 6,
        subsecond = 7, subsecond_precision = "millisecond"), 1)
    Condition
      Error in `calendar_require_any_of_precisions()`:
      ! `set_microsecond()` does not support a precision of 'millisecond'.

---

    Code
      set_microsecond(year_day(year = 1, day = 3, hour = 4, minute = 5, second = 6,
        subsecond = 7, subsecond_precision = "nanosecond"), 1)
    Condition
      Error in `calendar_require_any_of_precisions()`:
      ! `set_microsecond()` does not support a precision of 'nanosecond'.

---

    Code
      set_nanosecond(year_day(year = 1, day = 3, hour = 4, minute = 5, second = 6,
        subsecond = 7, subsecond_precision = "millisecond"), 1)
    Condition
      Error in `calendar_require_any_of_precisions()`:
      ! `set_nanosecond()` does not support a precision of 'millisecond'.

---

    Code
      set_nanosecond(year_day(year = 1, day = 3, hour = 4, minute = 5, second = 6,
        subsecond = 7, subsecond_precision = "microsecond"), 1)
    Condition
      Error in `calendar_require_any_of_precisions()`:
      ! `set_nanosecond()` does not support a precision of 'microsecond'.

# invalid dates must be resolved when converting to another calendar

    Conversion from a calendar requires that all dates are valid. Resolve invalid dates by calling `invalid_resolve()`.

# invalid dates must be resolved when converting to a sys-time

    Conversion from a calendar requires that all dates are valid. Resolve invalid dates by calling `invalid_resolve()`.

# invalid dates must be resolved when converting to a naive-time

    Conversion from a calendar requires that all dates are valid. Resolve invalid dates by calling `invalid_resolve()`.

# default formats are correct

    Code
      format(year_day(2019))
    Output
      [1] "2019"

---

    Code
      format(year_day(2019, 1))
    Output
      [1] "2019-001"

---

    Code
      format(year_day(2019, 1, 1))
    Output
      [1] "2019-001T01"

---

    Code
      format(year_day(2019, 1, 1, 2, 3, 50, subsecond_precision = "microsecond"))
    Output
      [1] "2019-001T01:02:03.000050"

# can't compute a unsupported difference precision

    Code
      (expect_error(calendar_count_between(x, x, "day")))
    Output
      <error/rlang_error>
      Error in `calendar_count_between_standardize_precision_n()`:
      ! `precision` must be one of: 'year'.

# only granular precisions are allowed

    `from` must be 'year' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

# throws known classed error

    Invalid date found at location 1.
    i Resolve invalid date issues by specifying the `invalid` argument.

