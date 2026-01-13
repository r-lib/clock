# Boundaries: calendars

- `calendar_start()` computes the start of a calendar at a particular
  `precision`, such as the "start of the quarter".

- `calendar_end()` computes the end of a calendar at a particular
  `precision`, such as the "end of the month".

For both `calendar_start()` and `calendar_end()`, the precision of `x`
is always retained.

Each calendar has its own help page describing the precisions that you
can compute a boundary at:

- [year-month-day](https://clock.r-lib.org/dev/reference/year-month-day-boundary.md)

- [year-month-weekday](https://clock.r-lib.org/dev/reference/year-month-weekday-boundary.md)

- [year-week-day](https://clock.r-lib.org/dev/reference/year-week-day-boundary.md)

- [iso-year-week-day](https://clock.r-lib.org/dev/reference/iso-year-week-day-boundary.md)

- [year-quarter-day](https://clock.r-lib.org/dev/reference/year-quarter-day-boundary.md)

- [year-day](https://clock.r-lib.org/dev/reference/year-day-boundary.md)

## Usage

``` r
calendar_start(x, precision)

calendar_end(x, precision)
```

## Arguments

- x:

  `[calendar]`

  A calendar vector.

- precision:

  `[character(1)]`

  A precision. Allowed precisions are dependent on the calendar used.

## Value

`x` at the same precision, but with some components altered to be at the
boundary value.

## Examples

``` r
# Hour precision
x <- year_month_day(2019, 2:4, 5, 6)
x
#> <year_month_day<hour>[3]>
#> [1] "2019-02-05T06" "2019-03-05T06" "2019-04-05T06"

# Compute the start of the month
calendar_start(x, "month")
#> <year_month_day<hour>[3]>
#> [1] "2019-02-01T00" "2019-03-01T00" "2019-04-01T00"

# Or the end of the month, notice that the hour value is adjusted as well
calendar_end(x, "month")
#> <year_month_day<hour>[3]>
#> [1] "2019-02-28T23" "2019-03-31T23" "2019-04-30T23"
```
