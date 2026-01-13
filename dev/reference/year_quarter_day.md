# Calendar: year-quarter-day

`year_quarter_day()` constructs a calendar from the fiscal year, fiscal
quarter, and day of the quarter, along with a value determining which
month the fiscal year `start`s in.

## Usage

``` r
year_quarter_day(
  year,
  quarter = NULL,
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

  The fiscal year. Values `[-32767, 32767]` are generally allowed.

- quarter:

  `[integer / NULL]`

  The fiscal quarter. Values `[1, 4]` are allowed.

- day:

  `[integer / "last" / NULL]`

  The day of the quarter. Values `[1, 92]` are allowed.

  If `"last"`, the last day of the quarter is returned.

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

  The month to start the fiscal year in. 1 = January and 12 = December.

  If `NULL`, a `start` of January will be used.

- subsecond_precision:

  `[character(1) / NULL]`

  The precision to interpret `subsecond` as. One of: `"millisecond"`,
  `"microsecond"`, or `"nanosecond"`.

## Value

A year-quarter-day calendar vector.

## Details

Fields are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

Fields are collected in order until the first `NULL` field is located.
No fields after the first `NULL` field are used.

## Examples

``` r
# Year-quarter type
x <- year_quarter_day(2019, 1:4)
x
#> <year_quarter_day<January><quarter>[4]>
#> [1] "2019-Q1" "2019-Q2" "2019-Q3" "2019-Q4"

add_quarters(x, 2)
#> <year_quarter_day<January><quarter>[4]>
#> [1] "2019-Q3" "2019-Q4" "2020-Q1" "2020-Q2"

# Set the day to the last day of the quarter
x <- set_day(x, "last")
x
#> <year_quarter_day<January><day>[4]>
#> [1] "2019-Q1-90" "2019-Q2-91" "2019-Q3-92" "2019-Q4-92"

# Start the fiscal year in June
june <- 6L
y <- year_quarter_day(2019, 1:4, "last", start = june)

# Compare the year-month-day values that result from having different
# fiscal year start months
as_year_month_day(x)
#> <year_month_day<day>[4]>
#> [1] "2019-03-31" "2019-06-30" "2019-09-30" "2019-12-31"
as_year_month_day(y)
#> <year_month_day<day>[4]>
#> [1] "2018-08-31" "2018-11-30" "2019-02-28" "2019-05-31"
```
