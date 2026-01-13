# Boundaries: date-time

This is a POSIXct/POSIXlt method for the
[`date_start()`](https://clock.r-lib.org/dev/reference/date-and-date-time-boundary.md)
and
[`date_end()`](https://clock.r-lib.org/dev/reference/date-and-date-time-boundary.md)
generics.

## Usage

``` r
# S3 method for class 'POSIXt'
date_start(
  x,
  precision,
  ...,
  invalid = NULL,
  nonexistent = NULL,
  ambiguous = x
)

# S3 method for class 'POSIXt'
date_end(x, precision, ..., invalid = NULL, nonexistent = NULL, ambiguous = x)
```

## Arguments

- x:

  `[POSIXct / POSIXlt]`

  A date-time vector.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"month"`

  - `"day"`

  - `"hour"`

  - `"minute"`

  - `"second"`

- ...:

  These dots are for future extensions and must be empty.

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

`x` but with some components altered to be at the boundary value.

## Examples

``` r
x <- date_time_build(2019:2021, 2:4, 3:5, 4, 5, 6, zone = "America/New_York")
x
#> [1] "2019-02-03 04:05:06 EST" "2020-03-04 04:05:06 EST"
#> [3] "2021-04-05 04:05:06 EDT"

# Last moment of the month
date_end(x, "month")
#> [1] "2019-02-28 23:59:59 EST" "2020-03-31 23:59:59 EDT"
#> [3] "2021-04-30 23:59:59 EDT"

# Notice that this is different from just setting the day to `"last"`
set_day(x, "last")
#> [1] "2019-02-28 04:05:06 EST" "2020-03-31 04:05:06 EDT"
#> [3] "2021-04-30 04:05:06 EDT"

# Last moment of the year
date_end(x, "year")
#> [1] "2019-12-31 23:59:59 EST" "2020-12-31 23:59:59 EST"
#> [3] "2021-12-31 23:59:59 EST"

# First moment of the hour
date_start(x, "hour")
#> [1] "2019-02-03 04:00:00 EST" "2020-03-04 04:00:00 EST"
#> [3] "2021-04-05 04:00:00 EDT"
```
