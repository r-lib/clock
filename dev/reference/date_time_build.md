# Building: date-time

`date_time_build()` builds a POSIXct from it's individual components.

To build a POSIXct, it is required that you specify the `zone`.

## Usage

``` r
date_time_build(
  year,
  month = 1L,
  day = 1L,
  hour = 0L,
  minute = 0L,
  second = 0L,
  ...,
  zone,
  invalid = NULL,
  nonexistent = NULL,
  ambiguous = NULL
)
```

## Arguments

- year:

  `[integer]`

  The year. Values `[-32767, 32767]` are generally allowed.

- month:

  `[integer]`

  The month. Values `[1, 12]` are allowed.

- day:

  `[integer / "last"]`

  The day of the month. Values `[1, 31]` are allowed.

  If `"last"`, then the last day of the month is returned.

- hour:

  `[integer]`

  The hour. Values `[0, 23]` are allowed.

- minute:

  `[integer]`

  The minute. Values `[0, 59]` are allowed.

- second:

  `[integer]`

  The second. Values `[0, 59]` are allowed.

- ...:

  These dots are for future extensions and must be empty.

- zone:

  `[character(1)]`

  A valid time zone name.

  This argument is required, and must be specified by name.

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

A POSIXct.

## Details

Components are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

## Examples

``` r
# The zone argument is required!
# clock always requires you to be explicit about your choice of `zone`.
try(date_time_build(2020))
#> Error in date_time_build(2020) : `zone` must be supplied.

date_time_build(2020, zone = "America/New_York")
#> [1] "2020-01-01 EST"

# Nonexistent time due to daylight saving time gap from 01:59:59 -> 03:00:00
try(date_time_build(1970, 4, 26, 1:12, 30, zone = "America/New_York"))
#> Error in as_zoned_time(x, zone = tz, nonexistent = nonexistent, ambiguous = ambiguous) : 
#>   Nonexistent time due to daylight saving time at location 2.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.

# Resolve with a nonexistent time resolution strategy
date_time_build(
  1970, 4, 26, 1:12, 30,
  zone = "America/New_York",
  nonexistent = "roll-forward"
)
#>  [1] "1970-04-26 01:30:00 EST" "1970-04-26 03:00:00 EDT"
#>  [3] "1970-04-26 03:30:00 EDT" "1970-04-26 04:30:00 EDT"
#>  [5] "1970-04-26 05:30:00 EDT" "1970-04-26 06:30:00 EDT"
#>  [7] "1970-04-26 07:30:00 EDT" "1970-04-26 08:30:00 EDT"
#>  [9] "1970-04-26 09:30:00 EDT" "1970-04-26 10:30:00 EDT"
#> [11] "1970-04-26 11:30:00 EDT" "1970-04-26 12:30:00 EDT"
```
