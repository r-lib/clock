# Info: zoned-time

`zoned_time_info()` retrieves a set of low-level information generally
not required for most date-time manipulations. It returns a data frame
with the same columns as
[`sys_time_info()`](https://clock.r-lib.org/dev/reference/sys_time_info.md),
but the `begin` and `end` columns are zoned-times with the same time
zone as `x`.

## Usage

``` r
zoned_time_info(x)
```

## Arguments

- x:

  `[clock_zoned_time]`

  A zoned-time.

## Value

A data frame of low level information.

## Examples

``` r
x <- year_month_day(2021, 03, 14, c(01, 03), c(59, 00), c(59, 00))
x <- as_naive_time(x)
x <- as_zoned_time(x, "America/New_York")

# x[1] is in EST, x[2] is in EDT
x
#> <zoned_time<second><America/New_York>[2]>
#> [1] "2021-03-14T01:59:59-05:00" "2021-03-14T03:00:00-04:00"

info <- zoned_time_info(x)
info
#>                                         begin
#> 1 2020-11-01T01:00:00-05:00[America/New_York]
#> 2 2021-03-14T03:00:00-04:00[America/New_York]
#>                                           end offset   dst
#> 1 2021-03-14T03:00:00-04:00[America/New_York] -18000 FALSE
#> 2 2021-11-07T01:00:00-05:00[America/New_York] -14400  TRUE
#>   abbreviation
#> 1          EST
#> 2          EDT

# `end` can be used to iterate through daylight saving time transitions
zoned_time_info(info$end)
#>                                         begin
#> 1 2021-03-14T03:00:00-04:00[America/New_York]
#> 2 2021-11-07T01:00:00-05:00[America/New_York]
#>                                           end offset   dst
#> 1 2021-11-07T01:00:00-05:00[America/New_York] -14400  TRUE
#> 2 2022-03-13T03:00:00-04:00[America/New_York] -18000 FALSE
#>   abbreviation
#> 1          EDT
#> 2          EST
```
