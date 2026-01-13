# Calendar getters

This family of functions extract fields from a calendar vector. Each
calendar has its own set of supported getters, which are documented on
their own help page:

- [year-month-day](https://clock.r-lib.org/reference/year-month-day-getters.md)

- [year-month-weekday](https://clock.r-lib.org/reference/year-month-weekday-getters.md)

- [year-week-day](https://clock.r-lib.org/reference/year-week-day-getters.md)

- [iso-year-week-day](https://clock.r-lib.org/reference/iso-year-week-day-getters.md)

- [year-quarter-day](https://clock.r-lib.org/reference/year-quarter-day-getters.md)

- [year-day](https://clock.r-lib.org/reference/year-day-getters.md)

There are also convenience methods for extracting certain components
directly from R's native date and date-time types.

- [dates (Date)](https://clock.r-lib.org/reference/Date-getters.md)

- [date-times (POSIXct /
  POSIXlt)](https://clock.r-lib.org/reference/posixt-getters.md)

## Usage

``` r
get_year(x)

get_quarter(x)

get_month(x)

get_week(x)

get_day(x)

get_hour(x)

get_minute(x)

get_second(x)

get_millisecond(x)

get_microsecond(x)

get_nanosecond(x)

get_index(x)
```

## Arguments

- x:

  `[object]`

  An object to get the component from.

## Value

The component.

## Details

You cannot extract components directly from a time point type, such as
sys-time or naive-time. Convert it to a calendar type first. Similarly,
a zoned-time must be converted to either a sys-time or naive-time, and
then to a calendar type, to be able to extract components from it.

## Examples

``` r
x <- year_month_day(2019, 1:3, 5:7, 1, 20, 30)
get_month(x)
#> [1] 1 2 3
get_day(x)
#> [1] 5 6 7
get_second(x)
#> [1] 30 30 30
```
