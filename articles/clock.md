# Getting Started

``` r
library(clock)
library(magrittr)
```

The goal of this vignette is to introduce you to clock’s high-level API,
which works directly on R’s built-in date-time types, Date and POSIXct.
For an overview of all of the functionality in the high-level API, check
out the pkgdown reference section, [High Level
API](https://clock.r-lib.org/reference/index.html#section-high-level-api).
One thing you should immediately notice is that every function specific
to R’s date and date-time types are prefixed with `date_*()`. There are
also additional functions for arithmetic (`add_*()`) and getting
(`get_*()`) or setting (`set_*()`) components that are also used by
other types in clock.

As you’ll quickly see in this vignette, one of the main goals of clock
is to guard you, the user, from unexpected issues caused by frustrating
date manipulation concepts like invalid dates and daylight saving time.
It does this by letting you know as soon as one of these issues happens,
giving you the power to handle it explicitly with one of a number of
different resolution strategies.

## Building

To create a vector of dates, you can use
[`date_build()`](https://clock.r-lib.org/reference/date_build.md). This
allows you to specify the components individually.

``` r
date_build(2019, 2, 1:5)
#> [1] "2019-02-01" "2019-02-02" "2019-02-03" "2019-02-04" "2019-02-05"
```

If you happen to specify an *invalid date*, you’ll get an error message:

``` r
date_build(2019, 1:12, 31)
#> Error in `invalid_resolve()`:
#> ! Invalid date found at location 2.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.
```

One way to resolve this is by specifying an invalid date resolution
strategy using the `invalid` argument. There are multiple options, but
in this case we’ll ask for the invalid dates to be set to the previous
valid moment in time.

``` r
date_build(2019, 1:12, 31, invalid = "previous")
#>  [1] "2019-01-31" "2019-02-28" "2019-03-31" "2019-04-30" "2019-05-31"
#>  [6] "2019-06-30" "2019-07-31" "2019-08-31" "2019-09-30" "2019-10-31"
#> [11] "2019-11-30" "2019-12-31"
```

To learn more about invalid dates, check out the documentation for
[`invalid_resolve()`](https://clock.r-lib.org/reference/clock-invalid.md).

If we were actually after the “last day of the month”, an easier way to
specify this would have been:

``` r
date_build(2019, 1:12, "last")
#>  [1] "2019-01-31" "2019-02-28" "2019-03-31" "2019-04-30" "2019-05-31"
#>  [6] "2019-06-30" "2019-07-31" "2019-08-31" "2019-09-30" "2019-10-31"
#> [11] "2019-11-30" "2019-12-31"
```

You can also create date-times using
[`date_time_build()`](https://clock.r-lib.org/reference/date_time_build.md),
which generates a POSIXct. Note that you must supply a time zone!

``` r
date_time_build(2019, 1:5, 1, 2, 30, zone = "America/New_York")
#> [1] "2019-01-01 02:30:00 EST" "2019-02-01 02:30:00 EST"
#> [3] "2019-03-01 02:30:00 EST" "2019-04-01 02:30:00 EDT"
#> [5] "2019-05-01 02:30:00 EDT"
```

If you “build” a time that doesn’t exist, you’ll get an error. For
example, on March 8th, 2020, there was a daylight saving time gap of 1
hour in the America/New_York time zone that took us from `01:59:59`
directly to `03:00:00`, skipping the 2 o’clock hour entirely. Let’s
“accidentally” create a time in that gap:

``` r
date_time_build(2019:2021, 3, 8, 2, 30, zone = "America/New_York")
#> Error in `as_zoned_time()`:
#> ! Nonexistent time due to daylight saving time at location 2.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.
```

To resolve this issue, we can specify a nonexistent time resolution
strategy through the `nonexistent` argument. There are a number of
options, including rolling forward or backward to the next or previous
valid moments in time:

``` r
zone <- "America/New_York"

date_time_build(2019:2021, 3, 8, 2, 30, zone = zone, nonexistent = "roll-forward")
#> [1] "2019-03-08 02:30:00 EST" "2020-03-08 03:00:00 EDT"
#> [3] "2021-03-08 02:30:00 EST"
date_time_build(2019:2021, 3, 8, 2, 30, zone = zone, nonexistent = "roll-backward")
#> [1] "2019-03-08 02:30:00 EST" "2020-03-08 01:59:59 EST"
#> [3] "2021-03-08 02:30:00 EST"
```

## Parsing

### Parsing dates

To parse dates, use
[`date_parse()`](https://clock.r-lib.org/reference/date_parse.md).
Parsing dates requires a *format string*, a combination of *commands*
that specify where date components are in your string. By default, it
assumes that you’re working with dates in the form `"%Y-%m-%d"`
(year-month-day).

``` r
date_parse("2019-01-05")
#> [1] "2019-01-05"
```

You can change the format string using `format`:

``` r
date_parse("January 5, 2020", format = "%B %d, %Y")
#> [1] "2020-01-05"
```

Various different locales are supported for parsing month and weekday
names in different languages. To parse a French month:

``` r
date_parse(
  "juillet 10, 2021", 
  format = "%B %d, %Y", 
  locale = clock_locale("fr")
)
#> [1] "2021-07-10"
```

You can learn about more locale options in the documentation for
[`clock_locale()`](https://clock.r-lib.org/reference/clock_locale.md).

If you have heterogeneous dates, you can supply multiple format strings:

``` r
x <- c("2020/1/5", "10-03-05", "2020/2/2")
formats <- c("%Y/%m/%d", "%y-%m-%d")

date_parse(x, format = formats)
#> [1] "2020-01-05" "2010-03-05" "2020-02-02"
```

### Parsing date-times

You have four options when parsing date-times:

- [`date_time_parse()`](https://clock.r-lib.org/reference/date-time-parse.md):
  For strings like `"2020-01-01 01:02:03"` where there is neither a time
  zone offset nor a full (not abbreviated!) time zone name.

- [`date_time_parse_complete()`](https://clock.r-lib.org/reference/date-time-parse.md):
  For strings like `"2020-01-01T01:02:03-05:00[America/New_York]"` where
  there is both a time zone offset and time zone name present in the
  string.

- [`date_time_parse_abbrev()`](https://clock.r-lib.org/reference/date-time-parse.md):
  For strings like `"2020-01-01 01:02:03 EST"` where there is a time
  zone abbreviation in the string.

- [`date_time_parse_RFC_3339()`](https://clock.r-lib.org/reference/date-time-parse.md):
  For strings like `"2020-01-01T01:02:03Z"` or
  `"2020-01-01T01:02:03-05:00"`, which are in RFC 3339 format and are
  intended to be interpreted as UTC.

#### date_time_parse()

[`date_time_parse()`](https://clock.r-lib.org/reference/date-time-parse.md)
requires a `zone` argument, and will ignore any other zone information
in the string (i.e. if you tried to specify `%z` and `%Z`). The default
format string is `"%Y-%m-%d %H:%M:%S"`.

``` r
date_time_parse("2020-01-01 01:02:03", "America/New_York")
#> [1] "2020-01-01 01:02:03 EST"
```

If you happen to parse an invalid or ambiguous date-time, you’ll get an
error. For example, on November 1st, 2020, there were *two* 1 o’clock
hours in the America/New_York time zone due to a daylight saving time
fallback. You can see that if we parse a time right before the fallback,
and then shift it forward by 1 second, and then 1 hour and 1 second,
respectively:

``` r
before <- date_time_parse("2020-11-01 00:59:59", "America/New_York")

# First 1 o'clock
before + 1
#> [1] "2020-11-01 01:00:00 EDT"

# Second 1 o'clock
before + 1 + 3600
#> [1] "2020-11-01 01:00:00 EST"
```

The following string doesn’t include any information about which of
these two 1 o’clocks it belongs to, so it is considered *ambiguous*.
Ambiguous times will error when parsing:

``` r
date_time_parse("2020-11-01 01:30:00", "America/New_York")
#> Error in `as_zoned_time()`:
#> ! Ambiguous time due to daylight saving time at location 1.
#> ℹ Resolve ambiguous time issues by specifying the `ambiguous` argument.
```

To fix that, you can specify an ambiguous time resolution strategy with
the `ambiguous` argument.

``` r
zone <- "America/New_York"

date_time_parse("2020-11-01 01:30:00", zone, ambiguous = "earliest")
#> [1] "2020-11-01 01:30:00 EDT"
date_time_parse("2020-11-01 01:30:00", zone, ambiguous = "latest")
#> [1] "2020-11-01 01:30:00 EST"
```

#### date_time_parse_complete()

[`date_time_parse_complete()`](https://clock.r-lib.org/reference/date-time-parse.md)
doesn’t have a `zone` argument, and doesn’t require `ambiguous` or
`nonexistent` arguments, since it assumes that the string you are
providing is completely unambiguous. The only way this is possible is by
having both a time zone offset, specified by `%z`, and a full time zone
name, specified by `%Z`, in the string.

The following is an example of an “extended” RFC 3339 format used by
Java 8’s time library to specify complete date-time strings. This is
something that
[`date_time_parse_complete()`](https://clock.r-lib.org/reference/date-time-parse.md)
can parse. The default format string follows this extended format, and
is `"%Y-%m-%dT%H:%M:%S%z[%Z]"`.

``` r
x <- "2020-01-01T01:02:03-05:00[America/New_York]"

date_time_parse_complete(x)
#> [1] "2020-01-01 01:02:03 EST"
```

#### date_time_parse_abbrev()

[`date_time_parse_abbrev()`](https://clock.r-lib.org/reference/date-time-parse.md)
is useful when your date-time strings contain a time zone abbreviation
rather than a time zone offset or full time zone name.

``` r
x <- "2020-01-01 01:02:03 EST"

date_time_parse_abbrev(x, "America/New_York")
#> [1] "2020-01-01 01:02:03 EST"
```

The string is first parsed as a naive time without considering the
abbreviation, and is then converted to a zoned-time using the supplied
`zone`. If an ambiguous time is parsed, the abbreviation is used to
resolve the ambiguity.

``` r
x <- c(
  "1970-10-25 01:30:00 EDT",
  "1970-10-25 01:30:00 EST"
)

date_time_parse_abbrev(x, "America/New_York")
#> [1] "1970-10-25 01:30:00 EDT" "1970-10-25 01:30:00 EST"
```

You might be wondering why you need to supply `zone` at all. Isn’t the
abbreviation enough? Unfortunately, multiple countries use the same time
zone abbreviations, even though they have different time zones. This
means that, in many cases, the abbreviation alone is ambiguous. For
example, both India and Israel use `IST` for their standard times.

``` r
x <- "1970-01-01 02:30:30 IST"

# IST = India Standard Time
date_time_parse_abbrev(x, "Asia/Kolkata")
#> [1] "1970-01-01 02:30:30 IST"

# IST = Israel Standard Time
date_time_parse_abbrev(x, "Asia/Jerusalem")
#> [1] "1970-01-01 02:30:30 IST"
```

#### date_time_parse_RFC_3339()

[`date_time_parse_RFC_3339()`](https://clock.r-lib.org/reference/date-time-parse.md)
is useful when your date-time strings come from an API, which means they
are likely in an ISO 8601 or RFC 3339 format, and should be interpreted
as UTC.

The default format string parses the typical RFC 3339 format of
`"%Y-%m-%dT%H:%M:%SZ"`.

``` r
x <- "2020-01-01T01:02:03Z"

date_time_parse_RFC_3339(x)
#> [1] "2020-01-01 01:02:03 UTC"
```

If your date-time strings contain a numeric offset from UTC rather than
a `"Z"`, then you’ll need to set the `offset` argument to one of the
following:

- `"%z"` if the offset is of the form `"-0500"`.
- `"%Ez"` if the offset is of the form `"-05:00"`.

``` r
x <- "2020-01-01T01:02:03-0500"

date_time_parse_RFC_3339(x, offset = "%z")
#> [1] "2020-01-01 06:02:03 UTC"

x <- "2020-01-01T01:02:03-05:00"

date_time_parse_RFC_3339(x, offset = "%Ez")
#> [1] "2020-01-01 06:02:03 UTC"
```

## Grouping, rounding and shifting

When performing time-series related data analysis, you often need to
summarize your series at a less precise precision. There are many
different ways to do this, and the differences between them are subtle,
but meaningful. clock offers three different sets of functions for
summarization:

- [`date_group()`](https://clock.r-lib.org/reference/date_group.md)

- [`date_floor()`](https://clock.r-lib.org/reference/date-and-date-time-rounding.md),
  [`date_ceiling()`](https://clock.r-lib.org/reference/date-and-date-time-rounding.md),
  and
  [`date_round()`](https://clock.r-lib.org/reference/date-and-date-time-rounding.md)

- [`date_shift()`](https://clock.r-lib.org/reference/date-and-date-time-shifting.md)

### Grouping

Grouping allows you to summarize a component of a date or date-time
*within* other components. An example of this is grouping by day of the
month, which summarizes the day component *within* the current
year-month.

``` r
x <- seq(date_build(2019, 1, 20), date_build(2019, 2, 5), by = 1)
x
#>  [1] "2019-01-20" "2019-01-21" "2019-01-22" "2019-01-23" "2019-01-24"
#>  [6] "2019-01-25" "2019-01-26" "2019-01-27" "2019-01-28" "2019-01-29"
#> [11] "2019-01-30" "2019-01-31" "2019-02-01" "2019-02-02" "2019-02-03"
#> [16] "2019-02-04" "2019-02-05"

# Grouping by 5 days of the current month
date_group(x, "day", n = 5)
#>  [1] "2019-01-16" "2019-01-21" "2019-01-21" "2019-01-21" "2019-01-21"
#>  [6] "2019-01-21" "2019-01-26" "2019-01-26" "2019-01-26" "2019-01-26"
#> [11] "2019-01-26" "2019-01-31" "2019-02-01" "2019-02-01" "2019-02-01"
#> [16] "2019-02-01" "2019-02-01"
```

The thing to note about grouping by day of the month is that at the end
of each month, the groups restart. So this created groups for January of
`[1, 5], [6, 10], [11, 15], [16, 20], [21, 25], [26, 30], [31]`.

You can also group by month or year:

``` r
date_group(x, "month")
#>  [1] "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01"
#>  [6] "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01"
#> [11] "2019-01-01" "2019-01-01" "2019-02-01" "2019-02-01" "2019-02-01"
#> [16] "2019-02-01" "2019-02-01"
```

This also works with date-times, adding the ability to group by hour of
the day, minute of the hour, and second of the minute.

``` r
x <- seq(
  date_time_build(2019, 1, 1, 1, 55, zone = "UTC"),
  date_time_build(2019, 1, 1, 2, 15, zone = "UTC"),
  by = 120
)
x
#>  [1] "2019-01-01 01:55:00 UTC" "2019-01-01 01:57:00 UTC"
#>  [3] "2019-01-01 01:59:00 UTC" "2019-01-01 02:01:00 UTC"
#>  [5] "2019-01-01 02:03:00 UTC" "2019-01-01 02:05:00 UTC"
#>  [7] "2019-01-01 02:07:00 UTC" "2019-01-01 02:09:00 UTC"
#>  [9] "2019-01-01 02:11:00 UTC" "2019-01-01 02:13:00 UTC"
#> [11] "2019-01-01 02:15:00 UTC"

date_group(x, "minute", n = 5)
#>  [1] "2019-01-01 01:55:00 UTC" "2019-01-01 01:55:00 UTC"
#>  [3] "2019-01-01 01:55:00 UTC" "2019-01-01 02:00:00 UTC"
#>  [5] "2019-01-01 02:00:00 UTC" "2019-01-01 02:05:00 UTC"
#>  [7] "2019-01-01 02:05:00 UTC" "2019-01-01 02:05:00 UTC"
#>  [9] "2019-01-01 02:10:00 UTC" "2019-01-01 02:10:00 UTC"
#> [11] "2019-01-01 02:15:00 UTC"
```

### Rounding

While grouping is useful for summarizing *within* a component, rounding
is useful for summarizing *across* components. It is great for
summarizing by, say, a rolling set of 60 days.

Rounding operates on the underlying count that makes up your date or
date-time. To see what I mean by this, try unclassing a date:

``` r
unclass(date_build(2020, 1, 1))
#> [1] 18262
```

This is a count of days since the *origin* that R uses, 1970-01-01,
which is considered day 0. If you were to floor by 60 days, this would
bundle `[1970-01-01, 1970-03-02), [1970-03-02, 1970-05-01)`, and so on.
Equivalently, it bundles counts of `[0, 60), [60, 120)`, etc.

``` r
x <- seq(date_build(1970, 01, 01), date_build(1970, 05, 10), by = 20)

date_floor(x, "day", n = 60)
#> [1] "1970-01-01" "1970-01-01" "1970-01-01" "1970-03-02" "1970-03-02"
#> [6] "1970-03-02" "1970-05-01"
date_ceiling(x, "day", n = 60)
#> [1] "1970-01-01" "1970-03-02" "1970-03-02" "1970-03-02" "1970-05-01"
#> [6] "1970-05-01" "1970-05-01"
```

If you prefer a different origin, you can supply a Date `origin` to
[`date_floor()`](https://clock.r-lib.org/reference/date-and-date-time-rounding.md),
which determines what “day 0” is considered to be. This can be useful
for grouping by multiple weeks if you want to control what is considered
the start of the week. Since 1970-01-01 is a Thursday, flooring by 2
weeks would normally generate all Thursdays:

``` r
as_weekday(date_floor(x, "week", n = 14))
#> <weekday[7]>
#> [1] Thu Thu Thu Thu Thu Thu Thu
```

To change this you can supply an `origin` on the weekday that you’d like
to be considered the first day of the week.

``` r
sunday <- date_build(1970, 01, 04)

date_floor(x, "week", n = 14, origin = sunday)
#> [1] "1969-09-28" "1970-01-04" "1970-01-04" "1970-01-04" "1970-01-04"
#> [6] "1970-01-04" "1970-04-12"

as_weekday(date_floor(x, "week", n = 14, origin = sunday))
#> <weekday[7]>
#> [1] Sun Sun Sun Sun Sun Sun Sun
```

If you only need to floor by 1 week, it is often easier to use
[`date_shift()`](https://clock.r-lib.org/reference/date-and-date-time-shifting.md),
as seen in the next section.

### Shifting

[`date_shift()`](https://clock.r-lib.org/reference/date-and-date-time-shifting.md)
allows you to target a weekday, and then shift a vector of dates forward
or backward to the next instance of that target. It requires using one
of the new types in clock, *weekday*, which is supplied as the target.

For example, to shift to the next Tuesday:

``` r
x <- date_build(2020, 1, 1:2)

# Wednesday / Thursday
as_weekday(x)
#> <weekday[2]>
#> [1] Wed Thu

# `clock_weekdays` is a helper that returns the code corresponding to
# the requested day of the week
clock_weekdays$tuesday
#> [1] 3

tuesday <- weekday(clock_weekdays$tuesday)
tuesday
#> <weekday[1]>
#> [1] Tue

date_shift(x, target = tuesday)
#> [1] "2020-01-07" "2020-01-07"
```

Shifting to the *previous* day of the week is a nice way to floor by 1
week. It allows you to control the start of the week in a way that is
slightly easier than using `date_floor(origin = )`.

``` r
x <- seq(date_build(1970, 01, 01), date_build(1970, 01, "last"), by = 3)

date_shift(x, tuesday, which = "previous")
#>  [1] "1969-12-30" "1969-12-30" "1970-01-06" "1970-01-06" "1970-01-13"
#>  [6] "1970-01-13" "1970-01-13" "1970-01-20" "1970-01-20" "1970-01-27"
#> [11] "1970-01-27"
```

## Arithmetic

You can do arithmetic with dates and date-times using the family of
`add_*()` functions. With dates, you can add years, months, and days.
With date-times, you can additionally add hours, minutes, and seconds.

``` r
x <- date_build(2020, 1, 1)

add_years(x, 1:5)
#> [1] "2021-01-01" "2022-01-01" "2023-01-01" "2024-01-01" "2025-01-01"
```

One of the neat parts about clock is that it requires you to be explicit
about how you want to handle invalid dates when doing arithmetic. What
is 1 month after January 31st? If you try and create this date, you’ll
get an error.

``` r
x <- date_build(2020, 1, 31)

add_months(x, 1)
#> Error in `invalid_resolve()`:
#> ! Invalid date found at location 1.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.
```

clock gives you the power to handle this through the `invalid` option:

``` r
# The previous valid moment in time
add_months(x, 1, invalid = "previous")
#> [1] "2020-02-29"

# The next valid moment in time
add_months(x, 1, invalid = "next")
#> [1] "2020-03-01"

# Overflow the days. There were 29 days in February, 2020, but we
# specified 31. So this overflows 2 days past day 29.
add_months(x, 1, invalid = "overflow")
#> [1] "2020-03-02"

# If you don't consider it to be a valid date
add_months(x, 1, invalid = "NA")
#> [1] NA
```

As a teaser, the low level library has a *calendar* type named
year-month-day that powers this operation. It actually gives you *more*
flexibility, allowing `"2020-02-31"` to exist in the wild:

``` r
ymd <- as_year_month_day(x) + duration_months(1)
ymd
#> <year_month_day<day>[1]>
#> [1] "2020-02-31"
```

You can use `invalid_resolve(invalid =)` to resolve this like you did in
[`add_months()`](https://clock.r-lib.org/reference/clock-arithmetic.md),
or you can let it hang around if you expect other operations to make it
“valid” again.

``` r
# Adding 1 more month makes it valid again
ymd + duration_months(1)
#> <year_month_day<day>[1]>
#> [1] "2020-03-31"
```

When working with date-times, you can additionally add hours, minutes,
and seconds.

``` r
x <- date_time_build(2020, 1, 1, 2, 30, zone = "America/New_York")

x %>%
  add_days(1) %>%
  add_hours(2:5)
#> [1] "2020-01-02 04:30:00 EST" "2020-01-02 05:30:00 EST"
#> [3] "2020-01-02 06:30:00 EST" "2020-01-02 07:30:00 EST"
```

When adding units of time to a POSIXct, you have to be very careful with
daylight saving time issues. clock tries to help you out by letting you
know when you run into an issue:

``` r
x <- date_time_build(1970, 04, 25, 02, 30, 00, zone = "America/New_York")
x
#> [1] "1970-04-25 02:30:00 EST"

# Daylight saving time gap on the 26th between 01:59:59 -> 03:00:00
x %>% add_days(1)
#> Error in `as_zoned_time()`:
#> ! Nonexistent time due to daylight saving time at location 1.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.
```

You can solve this using the `nonexistent` argument to control how these
times should be handled.

``` r
# Roll forward to the next valid moment in time
x %>% add_days(1, nonexistent = "roll-forward")
#> [1] "1970-04-26 03:00:00 EDT"

# Roll backward to the previous valid moment in time
x %>% add_days(1, nonexistent = "roll-backward")
#> [1] "1970-04-26 01:59:59 EST"

# Shift forward by adding the size of the DST gap
# (this often keeps the time of day,
# but doesn't guaratee that relative ordering in `x` is maintained
# so I don't recommend it)
x %>% add_days(1, nonexistent = "shift-forward")
#> [1] "1970-04-26 03:30:00 EDT"

# Replace nonexistent times with an NA
x %>% add_days(1, nonexistent = "NA")
#> [1] NA
```

## Getting and setting

clock provides a family of getters and setters for working with dates
and date-times. You can get and set the year, month, or day of a date.

``` r
x <- date_build(2019, 5, 6)

get_year(x)
#> [1] 2019
get_month(x)
#> [1] 5
get_day(x)
#> [1] 6

x %>%
  set_day(22) %>%
  set_month(10)
#> [1] "2019-10-22"
```

As you might expect by now, setting the date to an invalid date requires
you to explicitly handle this:

``` r
x %>%
  set_day(31) %>%
  set_month(4)
#> Error in `invalid_resolve()`:
#> ! Invalid date found at location 1.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.

x %>%
  set_day(31) %>%
  set_month(4, invalid = "previous")
#> [1] "2019-04-30"
```

You can additionally set the hour, minute, and second of a POSIXct.

``` r
x <- date_time_build(2020, 1, 2, 3, zone = "America/New_York")
x
#> [1] "2020-01-02 03:00:00 EST"

x %>%
  set_minute(5) %>%
  set_second(10)
#> [1] "2020-01-02 03:05:10 EST"
```

As with other manipulations of POSIXct, you’ll have to be aware of
daylight saving time when setting components. You may need to supply the
`nonexistent` or `ambiguous` arguments of the `set_*()` functions to
handle these issues.
