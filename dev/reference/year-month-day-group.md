# Grouping: year-month-day

This is a year-month-day method for the
[`calendar_group()`](https://clock.r-lib.org/dev/reference/calendar_group.md)
generic.

Grouping for a year-month-day object can be done at any precision, as
long as `x` is at least as precise as `precision`.

## Usage

``` r
# S3 method for class 'clock_year_month_day'
calendar_group(x, precision, ..., n = 1L)
```

## Arguments

- x:

  `[clock_year_month_day]`

  A year-month-day vector.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"month"`

  - `"day"`

  - `"hour"`

  - `"minute"`

  - `"second"`

  - `"millisecond"`

  - `"microsecond"`

  - `"nanosecond"`

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

## Value

`x` grouped at the specified `precision`.

## Examples

``` r
steps <- duration_days(seq(0, 100, by = 5))
x <- year_month_day(2019, 1, 1)
x <- as_naive_time(x) + steps
x <- as_year_month_day(x)
x
#> <year_month_day<day>[21]>
#>  [1] "2019-01-01" "2019-01-06" "2019-01-11" "2019-01-16" "2019-01-21"
#>  [6] "2019-01-26" "2019-01-31" "2019-02-05" "2019-02-10" "2019-02-15"
#> [11] "2019-02-20" "2019-02-25" "2019-03-02" "2019-03-07" "2019-03-12"
#> [16] "2019-03-17" "2019-03-22" "2019-03-27" "2019-04-01" "2019-04-06"
#> [21] "2019-04-11"

# Group by a single month
calendar_group(x, "month")
#> <year_month_day<month>[21]>
#>  [1] "2019-01" "2019-01" "2019-01" "2019-01" "2019-01" "2019-01"
#>  [7] "2019-01" "2019-02" "2019-02" "2019-02" "2019-02" "2019-02"
#> [13] "2019-03" "2019-03" "2019-03" "2019-03" "2019-03" "2019-03"
#> [19] "2019-04" "2019-04" "2019-04"

# Or multiple months
calendar_group(x, "month", n = 2)
#> <year_month_day<month>[21]>
#>  [1] "2019-01" "2019-01" "2019-01" "2019-01" "2019-01" "2019-01"
#>  [7] "2019-01" "2019-01" "2019-01" "2019-01" "2019-01" "2019-01"
#> [13] "2019-03" "2019-03" "2019-03" "2019-03" "2019-03" "2019-03"
#> [19] "2019-03" "2019-03" "2019-03"

# Group 3 days of the month together
y <- year_month_day(2019, 1, 1:12)
calendar_group(y, "day", n = 3)
#> <year_month_day<day>[12]>
#>  [1] "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-04" "2019-01-04"
#>  [6] "2019-01-04" "2019-01-07" "2019-01-07" "2019-01-07" "2019-01-10"
#> [11] "2019-01-10" "2019-01-10"

# Group by 5 nanosecond of the current second
z <- year_month_day(
  2019, 1, 2, 1, 5, 20, 1:20,
  subsecond_precision = "nanosecond"
)
calendar_group(z, "nanosecond", n = 5)
#> <year_month_day<nanosecond>[20]>
#>  [1] "2019-01-02T01:05:20.000000000" "2019-01-02T01:05:20.000000000"
#>  [3] "2019-01-02T01:05:20.000000000" "2019-01-02T01:05:20.000000000"
#>  [5] "2019-01-02T01:05:20.000000005" "2019-01-02T01:05:20.000000005"
#>  [7] "2019-01-02T01:05:20.000000005" "2019-01-02T01:05:20.000000005"
#>  [9] "2019-01-02T01:05:20.000000005" "2019-01-02T01:05:20.000000010"
#> [11] "2019-01-02T01:05:20.000000010" "2019-01-02T01:05:20.000000010"
#> [13] "2019-01-02T01:05:20.000000010" "2019-01-02T01:05:20.000000010"
#> [15] "2019-01-02T01:05:20.000000015" "2019-01-02T01:05:20.000000015"
#> [17] "2019-01-02T01:05:20.000000015" "2019-01-02T01:05:20.000000015"
#> [19] "2019-01-02T01:05:20.000000015" "2019-01-02T01:05:20.000000020"
```
