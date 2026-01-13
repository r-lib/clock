# Calendar: year-week-day

`year_week_day()` constructs a calendar from the year, week number, week
day, and the `start` of the week.

Using `start = clock_weekdays$monday` represents the ISO week calendar
and is equivalent to using
[`iso_year_week_day()`](https://clock.r-lib.org/dev/reference/iso_year_week_day.md).

Using `start = clock_weekdays$sunday` is how Epidemiologists encode
their week-based data.

## Usage

``` r
year_week_day(
  year,
  week = NULL,
  day = NULL,
  hour = NULL,
  minute = NULL,
  second = NULL,
  subsecond = NULL,
  ...,
  start = NULL,
  subsecond_precision = NULL
)
```

## Arguments

- year:

  `[integer]`

  The year. Values `[-32767, 32767]` are generally allowed.

- week:

  `[integer / "last" / NULL]`

  The week. Values `[1, 53]` are allowed.

  If `"last"`, then the last week of the year is returned.

- day:

  `[integer / NULL]`

  The day of the week. Values `[1, 7]` are allowed, with
  `1 = start of week` and `7 = end of week`, in accordance with `start`.

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

- start:

  `[integer(1) / NULL]`

  The day to consider the start of the week. 1 = Sunday and 7 =
  Saturday.

  Use
  [clock_weekdays](https://clock.r-lib.org/dev/reference/clock-codes.md)
  for a readable way to specify the start.

  If `NULL`, a `start` of Sunday will be used.

- subsecond_precision:

  `[character(1) / NULL]`

  The precision to interpret `subsecond` as. One of: `"millisecond"`,
  `"microsecond"`, or `"nanosecond"`.

## Value

A year-week-day calendar vector.

## Details

Fields are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

Fields are collected in order until the first `NULL` field is located.
No fields after the first `NULL` field are used.

## Examples

``` r
# Year-week
x <- year_week_day(2019:2025, "last")
x
#> <year_week_day<Sunday><week>[7]>
#> [1] "2019-W52" "2020-W53" "2021-W52" "2022-W52" "2023-W52" "2024-W52"
#> [7] "2025-W53"

# Start the week on Monday
y <- year_week_day(2019:2025, "last", start = clock_weekdays$monday)
y
#> <year_week_day<Monday><week>[7]>
#> [1] "2019-W52" "2020-W53" "2021-W52" "2022-W52" "2023-W52" "2024-W52"
#> [7] "2025-W52"

# Last days of the year
as_year_month_day(set_day(x, 7))
#> <year_month_day<day>[7]>
#> [1] "2019-12-28" "2021-01-02" "2022-01-01" "2022-12-31" "2023-12-30"
#> [6] "2024-12-28" "2026-01-03"
as_year_month_day(set_day(y, 7))
#> <year_month_day<day>[7]>
#> [1] "2019-12-29" "2021-01-03" "2022-01-02" "2023-01-01" "2023-12-31"
#> [6] "2024-12-29" "2025-12-28"
```
