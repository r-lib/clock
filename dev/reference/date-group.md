# Group date components

This is a Date method for the
[`date_group()`](https://clock.r-lib.org/dev/reference/date_group.md)
generic.

[`date_group()`](https://clock.r-lib.org/dev/reference/date_group.md)
groups by a single component of a Date, such as month of the year, or
day of the month.

If you need to group by more complex components, like ISO weeks, or
quarters, convert to a calendar type that contains the component you are
interested in grouping by.

## Usage

``` r
# S3 method for class 'Date'
date_group(x, precision, ..., n = 1L, invalid = NULL)
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

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

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

`x`, grouped at `precision`.

## Examples

``` r
x <- as.Date("2019-01-01") + -3:5
x
#> [1] "2018-12-29" "2018-12-30" "2018-12-31" "2019-01-01" "2019-01-02"
#> [6] "2019-01-03" "2019-01-04" "2019-01-05" "2019-01-06"

# Group by 2 days of the current month.
# Note that this resets at the beginning of the month, creating day groups
# of [29, 30] [31] [01, 02] [03, 04].
date_group(x, "day", n = 2)
#> [1] "2018-12-29" "2018-12-29" "2018-12-31" "2019-01-01" "2019-01-01"
#> [6] "2019-01-03" "2019-01-03" "2019-01-05" "2019-01-05"

# Group by month
date_group(x, "month")
#> [1] "2018-12-01" "2018-12-01" "2018-12-01" "2019-01-01" "2019-01-01"
#> [6] "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01"
```
