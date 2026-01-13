# Counting: date

This is a Date method for the
[`date_count_between()`](https://clock.r-lib.org/dev/reference/date_count_between.md)
generic.

[`date_count_between()`](https://clock.r-lib.org/dev/reference/date_count_between.md)
counts the number of `precision` units between `start` and `end` (i.e.,
the number of years or months). This count corresponds to the *whole
number* of units, and will never return a fractional value.

This is suitable for, say, computing the whole number of years or months
between two dates, accounting for the day of the month.

*Calendrical based counting:*

These precisions convert to a year-month-day calendar and count while in
that type.

- `"year"`

- `"quarter"`

- `"month"`

*Time point based counting:*

These precisions convert to a time point and count while in that type.

- `"week"`

- `"day"`

For dates, whether a calendar or time point is used is not all that
important, but is is fairly important for date-times.

## Usage

``` r
# S3 method for class 'Date'
date_count_between(start, end, precision, ..., n = 1L)
```

## Arguments

- start, end:

  `[Date]`

  A pair of date vectors. These will be recycled to their common size.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"quarter"`

  - `"month"`

  - `"week"`

  - `"day"`

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

## Value

An integer representing the number of `precision` units between `start`
and `end`.

## Details

`"quarter"` is equivalent to `"month"` precision with `n` set to
`n * 3L`.

## Comparison Direction

The computed count has the property that if `start <= end`, then
`start + <count> <= end`. Similarly, if `start >= end`, then
`start + <count> >= end`. In other words, the comparison direction
between `start` and `end` will never change after adding the count to
`start`. This makes this function useful for repeated count computations
at increasingly fine precisions.

## Examples

``` r
start <- date_parse("2000-05-05")
end <- date_parse(c("2020-05-04", "2020-05-06"))

# Age in years
date_count_between(start, end, "year")
#> [1] 19 20

# Number of "whole" months between these dates. i.e.
# `2000-05-05 -> 2020-04-05` is 239 months
# `2000-05-05 -> 2020-05-05` is 240 months
# Since 2020-05-04 occurs before the 5th of that month,
# it gets a count of 239
date_count_between(start, end, "month")
#> [1] 239 240

# Number of "whole" quarters between (same as `"month"` with `n * 3`)
date_count_between(start, end, "quarter")
#> [1] 79 80
date_count_between(start, end, "month", n = 3)
#> [1] 79 80

# Number of days between
date_count_between(start, end, "day")
#> [1] 7304 7306

# Number of full 3 day periods between these two dates
date_count_between(start, end, "day", n = 3)
#> [1] 2434 2435

# Essentially the truncated value of this
date_count_between(start, end, "day") / 3
#> [1] 2434.667 2435.333

# ---------------------------------------------------------------------------

# Breakdown into full years, months, and days between
x <- start

years <- date_count_between(x, end, "year")
x <- add_years(x, years)

months <- date_count_between(x, end, "month")
x <- add_months(x, months)

days <- date_count_between(x, end, "day")
x <- add_days(x, days)

data.frame(
  start = start,
  end = end,
  years = years,
  months = months,
  days = days
)
#>        start        end years months days
#> 1 2000-05-05 2020-05-04    19     11   29
#> 2 2000-05-05 2020-05-06    20      0    1

# Note that when breaking down a date like that, you may need to
# set `invalid` during intermediate calculations
start <- date_build(2019, c(3, 3, 4), c(30, 31, 1))
end <- date_build(2019, 5, 05)

# These are 1 month apart (plus a few days)
months <- date_count_between(start, end, "month")

# But adding that 1 month to `start` results in an invalid date
try(add_months(start, months))
#> Error in invalid_resolve(x, invalid = invalid) : 
#>   Invalid date found at location 2.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.

# You can choose various ways to resolve this
start_previous <- add_months(start, months, invalid = "previous")
start_next <- add_months(start, months, invalid = "next")

days_previous <- date_count_between(start_previous, end, "day")
days_next <- date_count_between(start_next, end, "day")

# Resulting in slightly different day values.
# No result is "perfect". Choosing "previous" or "next" both result
# in multiple `start` dates having the same month/day breakdown values.
data.frame(
  start = start,
  end = end,
  months = months,
  days_previous = days_previous,
  days_next = days_next
)
#>        start        end months days_previous days_next
#> 1 2019-03-30 2019-05-05      1             5         5
#> 2 2019-03-31 2019-05-05      1             5         4
#> 3 2019-04-01 2019-05-05      1             4         4
```
