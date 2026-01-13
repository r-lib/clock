# Group date-time components

This is a POSIXct/POSIXlt method for the
[`date_group()`](https://clock.r-lib.org/reference/date_group.md)
generic.

[`date_group()`](https://clock.r-lib.org/reference/date_group.md) groups
by a single component of a date-time, such as month of the year, day of
the month, or hour of the day.

If you need to group by more complex components, like ISO weeks, or
quarters, convert to a calendar type that contains the component you are
interested in grouping by.

## Usage

``` r
# S3 method for class 'POSIXt'
date_group(
  x,
  precision,
  ...,
  n = 1L,
  invalid = NULL,
  nonexistent = NULL,
  ambiguous = x
)
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

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

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

`x`, grouped at `precision`.

## Examples

``` r
x <- as.POSIXct("2019-01-01", "America/New_York")
x <- add_days(x, -3:5)

# Group by 2 days of the current month.
# Note that this resets at the beginning of the month, creating day groups
# of [29, 30] [31] [01, 02] [03, 04].
date_group(x, "day", n = 2)
#> [1] "2018-12-29 EST" "2018-12-29 EST" "2018-12-31 EST" "2019-01-01 EST"
#> [5] "2019-01-01 EST" "2019-01-03 EST" "2019-01-03 EST" "2019-01-05 EST"
#> [9] "2019-01-05 EST"

# Group by month
date_group(x, "month")
#> [1] "2018-12-01 EST" "2018-12-01 EST" "2018-12-01 EST" "2019-01-01 EST"
#> [5] "2019-01-01 EST" "2019-01-01 EST" "2019-01-01 EST" "2019-01-01 EST"
#> [9] "2019-01-01 EST"

# Group by hour of the day
y <- as.POSIXct("2019-12-30", "America/New_York")
y <- add_hours(y, 0:12)
y
#>  [1] "2019-12-30 00:00:00 EST" "2019-12-30 01:00:00 EST"
#>  [3] "2019-12-30 02:00:00 EST" "2019-12-30 03:00:00 EST"
#>  [5] "2019-12-30 04:00:00 EST" "2019-12-30 05:00:00 EST"
#>  [7] "2019-12-30 06:00:00 EST" "2019-12-30 07:00:00 EST"
#>  [9] "2019-12-30 08:00:00 EST" "2019-12-30 09:00:00 EST"
#> [11] "2019-12-30 10:00:00 EST" "2019-12-30 11:00:00 EST"
#> [13] "2019-12-30 12:00:00 EST"

date_group(y, "hour", n = 3)
#>  [1] "2019-12-30 00:00:00 EST" "2019-12-30 00:00:00 EST"
#>  [3] "2019-12-30 00:00:00 EST" "2019-12-30 03:00:00 EST"
#>  [5] "2019-12-30 03:00:00 EST" "2019-12-30 03:00:00 EST"
#>  [7] "2019-12-30 06:00:00 EST" "2019-12-30 06:00:00 EST"
#>  [9] "2019-12-30 06:00:00 EST" "2019-12-30 09:00:00 EST"
#> [11] "2019-12-30 09:00:00 EST" "2019-12-30 09:00:00 EST"
#> [13] "2019-12-30 12:00:00 EST"
```
