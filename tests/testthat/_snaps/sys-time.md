# failure to parse throws a warning

    Code
      sys_time_parse("foo")
    Condition
      Warning:
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <sys_time<second>[1]>
      [1] NA

# `precision` must be at least second

    Code
      sys_time_parse_RFC_3339(x, precision = "day")
    Condition
      Error in `sys_time_parse_RFC_3339()`:
      ! `precision` must be at least "second" precision.

# `separator` is validated

    Code
      sys_time_parse_RFC_3339(x, separator = 1)
    Condition
      Error in `sys_time_parse_RFC_3339()`:
      ! `separator` must be a string or character vector.

---

    Code
      sys_time_parse_RFC_3339(x, separator = "TT")
    Condition
      Error in `sys_time_parse_RFC_3339()`:
      ! `separator` must be one of "T", "t", or " ", not "TT".

# `offset` is validated

    Code
      sys_time_parse_RFC_3339(x, offset = 1)
    Condition
      Error in `sys_time_parse_RFC_3339()`:
      ! `offset` must be a string or character vector.

---

    Code
      sys_time_parse_RFC_3339(x, offset = "ZZ")
    Condition
      Error in `sys_time_parse_RFC_3339()`:
      ! `offset` must be one of "Z", "z", "%z", or "%Ez", not "ZZ".
      i Did you mean "%z"?

# sys-time-parse-RFC-3339: empty dots are checked

    Code
      sys_time_parse_RFC_3339(x, 1)
    Condition
      Error in `sys_time_parse()`:
      ! `...` must be empty.
      x Problematic argument:
      * ..1 = 1
      i Did you forget to name an argument?

# default format is correct

    Code
      format(sys_seconds(0))
    Output
      [1] "1970-01-01T00:00:00"

# empty dots are checked

    Code
      as_zoned_time(sys_seconds(), "UTC", 123)
    Condition
      Error in `as_zoned_time()`:
      ! `...` must be empty.
      x Problematic argument:
      * ..1 = 123
      i Did you forget to name an argument?

# `vec_ptype_full()` prints correctly

    Code
      vec_ptype_full(sys_days())
    Output
      [1] "sys_time<day>"
    Code
      vec_ptype_full(sys_seconds(1:5))
    Output
      [1] "sys_time<second>"

# `vec_ptype_abbr()` prints correctly

    Code
      vec_ptype_abbr(sys_days())
    Output
      [1] "sys<day>"
    Code
      vec_ptype_abbr(sys_seconds(1:5))
    Output
      [1] "sys<second>"

