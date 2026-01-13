# Rounding: date-time

These are POSIXct/POSIXlt methods for the [rounding
generics](https://clock.r-lib.org/reference/date-and-date-time-rounding.md).

- [`date_floor()`](https://clock.r-lib.org/reference/date-and-date-time-rounding.md)
  rounds a date-time down to a multiple of the specified `precision`.

- [`date_ceiling()`](https://clock.r-lib.org/reference/date-and-date-time-rounding.md)
  rounds a date-time up to a multiple of the specified `precision`.

- [`date_round()`](https://clock.r-lib.org/reference/date-and-date-time-rounding.md)
  rounds up or down depending on what is closer, rounding up on ties.

You can group by irregular periods such as `"month"` or `"year"` by
using [`date_group()`](https://clock.r-lib.org/reference/date_group.md).

## Usage

``` r
# S3 method for class 'POSIXt'
date_floor(
  x,
  precision,
  ...,
  n = 1L,
  origin = NULL,
  nonexistent = NULL,
  ambiguous = x
)

# S3 method for class 'POSIXt'
date_ceiling(
  x,
  precision,
  ...,
  n = 1L,
  origin = NULL,
  nonexistent = NULL,
  ambiguous = x
)

# S3 method for class 'POSIXt'
date_round(
  x,
  precision,
  ...,
  n = 1L,
  origin = NULL,
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

  - `"week"`

  - `"day"`

  - `"hour"`

  - `"minute"`

  - `"second"`

  `"week"` is an alias for `"day"` with `n * 7`.

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

- origin:

  `[POSIXct(1) / POSIXlt(1) / NULL]`

  An origin to start counting from.

  `origin` must have exactly the same time zone as `x`.

  `origin` will be floored to `precision`. If information is lost when
  flooring, a warning will be thrown.

  If `NULL`, defaults to midnight on 1970-01-01 in the time zone of `x`.

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

`x` rounded to the specified `precision`.

## Details

When rounding by `"week"`, remember that the `origin` determines the
"week start". By default, 1970-01-01 is the implicit origin, which is a
Thursday. If you would like to round by weeks with a different week
start, just supply an origin on the weekday you are interested in.

## Examples

``` r
x <- as.POSIXct("2019-03-31", "America/New_York")
x <- add_days(x, 0:5)

# Flooring by 2 days, note that this is not tied to the current month,
# and instead counts from the specified `origin`, so groups can cross
# the month boundary
date_floor(x, "day", n = 2)
#> [1] "2019-03-31 EDT" "2019-03-31 EDT" "2019-04-02 EDT" "2019-04-02 EDT"
#> [5] "2019-04-04 EDT" "2019-04-04 EDT"

# Compare to `date_group()`, which groups by the day of the month
date_group(x, "day", n = 2)
#> [1] "2019-03-31 EDT" "2019-04-01 EDT" "2019-04-01 EDT" "2019-04-03 EDT"
#> [5] "2019-04-03 EDT" "2019-04-05 EDT"

# Note that daylight saving time gaps can throw off rounding
x <- as.POSIXct("1970-04-26 01:59:59", "America/New_York") + c(0, 1)
x
#> [1] "1970-04-26 01:59:59 EST" "1970-04-26 03:00:00 EDT"

# Rounding is done in naive-time, which means that rounding by 2 hours
# will attempt to generate a time of 1970-04-26 02:00:00, which doesn't
# exist in this time zone
try(date_floor(x, "hour", n = 2))
#> Error in as_zoned_time(x, zone = tz, nonexistent = nonexistent, ambiguous = ambiguous) : 
#>   Nonexistent time due to daylight saving time at location 2.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.

# You can handle this by specifying a nonexistent time resolution strategy
date_floor(x, "hour", n = 2, nonexistent = "roll-forward")
#> [1] "1970-04-26 00:00:00 EST" "1970-04-26 03:00:00 EDT"
```
