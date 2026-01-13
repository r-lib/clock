# Is the year a leap year?

`date_leap_year()` detects if the year is a leap year.

## Usage

``` r
date_leap_year(x)
```

## Arguments

- x:

  `[Date / POSIXct / POSIXlt]`

  A date or date-time to detect leap years in.

## Value

A logical vector the same size as `x`. Returns `TRUE` if in a leap year,
`FALSE` if not in a leap year, and `NA` if `x` is `NA`.

## Examples

``` r
x <- as.Date("2019-01-01")
x <- add_years(x, 0:5)
date_leap_year(x)
#> [1] FALSE  TRUE FALSE FALSE FALSE  TRUE

y <- as.POSIXct("2019-01-01", "America/New_York")
y <- add_years(y, 0:5)
date_leap_year(y)
#> [1] FALSE  TRUE FALSE FALSE FALSE  TRUE
```
