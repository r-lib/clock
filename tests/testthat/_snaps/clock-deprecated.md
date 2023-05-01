# `date_zone()` is deprecated but works for POSIXt (#326)

    Code
      zone <- date_zone(x)
    Condition
      Warning:
      `date_zone()` was deprecated in clock 0.7.0.
      i Please use `date_time_zone()` instead.

# `date_zone()` is deprecated and errors for Date (#326)

    Code
      date_zone(x)
    Condition
      Warning:
      `date_zone()` was deprecated in clock 0.7.0.
      i Please use `date_time_zone()` instead.
      Error in `date_zone()`:
      ! Can't get the zone of a 'Date'.

# `date_set_zone()` is deprecated but works for POSIXt (#326)

    Code
      x <- date_set_zone(x, "America/Los_Angeles")
    Condition
      Warning:
      `date_set_zone()` was deprecated in clock 0.7.0.
      i Please use `date_time_set_zone()` instead.

# `date_set_zone()` is deprecated and errors for Date (#326)

    Code
      date_set_zone(x, "America/Los_Angeles")
    Condition
      Warning:
      `date_set_zone()` was deprecated in clock 0.7.0.
      i Please use `date_time_set_zone()` instead.
      Error in `date_set_zone()`:
      ! Can't set the zone of a 'Date'.

