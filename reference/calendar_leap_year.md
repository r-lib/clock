# Is the calendar year a leap year?

`calendar_leap_year()` detects if the calendar year is a leap year -
i.e. does it contain one or more extra components than other years?

A particular year is a leap year if:

- [`year_month_day()`](https://clock.r-lib.org/reference/year_month_day.md):
  February has 29 days.

- [`year_month_weekday()`](https://clock.r-lib.org/reference/year_month_weekday.md):
  February has a weekday that occurs 5 times.

- [`year_week_day()`](https://clock.r-lib.org/reference/year_week_day.md):
  There are 53 weeks in the year, resulting in 371 days in the year.

- [`iso_year_week_day()`](https://clock.r-lib.org/reference/iso_year_week_day.md):
  There are 53 weeks in the year, resulting in 371 days in the year.

- [`year_quarter_day()`](https://clock.r-lib.org/reference/year_quarter_day.md):
  One of the quarters has 1 more day than normal (the quarter with an
  extra day depends on the `start` used, but will always be the same for
  a particular `start`). This aligns with Gregorian leap years for all
  `start`s except February, in which case the leap year is always 1 year
  after the Gregorian leap year.

- [`year_day()`](https://clock.r-lib.org/reference/year_day.md): There
  are 366 days in the year.

## Usage

``` r
calendar_leap_year(x)
```

## Arguments

- x:

  `[calendar]`

  A calendar type to detect leap years in.

## Value

A logical vector the same size as `x`. Returns `TRUE` if in a leap year,
`FALSE` if not in a leap year, and `NA` if `x` is `NA`.

## Examples

``` r
x <- year_month_day(c(2019:2024, NA))
calendar_leap_year(x)
#> [1] FALSE  TRUE FALSE FALSE FALSE  TRUE    NA

# For year-quarter-day, the leap year typically aligns with the Gregorian
# leap year, unless the `start` is February, in which case the leap year is
# always 1 year after the Gregorian leap year
x <- year_quarter_day(2020:2021, start = clock_months$january)
calendar_leap_year(x)
#> [1]  TRUE FALSE

x <- year_quarter_day(2020:2021, start = clock_months$february)
calendar_leap_year(x)
#> [1] FALSE  TRUE

# With a January start, 2020 has the extra day
get_day(year_quarter_day(2020, 1:4, "last", start = clock_months$january))
#> [1] 91 91 92 92
get_day(year_quarter_day(2021, 1:4, "last", start = clock_months$january))
#> [1] 90 91 92 92
get_day(year_quarter_day(2022, 1:4, "last", start = clock_months$january))
#> [1] 90 91 92 92

# With a February start, 2021 has the extra day
get_day(year_quarter_day(2020, 1:4, "last", start = clock_months$february))
#> [1] 89 92 92 92
get_day(year_quarter_day(2021, 1:4, "last", start = clock_months$february))
#> [1] 90 92 92 92
get_day(year_quarter_day(2022, 1:4, "last", start = clock_months$february))
#> [1] 89 92 92 92
```
