# Grouping: year-month-weekday

This is a year-month-weekday method for the
[`calendar_group()`](https://clock.r-lib.org/reference/calendar_group.md)
generic.

Grouping for a year-month-weekday object can be done at any precision
except for `"day"`, as long as `x` is at least as precise as
`precision`.

## Usage

``` r
# S3 method for class 'clock_year_month_weekday'
calendar_group(x, precision, ..., n = 1L)
```

## Arguments

- x:

  `[clock_year_month_weekday]`

  A year-month-weekday vector.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"month"`

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

## Details

Grouping by `"day"` is undefined for a year-month-weekday because there
are two day fields, the weekday and the index, and there is no clear way
to define how to group by that.

## Examples

``` r
x <- year_month_weekday(2019, 1:12, clock_weekdays$sunday, 1, 00, 05, 05)
x
#> <year_month_weekday<second>[12]>
#>  [1] "2019-01-Sun[1]T00:05:05" "2019-02-Sun[1]T00:05:05"
#>  [3] "2019-03-Sun[1]T00:05:05" "2019-04-Sun[1]T00:05:05"
#>  [5] "2019-05-Sun[1]T00:05:05" "2019-06-Sun[1]T00:05:05"
#>  [7] "2019-07-Sun[1]T00:05:05" "2019-08-Sun[1]T00:05:05"
#>  [9] "2019-09-Sun[1]T00:05:05" "2019-10-Sun[1]T00:05:05"
#> [11] "2019-11-Sun[1]T00:05:05" "2019-12-Sun[1]T00:05:05"

# Group by 3 months - drops more precise components!
calendar_group(x, "month", n = 3)
#> <year_month_weekday<month>[12]>
#>  [1] "2019-01" "2019-01" "2019-01" "2019-04" "2019-04" "2019-04"
#>  [7] "2019-07" "2019-07" "2019-07" "2019-10" "2019-10" "2019-10"
```
