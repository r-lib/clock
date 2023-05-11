# x must be naive

    Code
      naive_time_info(sys_days(0), "UTC")
    Condition
      Error in `naive_time_info()`:
      ! `x` must be a <clock_naive_time>, not a <clock_sys_time> object.

# zone is vectorized and recycled against x

    Code
      naive_time_info(naive_days(0:3), c("UTC", "America/New_York"))
    Condition
      Error in `naive_time_info()`:
      ! Can't recycle `x` (size 4) to match `zone` (size 2).

# cannot parse invalid dates

    Code
      naive_time_parse(x, precision = "day")
    Condition
      Warning:
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <naive_time<day>[1]>
      [1] NA

# failure to parse throws a warning

    Code
      naive_time_parse("foo")
    Condition
      Warning:
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <naive_time<second>[1]>
      [1] NA

# `naive_time_parse()` validates `locale`

    Code
      naive_time_parse("2019-01-01T00:00:00", locale = 1)
    Condition
      Error in `naive_time_parse()`:
      ! `locale` must be a <clock_locale>, not the number 1.

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

    Code
      as_zoned_time(x, zone)
    Condition
      Error in `as_zoned_time()`:
      ! Ambiguous time due to daylight saving time at location 1.
      i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can resolve ambiguous issues - zoned-time

    Code
      as_zoned_time(nt_tweaked, zone, ambiguous = zt)
    Condition
      Error in `as_zoned_time()`:
      ! Ambiguous time due to daylight saving time at location 2.
      i Resolve ambiguous time issues by specifying the `ambiguous` argument.

---

    Code
      as_zoned_time(nt_tweaked, zone, ambiguous = zt)
    Condition
      Error in `as_zoned_time()`:
      ! Ambiguous time due to daylight saving time at location 1.
      i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can resolve nonexistent issues

    Code
      as_zoned_time(x, zone)
    Condition
      Error in `as_zoned_time()`:
      ! Nonexistent time due to daylight saving time at location 1.
      i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# `ambiguous` is validated

    Code
      as_zoned_time(naive_seconds(), zone, ambiguous = 1)
    Condition
      Error in `as_zoned_time()`:
      ! `ambiguous` must be a character vector, a zoned-time, a POSIXct, or a list, not a number.

---

    Code
      as_zoned_time(naive_seconds(), zone, ambiguous = "foo")
    Condition
      Error:
      ! 'foo' is not a recognized `ambiguous` option.

---

    Code
      as_zoned_time(naive_seconds(), zone, ambiguous = c("earliest", "latest"))
    Condition
      Error in `as_zoned_time()`:
      ! `ambiguous` must have length 1 or 0.

---

    Code
      as_zoned_time(naive_seconds(), zone, ambiguous = ambiguous)
    Condition
      Error in `as_zoned_time()`:
      ! A zoned-time or POSIXct `ambiguous` must have the same zone as `zone`.

---

    Code
      as_zoned_time(naive_seconds(), zone, ambiguous = list())
    Condition
      Error in `as_zoned_time()`:
      ! A list `ambiguous` must have length 2.

---

    Code
      as_zoned_time(naive_seconds(), zone, ambiguous = list(1, 1))
    Condition
      Error in `as_zoned_time()`:
      ! The first element of a list `ambiguous` must be a zoned-time or POSIXt.

---

    Code
      as_zoned_time(naive_seconds(), zone, ambiguous = list(reference, 1))
    Condition
      Error in `as_zoned_time()`:
      ! The second element of a list `ambiguous` must be a character vector, or `NULL`.

---

    Code
      as_zoned_time(naive_seconds(), zone, ambiguous = list(reference, "foo"))
    Condition
      Error:
      ! 'foo' is not a recognized `ambiguous` option.

# `nonexistent` is validated

    Code
      as_zoned_time(naive_seconds(), zone, nonexistent = 1)
    Condition
      Error in `as_zoned_time()`:
      ! `nonexistent` must be a character vector or `NULL`, not the number 1.

---

    Code
      as_zoned_time(naive_seconds(), zone, nonexistent = "foo")
    Condition
      Error:
      ! 'foo' is not a recognized `nonexistent` option.

---

    Code
      as_zoned_time(naive_seconds(), zone, nonexistent = c("roll-forward",
        "roll-forward"))
    Condition
      Error in `as_zoned_time()`:
      ! `nonexistent` must have length 1 or 0.

# zone is validated

    Code
      as_zoned_time(naive_seconds(), "foo")
    Condition
      Error in `as_zoned_time()`:
      ! `zone` must be a valid time zone name.
      i "foo" is invalid.
      i Allowed time zone names are listed in `clock::tzdb_names()`.

---

    Code
      as_zoned_time(naive_seconds(), 1)
    Condition
      Error in `as_zoned_time()`:
      ! `zone` must be a single string, not the number 1.

---

    Code
      as_zoned_time(naive_seconds(), c("America/New_York", "EST", "EDT"))
    Condition
      Error in `as_zoned_time()`:
      ! `zone` must be a single string, not a character vector.

# strict mode can be activated - nonexistent

    Code
      as_zoned_time(naive_seconds(), zone, ambiguous = "earliest")
    Condition
      Error in `as_zoned_time()`:
      ! The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `nonexistent` must be set and cannot be left as `NULL`.

# strict mode can be activated - ambiguous

    Code
      as_zoned_time(naive_seconds(), zone, nonexistent = "roll-forward")
    Condition
      Error in `as_zoned_time()`:
      ! The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `ambiguous` must be set and cannot be left as `NULL`. Additionally, `ambiguous` cannot be set to a zoned-time or POSIXct unless it is paired with an ambiguous time resolution strategy, like: `list(<zoned-time>, 'earliest')`.

---

    Code
      as_zoned_time(naive_seconds(), zone, nonexistent = "roll-forward", ambiguous = zt)
    Condition
      Error in `as_zoned_time()`:
      ! The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `ambiguous` must be set and cannot be left as `NULL`. Additionally, `ambiguous` cannot be set to a zoned-time or POSIXct unless it is paired with an ambiguous time resolution strategy, like: `list(<zoned-time>, 'earliest')`.

---

    Code
      as_zoned_time(naive_seconds(), zone, nonexistent = "roll-forward", ambiguous = list(
        zt, NULL))
    Condition
      Error in `as_zoned_time()`:
      ! The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `ambiguous` must be set and cannot be left as `NULL`. Additionally, `ambiguous` cannot be set to a zoned-time or POSIXct unless it is paired with an ambiguous time resolution strategy, like: `list(<zoned-time>, 'earliest')`.

# empty dots are checked

    Code
      as_zoned_time(naive_seconds(), "UTC", "roll-forward")
    Condition
      Error in `as_zoned_time()`:
      ! `...` must be empty.
      x Problematic argument:
      * ..1 = "roll-forward"
      i Did you forget to name an argument?

# `vec_ptype_full()` prints correctly

    Code
      vec_ptype_full(naive_days())
    Output
      [1] "naive_time<day>"
    Code
      vec_ptype_full(naive_seconds(1:5))
    Output
      [1] "naive_time<second>"

# `vec_ptype_abbr()` prints correctly

    Code
      vec_ptype_abbr(naive_days())
    Output
      [1] "naive<day>"
    Code
      vec_ptype_abbr(naive_seconds(1:5))
    Output
      [1] "naive<second>"

