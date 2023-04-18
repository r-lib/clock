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

# invalid dates must be resolved when converting to another calendar

    Conversion from a calendar requires that all dates are valid. Resolve invalid dates by calling `invalid_resolve()`.

# invalid dates must be resolved when converting to a sys-time

    Conversion from a calendar requires that all dates are valid. Resolve invalid dates by calling `invalid_resolve()`.

# invalid dates must be resolved when converting to a naive-time

    Conversion from a calendar requires that all dates are valid. Resolve invalid dates by calling `invalid_resolve()`.

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

    `x` must have at least 'month' precision.

# `labels` is validated

    `labels` must be a 'clock_labels' object.

# `abbreviate` is validated

    `abbreviate` must be `TRUE` or `FALSE`.

---

    `abbreviate` must be `TRUE` or `FALSE`.

---

    `abbreviate` must be `TRUE` or `FALSE`.

# can't compute a unsupported count precision

    Code
      (expect_error(calendar_count_between(x, x, "day")))
    Output
      <error/rlang_error>
      Error in `calendar_count_between_standardize_precision_n()`:
      ! `precision` must be one of: 'year', 'quarter', 'month'.

# only granular precisions are allowed

    `from` must be 'year' or 'month' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

# throws known classed error

    Invalid date found at location 1.
    i Resolve invalid date issues by specifying the `invalid` argument.

