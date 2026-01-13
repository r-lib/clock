# Counting: date-times

This is a POSIXct/POSIXlt method for the
[`date_count_between()`](https://clock.r-lib.org/reference/date_count_between.md)
generic.

[`date_count_between()`](https://clock.r-lib.org/reference/date_count_between.md)
counts the number of `precision` units between `start` and `end` (i.e.,
the number of years or months). This count corresponds to the *whole
number* of units, and will never return a fractional value.

This is suitable for, say, computing the whole number of years or months
between two dates, accounting for the day of the month and the time of
day.

Internally, the date-time is converted to one of the following three
clock types, and the counting is done directly on that type. The choice
of type is based on the most common interpretation of each precision,
but is ultimately a heuristic. See the examples for more information.

*Calendrical based counting:*

These precisions convert to a year-month-day calendar and count while in
that type.

- `"year"`

- `"quarter"`

- `"month"`

*Naive-time based counting:*

These precisions convert to a naive-time and count while in that type.

- `"week"`

- `"day"`

*Sys-time based counting:*

These precisions convert to a sys-time and count while in that type.

- `"hour"`

- `"minute"`

- `"second"`

## Usage

``` r
# S3 method for class 'POSIXt'
date_count_between(start, end, precision, ..., n = 1L)
```

## Arguments

- start, end:

  `[POSIXct / POSIXlt]`

  A pair of date-time vectors. These will be recycled to their common
  size.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"quarter"`

  - `"month"`

  - `"week"`

  - `"day"`

  - `"hour"`

  - `"minute"`

  - `"second"`

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

## Value

An integer representing the number of `precision` units between `start`
and `end`.

## Details

`"quarter"` is equivalent to `"month"` precision with `n` set to
`n * 3L`.

## Comparison Direction

The computed count has the property that if `start <= end`, then
`start + <count> <= end`. Similarly, if `start >= end`, then
`start + <count> >= end`. In other words, the comparison direction
between `start` and `end` will never change after adding the count to
`start`. This makes this function useful for repeated count computations
at increasingly fine precisions.

## Examples

``` r
start <- date_time_parse("2000-05-05 02:00:00", zone = "America/New_York")
end <- date_time_parse(
  c("2020-05-05 01:00:00", "2020-05-05 03:00:00"),
  zone = "America/New_York"
)

# Age in years
date_count_between(start, end, "year")
#> [1] 19 20

# Number of "whole" months between these dates. i.e.
# `2000-05-05 02:00:00 -> 2020-04-05 02:00:00` is 239 months
# `2000-05-05 02:00:00 -> 2020-05-05 02:00:00` is 240 months
# Since `2020-05-05 01:00:00` occurs before the 2nd hour,
# it gets a count of 239
date_count_between(start, end, "month")
#> [1] 239 240

# Number of seconds between
date_count_between(start, end, "second")
#> [1] 631148400 631155600

# ---------------------------------------------------------------------------
# Naive-time VS Sys-time interpretation

# The difference between whether `start` and `end` are converted to a
# naive-time vs a sys-time comes into play when dealing with daylight
# savings.

# Here are two times around a 1 hour DST gap where clocks jumped from
# 01:59:59 -> 03:00:00
x <- date_time_build(1970, 4, 26, 1, 50, 00, zone = "America/New_York")
y <- date_time_build(1970, 4, 26, 3, 00, 00, zone = "America/New_York")

# When treated like sys-times, these are considered to be 10 minutes apart,
# which is the amount of time that would have elapsed if you were watching
# a clock as it changed between these two times.
date_count_between(x, y, "minute")
#> [1] 10

# Lets add a 3rd date that is ~1 day ahead of these
z <- date_time_build(1970, 4, 27, 1, 55, 00, zone = "America/New_York")

# When treated like naive-times, `z` is considered to be at least 1 day ahead
# of `x`, because `01:55:00` is after `01:50:00`. This is probably what you
# expected.
date_count_between(x, z, "day")
#> [1] 1

# If these were interpreted like sys-times, then `z` would not be considered
# to be 1 day ahead. That would look something like this:
date_count_between(x, z, "second")
#> [1] 83100
trunc(date_count_between(x, z, "second") / 86400)
#> [1] 0

# This is because there have only been 83,100 elapsed seconds since `x`,
# which isn't a full day's worth (86,400 seconds). But we'd generally
# consider `z` to be 1 day ahead of `x` (and ignore the DST gap), so that is
# how it is implemented.

# You can override this by converting directly to sys-time, then using
# `time_point_count_between()`
x_st <- as_sys_time(x)
x_st
#> <sys_time<second>[1]>
#> [1] "1970-04-26T06:50:00"

z_st <- as_sys_time(z)
z_st
#> <sys_time<second>[1]>
#> [1] "1970-04-27T05:55:00"

time_point_count_between(x_st, z_st, "day")
#> [1] 0
```
