# Convert to a weekday

`as_weekday()` converts to a weekday type. This is normally useful for
converting to a weekday from a sys-time or naive-time. You can use this
function along with the *circular arithmetic* that weekday implements to
easily get to the "next Monday" or "previous Sunday".

## Usage

``` r
as_weekday(x, ...)
```

## Arguments

- x:

  `[object]`

  An object to convert to a weekday. Usually a sys-time or naive-time.

- ...:

  These dots are for future extensions and must be empty.

## Value

A weekday.

## Examples

``` r
x <- as_naive_time(year_month_day(2019, 01, 05))

# This is a Saturday!
as_weekday(x)
#> <weekday[1]>
#> [1] Sat

# See the examples in `?weekday` for more usage.
```
