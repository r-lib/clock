# Convert to iso-year-week-day

`as_iso_year_week_day()` converts a vector to the iso-year-week-day
calendar. Time points, Dates, POSIXct, and other calendars can all be
converted to iso-year-week-day.

## Usage

``` r
as_iso_year_week_day(x, ...)
```

## Arguments

- x:

  `[vector]`

  A vector to convert to iso-year-week-day.

- ...:

  These dots are for future extensions and must be empty.

## Value

A iso-year-week-day vector.

## Examples

``` r
# From Date
as_iso_year_week_day(as.Date("2019-01-01"))
#> <iso_year_week_day<day>[1]>
#> [1] "2019-W01-2"

# From POSIXct, which assumes that the naive time is what should be converted
as_iso_year_week_day(as.POSIXct("2019-01-01 02:30:30", "America/New_York"))
#> <iso_year_week_day<second>[1]>
#> [1] "2019-W01-2T02:30:30"

# From other calendars
as_iso_year_week_day(year_quarter_day(2019, quarter = 2, day = 50))
#> <iso_year_week_day<day>[1]>
#> [1] "2019-W21-1"
```
