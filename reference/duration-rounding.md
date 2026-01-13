# Duration rounding

- `duration_floor()` rounds a duration down to a multiple of the
  specified `precision`.

- `duration_ceiling()` rounds a duration up to a multiple of the
  specified `precision`.

- `duration_round()` rounds up or down depending on what is closer,
  rounding up on ties.

## Usage

``` r
duration_floor(x, precision, ..., n = 1L)

duration_ceiling(x, precision, ..., n = 1L)

duration_round(x, precision, ..., n = 1L)
```

## Arguments

- x:

  `[clock_duration]`

  A duration.

- precision:

  `[character(1)]`

  A precision. One of:

  - `"year"`

  - `"quarter"`

  - `"month"`

  - `"week"`

  - `"day"`

  - `"hour"`

  - `"minute"`

  - `"second"`

  - `"millisecond"`

  - `"microsecond"`

  - `"nanosecond"`

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A positive integer specifying the multiple of `precision` to use.

## Value

`x` rounded to the `precision`.

## Details

You can floor calendrical durations to other calendrical durations, and
chronological durations to other chronological durations, but you can't
floor a chronological duration to a calendrical duration (such as
flooring from day to month). For more information, see the documentation
on the [duration
helper](https://clock.r-lib.org/reference/duration-helper.md) page.

## Examples

``` r
x <- duration_seconds(c(86399, 86401))

duration_floor(x, "day")
#> <duration<day>[2]>
#> [1] 0 1
duration_ceiling(x, "day")
#> <duration<day>[2]>
#> [1] 1 2

# Can't floor from a chronological duration (seconds)
# to a calendrical duration (months)
try(duration_floor(x, "month"))
#> Error in duration_floor(x, "month") : 
#>   Can't floor from a chronological precision (second) to a
#> calendrical precision (month).

# Every 2 days, using an origin of day 0
y <- duration_seconds(c(0, 86400, 86400 * 2, 86400 * 3))
duration_floor(y, "day", n = 2)
#> <duration<day>[4]>
#> [1] 0 0 2 2

# Shifting the origin to be day 1
origin <- duration_days(1)
duration_floor(y - origin, "day", n = 2) + origin
#> <duration<day>[4]>
#> [1] -1 1  1  3 

# Rounding will round ties up
half_day <- 86400 / 2
half_day_durations <- duration_seconds(c(half_day - 1, half_day, half_day + 1))
duration_round(half_day_durations, "day")
#> <duration<day>[3]>
#> [1] 0 1 1

# With larger units
x <- duration_months(c(0, 15, 24))
duration_floor(x, "year")
#> <duration<year>[3]>
#> [1] 0 1 2
duration_floor(x, "quarter")
#> <duration<quarter>[3]>
#> [1] 0 5 8
```
