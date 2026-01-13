# Widen a calendar to a more precise precision

`calendar_widen()` widens `x` to the specified `precision`. It does so
by setting new components to their smallest value.

Each calendar has its own help page describing the precisions that you
can widen to:

- [year-month-day](https://clock.r-lib.org/dev/reference/year-month-day-widen.md)

- [year-month-weekday](https://clock.r-lib.org/dev/reference/year-month-weekday-widen.md)

- [year-week-day](https://clock.r-lib.org/dev/reference/year-week-day-widen.md)

- [iso-year-week-day](https://clock.r-lib.org/dev/reference/iso-year-week-day-widen.md)

- [year-quarter-day](https://clock.r-lib.org/dev/reference/year-quarter-day-widen.md)

- [year-day](https://clock.r-lib.org/dev/reference/year-day-widen.md)

## Usage

``` r
calendar_widen(x, precision)
```

## Arguments

- x:

  `[calendar]`

  A calendar vector.

- precision:

  `[character(1)]`

  A precision. Allowed precisions are dependent on the calendar used.

## Value

`x` widened to the supplied `precision`.

## Details

A subsecond precision `x` cannot be widened. You cannot widen from, say,
`"millisecond"` to `"nanosecond"` precision. clock operates under the
philosophy that once you have set the subsecond precision of a calendar,
it is "locked in" at that precision. If you expected this to multiply
the milliseconds by 1e6 to get to nanosecond precision, you probably
want to convert to a time point first, and use
[`time_point_cast()`](https://clock.r-lib.org/dev/reference/time_point_cast.md).

Generally, clock treats calendars at a specific precision as a *range*
of values. For example, a month precision year-month-day is treated as a
range over `[yyyy-mm-01, yyyy-mm-last]`, with no assumption about the
day of the month. However, occasionally it is useful to quickly widen a
calendar, assuming that you want the beginning of this range to be used
for each component. This is where `calendar_widen()` can come in handy.

## Examples

``` r
# Month precision
x <- year_month_day(2019, 1)
x
#> <year_month_day<month>[1]>
#> [1] "2019-01"

# Widen to day precision
calendar_widen(x, "day")
#> <year_month_day<day>[1]>
#> [1] "2019-01-01"

# Or second precision
calendar_widen(x, "second")
#> <year_month_day<second>[1]>
#> [1] "2019-01-01T00:00:00"
```
