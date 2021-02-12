# full ptype is correct

    [1] "year_month_day<year>"

---

    [1] "year_month_day<day>"

---

    [1] "year_month_day<nanosecond>"

---

    [1] "year_month_day<day>[invalid=1]"

# abbreviated ptype is correct

    [1] "ymd<year>"

---

    [1] "ymd<day>"

---

    [1] "ymd<nano>"

---

    [1] "ymd<day>[i=1]"

# invalid dates must be resolved when converting to another calendar

    Conversion from a calendar requires that all dates are valid. Resolve invalid dates by calling `invalid_resolve()`.

# invalid dates must be resolved when converting to a sys-time

    Conversion from a calendar requires that all dates are valid. Resolve invalid dates by calling `invalid_resolve()`.

# invalid dates must be resolved when converting to a naive-time

    Conversion from a calendar requires that all dates are valid. Resolve invalid dates by calling `invalid_resolve()`.

# failure to parse results in a warning

    Code
      year_month_day_parse("foo")
    Warning <clock_warning_parse_failures>
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <year_month_day<day>[1]>
      [1] NA

# requires month precision

    `x` must have at least 'month' precision.

# `labels` is validated

    `labels` must be a 'clock_labels' object.

# `abbreviate` is validated

    `abbreviate` must be `TRUE` or `FALSE`.

---

    `abbreviate` must be `TRUE` or `FALSE`.

---

    `abbreviate` must be `TRUE` or `FALSE`.

# only granular precisions are allowed

    `from` must be 'year' or 'month' precision.

# strict mode can be activated

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.

# throws known classed error

    Invalid date found at location 1. Resolve invalid date issues by specifying the `invalid` argument.

