# Sequences: date-time

This is a POSIXct method for the
[`date_seq()`](https://clock.r-lib.org/dev/reference/date_seq.md)
generic.

[`date_seq()`](https://clock.r-lib.org/dev/reference/date_seq.md)
generates a date-time (POSIXct) sequence.

When calling
[`date_seq()`](https://clock.r-lib.org/dev/reference/date_seq.md),
exactly two of the following must be specified:

- `to`

- `by`

- `total_size`

## Usage

``` r
# S3 method for class 'POSIXt'
date_seq(
  from,
  ...,
  to = NULL,
  by = NULL,
  total_size = NULL,
  invalid = NULL,
  nonexistent = NULL,
  ambiguous = NULL
)
```

## Arguments

- from:

  `[POSIXct(1) / POSIXlt(1)]`

  A date-time to start the sequence from.

- ...:

  These dots are for future extensions and must be empty.

- to:

  `[POSIXct(1) / POSIXlt(1) / NULL]`

  A date-time to stop the sequence at.

  `to` is only included in the result if the resulting sequence divides
  the distance between `from` and `to` exactly.

  If `to` is supplied along with `by`, all components of `to` more
  precise than the precision of `by` must match `from` exactly. For
  example, if `by = duration_months(1)`, the day, hour, minute, and
  second components of `to` must match the corresponding components of
  `from`. This ensures that the generated sequence is, at a minimum, a
  weakly monotonic sequence of date-times.

  The time zone of `to` must match the time zone of `from` exactly.

- by:

  `[integer(1) / clock_duration(1) / NULL]`

  The unit to increment the sequence by.

  If `by` is an integer, it is equivalent to `duration_seconds(by)`.

  If `by` is a duration, it is allowed to have a precision of:

  - year

  - quarter

  - month

  - week

  - day

  - hour

  - minute

  - second

- total_size:

  `[positive integer(1) / NULL]`

  The size of the resulting sequence.

  If specified alongside `to`, this must generate a non-fractional
  sequence between `from` and `to`.

- invalid:

  `[character(1) / NULL]`

  One of the following invalid date resolution strategies:

  - `"previous"`: The previous valid instant in time.

  - `"previous-day"`: The previous valid day in time, keeping the time
    of day.

  - `"next"`: The next valid instant in time.

  - `"next-day"`: The next valid day in time, keeping the time of day.

  - `"overflow"`: Overflow by the number of days that the input is
    invalid by. Time of day is dropped.

  - `"overflow-day"`: Overflow by the number of days that the input is
    invalid by. Time of day is kept.

  - `"NA"`: Replace invalid dates with `NA`.

  - `"error"`: Error on invalid dates.

  Using either `"previous"` or `"next"` is generally recommended, as
  these two strategies maintain the *relative ordering* between elements
  of the input.

  If `NULL`, defaults to `"error"`.

  If `getOption("clock.strict")` is `TRUE`, `invalid` must be supplied
  and cannot be `NULL`. This is a convenient way to make production code
  robust to invalid dates.

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

A date-time vector.

## Sequence Generation

Different methods are used to generate the sequences, depending on the
precision implied by `by`. They are intended to generate the most
intuitive sequences, especially around daylight saving time gaps and
fallbacks.

See the examples for more details.

### Calendrical based sequences:

These convert to a naive-time, then to a year-month-day, generate the
sequence, then convert back to a date-time.

- `by = duration_years()`

- `by = duration_quarters()`

- `by = duration_months()`

### Naive-time based sequences:

These convert to a naive-time, generate the sequence, then convert back
to a date-time.

- `by = duration_weeks()`

- `by = duration_days()`

### Sys-time based sequences:

These convert to a sys-time, generate the sequence, then convert back to
a date-time.

- `by = duration_hours()`

- `by = duration_minutes()`

- `by = duration_seconds()`

## Examples

``` r
zone <- "America/New_York"

from <- date_time_build(2019, 1, zone = zone)
to <- date_time_build(2019, 1, second = 50, zone = zone)

# Defaults to second precision sequence
date_seq(from, to = to, by = 7)
#> [1] "2019-01-01 00:00:00 EST" "2019-01-01 00:00:07 EST"
#> [3] "2019-01-01 00:00:14 EST" "2019-01-01 00:00:21 EST"
#> [5] "2019-01-01 00:00:28 EST" "2019-01-01 00:00:35 EST"
#> [7] "2019-01-01 00:00:42 EST" "2019-01-01 00:00:49 EST"

to <- date_time_build(2019, 1, 5, zone = zone)

# Use durations to change to alternative precisions
date_seq(from, to = to, by = duration_days(1))
#> [1] "2019-01-01 EST" "2019-01-02 EST" "2019-01-03 EST" "2019-01-04 EST"
#> [5] "2019-01-05 EST"
date_seq(from, to = to, by = duration_hours(10))
#>  [1] "2019-01-01 00:00:00 EST" "2019-01-01 10:00:00 EST"
#>  [3] "2019-01-01 20:00:00 EST" "2019-01-02 06:00:00 EST"
#>  [5] "2019-01-02 16:00:00 EST" "2019-01-03 02:00:00 EST"
#>  [7] "2019-01-03 12:00:00 EST" "2019-01-03 22:00:00 EST"
#>  [9] "2019-01-04 08:00:00 EST" "2019-01-04 18:00:00 EST"
date_seq(from, by = duration_minutes(-2), total_size = 3)
#> [1] "2019-01-01 00:00:00 EST" "2018-12-31 23:58:00 EST"
#> [3] "2018-12-31 23:56:00 EST"

# Note that components of `to` more precise than the precision of `by`
# must match `from` exactly. For example, this is not well defined:
from <- date_time_build(2019, 1, 1, 0, 1, 30, zone = zone)
to <- date_time_build(2019, 1, 1, 5, 2, 20, zone = zone)
try(date_seq(from, to = to, by = duration_hours(1)))
#> Error in date_seq(from, to = to, by = duration_hours(1)) : 
#>   All components of `from` and `to` more precise than "hour" must
#> match.
#> ℹ `from` is "2019-01-01T05:01:30".
#> ℹ `to` is "2019-01-01T10:02:20".

# The minute and second components of `to` must match `from`
to <- date_time_build(2019, 1, 1, 5, 1, 30, zone = zone)
date_seq(from, to = to, by = duration_hours(1))
#> [1] "2019-01-01 00:01:30 EST" "2019-01-01 01:01:30 EST"
#> [3] "2019-01-01 02:01:30 EST" "2019-01-01 03:01:30 EST"
#> [5] "2019-01-01 04:01:30 EST" "2019-01-01 05:01:30 EST"

# ---------------------------------------------------------------------------

# Invalid dates must be resolved with the `invalid` argument
from <- date_time_build(2019, 1, 31, zone = zone)
to <- date_time_build(2019, 12, 31, zone = zone)

try(date_seq(from, to = to, by = duration_months(1)))
#> Error in invalid_resolve(out, invalid = invalid) : 
#>   Invalid date found at location 2.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.
date_seq(from, to = to, by = duration_months(1), invalid = "previous-day")
#>  [1] "2019-01-31 EST" "2019-02-28 EST" "2019-03-31 EDT"
#>  [4] "2019-04-30 EDT" "2019-05-31 EDT" "2019-06-30 EDT"
#>  [7] "2019-07-31 EDT" "2019-08-31 EDT" "2019-09-30 EDT"
#> [10] "2019-10-31 EDT" "2019-11-30 EST" "2019-12-31 EST"

# Compare this to the base R result, which is often a source of confusion
seq(from, to = to, by = "1 month")
#>  [1] "2019-01-31 EST" "2019-03-03 EST" "2019-03-31 EDT"
#>  [4] "2019-05-01 EDT" "2019-05-31 EDT" "2019-07-01 EDT"
#>  [7] "2019-07-31 EDT" "2019-08-31 EDT" "2019-10-01 EDT"
#> [10] "2019-10-31 EDT" "2019-12-01 EST" "2019-12-31 EST"

# This is equivalent to the overflow invalid resolution strategy
date_seq(from, to = to, by = duration_months(1), invalid = "overflow")
#>  [1] "2019-01-31 EST" "2019-03-03 EST" "2019-03-31 EDT"
#>  [4] "2019-05-01 EDT" "2019-05-31 EDT" "2019-07-01 EDT"
#>  [7] "2019-07-31 EDT" "2019-08-31 EDT" "2019-10-01 EDT"
#> [10] "2019-10-31 EDT" "2019-12-01 EST" "2019-12-31 EST"

# ---------------------------------------------------------------------------

# This date-time is 2 days before a daylight saving time gap that occurred
# on 2021-03-14 between 01:59:59 -> 03:00:00
from <- as.POSIXct("2021-03-12 02:30:00", "America/New_York")

# So creating a daily sequence lands us in that daylight saving time gap,
# creating a nonexistent time
try(date_seq(from, by = duration_days(1), total_size = 5))
#> Error in as_zoned_time(x, zone = tz, nonexistent = nonexistent, ambiguous = ambiguous) : 
#>   Nonexistent time due to daylight saving time at location 3.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.

# Resolve the nonexistent time with `nonexistent`. Note that this importantly
# allows times after the gap to retain the `02:30:00` time.
date_seq(from, by = duration_days(1), total_size = 5, nonexistent = "roll-forward")
#> [1] "2021-03-12 02:30:00 EST" "2021-03-13 02:30:00 EST"
#> [3] "2021-03-14 03:00:00 EDT" "2021-03-15 02:30:00 EDT"
#> [5] "2021-03-16 02:30:00 EDT"

# Compare this to the base R behavior, where the hour is adjusted from 2->3
# as you cross the daylight saving time gap, and is never restored. This is
# equivalent to always using sys-time (rather than naive-time, like clock
# uses for daily sequences).
seq(from, by = "1 day", length.out = 5)
#> [1] "2021-03-12 02:30:00 EST" "2021-03-13 02:30:00 EST"
#> [3] "2021-03-14 03:30:00 EDT" "2021-03-15 03:30:00 EDT"
#> [5] "2021-03-16 03:30:00 EDT"

# You can replicate this behavior by generating a second precision sequence
# of 86,400 seconds. Seconds always add in sys-time.
date_seq(from, by = duration_seconds(86400), total_size = 5)
#> [1] "2021-03-12 02:30:00 EST" "2021-03-13 02:30:00 EST"
#> [3] "2021-03-14 03:30:00 EDT" "2021-03-15 03:30:00 EDT"
#> [5] "2021-03-16 03:30:00 EDT"

# ---------------------------------------------------------------------------

# Usage of `to` and `total_size` must generate a non-fractional sequence
# between `from` and `to`
from <- date_time_build(2019, 1, 1, 0, 0, 0, zone = "America/New_York")
to <- date_time_build(2019, 1, 1, 0, 0, 3, zone = "America/New_York")

# These are fine
date_seq(from, to = to, total_size = 2)
#> [1] "2019-01-01 00:00:00 EST" "2019-01-01 00:00:03 EST"
date_seq(from, to = to, total_size = 4)
#> [1] "2019-01-01 00:00:00 EST" "2019-01-01 00:00:01 EST"
#> [3] "2019-01-01 00:00:02 EST" "2019-01-01 00:00:03 EST"

# But this is not!
try(date_seq(from, to = to, total_size = 3))
#> Error : The supplied output size does not result in a non-fractional sequence between `from` and `to`.
```
