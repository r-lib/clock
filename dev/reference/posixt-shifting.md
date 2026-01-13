# Shifting: date and date-time

[`date_shift()`](https://clock.r-lib.org/dev/reference/date-and-date-time-shifting.md)
shifts `x` to the `target` weekday. You can shift to the next or
previous weekday. If `x` is currently on the `target` weekday, you can
choose to leave it alone or advance it to the next instance of the
`target`.

Shifting with date-times retains the time of day where possible. Be
aware that you can run into daylight saving time issues if you shift
into a daylight saving time gap or fallback period.

## Usage

``` r
# S3 method for class 'POSIXt'
date_shift(
  x,
  target,
  ...,
  which = "next",
  boundary = "keep",
  nonexistent = NULL,
  ambiguous = x
)
```

## Arguments

- x:

  `[POSIXct / POSIXlt]`

  A date-time vector.

- target:

  `[weekday]`

  A weekday created from
  [`weekday()`](https://clock.r-lib.org/dev/reference/weekday.md) to
  target.

  Generally this is length 1, but can also be the same length as `x`.

- ...:

  These dots are for future extensions and must be empty.

- which:

  `[character(1)]`

  One of:

  - `"next"`: Shift to the next instance of the `target` weekday.

  - `"previous`: Shift to the previous instance of the `target` weekday.

- boundary:

  `[character(1)]`

  One of:

  - `"keep"`: If `x` is currently on the `target` weekday, return it.

  - `"advance"`: If `x` is currently on the `target` weekday, advance it
    anyways.

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

`x` shifted to the `target` weekday.

## Examples

``` r
tuesday <- weekday(clock_weekdays$tuesday)

x <- as.POSIXct("1970-04-22 02:30:00", "America/New_York")

# Shift to the next Tuesday
date_shift(x, tuesday)
#> [1] "1970-04-28 02:30:00 EDT"

# Be aware that you can run into daylight saving time issues!
# Here we shift directly into a daylight saving time gap
# from 01:59:59 -> 03:00:00
sunday <- weekday(clock_weekdays$sunday)
try(date_shift(x, sunday))
#> Error in as_zoned_time(x, zone = tz, nonexistent = nonexistent, ambiguous = ambiguous) : 
#>   Nonexistent time due to daylight saving time at location 1.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.

# You can resolve this with the `nonexistent` argument
date_shift(x, sunday, nonexistent = "roll-forward")
#> [1] "1970-04-26 03:00:00 EDT"
```
