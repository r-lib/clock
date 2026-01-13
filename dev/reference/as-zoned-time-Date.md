# Convert to a zoned-time from a date

This is a Date method for the
[`as_zoned_time()`](https://clock.r-lib.org/dev/reference/as_zoned_time.md)
generic.

clock assumes that Dates are *naive* date-time types. Like naive-times,
they have a yet-to-be-specified time zone. This method allows you to
specify that time zone, keeping the printed time. If possible, the time
will be set to midnight (see Details for the rare case in which this is
not possible).

## Usage

``` r
# S3 method for class 'Date'
as_zoned_time(x, zone, ..., nonexistent = NULL, ambiguous = NULL)
```

## Arguments

- x:

  `[Date]`

  A Date.

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

A zoned-time.

## Details

In the rare instance that the specified time zone does not contain a
date-time at midnight due to daylight saving time, `nonexistent` can be
used to resolve the issue. Similarly, if there are two possible midnight
times due to a daylight saving time fallback, `ambiguous` can be used.

## Examples

``` r
x <- as.Date("2019-01-01")

# The resulting zoned-times have the same printed time, but are in
# different time zones
as_zoned_time(x, "UTC")
#> <zoned_time<second><UTC>[1]>
#> [1] "2019-01-01T00:00:00+00:00"
as_zoned_time(x, "America/New_York")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-01-01T00:00:00-05:00"

# Converting Date -> zoned-time is the same as naive-time -> zoned-time
x <- as_naive_time(year_month_day(2019, 1, 1))
as_zoned_time(x, "America/New_York")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-01-01T00:00:00-05:00"

# In Asia/Beirut, there was a DST gap from
# 2021-03-27 23:59:59 -> 2021-03-28 01:00:00,
# skipping the 0th hour entirely. This means there is no midnight value.
x <- as.Date("2021-03-28")
try(as_zoned_time(x, "Asia/Beirut"))
#> Error in as_zoned_time(x, zone = zone, nonexistent = nonexistent, ambiguous = ambiguous) : 
#>   Nonexistent time due to daylight saving time at location 1.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.

# To resolve this, set a `nonexistent` time resolution strategy
as_zoned_time(x, "Asia/Beirut", nonexistent = "roll-forward")
#> <zoned_time<second><Asia/Beirut>[1]>
#> [1] "2021-03-28T01:00:00+03:00"
```
