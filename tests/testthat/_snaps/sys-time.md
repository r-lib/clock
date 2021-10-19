# failure to parse throws a warning

    Code
      sys_time_parse("foo")
    Warning <clock_warning_parse_failures>
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <time_point<sys><second>[1]>
      [1] NA

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

