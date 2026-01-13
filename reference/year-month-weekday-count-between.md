# Counting: year-month-weekday

This is a year-month-weekday method for the
[`calendar_count_between()`](https://clock.r-lib.org/reference/calendar-count-between.md)
generic. It counts the number of `precision` units between `start` and
`end` (i.e., the number of years or months).

## Usage

``` r
# S3 method for class 'clock_year_month_weekday'
calendar_count_between(start, end, precision, ..., n = 1L)
```

## Arguments

- start, end:

  `[clock_year_month_weekday]`

  A pair of year-month-weekday vectors. These will be recycled to their
  common size.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"quarter"`

  - `"month"`

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

## Value

An integer representing the number of `precision` units between `start`
and `end`.

## Details

Remember that year-month-weekday is not comparable when it is `"day"`
precision or finer, so this method is only defined for `"year"` and
`"month"` precision year-month-weekday objects.

`"quarter"` is equivalent to `"month"` precision with `n` set to
`n * 3L`.

## Examples

``` r
# Compute the number of months between two dates
x <- year_month_weekday(2001, 2)
y <- year_month_weekday(2021, c(1, 3))

calendar_count_between(x, y, "month")
#> [1] 239 241

# Remember that day precision or finer year-month-weekday objects
# are not comparable, so this won't work
x <- year_month_weekday(2001, 2, 1, 1)
try(calendar_count_between(x, x, "month"))
#> Error in vec_proxy_compare(x = x) : 
#>   'year_month_weekday' types with a precision of >= 'day' cannot be trivially compared or ordered. Convert to 'year_month_day' to compare using day-of-month values.
```
