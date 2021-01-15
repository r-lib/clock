
<!-- README.md is generated from README.Rmd. Please edit that file -->

# clock

<!-- badges: start -->

[![Codecov test
coverage](https://codecov.io/gh/DavisVaughan/clock/branch/master/graph/badge.svg)](https://codecov.io/gh/DavisVaughan/clock?branch=master)
[![R-CMD-check](https://github.com/DavisVaughan/clock/workflows/R-CMD-check/badge.svg)](https://github.com/DavisVaughan/clock/actions)
<!-- badges: end -->

The goal of clock is to provide a comprehensive library for working with
dates and times. A few of the key features of clock are:

-   *Time points* with precision that can vary from second up to
    nanosecond.

-   *Calendar* types, such as year-month-day, year-month-weekday,
    year-quarter-day, and iso-year-week-day, that can vary in precision
    from year up to nanosecond.

-   *Duration* types for working with absolute intervals of time.

-   Clear and unambiguous handling of otherwise confusing date time
    manipulation issues, such as leap days and daylight savings.

-   A high level API supporting R’s Date and POSIXct classes to ease you
    into clock before having to learn about the more advanced features
    mentioned above.

## Installation

You can install the development version of clock with:

``` r
# install.packages("remotes")
remotes::install_github("DavisVaughan/clock")
```

## Example

``` r
library(clock)
library(magrittr)
```

With clock, there is a high level API supporting Date and POSIXct. This
is often the easiest way to begin using clock.

For example, to add 5 days:

``` r
x <- as.Date("2019-05-01")

add_days(x, 5)
#> [1] "2019-05-06"
```

To add 5 hours:

``` r
y <- as.POSIXct("2019-01-01 02:30:00", tz = "America/New_York")

add_hours(y, 5)
#> [1] "2019-01-01 07:30:00 EST"
```

To get the month:

``` r
get_month(x)
#> [1] 5

get_month(y)
#> [1] 1
```

To set the day:

``` r
set_day(x, 1:5)
#> [1] "2019-05-01" "2019-05-02" "2019-05-03" "2019-05-04" "2019-05-05"

# Last day in the month
set_day(x, "last")
#> [1] "2019-05-31"
```
