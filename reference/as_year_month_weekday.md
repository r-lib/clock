# Convert to year-month-weekday

`as_year_month_weekday()` converts a vector to the year-month-weekday
calendar. Time points, Dates, POSIXct, and other calendars can all be
converted to year-month-weekday.

## Usage

``` r
as_year_month_weekday(x, ...)
```

## Arguments

- x:

  `[vector]`

  A vector to convert to year-month-weekday.

- ...:

  These dots are for future extensions and must be empty.

## Value

A year-month-weekday vector.

## Examples

``` r
# From Date
as_year_month_weekday(as.Date("2019-01-01"))
#> <year_month_weekday<day>[1]>
#> [1] "2019-01-Tue[1]"

# From POSIXct, which assumes that the naive time is what should be converted
as_year_month_weekday(as.POSIXct("2019-01-01 02:30:30", "America/New_York"))
#> <year_month_weekday<second>[1]>
#> [1] "2019-01-Tue[1]T02:30:30"

# From other calendars
as_year_month_weekday(year_quarter_day(2019, quarter = 2, day = 50))
#> <year_month_weekday<day>[1]>
#> [1] "2019-05-Mon[3]"
```
