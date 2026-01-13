# Grouping: year-day

This is a year-day method for the
[`calendar_group()`](https://clock.r-lib.org/dev/reference/calendar_group.md)
generic.

Grouping for a year-day object can be done at any precision, as long as
`x` is at least as precise as `precision`.

## Usage

``` r
# S3 method for class 'clock_year_day'
calendar_group(x, precision, ..., n = 1L)
```

## Arguments

- x:

  `[clock_year_day]`

  A year-day vector.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

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
x <- seq(as_naive_time(year_month_day(2019, 1, 1)), by = 5, length.out = 20)
x <- as_year_day(x)
x
#> <year_day<day>[20]>
#>  [1] "2019-001" "2019-006" "2019-011" "2019-016" "2019-021" "2019-026"
#>  [7] "2019-031" "2019-036" "2019-041" "2019-046" "2019-051" "2019-056"
#> [13] "2019-061" "2019-066" "2019-071" "2019-076" "2019-081" "2019-086"
#> [19] "2019-091" "2019-096"

# Group by day of the current year
calendar_group(x, "day", n = 20)
#> <year_day<day>[20]>
#>  [1] "2019-001" "2019-001" "2019-001" "2019-001" "2019-021" "2019-021"
#>  [7] "2019-021" "2019-021" "2019-041" "2019-041" "2019-041" "2019-041"
#> [13] "2019-061" "2019-061" "2019-061" "2019-061" "2019-081" "2019-081"
#> [19] "2019-081" "2019-081"
```
