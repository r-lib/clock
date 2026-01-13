# Calendar: year-day

`year_day()` constructs a calendar vector from the Gregorian year and
day of the year.

## Usage

``` r
year_day(
  year,
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

- day:

  `[integer / NULL]`

  The day of the year. Values `[1, 366]` are allowed.

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

A year-day calendar vector.

## Details

Fields are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

Fields are collected in order until the first `NULL` field is located.
No fields after the first `NULL` field are used.

## Examples

``` r
# Just the year
x <- year_day(2019:2025)
x
#> <year_day<year>[7]>
#> [1] "2019" "2020" "2021" "2022" "2023" "2024" "2025"

year_day(2020, 1:10)
#> <year_day<day>[10]>
#>  [1] "2020-001" "2020-002" "2020-003" "2020-004" "2020-005" "2020-006"
#>  [7] "2020-007" "2020-008" "2020-009" "2020-010"

# Last day of the year, accounting for leap years
year_day(2019:2021, "last")
#> <year_day<day>[3]>
#> [1] "2019-365" "2020-366" "2021-365"

# Precision can go all the way out to nanosecond
year_day(2019, 100, 2, 40, 45, 200, subsecond_precision = "nanosecond")
#> <year_day<nanosecond>[1]>
#> [1] "2019-100T02:40:45.000000200"
```
