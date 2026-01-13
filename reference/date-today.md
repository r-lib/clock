# Current date and date-time

- `date_today()` returns the current date in the specified `zone` as a
  Date.

- `date_now()` returns the current date-time in the specified `zone` as
  a POSIXct.

## Usage

``` r
date_today(zone)

date_now(zone)
```

## Arguments

- zone:

  `[character(1)]`

  A time zone to get the current time for.

## Value

- `date_today()` a single Date.

- `date_now()` a single POSIXct.

## Details

clock assumes that Date is a *naive* type, like naive-time. This means
that `date_today()` first looks up the current date-time in the
specified `zone`, then converts that to a Date, retaining the printed
time while dropping any information about that time zone.

## Examples

``` r
# Current date in the local time zone
date_today("")
#> [1] "2026-01-13"

# Current date in a specified time zone
date_today("Europe/London")
#> [1] "2026-01-13"

# Current date-time in that same time zone
date_now("Europe/London")
#> [1] "2026-01-13 16:16:11 GMT"
```
