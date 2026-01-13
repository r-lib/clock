# Calendar: year-month-weekday

`year_month_weekday()` constructs a calendar vector from the Gregorian
year, month, weekday, and index specifying that this is the n-th weekday
of the month.

## Usage

``` r
year_month_weekday(
  year,
  month = NULL,
  day = NULL,
  index = NULL,
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

  `[integer / NULL]`

  The weekday of the month. Values `[1, 7]` are allowed, where `1` is
  Sunday and `7` is Saturday.

- index:

  `[integer / "last" / NULL]`

  The index specifying that `day` is the n-th weekday of the month.
  Values `[1, 5]` are allowed.

  If `"last"`, then the last instance of `day` in the current month is
  returned.

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

A year-month-weekday calendar vector.

## Details

Fields are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

Fields are collected in order until the first `NULL` field is located.
No fields after the first `NULL` field are used.

## Examples

``` r
# All Fridays in January, 2019
# Note that there was no 5th Friday in January
x <- year_month_weekday(
  2019,
  clock_months$january,
  clock_weekdays$friday,
  1:5
)
x
#> <year_month_weekday<day>[5]>
#> [1] "2019-01-Fri[1]" "2019-01-Fri[2]" "2019-01-Fri[3]" "2019-01-Fri[4]"
#> [5] "2019-01-Fri[5]"

invalid_detect(x)
#> [1] FALSE FALSE FALSE FALSE  TRUE

# Resolve this invalid date by using the previous valid date
invalid_resolve(x, invalid = "previous")
#> <year_month_weekday<day>[5]>
#> [1] "2019-01-Fri[1]" "2019-01-Fri[2]" "2019-01-Fri[3]" "2019-01-Fri[4]"
#> [5] "2019-01-Thu[5]"
```
