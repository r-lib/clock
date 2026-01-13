# Calendar setters

This family of functions sets fields in a calendar vector. Each calendar
has its own set of supported setters, which are documented on their own
help page:

- [year-month-day](https://clock.r-lib.org/reference/year-month-day-setters.md)

- [year-month-weekday](https://clock.r-lib.org/reference/year-month-weekday-setters.md)

- [year-week-day](https://clock.r-lib.org/reference/year-week-day-setters.md)

- [iso-year-week-day](https://clock.r-lib.org/reference/iso-year-week-day-setters.md)

- [year-quarter-day](https://clock.r-lib.org/reference/year-quarter-day-setters.md)

- [year-day](https://clock.r-lib.org/reference/year-day-setters.md)

There are also convenience methods for setting certain components
directly on R's native date and date-time types.

- [dates (Date)](https://clock.r-lib.org/reference/Date-setters.md)

- [date-times (POSIXct /
  POSIXlt)](https://clock.r-lib.org/reference/posixt-setters.md)

Some general rules about setting components on calendar types:

- You can only set components that are relevant to the calendar type
  that you are working with. For example, you can't set the quarter of a
  year-month-day type. You'd have to convert to year-quarter-day first.

- You can set a component that is at the current precision, or one level
  of precision more precise than the current precision. For example, you
  can set the day field of a month precision year-month-day type, but
  not the hour field.

- Setting a component can result in an *invalid date*, such as
  `set_day(year_month_day(2019, 02), 31)`, as long as it is eventually
  resolved either manually or with a strategy from
  [`invalid_resolve()`](https://clock.r-lib.org/reference/clock-invalid.md).

- With sub-second precisions, you can only set the component
  corresponding to the precision that you are at. For example, you can
  set the nanoseconds of the second while at nanosecond precision, but
  not milliseconds.

## Usage

``` r
set_year(x, value, ...)

set_quarter(x, value, ...)

set_month(x, value, ...)

set_week(x, value, ...)

set_day(x, value, ...)

set_hour(x, value, ...)

set_minute(x, value, ...)

set_second(x, value, ...)

set_millisecond(x, value, ...)

set_microsecond(x, value, ...)

set_nanosecond(x, value, ...)

set_index(x, value, ...)
```

## Arguments

- x:

  `[object]`

  An object to set the component for.

- value:

  `[integer]`

  The value to set the component to.

- ...:

  These dots are for future extensions and must be empty.

## Value

`x` with the component set.

## Details

You cannot set components directly on a time point type, such as
sys-time or naive-time. Convert it to a calendar type first. Similarly,
a zoned-time must be converted to either a sys-time or naive-time, and
then to a calendar type, to be able to set components on it.

## Examples

``` r
x <- year_month_day(2019, 1:3)

# Set the day
set_day(x, 12:14)
#> <year_month_day<day>[3]>
#> [1] "2019-01-12" "2019-02-13" "2019-03-14"

# Set to the "last" day of the month
set_day(x, "last")
#> <year_month_day<day>[3]>
#> [1] "2019-01-31" "2019-02-28" "2019-03-31"
```
