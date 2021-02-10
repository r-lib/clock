# failure to parse results in a warning

    Code
      year_month_day_parse("foo")
    Warning <clock_warning_parse_failures>
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <year_month_day<day>[invalid=0][1]>
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

