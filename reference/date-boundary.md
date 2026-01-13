# Boundaries: date

This is a Date method for the
[`date_start()`](https://clock.r-lib.org/reference/date-and-date-time-boundary.md)
and
[`date_end()`](https://clock.r-lib.org/reference/date-and-date-time-boundary.md)
generics.

## Usage

``` r
# S3 method for class 'Date'
date_start(x, precision, ..., invalid = NULL)

# S3 method for class 'Date'
date_end(x, precision, ..., invalid = NULL)
```

## Arguments

- x:

  `[Date]`

  A date vector.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"month"`

  - `"day"`

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

`x` but with some components altered to be at the boundary value.

## Examples

``` r
x <- date_build(2019:2021, 2:4, 3:5)
x
#> [1] "2019-02-03" "2020-03-04" "2021-04-05"

# Last day of the month
date_end(x, "month")
#> [1] "2019-02-28" "2020-03-31" "2021-04-30"

# Last day of the year
date_end(x, "year")
#> [1] "2019-12-31" "2020-12-31" "2021-12-31"

# First day of the year
date_start(x, "year")
#> [1] "2019-01-01" "2020-01-01" "2021-01-01"
```
