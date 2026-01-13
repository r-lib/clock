# Construct a weekday vector

A `weekday` is a simple type that represents a day of the week.

The most interesting thing about the weekday type is that it implements
*circular arithmetic*, which makes determining the "next Monday" or
"previous Tuesday" from a sys-time or naive-time easy to compute. See
the examples.

## Usage

``` r
weekday(code = integer(), ..., encoding = "western")
```

## Arguments

- code:

  `[integer]`

  Integer codes between `[1, 7]` representing days of the week. The
  interpretation of these values depends on `encoding`.

- ...:

  These dots are for future extensions and must be empty.

- encoding:

  `[character(1)]`

  One of:

  - `"western"`: Encode weekdays assuming `1 == Sunday` and
    `7 == Saturday`.

  - `"iso"`: Encode weekdays assuming `1 == Monday` and `7 == Sunday`.
    This is in line with the ISO standard.

## Value

A weekday vector.

## Examples

``` r
x <- as_naive_time(year_month_day(2019, 01, 05))

# This is a Saturday!
as_weekday(x)
#> <weekday[1]>
#> [1] Sat

# Adjust to the next Wednesday
wednesday <- weekday(clock_weekdays$wednesday)

# This returns the number of days until the next Wednesday using
# circular arithmetic
# "Wednesday - Saturday = 4 days until next Wednesday"
wednesday - as_weekday(x)
#> <duration<day>[1]>
#> [1] 4

# Advance to the next Wednesday
x_next_wednesday <- x + (wednesday - as_weekday(x))

as_weekday(x_next_wednesday)
#> <weekday[1]>
#> [1] Wed

# What about the previous Tuesday?
tuesday <- weekday(clock_weekdays$tuesday)
x - (as_weekday(x) - tuesday)
#> <naive_time<day>[1]>
#> [1] "2019-01-01"

# What about the next Saturday?
# With an additional condition that if today is a Saturday,
# then advance to the next one.
saturday <- weekday(clock_weekdays$saturday)
x + 1L + (saturday - as_weekday(x + 1L))
#> <naive_time<day>[1]>
#> [1] "2019-01-12"

# You can supply an ISO coding for `code` as well, where 1 == Monday.
weekday(1:7, encoding = "western")
#> <weekday[7]>
#> [1] Sun Mon Tue Wed Thu Fri Sat
weekday(1:7, encoding = "iso")
#> <weekday[7]>
#> [1] Mon Tue Wed Thu Fri Sat Sun
```
