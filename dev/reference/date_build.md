# Building: date

`date_build()` builds a Date from it's individual components.

## Usage

``` r
date_build(year, month = 1L, day = 1L, ..., invalid = NULL)
```

## Arguments

- year:

  `[integer]`

  The year. Values `[-32767, 32767]` are generally allowed.

- month:

  `[integer]`

  The month. Values `[1, 12]` are allowed.

- day:

  `[integer / "last"]`

  The day of the month. Values `[1, 31]` are allowed.

  If `"last"`, then the last day of the month is returned.

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

A Date.

## Details

Components are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

## Examples

``` r
date_build(2019)
#> [1] "2019-01-01"
date_build(2019, 1:3)
#> [1] "2019-01-01" "2019-02-01" "2019-03-01"

# Generating invalid dates will trigger an error
try(date_build(2019, 1:12, 31))
#> Error in invalid_resolve(x, invalid = invalid) : 
#>   Invalid date found at location 2.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.

# You can resolve this with `invalid`
date_build(2019, 1:12, 31, invalid = "previous")
#>  [1] "2019-01-31" "2019-02-28" "2019-03-31" "2019-04-30" "2019-05-31"
#>  [6] "2019-06-30" "2019-07-31" "2019-08-31" "2019-09-30" "2019-10-31"
#> [11] "2019-11-30" "2019-12-31"

# But this particular case (the last day of the month) is better
# specified as:
date_build(2019, 1:12, "last")
#>  [1] "2019-01-31" "2019-02-28" "2019-03-31" "2019-04-30" "2019-05-31"
#>  [6] "2019-06-30" "2019-07-31" "2019-08-31" "2019-09-30" "2019-10-31"
#> [11] "2019-11-30" "2019-12-31"
```
