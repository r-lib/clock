# full ptype is correct

    [1] "year_month_weekday<year>"

---

    [1] "year_month_weekday<day>"

---

    [1] "year_month_weekday<nanosecond>"

---

    [1] "year_month_weekday<day>[invalid=1]"

# abbreviated ptype is correct

    [1] "ymw<year>"

---

    [1] "ymw<day>"

---

    [1] "ymw<nano>"

---

    [1] "ymw<day>[i=1]"

# only granular precisions are allowed

    `from` must be 'year' or 'month' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

