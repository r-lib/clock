# Shifting: time point

`time_point_shift()` shifts `x` to the `target` weekday. You can shift
to the next or previous weekday. If `x` is currently on the `target`
weekday, you can choose to leave it alone or advance it to the next
instance of the `target`.

Weekday shifting is one of the easiest ways to floor by week while
controlling what is considered the first day of the week. You can also
accomplish this with the `origin` argument of
[`time_point_floor()`](https://clock.r-lib.org/dev/reference/time-point-rounding.md),
but this is slightly easier.

## Usage

``` r
time_point_shift(x, target, ..., which = "next", boundary = "keep")
```

## Arguments

- x:

  `[clock_time_point]`

  A time point.

- target:

  `[weekday]`

  A weekday created from
  [`weekday()`](https://clock.r-lib.org/dev/reference/weekday.md) to
  target.

  Generally this is length 1, but can also be the same length as `x`.

- ...:

  These dots are for future extensions and must be empty.

- which:

  `[character(1)]`

  One of:

  - `"next"`: Shift to the next instance of the `target` weekday.

  - `"previous`: Shift to the previous instance of the `target` weekday.

- boundary:

  `[character(1)]`

  One of:

  - `"keep"`: If `x` is currently on the `target` weekday, return it.

  - `"advance"`: If `x` is currently on the `target` weekday, advance it
    anyways.

## Value

`x` shifted to the `target` weekday.

## Examples

``` r
x <- as_naive_time(year_month_day(2019, 1, 1:2))

# A Tuesday and Wednesday
as_weekday(x)
#> <weekday[2]>
#> [1] Tue Wed

monday <- weekday(clock_weekdays$monday)

# Shift to the next Monday
time_point_shift(x, monday)
#> <naive_time<day>[2]>
#> [1] "2019-01-07" "2019-01-07"

# Shift to the previous Monday
# This is an easy way to "floor by week" with a target weekday in mind
time_point_shift(x, monday, which = "previous")
#> <naive_time<day>[2]>
#> [1] "2018-12-31" "2018-12-31"

# What about Tuesday?
tuesday <- weekday(clock_weekdays$tuesday)

# Notice that the day that was currently on a Tuesday was not shifted
time_point_shift(x, tuesday)
#> <naive_time<day>[2]>
#> [1] "2019-01-01" "2019-01-08"

# You can force it to `"advance"`
time_point_shift(x, tuesday, boundary = "advance")
#> <naive_time<day>[2]>
#> [1] "2019-01-08" "2019-01-08"
```
