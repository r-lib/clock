# both day and index must be specified

    If either `day` or `index` is specified, both must be specified.

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

    'year_month_weekday' types with a precision of >= 'day' cannot be trivially compared or ordered. Convert to 'year_month_day' to compare using day-of-month values.

---

    'year_month_weekday' types with a precision of >= 'day' cannot be trivially compared or ordered. Convert to 'year_month_day' to compare using day-of-month values.

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

    Computing the start of a 'year_month_weekday' with a precision equal to or more precise than 'day' is undefined.

---

    Computing the start of a 'year_month_weekday' with a precision equal to or more precise than 'day' is undefined.

# can't compute end with a year_month_weekday at day precision or greater

    Computing the end of a 'year_month_weekday' with a precision equal to or more precise than 'day' is undefined.

---

    Computing the end of a 'year_month_weekday' with a precision equal to or more precise than 'day' is undefined.

# can't compare a 'year_month_weekday' with day precision!

    Code
      (expect_error(calendar_count_between(x, x, "month")))
    Output
      <error/rlang_error>
      Error in `vec_proxy_compare()`:
      ! 'year_month_weekday' types with a precision of >= 'day' cannot be trivially compared or ordered. Convert to 'year_month_day' to compare using day-of-month values.

# only granular precisions are allowed

    `from` must be 'year' or 'month' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

