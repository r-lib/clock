# Info: sys-time

`sys_time_info()` retrieves a set of low-level information generally not
required for most date-time manipulations. It returns a data frame with
the following columns:

- `begin`, `end`: Second precision sys-times specifying the range of the
  current daylight saving time rule. The range is a half-open interval
  of `[begin, end)`.

- `offset`: A second precision `duration` specifying the offset from
  UTC.

- `dst`: A logical vector specifying if daylight saving time is
  currently active.

- `abbreviation`: The time zone abbreviation in use throughout this
  `begin` to `end` range.

## Usage

``` r
sys_time_info(x, zone)
```

## Arguments

- x:

  `[clock_sys_time]`

  A sys-time.

- zone:

  `[character]`

  A valid time zone name.

  Unlike most functions in clock, in `sys_time_info()` `zone` is
  vectorized and is recycled against `x`.

## Value

A data frame of low level information.

## Details

If there have never been any daylight saving time transitions, the
minimum supported year value is returned for `begin` (typically, a year
value of `-32767`).

If daylight saving time is no longer used in a time zone, the maximum
supported year value is returned for `end` (typically, a year value of
`32767`).

The `offset` is the bridge between sys-time and naive-time for the
`zone` being used. The relationship of the three values is:

    offset = naive_time - sys_time

## Examples

``` r
library(vctrs)

x <- year_month_day(2021, 03, 14, c(01, 03), c(59, 00), c(59, 00))
x <- as_naive_time(x)
x <- as_zoned_time(x, "America/New_York")

# x[1] is in EST, x[2] is in EDT
x
#> <zoned_time<second><America/New_York>[2]>
#> [1] "2021-03-14T01:59:59-05:00" "2021-03-14T03:00:00-04:00"

x_sys <- as_sys_time(x)

info <- sys_time_info(x_sys, zoned_time_zone(x))
info
#>                 begin                 end offset   dst abbreviation
#> 1 2020-11-01T06:00:00 2021-03-14T07:00:00 -18000 FALSE          EST
#> 2 2021-03-14T07:00:00 2021-11-07T06:00:00 -14400  TRUE          EDT

# Convert `begin` and `end` to zoned-times to see the previous and
# next daylight saving time transitions
data_frame(
  x = x,
  begin = as_zoned_time(info$begin, zoned_time_zone(x)),
  end = as_zoned_time(info$end, zoned_time_zone(x))
)
#>                                             x
#> 1 2021-03-14T01:59:59-05:00[America/New_York]
#> 2 2021-03-14T03:00:00-04:00[America/New_York]
#>                                         begin
#> 1 2020-11-01T01:00:00-05:00[America/New_York]
#> 2 2021-03-14T03:00:00-04:00[America/New_York]
#>                                           end
#> 1 2021-03-14T03:00:00-04:00[America/New_York]
#> 2 2021-11-07T01:00:00-05:00[America/New_York]

# `end` can be used to iterate through daylight saving time transitions
# by repeatedly calling `sys_time_info()`
sys_time_info(info$end, zoned_time_zone(x))
#>                 begin                 end offset   dst abbreviation
#> 1 2021-03-14T07:00:00 2021-11-07T06:00:00 -14400  TRUE          EDT
#> 2 2021-11-07T06:00:00 2022-03-13T07:00:00 -18000 FALSE          EST

# Multiple `zone`s can be supplied to look up daylight saving time
# information in different time zones
zones <- c("America/New_York", "America/Los_Angeles")

info2 <- sys_time_info(x_sys[1], zones)
info2
#>                 begin                 end offset   dst abbreviation
#> 1 2020-11-01T06:00:00 2021-03-14T07:00:00 -18000 FALSE          EST
#> 2 2020-11-01T09:00:00 2021-03-14T10:00:00 -28800 FALSE          PST

# The offset can be used to display the naive-time (i.e. the printed time)
# in both of those time zones
data_frame(
  zone = zones,
  naive_time = x_sys[1] + info2$offset
)
#>                  zone          naive_time
#> 1    America/New_York 2021-03-14T01:59:59
#> 2 America/Los_Angeles 2021-03-13T22:59:59
```
