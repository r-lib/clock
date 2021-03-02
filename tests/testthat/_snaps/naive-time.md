# x must be naive

    `x` must be a naive-time.

# zone is vectorized and recycled against x

    Can't recycle `x` (size 4) to match `zone` (size 2).

# cannot parse invalid dates

    Code
      naive_time_parse(x, precision = "day")
    Warning <clock_warning_parse_failures>
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <time_point<naive><day>[1]>
      [1] NA

# failure to parse throws a warning

    Code
      naive_time_parse("foo")
    Warning <clock_warning_parse_failures>
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <time_point<naive><second>[1]>
      [1] NA

# can resolve ambiguous issues - character

    Ambiguous time due to daylight saving time at location 1. Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can resolve ambiguous issues - zoned-time

    Ambiguous time due to daylight saving time at location 2. Resolve ambiguous time issues by specifying the `ambiguous` argument.

---

    Ambiguous time due to daylight saving time at location 1. Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can resolve nonexistent issues

    Nonexistent time due to daylight saving time at location 1. Resolve nonexistent time issues by specifying the `nonexistent` argument.

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

