# both day and index must be specified

    Code
      year_month_weekday(2020, 1, 1)
    Condition
      Error in `year_month_weekday()`:
      ! If either `day` or `index` is specified, both must be specified.

# validates value ranges

    Code
      year_month_weekday(50000)
    Condition
      Error in `year_month_weekday()`:
      ! `year` must be between [-32767, 32767].
      i Invalid results at locations: 1.

---

    Code
      year_month_weekday(2020, 13)
    Condition
      Error in `year_month_weekday()`:
      ! `month` must be between [1, 12].
      i Invalid results at locations: 1.

---

    Code
      year_month_weekday(2020, 1, 32, 1)
    Condition
      Error in `year_month_weekday()`:
      ! `day` must be between [1, 7].
      i Invalid results at locations: 1.

---

    Code
      year_month_weekday(2020, 1, 1, 6)
    Condition
      Error in `year_month_weekday()`:
      ! `index` must be between [1, 5].
      i Invalid results at locations: 1.

---

    Code
      year_month_weekday(2020, 1, 1, 1, 24)
    Condition
      Error in `year_month_weekday()`:
      ! `hour` must be between [0, 23].
      i Invalid results at locations: 1.

---

    Code
      year_month_weekday(2020, 1, 1, 1, 1, 60)
    Condition
      Error in `year_month_weekday()`:
      ! `minute` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      year_month_weekday(2020, 1, 1, 1, 1, 1, 60)
    Condition
      Error in `year_month_weekday()`:
      ! `second` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      year_month_weekday(2020, 1, 1, 1, 1, 1, 1, 1000, subsecond_precision = "millisecond")
    Condition
      Error in `year_month_weekday()`:
      ! `subsecond` must be between [0, 999].
      i Invalid results at locations: 1.

---

    Code
      year_month_weekday(2020, 1, 1, 1, 1, 1, 1, 1e+06, subsecond_precision = "microsecond")
    Condition
      Error in `year_month_weekday()`:
      ! `subsecond` must be between [0, 999999].
      i Invalid results at locations: 1.

---

    Code
      year_month_weekday(2020, 1, 1, 1, 1, 1, 1, 1e+09, subsecond_precision = "nanosecond")
    Condition
      Error in `year_month_weekday()`:
      ! `subsecond` must be between [0, 999999999].
      i Invalid results at locations: 1.

# cannot compare / sort with day precision or finer

    Code
      x > x
    Condition
      Error in `vec_proxy_compare()`:
      ! 'year_month_weekday' types with a precision of >= 'day' cannot be trivially compared or ordered. Convert to 'year_month_day' to compare using day-of-month values.

---

    Code
      vec_order(x)
    Condition
      Error in `vec_proxy_compare()`:
      ! 'year_month_weekday' types with a precision of >= 'day' cannot be trivially compared or ordered. Convert to 'year_month_day' to compare using day-of-month values.

# full ptype is correct

    [1] "year_month_weekday<year>"

---

    [1] "year_month_weekday<day>"

---

    [1] "year_month_weekday<nanosecond>"

---

    [1] "year_month_weekday<day>"

# abbreviated ptype is correct

    [1] "ymw<year>"

---

    [1] "ymw<day>"

---

    [1] "ymw<nano>"

---

    [1] "ymw<day>"

# setters recycling works both ways

    Code
      x <- year_month_weekday(1:2)
      y <- 1:3
      set_month(x, y)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! Can't recycle `x` (size 2) to match `value` (size 3).

# setters require integer `value`

    Code
      set_year(x, 1.5)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_month(x, 1.5)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_day(x, 1.5)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_index(x, 1.5)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_hour(x, 1.5)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_minute(x, 1.5)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_second(x, 1.5)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_millisecond(x, 1.5)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_microsecond(x, 1.5)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_nanosecond(x, 1.5)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

# setters check `value` range

    Code
      set_year(x, 1e+05)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! `value` must be between [-32767, 32767].
      i Invalid results at locations: 1.

---

    Code
      set_month(x, 13)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! `value` must be between [1, 12].
      i Invalid results at locations: 1.

---

    Code
      set_day(x, 8)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! `value` must be between [1, 7].
      i Invalid results at locations: 1.

---

    Code
      set_index(x, 6)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! `value` must be between [1, 5].
      i Invalid results at locations: 1.

---

    Code
      set_hour(x, 24)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! `value` must be between [0, 23].
      i Invalid results at locations: 1.

---

    Code
      set_minute(x, 60)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! `value` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      set_second(x, 60)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! `value` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      set_millisecond(x, -1)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! `value` must be between [0, 999].
      i Invalid results at locations: 1.

---

    Code
      set_microsecond(x, -1)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! `value` must be between [0, 999999].
      i Invalid results at locations: 1.

---

    Code
      set_nanosecond(x, -1)
    Condition
      Error in `set_field_year_month_weekday()`:
      ! `value` must be between [0, 999999999].
      i Invalid results at locations: 1.

# setters require minimum precision

    Code
      set_day(year_month_weekday(year = 1), 1, index = 1)
    Condition
      Error in `set_day()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "month".
      i `x` has a precision of "year".

---

    Code
      set_hour(year_month_weekday(year = 1, month = 2), 1)
    Condition
      Error in `set_hour()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "day".
      i `x` has a precision of "month".

---

    Code
      set_minute(year_month_weekday(year = 1, month = 2, day = 3, index = 1), 1)
    Condition
      Error in `set_minute()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "hour".
      i `x` has a precision of "day".

---

    Code
      set_second(year_month_weekday(year = 1, month = 2, day = 3, index = 1, hour = 4),
      1)
    Condition
      Error in `set_second()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "minute".
      i `x` has a precision of "hour".

---

    Code
      set_millisecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1,
        hour = 4, minute = 5), 1)
    Condition
      Error in `set_millisecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "millisecond".
      i `x` has a precision of "minute".

---

    Code
      set_microsecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1,
        hour = 4, minute = 5), 1)
    Condition
      Error in `set_microsecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "microsecond".
      i `x` has a precision of "minute".

---

    Code
      set_nanosecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1,
        hour = 4, minute = 5), 1)
    Condition
      Error in `set_nanosecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "nanosecond".
      i `x` has a precision of "minute".

# setters require correct subsecond precision

    Code
      set_millisecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1,
        hour = 4, minute = 5, second = 6, subsecond = 7, subsecond_precision = "microsecond"),
      1)
    Condition
      Error in `set_millisecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "millisecond".
      i `x` has a precision of "microsecond".

---

    Code
      set_millisecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1,
        hour = 4, minute = 5, second = 6, subsecond = 7, subsecond_precision = "nanosecond"),
      1)
    Condition
      Error in `set_millisecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "millisecond".
      i `x` has a precision of "nanosecond".

---

    Code
      set_microsecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1,
        hour = 4, minute = 5, second = 6, subsecond = 7, subsecond_precision = "millisecond"),
      1)
    Condition
      Error in `set_microsecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "microsecond".
      i `x` has a precision of "millisecond".

---

    Code
      set_microsecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1,
        hour = 4, minute = 5, second = 6, subsecond = 7, subsecond_precision = "nanosecond"),
      1)
    Condition
      Error in `set_microsecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "microsecond".
      i `x` has a precision of "nanosecond".

---

    Code
      set_nanosecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1,
        hour = 4, minute = 5, second = 6, subsecond = 7, subsecond_precision = "millisecond"),
      1)
    Condition
      Error in `set_nanosecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "nanosecond".
      i `x` has a precision of "millisecond".

---

    Code
      set_nanosecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1,
        hour = 4, minute = 5, second = 6, subsecond = 7, subsecond_precision = "microsecond"),
      1)
    Condition
      Error in `set_nanosecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "nanosecond".
      i `x` has a precision of "microsecond".

# default formats are correct

    Code
      format(year_month_weekday(2019))
    Output
      [1] "2019"

---

    Code
      format(year_month_weekday(2019, 1))
    Output
      [1] "2019-01"

---

    Code
      format(year_month_weekday(2019, 1, 1, 2, 1))
    Output
      [1] "2019-01-Sun[2]T01"

---

    Code
      format(year_month_weekday(2019, 1, 1, 2, 1, 2, 3, 50, subsecond_precision = "microsecond"))
    Output
      [1] "2019-01-Sun[2]T01:02:03.000050"

# can't compute start with a year_month_weekday at day precision or greater

    Code
      calendar_start(year_month_weekday(2019, 2, 2, 2), "day")
    Condition
      Error in `calendar_start()`:
      ! Computing the start of a 'year_month_weekday' with a precision equal to or more precise than 'day' is undefined.

---

    Code
      calendar_start(year_month_weekday(2019, 2, 2, 2), "month")
    Condition
      Error in `calendar_start()`:
      ! Computing the start of a 'year_month_weekday' with a precision equal to or more precise than 'day' is undefined.

# can't compute end with a year_month_weekday at day precision or greater

    Code
      calendar_end(year_month_weekday(2019, 2, 2, 2), "day")
    Condition
      Error in `calendar_end()`:
      ! Computing the end of a 'year_month_weekday' with a precision equal to or more precise than 'day' is undefined.

---

    Code
      calendar_end(year_month_weekday(2019, 2, 2, 2), "month")
    Condition
      Error in `calendar_end()`:
      ! Computing the end of a 'year_month_weekday' with a precision equal to or more precise than 'day' is undefined.

# can't compare a 'year_month_weekday' with day precision!

    Code
      (expect_error(calendar_count_between(x, x, "month")))
    Output
      <error/rlang_error>
      Error in `vec_proxy_compare()`:
      ! 'year_month_weekday' types with a precision of >= 'day' cannot be trivially compared or ordered. Convert to 'year_month_day' to compare using day-of-month values.

# only granular precisions are allowed

    Code
      seq(year_month_weekday(2019, 1, 1, 1), by = 1, length.out = 2)
    Condition
      Error in `seq.clock_year_month_day()`:
      ! `from` must be 'year' or 'month' precision.

# strict mode can be activated

    Code
      invalid_resolve(year_month_weekday(2019, 1, 1, 1))
    Condition
      Error in `strict_validate_invalid()`:
      ! The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

# throws known classed error

    Code
      invalid_resolve(year_month_weekday(2019, 1, 1, 5))
    Condition
      Error in `invalid_resolve()`:
      ! Invalid date found at location 1.
      i Resolve invalid date issues by specifying the `invalid` argument.

# minimums are right

    Code
      clock_minimum(clock_empty_year_month_weekday_year)
    Output
      <year_month_weekday<year>[1]>
      [1] "-32767"
    Code
      clock_minimum(clock_empty_year_month_weekday_month)
    Output
      <year_month_weekday<month>[1]>
      [1] "-32767-01"

---

    Code
      clock_minimum(year_month_weekday(1, 1, 1, 1))
    Condition
      Error in `clock_minimum()`:
      ! Can't compute the minimum of this <year_month_weekday>, as it is undefined.
      i The most precise allowed precision is "month".
      i `x` has precision "day".

# maximums are right

    Code
      clock_maximum(clock_empty_year_month_weekday_year)
    Output
      <year_month_weekday<year>[1]>
      [1] "32767"
    Code
      clock_maximum(clock_empty_year_month_weekday_month)
    Output
      <year_month_weekday<month>[1]>
      [1] "32767-12"

---

    Code
      clock_maximum(year_month_weekday(1, 1, 1, 1))
    Condition
      Error in `clock_maximum()`:
      ! Can't compute the maximum of this <year_month_weekday>, as it is undefined.
      i The most precise allowed precision is "month".
      i `x` has precision "day".

# min() / max() / range() aren't defined at or past day precision

    Code
      min(x)
    Condition
      Error in `vec_proxy_compare()`:
      ! 'year_month_weekday' types with a precision of >= 'day' cannot be trivially compared or ordered. Convert to 'year_month_day' to compare using day-of-month values.

---

    Code
      max(x)
    Condition
      Error in `vec_proxy_compare()`:
      ! 'year_month_weekday' types with a precision of >= 'day' cannot be trivially compared or ordered. Convert to 'year_month_day' to compare using day-of-month values.

---

    Code
      range(x)
    Condition
      Error in `vec_proxy_compare()`:
      ! 'year_month_weekday' types with a precision of >= 'day' cannot be trivially compared or ordered. Convert to 'year_month_day' to compare using day-of-month values.

---

    Code
      min(x)
    Condition
      Error in `clock_maximum()`:
      ! Can't compute the maximum of this <year_month_weekday>, as it is undefined.
      i The most precise allowed precision is "month".
      i `x` has precision "day".

---

    Code
      max(x)
    Condition
      Error in `clock_minimum()`:
      ! Can't compute the minimum of this <year_month_weekday>, as it is undefined.
      i The most precise allowed precision is "month".
      i `x` has precision "day".

---

    Code
      range(x)
    Condition
      Error in `clock_maximum()`:
      ! Can't compute the maximum of this <year_month_weekday>, as it is undefined.
      i The most precise allowed precision is "month".
      i `x` has precision "day".

# add_*() respect recycling rules

    Code
      add_years(year_month_weekday(1:2), 1:3)
    Condition
      Error in `add_years()`:
      ! Can't recycle `x` (size 2) to match `n` (size 3).

