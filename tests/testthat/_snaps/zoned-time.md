# cannot parse nonexistent time

    Code
      zoned_parse(x)
    Warning <clock_warning_parse_failures>
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <zoned_time<second><America/New_York>[1]>
      [1] NA

# offset must align with unique offset

    Code
      zoned_parse(x)
    Warning <clock_warning_parse_failures>
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      <zoned_time<second><America/New_York>[1]>
      [1] NA

# offset must align with one of two possible ambiguous offsets

    Code
      zoned_parse(x)
    Warning <clock_warning_parse_failures>
      Failed to parse 2 strings, beginning at location 1. Returning `NA` at the locations where there were parse failures.
    Output
      <zoned_time<second><America/New_York>[2]>
      [1] NA NA

# cannot have differing zone names

    All elements of `x` must have the same time zone name. Found different zone names of: 'America/New_York' and 'America/Los_Angeles'.

# zone name must be valid

    `%Z` must be used, and must result in a valid time zone name, not 'America/New_Yor'.

