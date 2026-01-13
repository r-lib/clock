# Convert to year-quarter-day

`as_year_quarter_day()` converts a vector to the year-quarter-day
calendar. Time points, Dates, POSIXct, and other calendars can all be
converted to year-quarter-day.

## Usage

``` r
as_year_quarter_day(x, ..., start = NULL)
```

## Arguments

- x:

  `[vector]`

  A vector to convert to year-quarter-day.

- ...:

  These dots are for future extensions and must be empty.

- start:

  `[integer(1) / NULL]`

  The month to start the fiscal year in. 1 = January and 12 = December.

  If `NULL`:

  - If `x` is a year-quarter-day, it will be returned as is.

  - Otherwise, a `start` of January will be used.

## Value

A year-quarter-day vector.

## Examples

``` r
# From Date
as_year_quarter_day(as.Date("2019-01-01"))
#> <year_quarter_day<January><day>[1]>
#> [1] "2019-Q1-01"
as_year_quarter_day(as.Date("2019-01-01"), start = 3)
#> <year_quarter_day<March><day>[1]>
#> [1] "2019-Q4-32"

# From POSIXct, which assumes that the naive time is what should be converted
as_year_quarter_day(as.POSIXct("2019-01-01 02:30:30", "America/New_York"))
#> <year_quarter_day<January><second>[1]>
#> [1] "2019-Q1-01T02:30:30"

# From other calendars
tuesday <- 3
as_year_quarter_day(year_month_weekday(2019, 2, tuesday, 2))
#> <year_quarter_day<January><day>[1]>
#> [1] "2019-Q1-43"

# Converting between `start`s
x <- year_quarter_day(2019, 01, 01, start = 2)
x
#> <year_quarter_day<February><day>[1]>
#> [1] "2019-Q1-01"

# Default keeps the same start
as_year_quarter_day(x)
#> <year_quarter_day<February><day>[1]>
#> [1] "2019-Q1-01"

# But you can change it
as_year_quarter_day(x, start = 1)
#> <year_quarter_day<January><day>[1]>
#> [1] "2018-Q1-32"
```
