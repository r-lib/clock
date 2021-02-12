# full ptype is correct

    [1] "iso_year_week_day<year>"

---

    [1] "iso_year_week_day<day>"

---

    [1] "iso_year_week_day<nanosecond>"

---

    [1] "iso_year_week_day<week>[invalid=1]"

# abbreviated ptype is correct

    [1] "iso_ywd<year>"

---

    [1] "iso_ywd<day>"

---

    [1] "iso_ywd<nano>"

---

    [1] "iso_ywd<week>[i=1]"

# only year precision is allowed

    `from` must be 'year' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

