# Group calendar components

`calendar_group()` groups at a multiple of the specified precision.
Grouping alters the value of a single component (i.e. the month
component if grouping by month). Components that are more precise than
the precision being grouped at are dropped altogether (i.e. the day
component is dropped if grouping by month).

Each calendar has its own help page describing the grouping process in
more detail:

- [year-month-day](https://clock.r-lib.org/reference/year-month-day-group.md)

- [year-month-weekday](https://clock.r-lib.org/reference/year-month-weekday-group.md)

- [year-week-day](https://clock.r-lib.org/reference/year-week-day-group.md)

- [iso-year-week-day](https://clock.r-lib.org/reference/iso-year-week-day-group.md)

- [year-quarter-day](https://clock.r-lib.org/reference/year-quarter-day-group.md)

- [year-day](https://clock.r-lib.org/reference/year-day-group.md)

## Usage

``` r
calendar_group(x, precision, ..., n = 1L)
```

## Arguments

- x:

  `[calendar]`

  A calendar vector.

- precision:

  `[character(1)]`

  A precision. Allowed precisions are dependent on the calendar used.

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

## Value

`x` grouped at the specified `precision`.

## Examples

``` r
# See the calendar specific help pages for more examples
x <- year_month_day(2019, c(1, 1, 2, 2, 3, 3, 4, 4), 1:8)
x
#> <year_month_day<day>[8]>
#> [1] "2019-01-01" "2019-01-02" "2019-02-03" "2019-02-04" "2019-03-05"
#> [6] "2019-03-06" "2019-04-07" "2019-04-08"

# Group by two months
calendar_group(x, "month", n = 2)
#> <year_month_day<month>[8]>
#> [1] "2019-01" "2019-01" "2019-01" "2019-01" "2019-03" "2019-03"
#> [7] "2019-03" "2019-03"

# Group by two days of the month
calendar_group(x, "day", n = 2)
#> <year_month_day<day>[8]>
#> [1] "2019-01-01" "2019-01-01" "2019-02-03" "2019-02-03" "2019-03-05"
#> [6] "2019-03-05" "2019-04-07" "2019-04-07"
```
