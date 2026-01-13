# Convert to a zoned-time from a naive-time

This is a naive-time method for the
[`as_zoned_time()`](https://clock.r-lib.org/reference/as_zoned_time.md)
generic.

Converting to a zoned-time from a naive-time retains the printed time,
but changes the underlying duration, depending on the `zone` that you
choose.

Naive-times are time points with a yet-to-be-determined time zone. By
converting them to a zoned-time, all you are doing is specifying that
time zone while attempting to keep all other printed information the
same (if possible).

If you want to retain the underlying duration, try converting to a
zoned-time [from a
sys-time](https://clock.r-lib.org/reference/as-zoned-time-sys-time.md),
which is a time point interpreted as having a UTC time zone.

## Usage

``` r
# S3 method for class 'clock_naive_time'
as_zoned_time(x, zone, ..., nonexistent = NULL, ambiguous = NULL)
```

## Arguments

- x:

  `[clock_naive_time]`

  A naive-time to convert to a zoned-time.

- zone:

  `[character(1)]`

  The zone to convert to.

- ...:

  These dots are for future extensions and must be empty.

- nonexistent:

  `[character / NULL]`

  One of the following nonexistent time resolution strategies, allowed
  to be either length 1, or the same length as the input:

  - `"roll-forward"`: The next valid instant in time.

  - `"roll-backward"`: The previous valid instant in time.

  - `"shift-forward"`: Shift the nonexistent time forward by the size of
    the daylight saving time gap.

  - `"shift-backward`: Shift the nonexistent time backward by the size
    of the daylight saving time gap.

  - `"NA"`: Replace nonexistent times with `NA`.

  - `"error"`: Error on nonexistent times.

  Using either `"roll-forward"` or `"roll-backward"` is generally
  recommended over shifting, as these two strategies maintain the
  *relative ordering* between elements of the input.

  If `NULL`, defaults to `"error"`.

  If `getOption("clock.strict")` is `TRUE`, `nonexistent` must be
  supplied and cannot be `NULL`. This is a convenient way to make
  production code robust to nonexistent times.

- ambiguous:

  `[character / zoned_time / POSIXct / list(2) / NULL]`

  One of the following ambiguous time resolution strategies, allowed to
  be either length 1, or the same length as the input:

  - `"earliest"`: Of the two possible times, choose the earliest one.

  - `"latest"`: Of the two possible times, choose the latest one.

  - `"NA"`: Replace ambiguous times with `NA`.

  - `"error"`: Error on ambiguous times.

  Alternatively, `ambiguous` is allowed to be a zoned_time (or POSIXct)
  that is either length 1, or the same length as the input. If an
  ambiguous time is encountered, the zoned_time is consulted. If the
  zoned_time corresponds to a naive_time that is also ambiguous *and*
  uses the same daylight saving time transition point as the original
  ambiguous time, then the offset of the zoned_time is used to resolve
  the ambiguity. If the ambiguity cannot be resolved by consulting the
  zoned_time, then this method falls back to `NULL`.

  Finally, `ambiguous` is allowed to be a list of size 2, where the
  first element of the list is a zoned_time (as described above), and
  the second element of the list is an ambiguous time resolution
  strategy to use when the ambiguous time cannot be resolved by
  consulting the zoned_time. Specifying a zoned_time on its own is
  identical to `list(<zoned_time>, NULL)`.

  If `NULL`, defaults to `"error"`.

  If `getOption("clock.strict")` is `TRUE`, `ambiguous` must be supplied
  and cannot be `NULL`. Additionally, `ambiguous` cannot be specified as
  a zoned_time on its own, as this implies `NULL` for ambiguous times
  that the zoned_time cannot resolve. Instead, it must be specified as a
  list alongside an ambiguous time resolution strategy as described
  above. This is a convenient way to make production code robust to
  ambiguous times.

## Value

A zoned-time vector.

## Daylight Saving Time

Converting from a naive-time to a zoned-time is not always possible due
to daylight saving time issues. There are two types of these issues:

*Nonexistent* times are the result of daylight saving time "gaps". For
example, in the America/New_York time zone, there was a daylight saving
time gap 1 second after `"2020-03-08 01:59:59"`, where the clocks
changed from `01:59:59 -> 03:00:00`, completely skipping the 2 o'clock
hour. This means that if you had a naive time of
`"2020-03-08 02:30:00"`, you couldn't convert that straight into a
zoned-time with this time zone. To resolve these issues, the
`nonexistent` argument can be used to specify one of many nonexistent
time resolution strategies.

*Ambiguous* times are the result of daylight saving time "fallbacks".
For example, in the America/New_York time zone, there was a daylight
saving time fallback 1 second after `"2020-11-01 01:59:59 EDT"`, at
which point the clocks "fell backwards" by 1 hour, resulting in a
printed time of `"2020-11-01 01:00:00 EST"` (note the EDT-\>EST shift).
This resulted in two 1 o'clock hours for this day, so if you had a naive
time of `"2020-11-01 01:30:00"`, you wouldn't be able to convert that
directly into a zoned-time with this time zone, as there is no way for
clock to know which of the two ambiguous times you wanted. To resolve
these issues, the `ambiguous` argument can be used to specify one of
many ambiguous time resolution strategies.

## Examples

``` r
library(magrittr)

x <- as_naive_time(year_month_day(2019, 1, 1))

# Converting a naive-time to a zoned-time generally retains the
# printed time, while changing the underlying duration.
as_zoned_time(x, "America/New_York")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-01-01T00:00:00-05:00"
as_zoned_time(x, "America/Los_Angeles")
#> <zoned_time<second><America/Los_Angeles>[1]>
#> [1] "2019-01-01T00:00:00-08:00"

# ---------------------------------------------------------------------------
# Nonexistent time:

new_york <- "America/New_York"

# There was a daylight saving gap in the America/New_York time zone on
# 2020-03-08 01:59:59 -> 03:00:00, which means that one of these
# naive-times don't exist in that time zone. By default, attempting to
# convert it to a zoned time will result in an error.
nonexistent_time <- year_month_day(2020, 03, 08, c(02, 03), c(45, 30), 00)
nonexistent_time <- as_naive_time(nonexistent_time)
try(as_zoned_time(nonexistent_time, new_york))
#> Error in as_zoned_time(nonexistent_time, new_york) : 
#>   Nonexistent time due to daylight saving time at location 1.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.

# Resolve this by specifying a nonexistent time resolution strategy
as_zoned_time(nonexistent_time, new_york, nonexistent = "roll-forward")
#> <zoned_time<second><America/New_York>[2]>
#> [1] "2020-03-08T03:00:00-04:00" "2020-03-08T03:30:00-04:00"
as_zoned_time(nonexistent_time, new_york, nonexistent = "roll-backward")
#> <zoned_time<second><America/New_York>[2]>
#> [1] "2020-03-08T01:59:59-05:00" "2020-03-08T03:30:00-04:00"

# Note that rolling backwards will choose the last possible moment in
# time at the current precision of the input
nonexistent_nanotime <- time_point_cast(nonexistent_time, "nanosecond")
nonexistent_nanotime
#> <naive_time<nanosecond>[2]>
#> [1] "2020-03-08T02:45:00.000000000" "2020-03-08T03:30:00.000000000"
as_zoned_time(nonexistent_nanotime, new_york, nonexistent = "roll-backward")
#> <zoned_time<nanosecond><America/New_York>[2]>
#> [1] "2020-03-08T01:59:59.999999999-05:00"
#> [2] "2020-03-08T03:30:00.000000000-04:00"

# A word of caution - Shifting does not guarantee that the relative ordering
# of the input is maintained
shifted <- as_zoned_time(
  nonexistent_time,
  new_york,
  nonexistent = "shift-forward"
)
shifted
#> <zoned_time<second><America/New_York>[2]>
#> [1] "2020-03-08T03:45:00-04:00" "2020-03-08T03:30:00-04:00"

# 02:45:00 < 03:30:00
nonexistent_time[1] < nonexistent_time[2]
#> [1] TRUE
# 03:45:00 > 03:30:00 (relative ordering is lost)
shifted[1] < shifted[2]
#> [1] FALSE

# ---------------------------------------------------------------------------
# Ambiguous time:

new_york <- "America/New_York"

# There was a daylight saving time fallback in the America/New_York time
# zone on 2020-11-01 01:59:59 EDT -> 2020-11-01 01:00:00 EST, resulting
# in two 1 o'clock hours. This means that the following naive time is
# ambiguous since we don't know which of the two 1 o'clocks it belongs to.
# By default, attempting to convert it to a zoned time will result in an
# error.
ambiguous_time <- year_month_day(2020, 11, 01, 01, 30, 00)
ambiguous_time <- as_naive_time(ambiguous_time)
try(as_zoned_time(ambiguous_time, new_york))
#> Error in as_zoned_time(ambiguous_time, new_york) : 
#>   Ambiguous time due to daylight saving time at location 1.
#> ℹ Resolve ambiguous time issues by specifying the `ambiguous` argument.

# Resolve this by specifying an ambiguous time resolution strategy
earliest <- as_zoned_time(ambiguous_time, new_york, ambiguous = "earliest")
latest <- as_zoned_time(ambiguous_time, new_york, ambiguous = "latest")
na <- as_zoned_time(ambiguous_time, new_york, ambiguous = "NA")
earliest
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2020-11-01T01:30:00-04:00"
latest
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2020-11-01T01:30:00-05:00"
na
#> <zoned_time<second><America/New_York>[1]>
#> [1] NA

# Now assume that you were given the following zoned-times, i.e.,
# you didn't build them from scratch so you already know their otherwise
# ambiguous offsets
x <- c(earliest, latest)
x
#> <zoned_time<second><America/New_York>[2]>
#> [1] "2020-11-01T01:30:00-04:00" "2020-11-01T01:30:00-05:00"

# To set the seconds to 5 in both, you might try:
x_naive <- x %>%
  as_naive_time() %>%
  as_year_month_day() %>%
  set_second(5) %>%
  as_naive_time()

x_naive
#> <naive_time<second>[2]>
#> [1] "2020-11-01T01:30:05" "2020-11-01T01:30:05"

# But this fails because you've "lost" the information about which
# offsets these ambiguous times started in
try(as_zoned_time(x_naive, zoned_time_zone(x)))
#> Error in as_zoned_time(x_naive, zoned_time_zone(x)) : 
#>   Ambiguous time due to daylight saving time at location 1.
#> ℹ Resolve ambiguous time issues by specifying the `ambiguous` argument.

# To get around this, you can use that information by specifying
# `ambiguous = x`, which will use the offset from `x` to resolve the
# ambiguity in `x_naive` as long as `x` is also an ambiguous time with the
# same daylight saving time transition point as `x_naive` (i.e. here
# everything has a transition point of `"2020-11-01 01:00:00 EST"`).
as_zoned_time(x_naive, zoned_time_zone(x), ambiguous = x)
#> <zoned_time<second><America/New_York>[2]>
#> [1] "2020-11-01T01:30:05-04:00" "2020-11-01T01:30:05-05:00"

# Say you added one more time to `x` that would not be considered ambiguous
# in naive-time
x <- c(x, as_zoned_time(as_sys_time(latest) + 3600, zoned_time_zone(latest)))
x
#> <zoned_time<second><America/New_York>[3]>
#> [1] "2020-11-01T01:30:00-04:00" "2020-11-01T01:30:00-05:00"
#> [3] "2020-11-01T02:30:00-05:00"

# Imagine you want to floor this vector to a multiple of 2 hours, with
# an origin of 1am that day. You can do this by subtracting the origin,
# flooring, then adding it back
origin <- year_month_day(2019, 11, 01, 01, 00, 00) %>%
  as_naive_time() %>%
  as_duration()

x_naive <- x %>%
  as_naive_time() %>%
  add_seconds(-origin) %>%
  time_point_floor("hour", n = 2) %>%
  add_seconds(origin)

x_naive
#> <naive_time<second>[3]>
#> [1] "2020-11-01T01:00:00" "2020-11-01T01:00:00" "2020-11-01T01:00:00"

# You again have ambiguous naive-time points, so you might try using
# `ambiguous = x`. It looks like this took care of the first two problems,
# but we have an issue at location 3.
try(as_zoned_time(x_naive, zoned_time_zone(x), ambiguous = x))
#> Error in as_zoned_time(x_naive, zoned_time_zone(x), ambiguous = x) : 
#>   Ambiguous time due to daylight saving time at location 3.
#> ℹ Resolve ambiguous time issues by specifying the `ambiguous` argument.

# When we floored from 02:30:00 -> 01:00:00, we went from being
# unambiguous -> ambiguous. In clock, this is something you must handle
# explicitly, and cannot be handled by using information from `x`. You can
# handle this while still retaining the behavior for the other two
# time points that were ambiguous before and after the floor by passing a
# list containing `x` and an ambiguous time resolution strategy to use
# when information from `x` can't resolve ambiguities:
as_zoned_time(x_naive, zoned_time_zone(x), ambiguous = list(x, "latest"))
#> <zoned_time<second><America/New_York>[3]>
#> [1] "2020-11-01T01:00:00-04:00" "2020-11-01T01:00:00-05:00"
#> [3] "2020-11-01T01:00:00-05:00"
```
