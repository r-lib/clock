# Time point rounding

- `time_point_floor()` rounds a sys-time or naive-time down to a
  multiple of the specified `precision`.

- `time_point_ceiling()` rounds a sys-time or naive-time up to a
  multiple of the specified `precision`.

- `time_point_round()` rounds up or down depending on what is closer,
  rounding up on ties.

Rounding time points is mainly useful for rounding sub-daily time points
up to daily time points.

It can also be useful for flooring by a set number of days (like 20)
with respect to some origin. By default, the origin is 1970-01-01
00:00:00.

If you want to group by components, such as "day of the month", rather
than by "n days", see
[`calendar_group()`](https://clock.r-lib.org/dev/reference/calendar_group.md).

## Usage

``` r
time_point_floor(x, precision, ..., n = 1L, origin = NULL)

time_point_ceiling(x, precision, ..., n = 1L, origin = NULL)

time_point_round(x, precision, ..., n = 1L, origin = NULL)
```

## Arguments

- x:

  `[clock_sys_time / clock_naive_time]`

  A sys-time or naive-time.

- precision:

  `[character(1)]`

  A time point precision. One of:

  - `"day"`

  - `"hour"`

  - `"minute"`

  - `"second"`

  - `"millisecond"`

  - `"microsecond"`

  - `"nanosecond"`

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A positive integer specifying the multiple of `precision` to use.

- origin:

  `[clock_sys_time(1) / clock_naive_time(1) / NULL]`

  An origin to begin counting from. Mostly useful when `n > 1` and you
  want to control how the rounding groups are created.

  If `x` is a sys-time, `origin` must be a sys-time.

  If `x` is a naive-time, `origin` must be a naive-time.

  The precision of `origin` must be equally precise as or less precise
  than `precision`.

  If `NULL`, a default origin of midnight on 1970-01-01 is used.

## Value

`x` rounded to the new `precision`.

## Boundary Handling

To understand how flooring and ceiling work, you need to know how they
create their intervals for rounding.

- `time_point_floor()` constructs intervals of `[lower, upper)` that
  bound each element of `x`, then always chooses the *left-hand side*.

- `time_point_ceiling()` constructs intervals of `(lower, upper]` that
  bound each element of `x`, then always chooses the *right-hand side*.

As an easy example, consider 2020-01-02 00:00:05.

To floor this to the nearest day, the following interval is constructed,
and the left-hand side is returned at day precision:

`[2020-01-02 00:00:00, 2020-01-03 00:00:00)`

To ceiling this to the nearest day, the following interval is
constructed, and the right-hand side is returned at day precision:

`(2020-01-02 00:00:00, 2020-01-03 00:00:00]`

Here is another example, this time with a time point on a boundary,
2020-01-02 00:00:00.

To floor this to the nearest day, the following interval is constructed,
and the left-hand side is returned at day precision:

`[2020-01-02 00:00:00, 2020-01-03 00:00:00)`

To ceiling this to the nearest day, the following interval is
constructed, and the right-hand side is returned at day precision:

`(2020-01-01 00:00:00, 2020-01-02 00:00:00]`

Notice that, regardless of whether you are doing a floor or ceiling, if
the input falls on a boundary then it will be returned as is.

## Examples

``` r
library(magrittr)

x <- as_naive_time(year_month_day(2019, 01, 01))
x <- add_days(x, 0:40)
head(x)
#> <naive_time<day>[6]>
#> [1] "2019-01-01" "2019-01-02" "2019-01-03" "2019-01-04" "2019-01-05"
#> [6] "2019-01-06"

# Floor by sets of 20 days
# The implicit origin to start the 20 day counter is 1970-01-01
time_point_floor(x, "day", n = 20)
#> <naive_time<day>[41]>
#>  [1] "2018-12-15" "2018-12-15" "2018-12-15" "2019-01-04" "2019-01-04"
#>  [6] "2019-01-04" "2019-01-04" "2019-01-04" "2019-01-04" "2019-01-04"
#> [11] "2019-01-04" "2019-01-04" "2019-01-04" "2019-01-04" "2019-01-04"
#> [16] "2019-01-04" "2019-01-04" "2019-01-04" "2019-01-04" "2019-01-04"
#> [21] "2019-01-04" "2019-01-04" "2019-01-04" "2019-01-24" "2019-01-24"
#> [26] "2019-01-24" "2019-01-24" "2019-01-24" "2019-01-24" "2019-01-24"
#> [31] "2019-01-24" "2019-01-24" "2019-01-24" "2019-01-24" "2019-01-24"
#> [36] "2019-01-24" "2019-01-24" "2019-01-24" "2019-01-24" "2019-01-24"
#> [41] "2019-01-24"

# You can easily customize the origin by supplying a new one
# as the `origin` argument
origin <- year_month_day(2019, 01, 01) %>%
  as_naive_time()

time_point_floor(x, "day", n = 20, origin = origin)
#> <naive_time<day>[41]>
#>  [1] "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01"
#>  [6] "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01"
#> [11] "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01"
#> [16] "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01"
#> [21] "2019-01-21" "2019-01-21" "2019-01-21" "2019-01-21" "2019-01-21"
#> [26] "2019-01-21" "2019-01-21" "2019-01-21" "2019-01-21" "2019-01-21"
#> [31] "2019-01-21" "2019-01-21" "2019-01-21" "2019-01-21" "2019-01-21"
#> [36] "2019-01-21" "2019-01-21" "2019-01-21" "2019-01-21" "2019-01-21"
#> [41] "2019-02-10"

# For times on the boundary, floor and ceiling both return the input
# at the new precision. Notice how the first element is on the boundary,
# and the second is 1 second after the boundary.
y <- as_naive_time(year_month_day(2020, 01, 02, 00, 00, c(00, 01)))
time_point_floor(y, "day")
#> <naive_time<day>[2]>
#> [1] "2020-01-02" "2020-01-02"
time_point_ceiling(y, "day")
#> <naive_time<day>[2]>
#> [1] "2020-01-02" "2020-01-03"
```
