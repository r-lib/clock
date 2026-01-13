# Frequently Asked Questions

``` r
library(clock)
library(magrittr)
```

## Why can’t I do day arithmetic on a year-month-day?

It might seem intuitive that since you can do:

``` r
x <- year_month_day(2019, 1, 5)

add_months(x, 1)
#> <year_month_day<day>[1]>
#> [1] "2019-02-05"
```

That you should also be able to do:

``` r
add_days(x, 1)
#> Error in `add_days()`:
#> ! Can't perform this operation on a <clock_year_month_day>.
#> ℹ Do you need to convert to a time point first?
#> ℹ Use `as_naive_time()` or `as_sys_time()` to convert to a time point.
```

Generally, calendars don’t support day based arithmetic, nor do they
support arithmetic at more precise precisions than day. Instead, you
have to convert to a time point, do the arithmetic there, and then
convert back (if you still need a year-month-day after that).

``` r
x %>%
  as_naive_time() %>%
  add_days(1) %>%
  as_year_month_day()
#> <year_month_day<day>[1]>
#> [1] "2019-01-06"
```

The first reason for this is performance. A year-month-day is a *field*
type, implemented as multiple parallel vectors holding the year, month,
day, and all other components separately. There are two ways that day
based arithmetic could be implemented for this:

- Increment the day field, then check the year and month field to see if
  they need to be incremented, accounting for months having a differing
  number of days, and leap years.

- Convert to naive-time, add days, convert back.

Both approaches are relatively expensive. One of the goals of the
low-level API of clock is to make these expensive operations explicit.
This helps make it apparent that when you need to chain together
multiple operations, you should try and do all of your *calendrical*
arithmetic steps first, then convert to a time point (i.e. the second
bullet point from above) to do all of your *chronological* arithmetic.

The second reason for this has to do with invalid dates, such as the
three in this vector:

``` r
odd_dates <- year_month_day(2019, 2, 28:31)
odd_dates
#> <year_month_day<day>[4]>
#> [1] "2019-02-28" "2019-02-29" "2019-02-30" "2019-02-31"
```

What does it mean to “add 1 day” to these? There is no obvious answer to
this question. Since clock requires that you first convert to a time
point to do day based arithmetic, you’ll be forced to call
[`invalid_resolve()`](https://clock.r-lib.org/reference/clock-invalid.md)
to handle these invalid dates first. After resolving them manually, then
day based arithmetic again makes sense.

``` r
odd_dates %>%
  invalid_resolve(invalid = "next")
#> <year_month_day<day>[4]>
#> [1] "2019-02-28" "2019-03-01" "2019-03-01" "2019-03-01"

odd_dates %>%
  invalid_resolve(invalid = "next") %>%
  as_naive_time() %>%
  add_days(2)
#> <naive_time<day>[4]>
#> [1] "2019-03-02" "2019-03-03" "2019-03-03" "2019-03-03"

odd_dates %>%
  invalid_resolve(invalid = "overflow")
#> <year_month_day<day>[4]>
#> [1] "2019-02-28" "2019-03-01" "2019-03-02" "2019-03-03"

odd_dates %>%
  invalid_resolve(invalid = "overflow") %>%
  as_naive_time() %>%
  add_days(2)
#> <naive_time<day>[4]>
#> [1] "2019-03-02" "2019-03-03" "2019-03-04" "2019-03-05"
```

## Why can’t I add time to a zoned-time?

If you have a zoned-time, such as:

``` r
x <- zoned_time_parse_complete("1970-04-26T01:30:00-05:00[America/New_York]")
x
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T01:30:00-05:00"
```

You might wonder why you can’t add any units of time to it:

``` r
add_days(x, 1)
#> Error in `add_days()`:
#> ! Can't perform this operation on a <clock_zoned_time>.
#> ℹ Do you need to convert to a time point first?
#> ℹ Use `as_naive_time()` or `as_sys_time()` to convert to a time point.

add_seconds(x, 1)
#> Error in `add_seconds()`:
#> ! Can't perform this operation on a <clock_zoned_time>.
#> ℹ Do you need to convert to a time point first?
#> ℹ Use `as_naive_time()` or `as_sys_time()` to convert to a time point.
```

In clock, you can’t do much with zoned-times directly. The best way to
understand this is to think of a zoned-time as containing 3 things: a
sys-time, a naive-time, and a time zone name. You can access those
things with:

``` r
x
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T01:30:00-05:00"

# The printed time with no time zone info
as_naive_time(x)
#> <naive_time<second>[1]>
#> [1] "1970-04-26T01:30:00"

# The equivalent time in UTC
as_sys_time(x)
#> <sys_time<second>[1]>
#> [1] "1970-04-26T06:30:00"

zoned_time_zone(x)
#> [1] "America/New_York"
```

Calling
[`add_days()`](https://clock.r-lib.org/reference/clock-arithmetic.md) on
a zoned-time is then an ambiguous operation. Should we add to the
sys-time or the naive-time that is contained in the zoned-time? The
answer changes depending on the scenario.

Because of this, you have to extract out the relevant time point that
you care about, operate on that, and then convert back to zoned-time.
This often produces the same result:

``` r
x %>%
  as_naive_time() %>%
  add_seconds(1) %>%
  as_zoned_time(zoned_time_zone(x))
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T01:30:01-05:00"

x %>%
  as_sys_time() %>%
  add_seconds(1) %>%
  as_zoned_time(zoned_time_zone(x))
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T01:30:01-05:00"
```

But not always! When daylight saving time is involved, the choice of
sys-time or naive-time matters. Let’s try adding 30 minutes:

``` r
# There is a DST gap 1 second after 01:59:59,
# which jumps us straight to 03:00:00,
# skipping the 2 o'clock hour entirely

x %>%
  as_naive_time() %>%
  add_minutes(30) %>%
  as_zoned_time(zoned_time_zone(x))
#> Error in `as_zoned_time()`:
#> ! Nonexistent time due to daylight saving time at location 1.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.

x %>%
  as_sys_time() %>%
  add_minutes(30) %>%
  as_zoned_time(zoned_time_zone(x))
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T03:00:00-04:00"
```

When adding to the naive-time, we got an error. With the sys-time,
everything seems okay. What happened?

The sys-time scenario is easy to explain. Technically this converts to
UTC, adds the time there, then converts back to your time zone. An
easier way to think about this is that you sat in front of your computer
for exactly 30 minutes (1800 seconds), then looked at the clock.
Assuming that that clock automatically changes itself correctly for
daylight saving time, it should read 3 o’clock.

The naive-time scenario makes more sense if you break down the steps.
First, we convert to naive-time, dropping all time zone information but
keeping the printed time:

``` r
x
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T01:30:00-05:00"

x %>%
  as_naive_time()
#> <naive_time<second>[1]>
#> [1] "1970-04-26T01:30:00"
```

We add 30 minutes to this. Because we don’t have any time zone
information, this lands us at 2 o’clock, which isn’t an issue when
working with naive-time:

``` r
x %>%
  as_naive_time() %>%
  add_minutes(30)
#> <naive_time<second>[1]>
#> [1] "1970-04-26T02:00:00"
```

Finally, we convert back to zoned-time. If possible, this tries to keep
the printed time, and just attaches the relevant time zone onto it.
However, in this case that isn’t possible, since 2 o’clock didn’t exist
in this time zone! This *nonexistent time* must be handled explicitly by
setting the `nonexistent` argument of
[`as_zoned_time()`](https://clock.r-lib.org/reference/as_zoned_time.md).
We can choose from a variety of strategies to handle nonexistent times,
but here we just roll forward to the next valid moment in time.

``` r
x %>%
  as_naive_time() %>%
  add_minutes(30) %>%
  as_zoned_time(zoned_time_zone(x), nonexistent = "roll-forward")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T03:00:00-04:00"
```

As a general rule, it often makes the most sense to add:

- Years, quarters, and months to a *calendar*.

- Weeks and days to a *naive time*.

- Hours, minutes, seconds, and subseconds to a *sys time*.

This is what the high-level API for POSIXct does. However, this isn’t
always what you want, so the low-level API requires you to be more
explicit.

## Where did my POSIXct subseconds go?

``` r
old <- options(digits.secs = 6, digits = 22)
```

Consider the following POSIXct:

``` r
x <- as.POSIXct("2019-01-01 01:00:00.2", "America/New_York")
x
#> [1] "2019-01-01 01:00:00.2 EST"
```

It looks like there is some fractional second information here, but
converting it to naive-time drops it:

``` r
as_naive_time(x)
#> <naive_time<second>[1]>
#> [1] "2019-01-01T01:00:00"
```

This is purposeful. clock treats POSIXct as a *second precision* data
type. The reason for this has to do with the fact that POSIXct is
implemented as a vector of doubles, which have a limit to how precisely
they can store information. For example, try parsing a slightly smaller
or larger fractional second:

``` r
y <- as.POSIXct(
  c("2019-01-01 01:00:00.1", "2019-01-01 01:00:00.3"), 
  "America/New_York"
)

# Oh dear!
y
#> [1] "2019-01-01 01:00:00.099999 EST" "2019-01-01 01:00:00.299999 EST"
```

It isn’t printing correctly, at the very least. Let’s look under the
hood:

``` r
unclass(y)
#> [1] 1546322400.099999904633 1546322400.299999952316
#> attr(,"tzone")
#> [1] "America/New_York"
```

Double vectors have a limit to how much precision they can represent,
and this is bumping up against that limit. So our `.1` seconds is
instead represented as `.099999etc`.

This precision loss gets worse the farther we get from the epoch,
1970-01-01, represented as `0` under the hood. For example, here we’ll
use a number of seconds that represents the year 2050, and add 5
microseconds to it:

``` r
new_utc <- function(x) {
  class(x) <- c("POSIXct", "POSIXt")
  attr(x, "tzone") <- "UTC"
  x
}

year_2050 <- 2524608000
five_microseconds <- 0.000005

new_utc(year_2050)
#> [1] "2050-01-01 UTC"

# Oh no!
new_utc(year_2050 + five_microseconds)
#> [1] "2050-01-01 00:00:00.000004 UTC"

# Represented internally as:
year_2050 + five_microseconds
#> [1] 2524608000.000004768372
```

Because of these issues, clock treats POSIXct as a second precision data
type, dropping all other information. Instead, you should parse directly
into a subsecond clock type:

``` r
naive_time_parse(
  c("2019-01-01T01:00:00.1", "2019-01-01T01:00:00.3"), 
  precision = "millisecond"
) %>%
  as_zoned_time("America/New_York")
#> <zoned_time<millisecond><America/New_York>[2]>
#> [1] "2019-01-01T01:00:00.100-05:00" "2019-01-01T01:00:00.300-05:00"
```

``` r
# Reset old options
options(old)
```

## What is the time zone of Date?

In clock, R’s native Date type is actually assumed to be *naive*,
i.e. clock assumes that there is a yet-to-be-specified time zone, like
with a naive-time. The other possibility is to assume that Date is UTC
(like sys-time), but it is often more intuitive for Dates to be naive
when manipulating them and converting them to zoned-time or POSIXct.

R does not consistently treat Dates as naive or UTC. Instead it switches
between them, depending on the function.

For example, the Date method of
[`as.POSIXct()`](https://rdrr.io/r/base/as.POSIXlt.html) does not expose
a `tz` argument. Instead, it assumes that Date is UTC, and that the
result should be shown in local time (as defined by
[`Sys.timezone()`](https://rdrr.io/r/base/timezones.html)). This often
results in confusing behavior, such as:

``` r
x <- as.Date("2019-01-01")
x
#> [1] "2019-01-01"

withr::with_timezone("America/New_York", {
  print(as.POSIXct(x))
})
#> [1] "2019-01-01 UTC"
```

With clock, converting to zoned-time from Date will always assume that
Date is naive, which will keep the printed date (if possible) and show
it in the `zone` you specified.

``` r
as_zoned_time(x, "UTC")
#> <zoned_time<second><UTC>[1]>
#> [1] "2019-01-01T00:00:00+00:00"

as_zoned_time(x, "America/New_York")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-01-01T00:00:00-05:00"

as_zoned_time(x, "Europe/London")
#> <zoned_time<second><Europe/London>[1]>
#> [1] "2019-01-01T00:00:00+00:00"
```

On the other hand, the POSIXct method for
[`as.Date()`](https://rdrr.io/r/base/as.Date.html) treats Date as a
naive type. This is probably what you want, and this example just shows
the inconsistency. It is a bit hard to see this, because the `tz`
argument of the method defaults to `"UTC"`, but if you set the `tz`
argument to the zone of your input, it becomes clear:

``` r
x <- as.POSIXct("2019-01-01 23:00:00", "America/New_York")

as.Date(x, tz = date_time_zone(x))
#> [1] "2019-01-01"
```

If this assumed that Date was UTC, then it would have resulted in
something like:

``` r
utc <- date_time_set_zone(x, "UTC")
utc
#> [1] "2019-01-02 04:00:00 UTC"

as.Date(utc, tz = date_time_zone(utc))
#> [1] "2019-01-02"
```

## What does clock do with leap seconds?

clock currently handles leap seconds in the same way that base R’s
date-time (POSIXct) class does - it ignores them entirely. While
[`strptime()`](https://rdrr.io/r/base/strptime.html) has some very
simple capabilities for parsing leap seconds, clock doesn’t allow them
at all:

``` r
raw <- c(
  "2015-12-31T23:59:59", 
  "2015-12-31T23:59:60", # A real leap second!
  "2016-01-01T00:00:00"
)

x <- sys_time_parse(raw)
#> Warning: Failed to parse 1 string at location 2. Returning `NA` at
#> that location.

x
#> <sys_time<second>[3]>
#> [1] "2015-12-31T23:59:59" NA                    "2016-01-01T00:00:00"
```

``` r
# Reported as exactly 1 second apart.
# In real life these are 2 seconds apart because of the leap second.
x[[3]] - x[[1]]
#> <duration<second>[1]>
#> [1] 1
```

Because none of the clock types handle leap seconds, clock currently
doesn’t offer a way to parse them. Your current best option if you
*really* need to parse leap seconds is to use
[`strptime()`](https://rdrr.io/r/base/strptime.html):

``` r
# This returns a POSIXlt, which can handle the special 60s field
x <- strptime(raw, format = "%Y-%m-%dT%H:%M:%S", tz = "UTC")
x
#> [1] "2015-12-31 23:59:59 UTC" "2015-12-31 23:59:60 UTC"
#> [3] "2016-01-01 00:00:00 UTC"

# On conversion to POSIXct, it "rolls" forward
as.POSIXct(x)
#> [1] "2015-12-31 23:59:59 UTC" "2016-01-01 00:00:00 UTC"
#> [3] "2016-01-01 00:00:00 UTC"
```

[`strptime()`](https://rdrr.io/r/base/strptime.html) isn’t a great
solution though, because the parsing is fairly simple. If you try to use
a “fake” leap second, it will still accept it, even though it isn’t a
real time:

``` r
# 2016-12-31 wasn't a leap second date, but it still tries to parse this fake time
strptime("2016-12-31T23:59:60", format = "%Y-%m-%dT%H:%M:%S", tz = "UTC")
#> [1] "2016-12-31 23:59:60 UTC"
```

A true solution would check this against a database of actual leap
seconds, and would only successfully parse it if it matched a real leap
second. The C++ library that powers clock does have this capability,
through a `utc_clock` class, and we may expose this in a limited form in
the future, with conversion to and from sys-time and naive-time.

## Why doesn’t this work with data.table?

While the entire high-level API for R’s native date (Date) and date-time
(POSIXct) types will work fine with data.table, if you try to put any of
the major clock types into a data.table, you will probably see this
error message:

``` r
library(data.table)

data.table(x = year_month_day(2019, 1, 1))
#> Error in dimnames(x) <- dn : 
#>   length of 'dimnames' [1] not equal to array extent
```

You won’t see this issue when working with data.frames or tibbles.

As of now, data.table doesn’t support the concept of *record types*.
These are implemented as a list of vectors of equal length, that
together represent a single idea. The
[`length()`](https://rdrr.io/r/base/length.html) of these types should
be taken from the length of the vectors, not the length of the list. If
you unclass any of the clock types, you’ll see that they are implemented
in this way:

``` r
ymdh <- year_month_day(2019, 1, 1:2, 1)

unclass(ymdh)
#> $year
#> [1] 2019 2019
#> 
#> $month
#> [1] 1 1
#> 
#> $day
#> [1] 1 2
#> 
#> $hour
#> [1] 1 1
#> 
#> attr(,"precision")
#> [1] 5

unclass(as_naive_time(ymdh))
#> $lower
#> [1] 2147483648 2147483648
#> 
#> $upper
#> [1] 429529 429553
#> 
#> attr(,"precision")
#> [1] 5
#> attr(,"clock")
#> [1] 1
```

I find that record types are extremely useful data structures for
building upon R’s basic atomic types in ways that otherwise couldn’t be
done. They allow calendar types to hold information about each
component, enabling instant access for retrieval, modification, and
grouping. They also allow calendars to represent invalid dates, such as
`2019-02-31`, without any issues. Time points use them to store up to
nanosecond precision date-times, which are really C++ `int64_t` types
that don’t nicely fit into any R atomic type (I am aware of the bit64
package, and made a conscious decision to implement as a record type
instead. This partly had to do with how missing values are handled, and
how that integrates with vctrs).

The idea of a record type actually isn’t new. R’s own POSIXlt type is a
record type:

``` r
x <- as.POSIXct("2019-01-01", "America/New_York")

# POSIXct is implemented as a double
unclass(x)
#> [1] 1546318800
#> attr(,"tzone")
#> [1] "America/New_York"

# POSIXlt is a record type
unclass(as.POSIXlt(x))
#> $sec
#> [1] 0
#> 
#> $min
#> [1] 0
#> 
#> $hour
#> [1] 0
#> 
#> $mday
#> [1] 1
#> 
#> $mon
#> [1] 0
#> 
#> $year
#> [1] 119
#> 
#> $wday
#> [1] 2
#> 
#> $yday
#> [1] 0
#> 
#> $isdst
#> [1] 0
#> 
#> $zone
#> [1] "EST"
#> 
#> $gmtoff
#> [1] -18000
#> 
#> attr(,"tzone")
#> [1] "America/New_York" "EST"              "EDT"             
#> attr(,"balanced")
#> [1] TRUE
```

data.table doesn’t truly support POSIXlt either. Instead, you get a
warning about them converting it to a POSIXct. This is pretty reasonable
considering their focus on performance.

``` r
data.table(x = as.POSIXlt("2019-01-01", "America/New_York"))
#>             x
#> 1: 2019-01-01
#> Warning message:
#> In as.data.table.list(x, keep.rownames = keep.rownames, check.names = check.names,  :
#>   POSIXlt column type detected and converted to POSIXct. We do not recommend use of POSIXlt at all because it uses 40 bytes to store one date.
```

It was previously a bit difficult to create record types in R because
there were few examples and no resources to build on. In vctrs, we’ve
added a `vctrs_rcrd` type that serves as a base to build new record
types on. Many S3 methods have been written for `vctrs_rcrd`s in a way
that should work for any type that builds on top of it, giving you a lot
of scaffolding for free.

I am hopeful that as more record types make their way into the R
ecosystem built on this common foundation, it might be possible for
data.table to enable this as an approved type in their package.
