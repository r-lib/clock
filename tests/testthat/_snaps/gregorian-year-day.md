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

# only granular precisions are allowed

    `from` must be 'year' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

# throws known classed error

    Invalid date found at location 1. Resolve invalid date issues by specifying the `invalid` argument.

