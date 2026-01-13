# Convert to year-week-day

`as_year_week_day()` converts a vector to the year-week-day calendar.
Time points, Dates, POSIXct, and other calendars can all be converted to
year-week-day.

## Usage

``` r
as_year_week_day(x, ..., start = NULL)
```

## Arguments

- x:

  `[vector]`

  A vector to convert to year-week-day.

- ...:

  These dots are for future extensions and must be empty.

- start:

  `[integer(1) / NULL]`

  The day to consider the start of the week. 1 = Sunday and 7 =
  Saturday.

  If `NULL`:

  - If `x` is a year-week-day, it will be returned as is.

  - Otherwise, a `start` of Sunday will be used.

## Value

A year-week-day vector.

## Examples

``` r
# From Date
as_year_week_day(as.Date("2019-01-01"))
#> <year_week_day<Sunday><day>[1]>
#> [1] "2019-W01-3"
as_year_week_day(as.Date("2019-01-01"), start = clock_weekdays$monday)
#> <year_week_day<Monday><day>[1]>
#> [1] "2019-W01-2"

# From POSIXct, which assumes that the naive time is what should be converted
as_year_week_day(as.POSIXct("2019-01-01 02:30:30", "America/New_York"))
#> <year_week_day<Sunday><second>[1]>
#> [1] "2019-W01-3T02:30:30"

# From other calendars
as_year_week_day(year_quarter_day(2019, quarter = 2, day = 50))
#> <year_week_day<Sunday><day>[1]>
#> [1] "2019-W21-2"
```
