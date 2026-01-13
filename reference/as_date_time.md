# Convert to a date-time

`as_date_time()` is a generic function that converts its input to a
date-time (POSIXct).

There are methods for converting dates (Date), calendars, time points,
and zoned-times to date-times.

For converting to a date, see
[`as_date()`](https://clock.r-lib.org/reference/as_date.md).

## Usage

``` r
as_date_time(x, ...)

# S3 method for class 'POSIXt'
as_date_time(x, ...)

# S3 method for class 'Date'
as_date_time(x, zone, ..., nonexistent = NULL, ambiguous = NULL)

# S3 method for class 'clock_calendar'
as_date_time(x, zone, ..., nonexistent = NULL, ambiguous = NULL)

# S3 method for class 'clock_sys_time'
as_date_time(x, zone, ...)

# S3 method for class 'clock_naive_time'
as_date_time(x, zone, ..., nonexistent = NULL, ambiguous = NULL)

# S3 method for class 'clock_zoned_time'
as_date_time(x, ...)
```

## Arguments

- x:

  `[vector]`

  A vector.

- ...:

  These dots are for future extensions and must be empty.

- zone:

  `[character(1)]`

  The zone to convert to.

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

A date-time with the same length as `x`.

## Details

Note that clock always assumes that R's Date class is naive, so
converting a Date to a POSIXct will always attempt to retain the printed
year, month, and day. Where possible, the resulting time will be at
midnight (`00:00:00`), but in some rare cases this is not possible due
to daylight saving time. If that issue ever arises, an error will be
thrown, which can be resolved by explicitly supplying `nonexistent` or
`ambiguous`.

This is not a drop-in replacement for
[`as.POSIXct()`](https://rdrr.io/r/base/as.POSIXlt.html), as it only
converts a limited set of types to POSIXct. For parsing characters as
date-times, see
[`date_time_parse()`](https://clock.r-lib.org/reference/date-time-parse.md).
For converting numerics to date-times, see
[`vctrs::new_datetime()`](https://vctrs.r-lib.org/reference/new_date.html)
or continue to use
[`as.POSIXct()`](https://rdrr.io/r/base/as.POSIXlt.html).

## Examples

``` r
x <- as.Date("2019-01-01")

# `as.POSIXct()` will always treat Date as UTC, but will show the result
# of the conversion in your system time zone, which can be somewhat confusing
if (rlang::is_installed("withr")) {
  withr::with_timezone("UTC", print(as.POSIXct(x)))
  withr::with_timezone("Europe/Paris", print(as.POSIXct(x)))
  withr::with_timezone("America/New_York", print(as.POSIXct(x)))
}
#> [1] "2019-01-01 UTC"
#> [1] "2019-01-01 UTC"
#> [1] "2019-01-01 UTC"

# `as_date_time()` will treat Date as naive, which means that the original
# printed date will attempt to be kept wherever possible, no matter the
# time zone. The time will be set to midnight.
as_date_time(x, "UTC")
#> [1] "2019-01-01 UTC"
as_date_time(x, "Europe/Paris")
#> [1] "2019-01-01 CET"
as_date_time(x, "America/New_York")
#> [1] "2019-01-01 EST"

# In some rare cases, this is not possible.
# For example, in Asia/Beirut, there was a DST gap from
# 2021-03-27 23:59:59 -> 2021-03-28 01:00:00,
# skipping the 0th hour entirely.
x <- as.Date("2021-03-28")
try(as_date_time(x, "Asia/Beirut"))
#> Error in as_zoned_time(x, zone = tz, nonexistent = nonexistent, ambiguous = ambiguous) : 
#>   Nonexistent time due to daylight saving time at location 1.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.

# To resolve this, set a `nonexistent` time resolution strategy
as_date_time(x, "Asia/Beirut", nonexistent = "roll-forward")
#> [1] "2021-03-28 01:00:00 EEST"


# You can also convert to date-time from other clock types
as_date_time(year_month_day(2019, 2, 3, 03), "America/New_York")
#> [1] "2019-02-03 03:00:00 EST"
```
