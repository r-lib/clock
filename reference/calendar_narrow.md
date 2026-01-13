# Narrow a calendar to a less precise precision

`calendar_narrow()` narrows `x` to the specified `precision`. It does so
by dropping components that represent a precision that is finer than
`precision`.

Each calendar has its own help page describing the precisions that you
can narrow to:

- [year-month-day](https://clock.r-lib.org/reference/year-month-day-narrow.md)

- [year-month-weekday](https://clock.r-lib.org/reference/year-month-weekday-narrow.md)

- [year-week-day](https://clock.r-lib.org/reference/year-week-day-narrow.md)

- [iso-year-week-day](https://clock.r-lib.org/reference/iso-year-week-day-narrow.md)

- [year-quarter-day](https://clock.r-lib.org/reference/year-quarter-day-narrow.md)

- [year-day](https://clock.r-lib.org/reference/year-day-narrow.md)

## Usage

``` r
calendar_narrow(x, precision)
```

## Arguments

- x:

  `[calendar]`

  A calendar vector.

- precision:

  `[character(1)]`

  A precision. Allowed precisions are dependent on the calendar used.

## Value

`x` narrowed to the supplied `precision`.

## Details

A subsecond precision `x` cannot be narrowed to another subsecond
precision. You cannot narrow from, say, `"nanosecond"` to
`"millisecond"` precision. clock operates under the philosophy that once
you have set the subsecond precision of a calendar, it is "locked in" at
that precision. If you expected this to use integer division to divide
the nanoseconds by 1e6 to get to millisecond precision, you probably
want to convert to a time point first, and use
[`time_point_floor()`](https://clock.r-lib.org/reference/time-point-rounding.md).

## Examples

``` r
# Hour precision
x <- year_month_day(2019, 1, 3, 4)
x
#> <year_month_day<hour>[1]>
#> [1] "2019-01-03T04"

# Narrowed to day precision
calendar_narrow(x, "day")
#> <year_month_day<day>[1]>
#> [1] "2019-01-03"

# Or month precision
calendar_narrow(x, "month")
#> <year_month_day<month>[1]>
#> [1] "2019-01"
```
