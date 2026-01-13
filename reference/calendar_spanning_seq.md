# Spanning sequence: calendars

`calendar_spanning_seq()` generates a regular sequence along the span of
`x`, i.e. along `[min(x), max(x)]`. The sequence is generated at the
precision of `x`.

Importantly, sequences can only be generated if the underlying
[`seq()`](https://rdrr.io/r/base/seq.html) method for the calendar in
question supports a `from` and `to` value at the same precision as `x`.
For example, you can't compute a day precision spanning sequence for a
[`year_month_day()`](https://clock.r-lib.org/reference/year_month_day.md)
calendar (you can only compute a year and month one). To create a day
precision sequence, you'd have to convert to a time-point first. See the
individual [`seq()`](https://rdrr.io/r/base/seq.html) method
documentation to learn what precisions are allowed.

## Usage

``` r
calendar_spanning_seq(x)
```

## Arguments

- x:

  `[clock_calendar]`

  A calendar vector.

## Value

A sequence along `[min(x), max(x)]`.

## Details

Missing values are automatically removed before the sequence is
generated.

If you need more precise sequence generation, call
[`range()`](https://rdrr.io/r/base/range.html) and
[`seq()`](https://rdrr.io/r/base/seq.html) directly.

## Examples

``` r
x <- year_month_day(c(2019, 2022, 2020), c(2, 5, 3))
x
#> <year_month_day<month>[3]>
#> [1] "2019-02" "2022-05" "2020-03"

# Month precision spanning sequence
calendar_spanning_seq(x)
#> <year_month_day<month>[40]>
#>  [1] "2019-02" "2019-03" "2019-04" "2019-05" "2019-06" "2019-07"
#>  [7] "2019-08" "2019-09" "2019-10" "2019-11" "2019-12" "2020-01"
#> [13] "2020-02" "2020-03" "2020-04" "2020-05" "2020-06" "2020-07"
#> [19] "2020-08" "2020-09" "2020-10" "2020-11" "2020-12" "2021-01"
#> [25] "2021-02" "2021-03" "2021-04" "2021-05" "2021-06" "2021-07"
#> [31] "2021-08" "2021-09" "2021-10" "2021-11" "2021-12" "2022-01"
#> [37] "2022-02" "2022-03" "2022-04" "2022-05"

# Quarter precision:
x <- year_quarter_day(c(2005, 2006, 2003), c(4, 2, 3))
calendar_spanning_seq(x)
#> <year_quarter_day<January><quarter>[12]>
#>  [1] "2003-Q3" "2003-Q4" "2004-Q1" "2004-Q2" "2004-Q3" "2004-Q4"
#>  [7] "2005-Q1" "2005-Q2" "2005-Q3" "2005-Q4" "2006-Q1" "2006-Q2"

# Can't generate sequences if `seq()` doesn't allow the precision
x <- year_month_day(2019, c(1, 2, 1), c(20, 3, 25))
try(calendar_spanning_seq(x))
#> Error in seq(from = from, to = to, by = 1L) : 
#>   `from` must be 'year' or 'month' precision.

# Generally this means you need to convert to a time point and use
# `time_point_spanning_seq()` instead
time_point_spanning_seq(as_sys_time(x))
#> <sys_time<day>[15]>
#>  [1] "2019-01-20" "2019-01-21" "2019-01-22" "2019-01-23" "2019-01-24"
#>  [6] "2019-01-25" "2019-01-26" "2019-01-27" "2019-01-28" "2019-01-29"
#> [11] "2019-01-30" "2019-01-31" "2019-02-01" "2019-02-02" "2019-02-03"
```
