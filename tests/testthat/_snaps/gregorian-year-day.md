# validates value ranges

    `year` must be within the range of [-32767, 32767], not 50000.

---

    `day` must be within the range of [1, 366], not 367.

---

    `hour` must be within the range of [0, 23], not 24.

---

    `minute` must be within the range of [0, 59], not 60.

---

    `second` must be within the range of [0, 59], not 60.

---

    `subsecond` must be within the range of [0, 999], not 1000.

---

    `subsecond` must be within the range of [0, 999999], not 1000000.

---

    `subsecond` must be within the range of [0, 999999999], not 1000000000.

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
      `precision` must be one of: 'year'.

# only granular precisions are allowed

    `from` must be 'year' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

# throws known classed error

    Invalid date found at location 1.
    i Resolve invalid date issues by specifying the `invalid` argument.

