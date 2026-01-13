# Info: naive-time

`naive_time_info()` retrieves a set of low-level information generally
not required for most date-time manipulations. It is used implicitly by
[`as_zoned_time()`](https://clock.r-lib.org/reference/as_zoned_time.md)
when converting from a naive-time.

It returns a data frame with the following columns:

- `type`: A character vector containing one of:

  - `"unique"`: The naive-time maps uniquely to a zoned-time that can be
    created with `zone`.

  - `"nonexistent"`: The naive-time does not exist as a zoned-time that
    can be created with `zone`.

  - `"ambiguous"`: The naive-time exists twice as a zoned-time that can
    be created with `zone`.

- `first`: A
  [`sys_time_info()`](https://clock.r-lib.org/reference/sys_time_info.md)
  data frame.

- `second`: A
  [`sys_time_info()`](https://clock.r-lib.org/reference/sys_time_info.md)
  data frame.

### type == "unique"

- `first` will be filled out with sys-info representing daylight saving
  time information for that time point in `zone`.

- `second` will contain only `NA` values, as there is no ambiguity to
  represent information for.

### type == "nonexistent"

- `first` will be filled out with the sys-info that ends just prior to
  `x`.

- `second` will be filled out with the sys-info that begins just after
  `x`.

### type == "ambiguous"

- `first` will be filled out with the sys-info that ends just after `x`.

- `second` will be filled out with the sys-info that starts just before
  `x`.

## Usage

``` r
naive_time_info(x, zone)
```

## Arguments

- x:

  `[clock_naive_time]`

  A naive-time.

- zone:

  `[character]`

  A valid time zone name.

  Unlike most functions in clock, in `naive_time_info()` `zone` is
  vectorized and is recycled against `x`.

## Value

A data frame of low level information.

## Details

If the tibble package is installed, it is recommended to convert the
output to a tibble with `as_tibble()`, as that will print the df-cols
much nicer.

## Examples

``` r
library(vctrs)

x <- year_month_day(1970, 04, 26, 02, 30, 00)
x <- as_naive_time(x)

# Maps uniquely to a time in London
naive_time_info(x, "Europe/London")
#>     type         first.begin           first.end first.offset
#> 1 unique 1968-10-26T23:00:00 1971-10-31T02:00:00         3600
#>   first.dst first.abbreviation second.begin second.end second.offset
#> 1     FALSE                BST         <NA>       <NA>          <NA>
#>   second.dst second.abbreviation
#> 1         NA                <NA>

# This naive-time never existed in New York!
# A DST gap jumped the time from 01:59:59 -> 03:00:00,
# skipping the 2 o'clock hour
zone <- "America/New_York"
info <- naive_time_info(x, zone)
info
#>          type         first.begin           first.end first.offset
#> 1 nonexistent 1969-10-26T06:00:00 1970-04-26T07:00:00       -18000
#>   first.dst first.abbreviation        second.begin          second.end
#> 1     FALSE                EST 1970-04-26T07:00:00 1970-10-25T06:00:00
#>   second.offset second.dst second.abbreviation
#> 1        -14400       TRUE                 EDT

# You can recreate various `nonexistent` strategies with this info
as_zoned_time(x, zone, nonexistent = "roll-forward")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T03:00:00-04:00"
as_zoned_time(info$first$end, zone)
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T03:00:00-04:00"

as_zoned_time(x, zone, nonexistent = "roll-backward")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T01:59:59-05:00"
as_zoned_time(info$first$end - 1, zone)
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T01:59:59-05:00"

as_zoned_time(x, zone, nonexistent = "shift-forward")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T03:30:00-04:00"
as_zoned_time(as_sys_time(x) - info$first$offset, zone)
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T03:30:00-04:00"

as_zoned_time(x, zone, nonexistent = "shift-backward")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T01:30:00-05:00"
as_zoned_time(as_sys_time(x) - info$second$offset, zone)
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T01:30:00-05:00"

# ---------------------------------------------------------------------------
# Normalizing to UTC

# Imagine you had the following printed times, and knowledge that they
# are to be interpreted as in the corresponding time zones
df <- data_frame(
  x = c("2020-01-05 02:30:00", "2020-06-03 12:20:05"),
  zone = c("America/Los_Angeles", "Europe/London")
)

# The times are assumed to be naive-times, i.e. if you lived in the `zone`
# at the moment the time was recorded, then you would have seen that time
# printed on the clock. Currently, these are strings. To convert them to
# a time based type, you'll have to acknowledge that R only lets you have
# 1 time zone in a vector of date-times at a time. So you'll need to
# normalize these naive-times. The easiest thing to normalize them to
# is UTC.
df$naive <- naive_time_parse(df$x)
#> Warning: Failed to parse 2 strings, beginning at location 1. Returning `NA` at the locations where there were parse failures.

# Get info about the naive times using a vector of zones
info <- naive_time_info(df$naive, df$zone)
info
#>   type first.begin first.end first.offset first.dst first.abbreviation
#> 1 <NA>        <NA>      <NA>         <NA>        NA               <NA>
#> 2 <NA>        <NA>      <NA>         <NA>        NA               <NA>
#>   second.begin second.end second.offset second.dst second.abbreviation
#> 1         <NA>       <NA>          <NA>         NA                <NA>
#> 2         <NA>       <NA>          <NA>         NA                <NA>

# We'll assume that some system generated these naive-times with no
# chance of them ever being nonexistent or ambiguous. So now all we have
# to do is use the offset to convert the naive-time to a sys-time. The
# relationship used is:
# offset = naive_time - sys_time
df$sys <- as_sys_time(df$naive) - info$first$offset
df
#>                     x                zone naive  sys
#> 1 2020-01-05 02:30:00 America/Los_Angeles  <NA> <NA>
#> 2 2020-06-03 12:20:05       Europe/London  <NA> <NA>

# At this point, both times are in UTC. From here, you can convert them
# both to either America/Los_Angeles or Europe/London as required.
as_zoned_time(df$sys, "America/Los_Angeles")
#> <zoned_time<second><America/Los_Angeles>[2]>
#> [1] NA NA
as_zoned_time(df$sys, "Europe/London")
#> <zoned_time<second><Europe/London>[2]>
#> [1] NA NA
```
