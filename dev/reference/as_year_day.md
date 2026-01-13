# Convert to year-day

`as_year_day()` converts a vector to the year-day calendar. Time points,
Dates, POSIXct, and other calendars can all be converted to year-day.

## Usage

``` r
as_year_day(x, ...)
```

## Arguments

- x:

  `[vector]`

  A vector to convert to year-day.

- ...:

  These dots are for future extensions and must be empty.

## Value

A year-day vector.

## Examples

``` r
# From Date
as_year_day(as.Date("2019-05-01"))
#> <year_day<day>[1]>
#> [1] "2019-121"

# From POSIXct, which assumes that the naive time is what should be converted
as_year_day(as.POSIXct("2019-05-01 02:30:30", "America/New_York"))
#> <year_day<second>[1]>
#> [1] "2019-121T02:30:30"

# From other calendars
as_year_day(year_quarter_day(2019, quarter = 2, day = 50))
#> <year_day<day>[1]>
#> [1] "2019-140"
```
