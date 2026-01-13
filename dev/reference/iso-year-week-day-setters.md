# Setters: iso-year-week-day

These are iso-year-week-day methods for the [setter
generics](https://clock.r-lib.org/dev/reference/clock-setters.md).

- [`set_year()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the ISO year.

- [`set_week()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the ISO week of the year. Valid values are in the range of
  `[1, 53]`.

- [`set_day()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the day of the week. Valid values are in the range of `[1, 7]`,
  with 1 = Monday, and 7 = Sunday.

- There are sub-daily setters for setting more precise components.

## Usage

``` r
# S3 method for class 'clock_iso_year_week_day'
set_year(x, value, ...)

# S3 method for class 'clock_iso_year_week_day'
set_week(x, value, ...)

# S3 method for class 'clock_iso_year_week_day'
set_day(x, value, ...)

# S3 method for class 'clock_iso_year_week_day'
set_hour(x, value, ...)

# S3 method for class 'clock_iso_year_week_day'
set_minute(x, value, ...)

# S3 method for class 'clock_iso_year_week_day'
set_second(x, value, ...)

# S3 method for class 'clock_iso_year_week_day'
set_millisecond(x, value, ...)

# S3 method for class 'clock_iso_year_week_day'
set_microsecond(x, value, ...)

# S3 method for class 'clock_iso_year_week_day'
set_nanosecond(x, value, ...)
```

## Arguments

- x:

  `[clock_iso_year_week_day]`

  A iso-year-week-day vector.

- value:

  `[integer / "last"]`

  The value to set the component to.

  For
  [`set_week()`](https://clock.r-lib.org/dev/reference/clock-setters.md),
  this can also be `"last"` to adjust to the last week of the current
  ISO year.

- ...:

  These dots are for future extensions and must be empty.

## Value

`x` with the component set.

## Examples

``` r
# Year precision vector
x <- iso_year_week_day(2019:2023)

# Promote to week precision by setting the week
# (Note that some ISO weeks have 52 weeks, and others have 53)
x <- set_week(x, "last")
x
#> <iso_year_week_day<week>[5]>
#> [1] "2019-W52" "2020-W53" "2021-W52" "2022-W52" "2023-W52"

# Set to an invalid week
invalid <- set_week(x, 53)
invalid
#> <iso_year_week_day<week>[5]>
#> [1] "2019-W53" "2020-W53" "2021-W53" "2022-W53" "2023-W53"

# Here are the invalid ones (they only have 52 weeks)
invalid[invalid_detect(invalid)]
#> <iso_year_week_day<week>[4]>
#> [1] "2019-W53" "2021-W53" "2022-W53" "2023-W53"

# Resolve the invalid dates by choosing the previous/next valid moment
invalid_resolve(invalid, invalid = "previous")
#> <iso_year_week_day<week>[5]>
#> [1] "2019-W52" "2020-W53" "2021-W52" "2022-W52" "2023-W52"
invalid_resolve(invalid, invalid = "next")
#> <iso_year_week_day<week>[5]>
#> [1] "2020-W01" "2020-W53" "2022-W01" "2023-W01" "2024-W01"
```
