# Sequences: date

This is a Date method for the
[`date_seq()`](https://clock.r-lib.org/reference/date_seq.md) generic.

[`date_seq()`](https://clock.r-lib.org/reference/date_seq.md) generates
a date (Date) sequence.

When calling
[`date_seq()`](https://clock.r-lib.org/reference/date_seq.md), exactly
two of the following must be specified:

- `to`

- `by`

- `total_size`

## Usage

``` r
# S3 method for class 'Date'
date_seq(from, ..., to = NULL, by = NULL, total_size = NULL, invalid = NULL)
```

## Arguments

- from:

  `[Date(1)]`

  A date to start the sequence from.

- ...:

  These dots are for future extensions and must be empty.

- to:

  `[Date(1) / NULL]`

  A date to stop the sequence at.

  `to` is only included in the result if the resulting sequence divides
  the distance between `from` and `to` exactly.

  If `to` is supplied along with `by`, all components of `to` more
  precise than the precision of `by` must match `from` exactly. For
  example, if `by = duration_months(1)`, the day component of `to` must
  match the day component of `from`. This ensures that the generated
  sequence is, at a minimum, a weakly monotonic sequence of dates.

- by:

  `[integer(1) / clock_duration(1) / NULL]`

  The unit to increment the sequence by.

  If `by` is an integer, it is equivalent to `duration_days(by)`.

  If `by` is a duration, it is allowed to have a precision of:

  - year

  - quarter

  - month

  - week

  - day

- total_size:

  `[positive integer(1) / NULL]`

  The size of the resulting sequence.

  If specified alongside `to`, this must generate a non-fractional
  sequence between `from` and `to`.

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

A date vector.

## Examples

``` r
from <- date_build(2019, 1)
to <- date_build(2019, 4)

# Defaults to daily sequence
date_seq(from, to = to, by = 7)
#>  [1] "2019-01-01" "2019-01-08" "2019-01-15" "2019-01-22" "2019-01-29"
#>  [6] "2019-02-05" "2019-02-12" "2019-02-19" "2019-02-26" "2019-03-05"
#> [11] "2019-03-12" "2019-03-19" "2019-03-26"

# Use durations to change to monthly or yearly sequences
date_seq(from, to = to, by = duration_months(1))
#> [1] "2019-01-01" "2019-02-01" "2019-03-01" "2019-04-01"
date_seq(from, by = duration_years(-2), total_size = 3)
#> [1] "2019-01-01" "2017-01-01" "2015-01-01"

# Note that components of `to` more precise than the precision of `by`
# must match `from` exactly. For example, this is not well defined:
from <- date_build(2019, 5, 2)
to <- date_build(2025, 7, 5)
try(date_seq(from, to = to, by = duration_years(1)))
#> Error in date_seq(from, to = to, by = duration_years(1)) : 
#>   All components of `from` and `to` more precise than "year" must
#> match.
#> ℹ `from` is "2019-05-02".
#> ℹ `to` is "2025-07-05".

# The month and day components of `to` must match `from`
to <- date_build(2025, 5, 2)
date_seq(from, to = to, by = duration_years(1))
#> [1] "2019-05-02" "2020-05-02" "2021-05-02" "2022-05-02" "2023-05-02"
#> [6] "2024-05-02" "2025-05-02"

# ---------------------------------------------------------------------------

# Invalid dates must be resolved with the `invalid` argument
from <- date_build(2019, 1, 31)
to <- date_build(2019, 12, 31)

try(date_seq(from, to = to, by = duration_months(1)))
#> Error in invalid_resolve(out, invalid = invalid) : 
#>   Invalid date found at location 2.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.
date_seq(from, to = to, by = duration_months(1), invalid = "previous")
#>  [1] "2019-01-31" "2019-02-28" "2019-03-31" "2019-04-30" "2019-05-31"
#>  [6] "2019-06-30" "2019-07-31" "2019-08-31" "2019-09-30" "2019-10-31"
#> [11] "2019-11-30" "2019-12-31"

# Compare this to the base R result, which is often a source of confusion
seq(from, to = to, by = "1 month")
#>  [1] "2019-01-31" "2019-03-03" "2019-03-31" "2019-05-01" "2019-05-31"
#>  [6] "2019-07-01" "2019-07-31" "2019-08-31" "2019-10-01" "2019-10-31"
#> [11] "2019-12-01" "2019-12-31"

# This is equivalent to the overflow invalid resolution strategy
date_seq(from, to = to, by = duration_months(1), invalid = "overflow")
#>  [1] "2019-01-31" "2019-03-03" "2019-03-31" "2019-05-01" "2019-05-31"
#>  [6] "2019-07-01" "2019-07-31" "2019-08-31" "2019-10-01" "2019-10-31"
#> [11] "2019-12-01" "2019-12-31"

# ---------------------------------------------------------------------------

# Usage of `to` and `total_size` must generate a non-fractional sequence
# between `from` and `to`
from <- date_build(2019, 1, 1)
to <- date_build(2019, 1, 4)

# These are fine
date_seq(from, to = to, total_size = 2)
#> [1] "2019-01-01" "2019-01-04"
date_seq(from, to = to, total_size = 4)
#> [1] "2019-01-01" "2019-01-02" "2019-01-03" "2019-01-04"

# But this is not!
try(date_seq(from, to = to, total_size = 3))
#> Error : The supplied output size does not result in a non-fractional sequence between `from` and `to`.
```
