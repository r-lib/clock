# validates value ranges

    `year` must be within the range of [-32767, 32767], not 50000.

---

    `quarter` must be within the range of [1, 4], not 5.

---

    `day` must be within the range of [1, 92], not 93.

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
      `precision` must be one of: 'year', 'quarter'.

# only granular precisions are allowed

    `from` must be 'year' or 'quarter' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

