# Get or set the time zone

**\[deprecated\]**

- `date_zone()` is deprecated in favor of
  [`date_time_zone()`](https://clock.r-lib.org/dev/reference/date-time-zone.md).

- `date_set_zone()` is deprecated in favor of
  [`date_time_set_zone()`](https://clock.r-lib.org/dev/reference/date-time-zone.md).

## Usage

``` r
date_zone(x)

date_set_zone(x, zone)
```

## Arguments

- x:

  `[POSIXct / POSIXlt]`

  A date-time vector.

- zone:

  `[character(1)]`

  A valid time zone to switch to.
