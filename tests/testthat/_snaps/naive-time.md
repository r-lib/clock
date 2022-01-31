# x must be naive

    `x` must be a naive-time.

# zone is vectorized and recycled against x

    Can't recycle `x` (size 4) to match `zone` (size 2).

# cannot parse invalid dates

    Code
      naive_time_parse(x, precision = "day")
    Condition
      Warning:
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <time_point<naive><day>[1]>
      [1] NA

# failure to parse throws a warning

    Code
      naive_time_parse("foo")
    Condition
      Warning:
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <time_point<naive><second>[1]>
      [1] NA

# default format is correct

    Code
      format(naive_seconds(0))
    Output
      [1] "1970-01-01T00:00:00"

# `%Z` generates format warnings (#204)

    Code
      format(x, format = "%Z")
    Condition
      Warning:
      Failed to format 1 string at location 1. Returning `NA` at that location.
    Output
      [1] NA

---

    Code
      format(c(x, x), format = "%Z")
    Condition
      Warning:
      Failed to format 2 strings, beginning at location 1. Returning `NA` at the locations where there were format failures.
    Output
      [1] NA NA

# `%z` generates format warnings (#204)

    Code
      format(x, format = "%z")
    Condition
      Warning:
      Failed to format 1 string at location 1. Returning `NA` at that location.
    Output
      [1] NA

---

    Code
      format(c(x, x), format = "%z")
    Condition
      Warning:
      Failed to format 2 strings, beginning at location 1. Returning `NA` at the locations where there were format failures.
    Output
      [1] NA NA

# can resolve ambiguous issues - character

    Ambiguous time due to daylight saving time at location 1.
    i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can resolve ambiguous issues - zoned-time

    Ambiguous time due to daylight saving time at location 2.
    i Resolve ambiguous time issues by specifying the `ambiguous` argument.

---

    Ambiguous time due to daylight saving time at location 1.
    i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can resolve nonexistent issues

    Nonexistent time due to daylight saving time at location 1.
    i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# `ambiguous` is validated

    `ambiguous` must be a character vector, a zoned-time, a POSIXct, or a list.

---

    'foo' is not a recognized `ambiguous` option.

---

    `ambiguous` must have length 1, or 0.

---

    A zoned-time or POSIXct `ambiguous` must have the same zone as `zone`.

---

    A list `ambiguous` must have length 2.

---

    The first element of a list `ambiguous` must be a zoned-time or POSIXt.

---

    The second element of a list `ambiguous` must be a character vector, or `NULL`.

---

    'foo' is not a recognized `ambiguous` option.

# `nonexistent` is validated

    `nonexistent` must be a character vector, or `NULL`.

---

    'foo' is not a recognized `nonexistent` option.

---

    `nonexistent` must have length 1, or 0.

# zone is validated

    'foo' is not a known time zone.

---

    `zone` must be a single string.

---

    `zone` must be a single string.

# strict mode can be activated - nonexistent

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `nonexistent` must be set and cannot be left as `NULL`.

# strict mode can be activated - ambiguous

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `ambiguous` must be set and cannot be left as `NULL`. Additionally, `ambiguous` cannot be set to a zoned-time or POSIXct unless it is paired with an ambiguous time resolution strategy, like: `list(<zoned-time>, 'earliest')`.

---

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `ambiguous` must be set and cannot be left as `NULL`. Additionally, `ambiguous` cannot be set to a zoned-time or POSIXct unless it is paired with an ambiguous time resolution strategy, like: `list(<zoned-time>, 'earliest')`.

---

    The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `ambiguous` must be set and cannot be left as `NULL`. Additionally, `ambiguous` cannot be set to a zoned-time or POSIXct unless it is paired with an ambiguous time resolution strategy, like: `list(<zoned-time>, 'earliest')`.

# empty dots are checked

    `...` must be empty.
    x Problematic argument:
    * ..1 = "roll-forward"
    i Did you forget to name an argument?

