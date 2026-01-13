# Calendar: iso-year-week-day

`iso_year_week_day()` constructs a calendar from the ISO year, week
number, and week day.

## Usage

``` r
iso_year_week_day(
  year,
  week = NULL,
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

  The ISO year. Values `[-32767, 32767]` are generally allowed.

- week:

  `[integer / "last" / NULL]`

  The ISO week. Values `[1, 53]` are allowed.

  If `"last"`, then the last week of the ISO year is returned.

- day:

  `[integer / NULL]`

  The day of the week. Values `[1, 7]` are allowed, with 1 = Monday and
  7 = Sunday, in accordance with the ISO specifications.

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

A iso-year-week-day calendar vector.

## Details

Fields are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

Fields are collected in order until the first `NULL` field is located.
No fields after the first `NULL` field are used.

## Examples

``` r
# Year-week
x <- iso_year_week_day(2019:2025, 1)
x
#> <iso_year_week_day<week>[7]>
#> [1] "2019-W01" "2020-W01" "2021-W01" "2022-W01" "2023-W01" "2024-W01"
#> [7] "2025-W01"

# 2nd day of the first ISO week in multiple years
iso_days <- set_day(x, clock_iso_weekdays$tuesday)
iso_days
#> <iso_year_week_day<day>[7]>
#> [1] "2019-W01-2" "2020-W01-2" "2021-W01-2" "2022-W01-2" "2023-W01-2"
#> [6] "2024-W01-2" "2025-W01-2"

# What year-month-day is this?
as_year_month_day(iso_days)
#> <year_month_day<day>[7]>
#> [1] "2019-01-01" "2019-12-31" "2021-01-05" "2022-01-04" "2023-01-03"
#> [6] "2024-01-02" "2024-12-31"
```
