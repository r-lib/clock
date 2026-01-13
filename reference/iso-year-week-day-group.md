# Grouping: iso-year-week-day

This is a iso-year-week-day method for the
[`calendar_group()`](https://clock.r-lib.org/reference/calendar_group.md)
generic.

Grouping for a iso-year-week-day object can be done at any precision, as
long as `x` is at least as precise as `precision`.

## Usage

``` r
# S3 method for class 'clock_iso_year_week_day'
calendar_group(x, precision, ..., n = 1L)
```

## Arguments

- x:

  `[clock_iso_year_week_day]`

  A iso-year-week-day vector.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"week"`

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
x <- iso_year_week_day(2019, 1:52)

# Group by 3 ISO weeks
calendar_group(x, "week", n = 3)
#> <iso_year_week_day<week>[52]>
#>  [1] "2019-W01" "2019-W01" "2019-W01" "2019-W04" "2019-W04" "2019-W04"
#>  [7] "2019-W07" "2019-W07" "2019-W07" "2019-W10" "2019-W10" "2019-W10"
#> [13] "2019-W13" "2019-W13" "2019-W13" "2019-W16" "2019-W16" "2019-W16"
#> [19] "2019-W19" "2019-W19" "2019-W19" "2019-W22" "2019-W22" "2019-W22"
#> [25] "2019-W25" "2019-W25" "2019-W25" "2019-W28" "2019-W28" "2019-W28"
#> [31] "2019-W31" "2019-W31" "2019-W31" "2019-W34" "2019-W34" "2019-W34"
#> [37] "2019-W37" "2019-W37" "2019-W37" "2019-W40" "2019-W40" "2019-W40"
#> [43] "2019-W43" "2019-W43" "2019-W43" "2019-W46" "2019-W46" "2019-W46"
#> [49] "2019-W49" "2019-W49" "2019-W49" "2019-W52"

y <- iso_year_week_day(2000:2020, 1, 1)

# Group by 2 ISO years
calendar_group(y, "year", n = 2)
#> <iso_year_week_day<year>[21]>
#>  [1] "2000" "2000" "2002" "2002" "2004" "2004" "2006" "2006" "2008"
#> [10] "2008" "2010" "2010" "2012" "2012" "2014" "2014" "2016" "2016"
#> [19] "2018" "2018" "2020"
```
