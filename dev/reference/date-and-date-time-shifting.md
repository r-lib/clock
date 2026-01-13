# Shifting: date and date-time

`date_shift()` shifts `x` to the `target` weekday. You can shift to the
next or previous weekday. If `x` is currently on the `target` weekday,
you can choose to leave it alone or advance it to the next instance of
the `target`.

There are separate help pages for shifting dates and date-times:

- [dates (Date)](https://clock.r-lib.org/dev/reference/date-shifting.md)

- [date-times
  (POSIXct/POSIXlt)](https://clock.r-lib.org/dev/reference/posixt-shifting.md)

## Usage

``` r
date_shift(x, target, ..., which = "next", boundary = "keep")
```

## Arguments

- x:

  `[Date / POSIXct / POSIXlt]`

  A date or date-time vector.

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

## Value

`x` shifted to the `target` weekday.

## Examples

``` r
# See the type specific documentation for more examples

x <- as.Date("2019-01-01") + 0:1

# A Tuesday and Wednesday
as_weekday(x)
#> <weekday[2]>
#> [1] Tue Wed

monday <- weekday(clock_weekdays$monday)

# Shift to the next Monday
date_shift(x, monday)
#> [1] "2019-01-07" "2019-01-07"
```
