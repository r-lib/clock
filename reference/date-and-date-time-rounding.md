# Date and date-time rounding

- `date_floor()` rounds a date or date-time down to a multiple of the
  specified `precision`.

- `date_ceiling()` rounds a date or date-time up to a multiple of the
  specified `precision`.

- `date_round()` rounds up or down depending on what is closer, rounding
  up on ties.

There are separate help pages for rounding dates and date-times:

- [dates (Date)](https://clock.r-lib.org/reference/date-rounding.md)

- [date-times
  (POSIXct/POSIXlt)](https://clock.r-lib.org/reference/posixt-rounding.md)

These functions round the underlying duration itself, relative to an
`origin`. For example, rounding to 15 hours will construct groups of 15
hours, starting from `origin`, which defaults to a naive time of
1970-01-01 00:00:00.

If you want to group by components, such as "day of the month", see
[`date_group()`](https://clock.r-lib.org/reference/date_group.md).

## Usage

``` r
date_floor(x, precision, ..., n = 1L, origin = NULL)

date_ceiling(x, precision, ..., n = 1L, origin = NULL)

date_round(x, precision, ..., n = 1L, origin = NULL)
```

## Arguments

- x:

  `[Date / POSIXct / POSIXlt]`

  A date or date-time vector.

- precision:

  `[character(1)]`

  A precision. Allowed precisions are dependent on the input used.

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

- origin:

  `[Date(1) / POSIXct(1) / POSIXlt(1) / NULL]`

  An origin to start counting from. The default `origin` is midnight on
  1970-01-01 in the time zone of `x`.

## Value

`x` rounded to the specified `precision`.

## Examples

``` r
# See the type specific documentation for more examples

x <- as.Date("2019-03-31") + 0:5
x
#> [1] "2019-03-31" "2019-04-01" "2019-04-02" "2019-04-03" "2019-04-04"
#> [6] "2019-04-05"

# Flooring by 2 days, note that this is not tied to the current month,
# and instead counts from the specified `origin`.
date_floor(x, "day", n = 2)
#> [1] "2019-03-31" "2019-03-31" "2019-04-02" "2019-04-02" "2019-04-04"
#> [6] "2019-04-04"
```
