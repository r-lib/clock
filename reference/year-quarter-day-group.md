# Grouping: year-quarter-day

This is a year-quarter-day method for the
[`calendar_group()`](https://clock.r-lib.org/reference/calendar_group.md)
generic.

Grouping for a year-quarter-day object can be done at any precision, as
long as `x` is at least as precise as `precision`.

## Usage

``` r
# S3 method for class 'clock_year_quarter_day'
calendar_group(x, precision, ..., n = 1L)
```

## Arguments

- x:

  `[clock_year_quarter_day]`

  A year-quarter-day vector.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"quarter"`

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
x <- year_quarter_day(2019, 1:4)
x <- c(x, set_year(x, 2020))

# Group by 3 quarters
# Note that this is a grouping of 3 quarters of the current year
# (i.e. the count resets at the beginning of the next year)
calendar_group(x, "quarter", n = 3)
#> <year_quarter_day<January><quarter>[8]>
#> [1] "2019-Q1" "2019-Q1" "2019-Q1" "2019-Q4" "2020-Q1" "2020-Q1"
#> [7] "2020-Q1" "2020-Q4"

# Group by 5 days of the current quarter
y <- year_quarter_day(2019, 1, 1:90)
calendar_group(y, "day", n = 5)
#> <year_quarter_day<January><day>[90]>
#>  [1] "2019-Q1-01" "2019-Q1-01" "2019-Q1-01" "2019-Q1-01" "2019-Q1-01"
#>  [6] "2019-Q1-06" "2019-Q1-06" "2019-Q1-06" "2019-Q1-06" "2019-Q1-06"
#> [11] "2019-Q1-11" "2019-Q1-11" "2019-Q1-11" "2019-Q1-11" "2019-Q1-11"
#> [16] "2019-Q1-16" "2019-Q1-16" "2019-Q1-16" "2019-Q1-16" "2019-Q1-16"
#> [21] "2019-Q1-21" "2019-Q1-21" "2019-Q1-21" "2019-Q1-21" "2019-Q1-21"
#> [26] "2019-Q1-26" "2019-Q1-26" "2019-Q1-26" "2019-Q1-26" "2019-Q1-26"
#> [31] "2019-Q1-31" "2019-Q1-31" "2019-Q1-31" "2019-Q1-31" "2019-Q1-31"
#> [36] "2019-Q1-36" "2019-Q1-36" "2019-Q1-36" "2019-Q1-36" "2019-Q1-36"
#> [41] "2019-Q1-41" "2019-Q1-41" "2019-Q1-41" "2019-Q1-41" "2019-Q1-41"
#> [46] "2019-Q1-46" "2019-Q1-46" "2019-Q1-46" "2019-Q1-46" "2019-Q1-46"
#> [51] "2019-Q1-51" "2019-Q1-51" "2019-Q1-51" "2019-Q1-51" "2019-Q1-51"
#> [56] "2019-Q1-56" "2019-Q1-56" "2019-Q1-56" "2019-Q1-56" "2019-Q1-56"
#> [61] "2019-Q1-61" "2019-Q1-61" "2019-Q1-61" "2019-Q1-61" "2019-Q1-61"
#> [66] "2019-Q1-66" "2019-Q1-66" "2019-Q1-66" "2019-Q1-66" "2019-Q1-66"
#> [71] "2019-Q1-71" "2019-Q1-71" "2019-Q1-71" "2019-Q1-71" "2019-Q1-71"
#> [76] "2019-Q1-76" "2019-Q1-76" "2019-Q1-76" "2019-Q1-76" "2019-Q1-76"
#> [81] "2019-Q1-81" "2019-Q1-81" "2019-Q1-81" "2019-Q1-81" "2019-Q1-81"
#> [86] "2019-Q1-86" "2019-Q1-86" "2019-Q1-86" "2019-Q1-86" "2019-Q1-86"
```
