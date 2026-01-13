# Get or set the time zone

- `zoned_time_zone()` gets the time zone.

- `zoned_time_set_zone()` sets the time zone *without changing the
  underlying instant*. This means that the result will represent the
  equivalent time in the new time zone.

## Usage

``` r
zoned_time_zone(x)

zoned_time_set_zone(x, zone)
```

## Arguments

- x:

  `[zoned_time]`

  A zoned time to get or set the time zone of.

- zone:

  `[character(1)]`

  A valid time zone to switch to.

## Value

- `zoned_time_zone()` returns a string containing the time zone.

- `zoned_time_set_zone()` returns `x` with an altered time zone
  attribute. The underlying instant is *not* changed.

## Examples

``` r
x <- year_month_day(2019, 1, 1)
x <- as_zoned_time(as_naive_time(x), "America/New_York")
x
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-01-01T00:00:00-05:00"

zoned_time_zone(x)
#> [1] "America/New_York"

# Equivalent UTC time
zoned_time_set_zone(x, "UTC")
#> <zoned_time<second><UTC>[1]>
#> [1] "2019-01-01T05:00:00+00:00"

# To force a new time zone with the same printed time,
# convert to a naive time that has no implied time zone,
# then convert back to a zoned time in the new time zone.
nt <- as_naive_time(x)
nt
#> <naive_time<second>[1]>
#> [1] "2019-01-01T00:00:00"
as_zoned_time(nt, "UTC")
#> <zoned_time<second><UTC>[1]>
#> [1] "2019-01-01T00:00:00+00:00"
```
