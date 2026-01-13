# Invalid calendar dates

This family of functions is for working with *invalid* calendar dates.

Invalid dates represent dates made up of valid individual components,
which taken as a whole don't represent valid calendar dates. For
example, for
[`year_month_day()`](https://clock.r-lib.org/dev/reference/year_month_day.md)
the following component ranges are valid: `year: [-32767, 32767]`,
`month: [1, 12]`, `day: [1, 31]`. However, the date `2019-02-31` doesn't
exist even though it is made up of valid components. This is an example
of an invalid date.

Invalid dates are allowed in clock, provided that they are eventually
resolved by using `invalid_resolve()` or by manually resolving them
through arithmetic or setter functions.

## Usage

``` r
invalid_detect(x)

invalid_any(x)

invalid_count(x)

invalid_remove(x)

invalid_resolve(x, ..., invalid = NULL)
```

## Arguments

- x:

  `[calendar]`

  A calendar vector.

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

- `invalid_detect()`: Returns a logical vector detecting invalid dates.

- `invalid_any()`: Returns `TRUE` if any invalid dates are detected.

- `invalid_count()`: Returns a single integer containing the number of
  invalid dates.

- `invalid_remove()`: Returns `x` with invalid dates removed.

- `invalid_resolve()`: Returns `x` with invalid dates resolved using the
  `invalid` strategy.

## Details

Invalid dates must be resolved before converting them to a time point.

It is recommended to use `"previous"` or `"next"` for resolving invalid
dates, as these ensure that *relative ordering* among `x` is maintained.
This is a often a very important property to maintain when doing time
series data analysis. See the examples for more information.

## Examples

``` r
# Invalid date
x <- year_month_day(2019, 04, 30:31, c(3, 2), 30, 00)
x
#> <year_month_day<second>[2]>
#> [1] "2019-04-30T03:30:00" "2019-04-31T02:30:00"

invalid_detect(x)
#> [1] FALSE  TRUE

# Previous valid moment in time
x_previous <- invalid_resolve(x, invalid = "previous")
x_previous
#> <year_month_day<second>[2]>
#> [1] "2019-04-30T03:30:00" "2019-04-30T23:59:59"

# Previous valid day, retaining time of day
x_previous_day <- invalid_resolve(x, invalid = "previous-day")
x_previous_day
#> <year_month_day<second>[2]>
#> [1] "2019-04-30T03:30:00" "2019-04-30T02:30:00"

# Note that `"previous"` retains the relative ordering in `x`
x[1] < x[2]
#> [1] TRUE
x_previous[1] < x_previous[2]
#> [1] TRUE

# But `"previous-day"` here does not!
x_previous_day[1] < x_previous_day[2]
#> [1] FALSE

# Remove invalid dates entirely
invalid_remove(x)
#> <year_month_day<second>[1]>
#> [1] "2019-04-30T03:30:00"

y <- year_quarter_day(2019, 1, 90:92)
y
#> <year_quarter_day<January><day>[3]>
#> [1] "2019-Q1-90" "2019-Q1-91" "2019-Q1-92"

# Overflow rolls forward by the number of days between `y` and the previous
# valid date
invalid_resolve(y, invalid = "overflow")
#> <year_quarter_day<January><day>[3]>
#> [1] "2019-Q1-90" "2019-Q2-01" "2019-Q2-02"
```
