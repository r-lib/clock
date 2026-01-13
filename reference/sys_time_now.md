# What is the current sys-time?

`sys_time_now()` returns the current time in UTC.

## Usage

``` r
sys_time_now()
```

## Value

A sys-time of the current time in UTC.

## Details

The time is returned with a nanosecond precision, but the actual amount
of data returned is OS dependent. Usually, information at at least the
microsecond level is returned, with some platforms returning nanosecond
information.

## Examples

``` r
x <- sys_time_now()
```
