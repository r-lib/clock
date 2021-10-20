# failure to parse throws a warning

    Code
      sys_time_parse("foo")
    Warning <clock_warning_parse_failures>
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <time_point<sys><second>[1]>
      [1] NA

# `precision` must be at least second

    Code
      sys_time_parse_RFC_3339(x, precision = "day")
    Error <rlang_error>
      `precision` must be at least 'second' precision.

# `separator` is validated

    Code
      sys_time_parse_RFC_3339(x, separator = 1)
    Error <rlang_error>
      `separator` must be a character vector.

---

    Code
      sys_time_parse_RFC_3339(x, separator = "TT")
    Error <rlang_error>
      `separator` must be one of "T", "t", or " ".

# `offset` is validated

    Code
      sys_time_parse_RFC_3339(x, offset = 1)
    Error <rlang_error>
      `offset` must be a character vector.

---

    Code
      sys_time_parse_RFC_3339(x, offset = "ZZ")
    Error <rlang_error>
      `offset` must be one of "Z", "z", "%z", or "%Ez".

# sys-time-parse-RFC-3339: empty dots are checked

    Code
      sys_time_parse_RFC_3339(x, 1)
    Error <rlib_error_dots_nonempty>
      `...` is not empty.
      
      We detected these problematic arguments:
      * `..1`
      
      These dots only exist to allow future extensions and should be empty.
      Did you misspecify an argument?

# default format is correct

    Code
      format(sys_seconds(0))
    Output
      [1] "1970-01-01T00:00:00"

# empty dots are checked

    `...` is not empty.
    
    We detected these problematic arguments:
    * `..1`
    
    These dots only exist to allow future extensions and should be empty.
    Did you misspecify an argument?

