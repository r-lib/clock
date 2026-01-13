# Rounding: date

These are Date methods for the [rounding
generics](https://clock.r-lib.org/dev/reference/date-and-date-time-rounding.md).

- [`date_floor()`](https://clock.r-lib.org/dev/reference/date-and-date-time-rounding.md)
  rounds a date down to a multiple of the specified `precision`.

- [`date_ceiling()`](https://clock.r-lib.org/dev/reference/date-and-date-time-rounding.md)
  rounds a date up to a multiple of the specified `precision`.

- [`date_round()`](https://clock.r-lib.org/dev/reference/date-and-date-time-rounding.md)
  rounds up or down depending on what is closer, rounding up on ties.

The only supported rounding `precision`s for Dates are `"day"` and
`"week"`. You can group by irregular periods such as `"month"` or
`"year"` by using
[`date_group()`](https://clock.r-lib.org/dev/reference/date_group.md).

## Usage

``` r
# S3 method for class 'Date'
date_floor(x, precision, ..., n = 1L, origin = NULL)

# S3 method for class 'Date'
date_ceiling(x, precision, ..., n = 1L, origin = NULL)

# S3 method for class 'Date'
date_round(x, precision, ..., n = 1L, origin = NULL)
```

## Arguments

- x:

  `[Date]`

  A date vector.

- precision:

  `[character(1)]`

  One of:

  - `"week"`

  - `"day"`

  `"week"` is an alias for `"day"` with `n * 7`.

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

- origin:

  `[Date(1) / NULL]`

  An origin to start counting from. The default `origin` is 1970-01-01.

## Value

`x` rounded to the specified `precision`.

## Details

When rounding by `"week"`, remember that the `origin` determines the
"week start". By default, 1970-01-01 is the implicit origin, which is a
Thursday. If you would like to round by weeks with a different week
start, just supply an origin on the weekday you are interested in.

## Examples

``` r
x <- as.Date("2019-03-31") + 0:5
x
#> [1] "2019-03-31" "2019-04-01" "2019-04-02" "2019-04-03" "2019-04-04"
#> [6] "2019-04-05"

# Flooring by 2 days, note that this is not tied to the current month,
# and instead counts from the specified `origin`, so groups can cross
# the month boundary
date_floor(x, "day", n = 2)
#> [1] "2019-03-31" "2019-03-31" "2019-04-02" "2019-04-02" "2019-04-04"
#> [6] "2019-04-04"

# Compare to `date_group()`, which groups by the day of the month
date_group(x, "day", n = 2)
#> [1] "2019-03-31" "2019-04-01" "2019-04-01" "2019-04-03" "2019-04-03"
#> [6] "2019-04-05"

y <- as.Date("2019-01-01") + 0:20
y
#>  [1] "2019-01-01" "2019-01-02" "2019-01-03" "2019-01-04" "2019-01-05"
#>  [6] "2019-01-06" "2019-01-07" "2019-01-08" "2019-01-09" "2019-01-10"
#> [11] "2019-01-11" "2019-01-12" "2019-01-13" "2019-01-14" "2019-01-15"
#> [16] "2019-01-16" "2019-01-17" "2019-01-18" "2019-01-19" "2019-01-20"
#> [21] "2019-01-21"

# Flooring by week uses an implicit `origin` of 1970-01-01, which
# is a Thursday
date_floor(y, "week")
#>  [1] "2018-12-27" "2018-12-27" "2019-01-03" "2019-01-03" "2019-01-03"
#>  [6] "2019-01-03" "2019-01-03" "2019-01-03" "2019-01-03" "2019-01-10"
#> [11] "2019-01-10" "2019-01-10" "2019-01-10" "2019-01-10" "2019-01-10"
#> [16] "2019-01-10" "2019-01-17" "2019-01-17" "2019-01-17" "2019-01-17"
#> [21] "2019-01-17"
as_weekday(date_floor(y, "week"))
#> <weekday[21]>
#>  [1] Thu Thu Thu Thu Thu Thu Thu Thu Thu Thu Thu Thu Thu Thu Thu Thu
#> [17] Thu Thu Thu Thu Thu

# If you want to round by weeks with a different week start, supply an
# `origin` that falls on the weekday you care about. This uses a Monday.
origin <- as.Date("1970-01-05")
as_weekday(origin)
#> <weekday[1]>
#> [1] Mon

date_floor(y, "week", origin = origin)
#>  [1] "2018-12-31" "2018-12-31" "2018-12-31" "2018-12-31" "2018-12-31"
#>  [6] "2018-12-31" "2019-01-07" "2019-01-07" "2019-01-07" "2019-01-07"
#> [11] "2019-01-07" "2019-01-07" "2019-01-07" "2019-01-14" "2019-01-14"
#> [16] "2019-01-14" "2019-01-14" "2019-01-14" "2019-01-14" "2019-01-14"
#> [21] "2019-01-21"
as_weekday(date_floor(y, "week", origin = origin))
#> <weekday[21]>
#>  [1] Mon Mon Mon Mon Mon Mon Mon Mon Mon Mon Mon Mon Mon Mon Mon Mon
#> [17] Mon Mon Mon Mon Mon
```
