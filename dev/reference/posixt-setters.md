# Setters: date-time

These are POSIXct/POSIXlt methods for the [setter
generics](https://clock.r-lib.org/dev/reference/clock-setters.md).

- [`set_year()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the year.

- [`set_month()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the month of the year. Valid values are in the range of
  `[1, 12]`.

- [`set_day()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the day of the month. Valid values are in the range of `[1, 31]`.

- There are sub-daily setters for setting more precise components, up to
  a precision of seconds.

## Usage

``` r
# S3 method for class 'POSIXt'
set_year(x, value, ..., invalid = NULL, nonexistent = NULL, ambiguous = x)

# S3 method for class 'POSIXt'
set_month(x, value, ..., invalid = NULL, nonexistent = NULL, ambiguous = x)

# S3 method for class 'POSIXt'
set_day(x, value, ..., invalid = NULL, nonexistent = NULL, ambiguous = x)

# S3 method for class 'POSIXt'
set_hour(x, value, ..., invalid = NULL, nonexistent = NULL, ambiguous = x)

# S3 method for class 'POSIXt'
set_minute(x, value, ..., invalid = NULL, nonexistent = NULL, ambiguous = x)

# S3 method for class 'POSIXt'
set_second(x, value, ..., invalid = NULL, nonexistent = NULL, ambiguous = x)
```

## Arguments

- x:

  `[POSIXct / POSIXlt]`

  A date-time vector.

- value:

  `[integer / "last"]`

  The value to set the component to.

  For
  [`set_day()`](https://clock.r-lib.org/dev/reference/clock-setters.md),
  this can also be `"last"` to set the day to the last day of the month.

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

`x` with the component set.

## Examples

``` r
x <- as.POSIXct("2019-02-01", tz = "America/New_York")

# Set the day
set_day(x, 12:14)
#> [1] "2019-02-12 EST" "2019-02-13 EST" "2019-02-14 EST"

# Set to the "last" day of the month
set_day(x, "last")
#> [1] "2019-02-28 EST"

# You cannot set a date-time to an invalid date like you can with
# a year-month-day. Instead, the default strategy is to error.
try(set_day(x, 31))
#> Error in invalid_resolve(x, invalid = invalid) : 
#>   Invalid date found at location 1.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.
set_day(as_year_month_day(x), 31)
#> <year_month_day<second>[1]>
#> [1] "2019-02-31T00:00:00"

# You can resolve these issues while setting the day by specifying
# an invalid date resolution strategy with `invalid`
set_day(x, 31, invalid = "previous")
#> [1] "2019-02-28 23:59:59 EST"

y <- as.POSIXct("2020-03-08 01:30:00", tz = "America/New_York")

# Nonexistent and ambiguous times must be resolved immediately when
# working with R's native date-time types. An error is thrown by default.
try(set_hour(y, 2))
#> Error in as_zoned_time(x, zone = tz, nonexistent = nonexistent, ambiguous = ambiguous) : 
#>   Nonexistent time due to daylight saving time at location 1.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.
set_hour(y, 2, nonexistent = "roll-forward")
#> [1] "2020-03-08 03:00:00 EDT"
set_hour(y, 2, nonexistent = "roll-backward")
#> [1] "2020-03-08 01:59:59 EST"
```
