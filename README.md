
<!-- README.md is generated from README.Rmd. Please edit that file -->

# civil

<!-- badges: start -->

[![Codecov test
coverage](https://codecov.io/gh/DavisVaughan/civil/branch/master/graph/badge.svg)](https://codecov.io/gh/DavisVaughan/civil?branch=master)
[![R-CMD-check](https://github.com/DavisVaughan/civil/workflows/R-CMD-check/badge.svg)](https://github.com/DavisVaughan/civil/actions)
<!-- badges: end -->

The goal of civil is to provide new types for working with *naive* and
*zoned* date-times. A *naive* date-time is completely unaware of any
time zone. A *zoned* date-time can be thought of as a naive date-time
plus a time zone.

The standard Gregorian calendar is supported through naive classes like
`year_month_day()` and `naive_datetime()` and zoned classes like
`zoned_datetime()`. Additionally, there is support for other calendars,
such as a quarterly calendar, through classes like `year_quarter_day()`.
*Partial* calendars, which have a precision that is coarser than a
single day, are also supported through classes like `year_month()` and
`year_quarter()`.

Nanotime precision is supported for both naive and zoned types through
`naive_nano_datetime()` and `zoned_nano_datetime()`.

Additionally, all functionality works with Date and POSIXct, with the
goal of expanding on the groundwork laid by lubridate to make working
around issues of daylight savings and leap years a little more
intuitive.

## Installation

You can install the development version of civil with:

``` r
# install.packages("remotes")
remotes::install_github("DavisVaughan/civil")
```
