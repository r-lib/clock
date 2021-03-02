# failure to parse throws a warning

    Code
      sys_time_parse("foo")
    Warning <clock_warning_parse_failures>
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <time_point<sys><second>[1]>
      [1] NA

