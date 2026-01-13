# Calendar: year-month-day

`year_month_day()` constructs the most common calendar type using the
Gregorian year, month, day, and time of day components.

## Usage

``` r
year_month_day(
  year,
  month = NULL,
  day = NULL,
  hour = NULL,
  minute = NULL,
  second = NULL,
  subsecond = NULL,
  ...,
  subsecond_precision = NULL
)
```

## Arguments

- year:

  `[integer]`

  The year. Values `[-32767, 32767]` are generally allowed.

- month:

  `[integer / NULL]`

  The month. Values `[1, 12]` are allowed.

- day:

  `[integer / "last" / NULL]`

  The day of the month. Values `[1, 31]` are allowed.

  If `"last"`, then the last day of the month is returned.

- hour:

  `[integer / NULL]`

  The hour. Values `[0, 23]` are allowed.

- minute:

  `[integer / NULL]`

  The minute. Values `[0, 59]` are allowed.

- second:

  `[integer / NULL]`

  The second. Values `[0, 59]` are allowed.

- subsecond:

  `[integer / NULL]`

  The subsecond. If specified, `subsecond_precision` must also be
  specified to determine how to interpret the `subsecond`.

  If using milliseconds, values `[0, 999]` are allowed.

  If using microseconds, values `[0, 999999]` are allowed.

  If using nanoseconds, values `[0, 999999999]` are allowed.

- ...:

  These dots are for future extensions and must be empty.

- subsecond_precision:

  `[character(1) / NULL]`

  The precision to interpret `subsecond` as. One of: `"millisecond"`,
  `"microsecond"`, or `"nanosecond"`.

## Value

A year-month-day calendar vector.

## Details

Fields are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

Fields are collected in order until the first `NULL` field is located.
No fields after the first `NULL` field are used.

## Examples

``` r
# Just the year
x <- year_month_day(2019:2025)

# Year-month type
year_month_day(2020, 1:12)
#> <year_month_day<month>[12]>
#>  [1] "2020-01" "2020-02" "2020-03" "2020-04" "2020-05" "2020-06"
#>  [7] "2020-07" "2020-08" "2020-09" "2020-10" "2020-11" "2020-12"

# The most common use case involves year, month, and day fields
x <- year_month_day(2020, clock_months$january, 1:5)
x
#> <year_month_day<day>[5]>
#> [1] "2020-01-01" "2020-01-02" "2020-01-03" "2020-01-04" "2020-01-05"

# Precision can go all the way out to nanosecond
year_month_day(2019, 1, 2, 2, 40, 45, 200, subsecond_precision = "nanosecond")
#> <year_month_day<nanosecond>[1]>
#> [1] "2019-01-02T02:40:45.000000200"
```
