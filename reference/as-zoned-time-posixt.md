# Convert to a zoned-time from a date-time

This is a POSIXct/POSIXlt method for the
[`as_zoned_time()`](https://clock.r-lib.org/reference/as_zoned_time.md)
generic.

Converting from one of R's native date-time classes (POSIXct or POSIXlt)
will retain the time zone of that object. There is no `zone` argument.

## Usage

``` r
# S3 method for class 'POSIXt'
as_zoned_time(x, ...)
```

## Arguments

- x:

  `[POSIXct / POSIXlt]`

  A date-time.

- ...:

  These dots are for future extensions and must be empty.

## Value

A zoned-time.

## Examples

``` r
x <- as.POSIXct("2019-01-01", tz = "America/New_York")
as_zoned_time(x)
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-01-01T00:00:00-05:00"
```
