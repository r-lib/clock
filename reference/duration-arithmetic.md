# Arithmetic: duration

These are duration methods for the [arithmetic
generics](https://clock.r-lib.org/reference/clock-arithmetic.md).

- [`add_years()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_quarters()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_months()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_weeks()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_days()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_hours()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_minutes()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_seconds()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_milliseconds()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_microseconds()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_nanoseconds()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

When adding to a duration using one of these functions, a second
duration is created based on the function name and `n`. The two
durations are then added together, and the precision of the result is
determined as the *more precise precision* of the two durations.

## Usage

``` r
# S3 method for class 'clock_duration'
add_years(x, n, ...)

# S3 method for class 'clock_duration'
add_quarters(x, n, ...)

# S3 method for class 'clock_duration'
add_months(x, n, ...)

# S3 method for class 'clock_duration'
add_weeks(x, n, ...)

# S3 method for class 'clock_duration'
add_days(x, n, ...)

# S3 method for class 'clock_duration'
add_hours(x, n, ...)

# S3 method for class 'clock_duration'
add_minutes(x, n, ...)

# S3 method for class 'clock_duration'
add_seconds(x, n, ...)

# S3 method for class 'clock_duration'
add_milliseconds(x, n, ...)

# S3 method for class 'clock_duration'
add_microseconds(x, n, ...)

# S3 method for class 'clock_duration'
add_nanoseconds(x, n, ...)
```

## Arguments

- x:

  `[clock_duration]`

  A duration vector.

- n:

  `[integer / clock_duration]`

  An integer vector to be converted to a duration, or a duration
  corresponding to the arithmetic function being used. This corresponds
  to the number of duration units to add. `n` may be negative to
  subtract units of duration.

- ...:

  These dots are for future extensions and must be empty.

## Value

`x` after performing the arithmetic, possibly with a more precise
precision.

## Details

You can add calendrical durations to other calendrical durations, and
chronological durations to other chronological durations, but you can't
add a chronological duration to a calendrical duration (such as adding
days and months). For more information, see the documentation on the
[duration helper](https://clock.r-lib.org/reference/duration-helper.md)
page.

`x` and `n` are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

## Examples

``` r
x <- duration_seconds(5)

# Addition in the same precision
add_seconds(x, 1:10)
#> <duration<second>[10]>
#>  [1] 6  7  8  9  10 11 12 13 14 15

# Addition with days, defined as 86400 seconds
add_days(x, 1)
#> <duration<second>[1]>
#> [1] 86405

# Similarly, if you start with days and add seconds, you get the common
# precision of the two back, which is seconds
y <- duration_days(1)
add_seconds(y, 5)
#> <duration<second>[1]>
#> [1] 86405

# But you can't add a chronological duration (days) and
# a calendrical duration (months)
try(add_months(y, 1))
#> Error in duration_arith(x, y, names, duration_plus_cpp) : 
#>   Can't combine `x` <duration<day>> and `y` <duration<month>>.
#> Can't combine calendrical durations with chronological durations.

# You can add years to a duration of months, which adds
# an additional 12 months / year
z <- duration_months(5)
add_years(z, 1)
#> <duration<month>[1]>
#> [1] 17
```
