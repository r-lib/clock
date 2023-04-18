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

