# validates value ranges

    Code
      year_quarter_day(50000)
    Condition
      Error in `year_quarter_day()`:
      ! `year` must be between [-32767, 32767].
      i Invalid results at locations: 1.

---

    Code
      year_quarter_day(2020, 5)
    Condition
      Error in `year_quarter_day()`:
      ! `quarter` must be between [1, 4].
      i Invalid results at locations: 1.

---

    Code
      year_quarter_day(2020, 1, 93)
    Condition
      Error in `year_quarter_day()`:
      ! `day` must be between [1, 92].
      i Invalid results at locations: 1.

---

    Code
      year_quarter_day(2020, 1, 1, 24)
    Condition
      Error in `year_quarter_day()`:
      ! `hour` must be between [0, 23].
      i Invalid results at locations: 1.

---

    Code
      year_quarter_day(2020, 1, 1, 1, 60)
    Condition
      Error in `year_quarter_day()`:
      ! `minute` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      year_quarter_day(2020, 1, 1, 1, 1, 60)
    Condition
      Error in `year_quarter_day()`:
      ! `second` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      year_quarter_day(2020, 1, 1, 1, 1, 1, 1000, subsecond_precision = "millisecond")
    Condition
      Error in `year_quarter_day()`:
      ! `subsecond` must be between [0, 999].
      i Invalid results at locations: 1.

---

    Code
      year_quarter_day(2020, 1, 1, 1, 1, 1, 1e+06, subsecond_precision = "microsecond")
    Condition
      Error in `year_quarter_day()`:
      ! `subsecond` must be between [0, 999999].
      i Invalid results at locations: 1.

---

    Code
      year_quarter_day(2020, 1, 1, 1, 1, 1, 1e+09, subsecond_precision = "nanosecond")
    Condition
      Error in `year_quarter_day()`:
      ! `subsecond` must be between [0, 999999999].
      i Invalid results at locations: 1.

# full ptype is correct

    [1] "year_quarter_day<January><year>"

---

    [1] "year_quarter_day<February><year>"

---

    [1] "year_quarter_day<January><day>"

---

    [1] "year_quarter_day<January><nanosecond>"

---

    [1] "year_quarter_day<January><day>"

# abbreviated ptype is correct

    [1] "yqd<Jan><year>"

---

    [1] "yqd<Feb><year>"

---

    [1] "yqd<Jan><day>"

---

    [1] "yqd<Jan><nano>"

---

    [1] "yqd<Jan><day>"

# setters recycling works both ways

    Code
      x <- year_quarter_day(1:2)
      y <- 1:3
      set_quarter(x, y)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! Can't recycle `x` (size 2) to match `value` (size 3).

# setters require integer `value`

    Code
      set_year(x, 1.5)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_quarter(x, 1.5)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_day(x, 1.5)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_hour(x, 1.5)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_minute(x, 1.5)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_second(x, 1.5)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_millisecond(x, 1.5)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_microsecond(x, 1.5)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      set_nanosecond(x, 1.5)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! Can't convert from `value` <double> to <integer> due to loss of precision.
      * Locations: 1

# setters check `value` range

    Code
      set_year(x, 1e+05)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! `value` must be between [-32767, 32767].
      i Invalid results at locations: 1.

---

    Code
      set_quarter(x, 5)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! `value` must be between [1, 4].
      i Invalid results at locations: 1.

---

    Code
      set_day(x, 93)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! `value` must be between [1, 92].
      i Invalid results at locations: 1.

---

    Code
      set_hour(x, 24)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! `value` must be between [0, 23].
      i Invalid results at locations: 1.

---

    Code
      set_minute(x, 60)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! `value` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      set_second(x, 60)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! `value` must be between [0, 59].
      i Invalid results at locations: 1.

---

    Code
      set_millisecond(x, -1)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! `value` must be between [0, 999].
      i Invalid results at locations: 1.

---

    Code
      set_microsecond(x, -1)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! `value` must be between [0, 999999].
      i Invalid results at locations: 1.

---

    Code
      set_nanosecond(x, -1)
    Condition
      Error in `set_field_year_quarter_day()`:
      ! `value` must be between [0, 999999999].
      i Invalid results at locations: 1.

# setters require minimum precision

    Code
      set_day(year_quarter_day(year = 1), 1)
    Condition
      Error in `set_day()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "quarter".
      i `x` has a precision of "year".

---

    Code
      set_hour(year_quarter_day(year = 1, quarter = 2), 1)
    Condition
      Error in `set_hour()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "day".
      i `x` has a precision of "quarter".

---

    Code
      set_minute(year_quarter_day(year = 1, quarter = 2, day = 3), 1)
    Condition
      Error in `set_minute()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "hour".
      i `x` has a precision of "day".

---

    Code
      set_second(year_quarter_day(year = 1, quarter = 2, day = 3, hour = 4), 1)
    Condition
      Error in `set_second()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at least "minute".
      i `x` has a precision of "hour".

---

    Code
      set_millisecond(year_quarter_day(year = 1, quarter = 2, day = 3, hour = 4,
        minute = 5), 1)
    Condition
      Error in `set_millisecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "millisecond".
      i `x` has a precision of "minute".

---

    Code
      set_microsecond(year_quarter_day(year = 1, quarter = 2, day = 3, hour = 4,
        minute = 5), 1)
    Condition
      Error in `set_microsecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "microsecond".
      i `x` has a precision of "minute".

---

    Code
      set_nanosecond(year_quarter_day(year = 1, quarter = 2, day = 3, hour = 4,
        minute = 5), 1)
    Condition
      Error in `set_nanosecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "nanosecond".
      i `x` has a precision of "minute".

# setters require correct subsecond precision

    Code
      set_millisecond(year_quarter_day(year = 1, quarter = 2, day = 3, hour = 4,
        minute = 5, second = 6, subsecond = 7, subsecond_precision = "microsecond"),
      1)
    Condition
      Error in `set_millisecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "millisecond".
      i `x` has a precision of "microsecond".

---

    Code
      set_millisecond(year_quarter_day(year = 1, quarter = 2, day = 3, hour = 4,
        minute = 5, second = 6, subsecond = 7, subsecond_precision = "nanosecond"), 1)
    Condition
      Error in `set_millisecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "millisecond".
      i `x` has a precision of "nanosecond".

---

    Code
      set_microsecond(year_quarter_day(year = 1, quarter = 2, day = 3, hour = 4,
        minute = 5, second = 6, subsecond = 7, subsecond_precision = "millisecond"),
      1)
    Condition
      Error in `set_microsecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "microsecond".
      i `x` has a precision of "millisecond".

---

    Code
      set_microsecond(year_quarter_day(year = 1, quarter = 2, day = 3, hour = 4,
        minute = 5, second = 6, subsecond = 7, subsecond_precision = "nanosecond"), 1)
    Condition
      Error in `set_microsecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "microsecond".
      i `x` has a precision of "nanosecond".

---

    Code
      set_nanosecond(year_quarter_day(year = 1, quarter = 2, day = 3, hour = 4,
        minute = 5, second = 6, subsecond = 7, subsecond_precision = "millisecond"),
      1)
    Condition
      Error in `set_nanosecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "nanosecond".
      i `x` has a precision of "millisecond".

---

    Code
      set_nanosecond(year_quarter_day(year = 1, quarter = 2, day = 3, hour = 4,
        minute = 5, second = 6, subsecond = 7, subsecond_precision = "microsecond"),
      1)
    Condition
      Error in `set_nanosecond()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be "second" or "nanosecond".
      i `x` has a precision of "microsecond".

# default formats are correct

    Code
      format(year_quarter_day(2019))
    Output
      [1] "2019"

---

    Code
      format(year_quarter_day(2019, 1))
    Output
      [1] "2019-Q1"

---

    Code
      format(year_quarter_day(2019, 1, 1, 1))
    Output
      [1] "2019-Q1-01T01"

---

    Code
      format(year_quarter_day(2019, 1, 1, 1, 2, 3, 50, subsecond_precision = "microsecond"))
    Output
      [1] "2019-Q1-01T01:02:03.000050"

# can't compute a unsupported count precision

    Code
      (expect_error(calendar_count_between(x, x, "day")))
    Output
      <error/rlang_error>
      Error in `calendar_count_between_standardize_precision_n()`:
      ! `precision` must be one of: 'year', 'quarter'.

# only granular precisions are allowed

    Code
      seq(year_quarter_day(2019, 1, 1), by = 1, length.out = 2)
    Condition
      Error in `seq()`:
      ! `from` must be 'year' or 'quarter' precision.

# strict mode can be activated

    Code
      invalid_resolve(year_quarter_day(2019, 1, 1))
    Condition
      Error in `strict_validate_invalid()`:
      ! The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

# throws known classed error

    Code
      invalid_resolve(year_quarter_day(2019, 1, 91))
    Condition
      Error in `invalid_resolve()`:
      ! Invalid date found at location 1.
      i Resolve invalid date issues by specifying the `invalid` argument.

# minimums are right

    Code
      clock_minimum(year_quarter_day(1))
    Output
      <year_quarter_day<January><year>[1]>
      [1] "-32767"
    Code
      clock_minimum(year_quarter_day(1, 1))
    Output
      <year_quarter_day<January><quarter>[1]>
      [1] "-32767-Q1"
    Code
      clock_minimum(year_quarter_day(1, 1, 1))
    Output
      <year_quarter_day<January><day>[1]>
      [1] "-32767-Q1-01"
    Code
      clock_minimum(year_quarter_day(1, 1, 1, 1))
    Output
      <year_quarter_day<January><hour>[1]>
      [1] "-32767-Q1-01T00"
    Code
      clock_minimum(year_quarter_day(1, 1, 1, 1, 1))
    Output
      <year_quarter_day<January><minute>[1]>
      [1] "-32767-Q1-01T00:00"
    Code
      clock_minimum(year_quarter_day(1, 1, 1, 1, 1, 1))
    Output
      <year_quarter_day<January><second>[1]>
      [1] "-32767-Q1-01T00:00:00"
    Code
      clock_minimum(year_quarter_day(1, 1, 1, 1, 1, 1, 1, subsecond_precision = "millisecond"))
    Output
      <year_quarter_day<January><millisecond>[1]>
      [1] "-32767-Q1-01T00:00:00.000"
    Code
      clock_minimum(year_quarter_day(1, 1, 1, 1, 1, 1, 1, subsecond_precision = "microsecond"))
    Output
      <year_quarter_day<January><microsecond>[1]>
      [1] "-32767-Q1-01T00:00:00.000000"
    Code
      clock_minimum(year_quarter_day(1, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond"))
    Output
      <year_quarter_day<January><nanosecond>[1]>
      [1] "-32767-Q1-01T00:00:00.000000000"

# maximums are right

    Code
      clock_maximum(year_quarter_day(1))
    Output
      <year_quarter_day<January><year>[1]>
      [1] "32767"
    Code
      clock_maximum(year_quarter_day(1, 1))
    Output
      <year_quarter_day<January><quarter>[1]>
      [1] "32767-Q4"
    Code
      clock_maximum(year_quarter_day(1, 1, 1))
    Output
      <year_quarter_day<January><day>[1]>
      [1] "32767-Q4-92"
    Code
      clock_maximum(year_quarter_day(1, 1, 1, 1))
    Output
      <year_quarter_day<January><hour>[1]>
      [1] "32767-Q4-92T23"
    Code
      clock_maximum(year_quarter_day(1, 1, 1, 1, 1))
    Output
      <year_quarter_day<January><minute>[1]>
      [1] "32767-Q4-92T23:59"
    Code
      clock_maximum(year_quarter_day(1, 1, 1, 1, 1, 1))
    Output
      <year_quarter_day<January><second>[1]>
      [1] "32767-Q4-92T23:59:59"
    Code
      clock_maximum(year_quarter_day(1, 1, 1, 1, 1, 1, 1, subsecond_precision = "millisecond"))
    Output
      <year_quarter_day<January><millisecond>[1]>
      [1] "32767-Q4-92T23:59:59.999"
    Code
      clock_maximum(year_quarter_day(1, 1, 1, 1, 1, 1, 1, subsecond_precision = "microsecond"))
    Output
      <year_quarter_day<January><microsecond>[1]>
      [1] "32767-Q4-92T23:59:59.999999"
    Code
      clock_maximum(year_quarter_day(1, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond"))
    Output
      <year_quarter_day<January><nanosecond>[1]>
      [1] "32767-Q4-92T23:59:59.999999999"

# minimums and maximums respect `start`

    Code
      clock_minimum(year_quarter_day(1, start = clock_months$february))
    Output
      <year_quarter_day<February><year>[1]>
      [1] "-32767"
    Code
      clock_maximum(year_quarter_day(1, start = clock_months$february))
    Output
      <year_quarter_day<February><year>[1]>
      [1] "32767"

# add_*() respect recycling rules

    Code
      add_years(year_quarter_day(1:2), 1:3)
    Condition
      Error in `add_years()`:
      ! Can't recycle `x` (size 2) to match `n` (size 3).

# errors on invalid precisions

    Code
      diff(year_quarter_day(2019, 1, 1))
    Condition
      Error in `diff()`:
      ! Can't perform this operation because of the precision of `x`.
      i The precision of `x` must be at most "quarter".
      i `x` has a precision of "day".

