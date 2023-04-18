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

    `from` must be 'year' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

