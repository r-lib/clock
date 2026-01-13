# What is the current zoned-time?

`zoned_time_now()` returns the current time in the corresponding `zone`.
It is a wrapper around
[`sys_time_now()`](https://clock.r-lib.org/dev/reference/sys_time_now.md)
that attaches the time zone.

## Usage

``` r
zoned_time_now(zone)
```

## Arguments

- zone:

  `[character(1)]`

  A time zone to get the current time for.

## Value

A zoned-time of the current time.

## Details

The time is returned with a nanosecond precision, but the actual amount
of data returned is OS dependent. Usually, information at at least the
microsecond level is returned, with some platforms returning nanosecond
information.

## Examples

``` r
x <- zoned_time_now("America/New_York")
```
