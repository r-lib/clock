# full ptype is correct

    [1] "year_quarter_day<January><year>"

---

    [1] "year_quarter_day<February><year>"

---

    [1] "year_quarter_day<January><day>"

---

    [1] "year_quarter_day<January><nanosecond>"

---

    [1] "year_quarter_day<January><day>[invalid=1]"

# abbreviated ptype is correct

    [1] "yqd<Jan><year>"

---

    [1] "yqd<Feb><year>"

---

    [1] "yqd<Jan><day>"

---

    [1] "yqd<Jan><nano>"

---

    [1] "yqd<Jan><day>[i=1]"

# only granular precisions are allowed

    `from` must be 'year' or 'quarter' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

