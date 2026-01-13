# Arithmetic: date-time

These are POSIXct/POSIXlt methods for the [arithmetic
generics](https://clock.r-lib.org/reference/clock-arithmetic.md).

Calendrical based arithmetic:

These functions convert to a naive-time, then to a year-month-day,
perform the arithmetic, then convert back to a date-time.

- [`add_years()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_quarters()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_months()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

Naive-time based arithmetic:

These functions convert to a naive-time, perform the arithmetic, then
convert back to a date-time.

- [`add_weeks()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_days()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

Sys-time based arithmetic:

These functions convert to a sys-time, perform the arithmetic, then
convert back to a date-time.

- [`add_hours()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_minutes()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_seconds()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

## Usage

``` r
# S3 method for class 'POSIXt'
add_years(x, n, ..., invalid = NULL, nonexistent = NULL, ambiguous = x)

# S3 method for class 'POSIXt'
add_quarters(x, n, ..., invalid = NULL, nonexistent = NULL, ambiguous = x)

# S3 method for class 'POSIXt'
add_months(x, n, ..., invalid = NULL, nonexistent = NULL, ambiguous = x)

# S3 method for class 'POSIXt'
add_weeks(x, n, ..., nonexistent = NULL, ambiguous = x)

# S3 method for class 'POSIXt'
add_days(x, n, ..., nonexistent = NULL, ambiguous = x)

# S3 method for class 'POSIXt'
add_hours(x, n, ...)

# S3 method for class 'POSIXt'
add_minutes(x, n, ...)

# S3 method for class 'POSIXt'
add_seconds(x, n, ...)
```

## Arguments

- x:

  `[POSIXct / POSIXlt]`

  A date-time vector.

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

- nonexistent:

  `[character / NULL]`

  One of the following nonexistent time resolution strategies, allowed
  to be either length 1, or the same length as the input:

  - `"roll-forward"`: The next valid instant in time.

  - `"roll-backward"`: The previous valid instant in time.

  - `"shift-forward"`: Shift the nonexistent time forward by the size of
    the daylight saving time gap.

  - `"shift-backward`: Shift the nonexistent time backward by the size
    of the daylight saving time gap.

  - `"NA"`: Replace nonexistent times with `NA`.

  - `"error"`: Error on nonexistent times.

  Using either `"roll-forward"` or `"roll-backward"` is generally
  recommended over shifting, as these two strategies maintain the
  *relative ordering* between elements of the input.

  If `NULL`, defaults to `"error"`.

  If `getOption("clock.strict")` is `TRUE`, `nonexistent` must be
  supplied and cannot be `NULL`. This is a convenient way to make
  production code robust to nonexistent times.

- ambiguous:

  `[character / zoned_time / POSIXct / list(2) / NULL]`

  One of the following ambiguous time resolution strategies, allowed to
  be either length 1, or the same length as the input:

  - `"earliest"`: Of the two possible times, choose the earliest one.

  - `"latest"`: Of the two possible times, choose the latest one.

  - `"NA"`: Replace ambiguous times with `NA`.

  - `"error"`: Error on ambiguous times.

  Alternatively, `ambiguous` is allowed to be a zoned_time (or POSIXct)
  that is either length 1, or the same length as the input. If an
  ambiguous time is encountered, the zoned_time is consulted. If the
  zoned_time corresponds to a naive_time that is also ambiguous *and*
  uses the same daylight saving time transition point as the original
  ambiguous time, then the offset of the zoned_time is used to resolve
  the ambiguity. If the ambiguity cannot be resolved by consulting the
  zoned_time, then this method falls back to `NULL`.

  Finally, `ambiguous` is allowed to be a list of size 2, where the
  first element of the list is a zoned_time (as described above), and
  the second element of the list is an ambiguous time resolution
  strategy to use when the ambiguous time cannot be resolved by
  consulting the zoned_time. Specifying a zoned_time on its own is
  identical to `list(<zoned_time>, NULL)`.

  If `NULL`, defaults to `"error"`.

  If `getOption("clock.strict")` is `TRUE`, `ambiguous` must be supplied
  and cannot be `NULL`. Additionally, `ambiguous` cannot be specified as
  a zoned_time on its own, as this implies `NULL` for ambiguous times
  that the zoned_time cannot resolve. Instead, it must be specified as a
  list alongside an ambiguous time resolution strategy as described
  above. This is a convenient way to make production code robust to
  ambiguous times.

## Value

`x` after performing the arithmetic.

## Details

Adding a single quarter with
[`add_quarters()`](https://clock.r-lib.org/reference/clock-arithmetic.md)
is equivalent to adding 3 months.

`x` and `n` are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

Calendrical based arithmetic has the potential to generate invalid dates
(like the 31st of February), nonexistent times (due to daylight saving
time gaps), and ambiguous times (due to daylight saving time fallbacks).

Naive-time based arithmetic will never generate an invalid date, but may
generate a nonexistent or ambiguous time (i.e. you added 1 day and
landed in a daylight saving time gap).

Sys-time based arithmetic operates in the UTC time zone, which means
that it will never generate any invalid dates or nonexistent / ambiguous
times.

The conversion from POSIXct/POSIXlt to the corresponding clock type uses
a "best guess" about whether you want to do the arithmetic using a
naive-time or a sys-time. For example, when adding months, you probably
want to retain the printed time when converting to a year-month-day to
perform the arithmetic, so the conversion goes through naive-time.
However, when adding smaller units like seconds, you probably want
`"2020-03-08 01:59:59" + 1 second` in the America/New_York time zone to
return `"2020-03-08 03:00:00"`, taking into account the fact that there
was a daylight saving time gap. This requires doing the arithmetic in
sys-time, so that is what clock converts to. If you disagree with this
heuristic for any reason, you can take control and perform the
conversions yourself. For example, you could convert the previous
example to a naive-time instead of a sys-time manually with
[`as_naive_time()`](https://clock.r-lib.org/reference/as_naive_time.md),
add 1 second giving `"2020-03-08 02:00:00"`, then convert back to a
POSIXct/POSIXlt, dealing with the nonexistent time that gets created by
using the `nonexistent` argument of
[`as.POSIXct()`](https://rdrr.io/r/base/as.POSIXlt.html).

## Relative ordering

For the most part, adding time based units to date-times will retain the
relative ordering of the input. For example, if `x[1] < x[2]` before the
`add_*()` call, then it is generally also true of the result. Using
`invalid = "previous" / "next"` and
`nonexistent = "roll-forward" / "roll-backward"` ensures that this holds
when invalid and nonexistent issues are encountered.

That said, with date-times there is an edge case related to ambiguous
times where the relative ordering could change. Consider these three
date-times:

    x <- c(
      date_time_build(2012, 4, 1, 2, 30, zone = "Australia/Melbourne", ambiguous = "earliest"),
      date_time_build(2012, 4, 1, 2, 00, zone = "Australia/Melbourne", ambiguous = "latest"),
      date_time_build(2012, 4, 1, 2, 30, zone = "Australia/Melbourne", ambiguous = "latest")
    )
    x
    #> [1] "2012-04-01 02:30:00 AEDT" "2012-04-01 02:00:00 AEST"
    #> [3] "2012-04-01 02:30:00 AEST"

In this case, there was a daylight saving time fallback on `2012-04-01`
where the clocks went from `02:59:59 AEDT -> 02:00:00 AEST`. So the
times above are precisely 30 minutes apart, and they are in increasing
order.

If we add sys-time based units like hours, minutes, or seconds, then the
relative ordering of these date-times will be preserved. However,
arithmetic that goes through naive-time, like adding days or months,
won't preserve the ordering here:

    add_days(x, 1)
    #> [1] "2012-04-02 02:30:00 AEST" "2012-04-02 02:00:00 AEST"
    #> [3] "2012-04-02 02:30:00 AEST"
    add_months(x, 1)
    #> [1] "2012-05-01 02:30:00 AEST" "2012-05-01 02:00:00 AEST"
    #> [3] "2012-05-01 02:30:00 AEST"

Note that the 1st and 3rd values of the result are the same, and the 1st
value is no longer before the 2nd value.

Adding larger units of time in naive-time generally does make more sense
than adding it in sys-time, but it does come with this one edge case to
be aware of when working with date-times (this does not affect dates).
If this has the potential to be an issue, consider only adding sys-time
based units (hours, minutes, and seconds) which can't have these issues.

## Examples

``` r
x <- as.POSIXct("2019-01-01", tz = "America/New_York")

add_years(x, 1:5)
#> [1] "2020-01-01 EST" "2021-01-01 EST" "2022-01-01 EST" "2023-01-01 EST"
#> [5] "2024-01-01 EST"

y <- as.POSIXct("2019-01-31 00:30:00", tz = "America/New_York")

# Adding 1 month to `y` generates an invalid date. Unlike year-month-day
# types, R's native date-time types cannot handle invalid dates, so you must
# resolve them immediately. If you don't you get an error:
try(add_months(y, 1:2))
#> Error in invalid_resolve(x, invalid = invalid) : 
#>   Invalid date found at location 1.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.
add_months(as_year_month_day(y), 1:2)
#> <year_month_day<second>[2]>
#> [1] "2019-02-31T00:30:00" "2019-03-31T00:30:00"

# Resolve invalid dates by specifying an invalid date resolution strategy
# with the `invalid` argument. Using `"previous"` here sets the date-time to
# the previous valid moment in time - i.e. the end of the month. The
# time is set to the last moment in the day to retain the relative ordering
# within your input. If you are okay with potentially losing this, and
# want to retain your time of day, you can use `"previous-day"` to set the
# date-time to the previous valid day, while keeping the time of day.
add_months(y, 1:2, invalid = "previous")
#> [1] "2019-02-28 23:59:59 EST" "2019-03-31 00:30:00 EDT"
add_months(y, 1:2, invalid = "previous-day")
#> [1] "2019-02-28 00:30:00 EST" "2019-03-31 00:30:00 EDT"
```
