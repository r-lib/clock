# validates value ranges

    `year` must be within the range of [-32767, 32767], not 50000.

---

    `week` must be within the range of [1, 53], not 54.

---

    `day` must be within the range of [1, 7], not 8.

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

# only year precision is allowed

    `from` must be 'year' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

