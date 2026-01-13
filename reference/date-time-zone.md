# Get or set the time zone

- `date_time_zone()` gets the time zone.

- `date_time_set_zone()` sets the time zone. This retains the
  *underlying duration*, but changes the *printed time* depending on the
  zone that is chosen.

## Usage

``` r
date_time_zone(x)

date_time_set_zone(x, zone)
```

## Arguments

- x:

  `[POSIXct / POSIXlt]`

  A date-time vector.

- zone:

  `[character(1)]`

  A valid time zone to switch to.

## Value

- `date_time_zone()` returns a string containing the time zone.

- `date_time_set_zone()` returns `x` with an altered printed time. The
  underlying duration is not changed.

## Details

This function is only valid for date-times, as clock treats R's Date
class as a *naive* type, which always has a yet-to-be-specified time
zone.

## Examples

``` r
library(magrittr)

# Cannot set or get the zone of Date.
# clock assumes that Dates are naive types, like naive-time.
x <- date_parse("2019-01-01")
try(date_time_zone(x))
#> Error in date_time_zone(x) : `x` can't be a <Date>.
#> ℹ <Date> is considered a naive time with an unspecified time zone.
#> ℹ Time zones can only be get or set for date-times (<POSIXct> or
#>   <POSIXlt>).
try(date_time_set_zone(x, "America/New_York"))
#> Error in date_time_set_zone(x, "America/New_York") : 
#>   `x` can't be a <Date>.
#> ℹ <Date> is considered a naive time with an unspecified time zone.
#> ℹ Time zones can only be get or set for date-times (<POSIXct> or
#>   <POSIXlt>).

x <- date_time_parse("2019-01-02 01:30:00", "America/New_York")
x
#> [1] "2019-01-02 01:30:00 EST"

date_time_zone(x)
#> [1] "America/New_York"

# If it is 1:30am in New York, what time is it in Los Angeles?
# Same underlying duration, new printed time
date_time_set_zone(x, "America/Los_Angeles")
#> [1] "2019-01-01 22:30:00 PST"

# If you want to retain the printed time, but change the underlying duration,
# convert to a naive-time to drop the time zone, then convert back to a
# date-time. Be aware that this requires that you handle daylight saving time
# irregularities with the `nonexistent` and `ambiguous` arguments to
# `as_date_time()`!
x %>%
  as_naive_time() %>%
  as_date_time("America/Los_Angeles")
#> [1] "2019-01-02 01:30:00 PST"

y <- date_time_parse("2021-03-28 03:30:00", "America/New_York")
y
#> [1] "2021-03-28 03:30:00 EDT"

y_nt <- as_naive_time(y)
y_nt
#> <naive_time<second>[1]>
#> [1] "2021-03-28T03:30:00"

# Helsinki had a daylight saving time gap where they jumped from
# 02:59:59 -> 04:00:00
try(as_date_time(y_nt, "Europe/Helsinki"))
#> Error in as_zoned_time(x, zone = tz, nonexistent = nonexistent, ambiguous = ambiguous) : 
#>   Nonexistent time due to daylight saving time at location 1.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.

as_date_time(y_nt, "Europe/Helsinki", nonexistent = "roll-forward")
#> [1] "2021-03-28 04:00:00 EEST"
as_date_time(y_nt, "Europe/Helsinki", nonexistent = "roll-backward")
#> [1] "2021-03-28 02:59:59 EET"
```
