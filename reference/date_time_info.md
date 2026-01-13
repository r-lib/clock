# Info: date-time

`date_time_info()` retrieves a set of low-level information generally
not required for most date-time manipulations. It returns a data frame
with the same columns as
[`sys_time_info()`](https://clock.r-lib.org/reference/sys_time_info.md),
but the `begin` and `end` columns are date-times with the same time zone
as `x`, and the `offset` column is an integer rather than a second based
[duration](https://clock.r-lib.org/reference/duration-helper.md) column
since this is part of the high-level API.

## Usage

``` r
date_time_info(x)
```

## Arguments

- x:

  `[POSIXct / POSIXlt]`

  A date-time.

## Value

A data frame of low level information.

## Examples

``` r
x <- date_time_build(
  2021, 03, 14, c(01, 03), c(59, 00), c(59, 00),
  zone = "America/New_York"
)

# x[1] is in EST, x[2] is in EDT
x
#> [1] "2021-03-14 01:59:59 EST" "2021-03-14 03:00:00 EDT"

info <- date_time_info(x)
info
#>                 begin                 end offset   dst abbreviation
#> 1 2020-11-01 01:00:00 2021-03-14 03:00:00 -18000 FALSE          EST
#> 2 2021-03-14 03:00:00 2021-11-07 01:00:00 -14400  TRUE          EDT

# `end` can be used to iterate through daylight saving time transitions
date_time_info(info$end)
#>                 begin                 end offset   dst abbreviation
#> 1 2021-03-14 03:00:00 2021-11-07 01:00:00 -14400  TRUE          EDT
#> 2 2021-11-07 01:00:00 2022-03-13 03:00:00 -18000 FALSE          EST
```
