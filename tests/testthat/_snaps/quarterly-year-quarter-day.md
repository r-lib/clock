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

    `from` must be 'year' or 'quarter' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

