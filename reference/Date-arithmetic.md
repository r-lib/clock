# Arithmetic: date

These are Date methods for the [arithmetic
generics](https://clock.r-lib.org/reference/clock-arithmetic.md).

Calendrical based arithmetic:

These functions convert to a year-month-day calendar, perform the
arithmetic, then convert back to a Date.

- [`add_years()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_quarters()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_months()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

Time point based arithmetic:

These functions convert to a time point, perform the arithmetic, then
convert back to a Date.

- [`add_weeks()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_days()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

## Usage

``` r
# S3 method for class 'Date'
add_years(x, n, ..., invalid = NULL)

# S3 method for class 'Date'
add_quarters(x, n, ..., invalid = NULL)

# S3 method for class 'Date'
add_months(x, n, ..., invalid = NULL)

# S3 method for class 'Date'
add_weeks(x, n, ...)

# S3 method for class 'Date'
add_days(x, n, ...)
```

## Arguments

- x:

  `[Date]`

  A Date vector.

- n:

  `[integer / clock_duration]`

  An integer vector to be converted to a duration, or a duration
  corresponding to the arithmetic function being used. This corresponds
  to the number of duration units to add. `n` may be negative to
  subtract units of duration.

- ...:

  These dots are for future extensions and must be empty.

- invalid:

  `[character(1) / NULL]`

  One of the following invalid date resolution strategies:

  - `"previous"`: The previous valid instant in time.

  - `"previous-day"`: The previous valid day in time, keeping the time
    of day.

  - `"next"`: The next valid instant in time.

  - `"next-day"`: The next valid day in time, keeping the time of day.

  - `"overflow"`: Overflow by the number of days that the input is
    invalid by. Time of day is dropped.

  - `"overflow-day"`: Overflow by the number of days that the input is
    invalid by. Time of day is kept.

  - `"NA"`: Replace invalid dates with `NA`.

  - `"error"`: Error on invalid dates.

  Using either `"previous"` or `"next"` is generally recommended, as
  these two strategies maintain the *relative ordering* between elements
  of the input.

  If `NULL`, defaults to `"error"`.

  If `getOption("clock.strict")` is `TRUE`, `invalid` must be supplied
  and cannot be `NULL`. This is a convenient way to make production code
  robust to invalid dates.

## Value

`x` after performing the arithmetic.

## Details

Adding a single quarter with
[`add_quarters()`](https://clock.r-lib.org/reference/clock-arithmetic.md)
is equivalent to adding 3 months.

`x` and `n` are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

Only calendrical based arithmetic has the potential to generate invalid
dates. Time point based arithmetic, like adding days, will always
generate a valid date.

## Examples

``` r
x <- as.Date("2019-01-01")

add_years(x, 1:5)
#> [1] "2020-01-01" "2021-01-01" "2022-01-01" "2023-01-01" "2024-01-01"

y <- as.Date("2019-01-31")

# Adding 1 month to `y` generates an invalid date. Unlike year-month-day
# types, R's native Date type cannot handle invalid dates, so you must
# resolve them immediately. If you don't you get an error:
try(add_months(y, 1:2))
#> Error in invalid_resolve(x, invalid = invalid) : 
#>   Invalid date found at location 1.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.
add_months(as_year_month_day(y), 1:2)
#> <year_month_day<day>[2]>
#> [1] "2019-02-31" "2019-03-31"

# Resolve invalid dates by specifying an invalid date resolution strategy
# with the `invalid` argument. Using `"previous"` here sets the date to
# the previous valid date - i.e. the end of the month.
add_months(y, 1:2, invalid = "previous")
#> [1] "2019-02-28" "2019-03-31"
```
