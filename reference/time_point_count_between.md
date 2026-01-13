# Counting: time point

`time_point_count_between()` counts the number of `precision` units
between `start` and `end` (i.e., the number of days or hours). This
count corresponds to the *whole number* of units, and will never return
a fractional value.

This is suitable for, say, computing the whole number of days between
two time points, accounting for the time of day.

## Usage

``` r
time_point_count_between(start, end, precision, ..., n = 1L)
```

## Arguments

- start, end:

  `[clock_time_point]`

  A pair of time points. These will be recycled to their common size.

- precision:

  `[character(1)]`

  One of:

  - `"week"`

  - `"day"`

  - `"hour"`

  - `"minute"`

  - `"second"`

  - `"millisecond"`

  - `"microsecond"`

  - `"nanosecond"`

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

## Value

An integer representing the number of `precision` units between `start`
and `end`.

## Details

Remember that `time_point_count_between()` returns an integer vector.
With extremely fine precisions, such as nanoseconds, the count can
quickly exceed the maximum value that is allowed in an integer. In this
case, an `NA` will be returned with a warning.

## Comparison Direction

The computed count has the property that if `start <= end`, then
`start + <count> <= end`. Similarly, if `start >= end`, then
`start + <count> >= end`. In other words, the comparison direction
between `start` and `end` will never change after adding the count to
`start`. This makes this function useful for repeated count computations
at increasingly fine precisions.

## Examples

``` r
x <- as_naive_time(year_month_day(2019, 2, 3))
y <- as_naive_time(year_month_day(2019, 2, 10))

# Whole number of days or hours between two time points
time_point_count_between(x, y, "day")
#> [1] 7
time_point_count_between(x, y, "hour")
#> [1] 168

# Whole number of 2-day units
time_point_count_between(x, y, "day", n = 2)
#> [1] 3

# Leap years are taken into account
x <- as_naive_time(year_month_day(c(2020, 2021), 2, 28))
y <- as_naive_time(year_month_day(c(2020, 2021), 3, 01))
time_point_count_between(x, y, "day")
#> [1] 2 1

# Time of day is taken into account.
# `2020-02-02T04 -> 2020-02-03T03` is not a whole day (because of the hour)
# `2020-02-02T04 -> 2020-02-03T05` is a whole day
x <- as_naive_time(year_month_day(2020, 2, 2, 4))
y <- as_naive_time(year_month_day(2020, 2, 3, c(3, 5)))
time_point_count_between(x, y, "day")
#> [1] 0 1
time_point_count_between(x, y, "hour")
#> [1] 23 25

# Can compute negative counts (using the same example from above)
time_point_count_between(y, x, "day")
#> [1]  0 -1
time_point_count_between(y, x, "hour")
#> [1] -23 -25

# Repeated computation at increasingly fine precisions
x <- as_naive_time(year_month_day(
  2020, 2, 2, 4, 5, 6, 200,
  subsecond_precision = "microsecond"
))
y <- as_naive_time(year_month_day(
  2020, 3, 1, 8, 9, 10, 100,
  subsecond_precision = "microsecond"
))

days <- time_point_count_between(x, y, "day")
x <- x + duration_days(days)

hours <- time_point_count_between(x, y, "hour")
x <- x + duration_hours(hours)

minutes <- time_point_count_between(x, y, "minute")
x <- x + duration_minutes(minutes)

seconds <- time_point_count_between(x, y, "second")
x <- x + duration_seconds(seconds)

microseconds <- time_point_count_between(x, y, "microsecond")
x <- x + duration_microseconds(microseconds)

data.frame(
  days = days,
  hours = hours,
  minutes = minutes,
  seconds = seconds,
  microseconds = microseconds
)
#>   days hours minutes seconds microseconds
#> 1   28     4       4       3       999900
```
