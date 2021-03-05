# both day and index must be specified

    If either `day` or `index` is specified, both must be specified.

# validates value ranges

    `year` must be within the range of [-32767, 32767], not 50000.

---

    `month` must be within the range of [1, 12], not 13.

---

    `day` must be within the range of [1, 7], not 32.

---

    `index` must be within the range of [1, 5], not 6.

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

# only granular precisions are allowed

    `from` must be 'year' or 'month' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

