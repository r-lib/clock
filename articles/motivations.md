# Motivations for clock

``` r
library(clock)
library(lubridate)
library(magrittr)
library(rlang)
```

## Motivations

R has always had strong date and date-time capabilities. The Date,
POSIXct, and POSIXlt classes provide a powerful foundation to build on,
and the R community has done just that.
[lubridate](https://lubridate.tidyverse.org/),
[zoo](https://CRAN.R-project.org/package=zoo), and
[tsibble](https://CRAN.R-project.org/package=tsibble) are just a few of
the popular packages that have extended R’s native date-time
capabilities.

So, why clock?

The following are a few of clock’s goals:

- Explicitly handle invalid dates

- Explicitly handle daylight saving time issues

- Expose naive types for representing date-times without a time zone

- Provide calendar types for representing calendar “dates” in
  alternative ways

- Provide variable precision date-time types

Dealing with dates and date-times is difficult enough as it is. clock’s
overarching mission is to accomplish its goals in the most explicit and
unambiguous way possible. It does this through a combination of:

- Erroring early and verbosely

- Requiring named optional arguments everywhere

- Providing an optional “strict” mode for production systems that forces
  you to explicitly type out your assumptions for invalid dates and
  daylight saving time issues

You might also be wondering, why not add these features to lubridate
instead? While clock’s high-level API and lubridate have similar overall
scopes and feel, they are powered by two very different systems. The way
that you solve problems with clock is often very different from what you
might do with lubridate. Merging this new system into lubridate would be
a very difficult task, and would involve many backwards incompatible
changes. Additionally, clock’s low-level API provides new types and
capabilities that are out of scope even for lubridate, which focuses on
R’s native date-time types.

The rest of this vignette investigates each of clock’s goals in more
detail. Where relevant, a comparison against lubridate is made to
demonstrate some of the improvements and consistency that come with
clock.

Parts of this document are extremely detailed, and can get very
technical. They are included to provide examples of tough date-time
manipulation problems that I feel are under-discussed, but feel free to
skip around.

### Invalid dates

#### Dates

What is 1 month after January 31st, 2020? Let’s ask lubridate:

``` r
x <- add_days(as.Date("2020-01-31"), c(-2, -1, 0, 1))
x
#> [1] "2020-01-29" "2020-01-30" "2020-01-31" "2020-02-01"

x + months(1)
#> [1] "2020-02-29" NA           NA           "2020-03-01"
```

In this example, we’ve asked lubridate to create two *invalid dates*,
`2020-02-30` and `2020-02-31`. Invalid dates often arise with
month-based arithmetic, but they also arise when forcibly setting the
month to a specific value. Because these dates don’t exist, lubridate
returns `NA`. When working with time series, it is often more useful to
return *something* back, but what? We could:

- Return `NA` as lubridate does

- Return the previous valid moment in time

- Return the next valid moment in time

- Overflow our invalid date into March by the number of days past the
  true end of February that it landed at

Only the first is available to you with `+ months(1)`. To supplement
this, lubridate added `%m+%` and
[`add_with_rollback()`](https://lubridate.tidyverse.org/reference/mplus.html).

``` r
# Equivalent to `add_with_rollback(x, months(1))`
x %m+% months(1)
#> [1] "2020-02-29" "2020-02-29" "2020-02-29" "2020-03-01"

add_with_rollback(x, months(1), roll_to_first = TRUE)
#> [1] "2020-02-29" "2020-03-01" "2020-03-01" "2020-03-01"
```

With Dates, the hardest part about `%m+%` is remembering to use it. It
is a common bug to forget to use this helper until *after* you have been
bitten by an invalid date issue with an unexpected `NA`.

In clock, invalid dates must be handled explicitly as they occur. The
default behavior raises an error.

``` r
add_months(x, 1)
#> Error in `invalid_resolve()`:
#> ! Invalid date found at location 2.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.
```

You can handle these issues with an *invalid date resolution strategy*:

``` r
# Previous moment in time
add_months(x, 1, invalid = "previous")
#> [1] "2020-02-29" "2020-02-29" "2020-02-29" "2020-03-01"

# Next moment in time
add_months(x, 1, invalid = "next")
#> [1] "2020-02-29" "2020-03-01" "2020-03-01" "2020-03-01"

# Overflow by 1 day for 30th, 2 days for 31st
add_months(x, 1, invalid = "overflow")
#> [1] "2020-02-29" "2020-03-01" "2020-03-02" "2020-03-01"

# Like lubridate
add_months(x, 1, invalid = "NA")
#> [1] "2020-02-29" NA           NA           "2020-03-01"
```

I recommend using either `"previous"` or `"next"` at all times.
Overflowing can be useful in certain scenarios, but it can also result
in losing the relative ordering of your input. In the previous example,
note that `x` is no longer sorted after adding 1 month when using
`"overflow"`. If your analysis requires a (weakly) increasing sequence
of dates, as many time series analyses do, then you should use
`"previous"` or `"next"`, which are guaranteed to maintain relative
ordering.

``` r
order(x)
#> [1] 1 2 3 4

order(add_months(x, 1, invalid = "overflow"))
#> [1] 1 2 4 3
```

#### Date-times

With date-times, when you hit invalid dates you also have to consider
what to do with the time of day.

``` r
x <- as.POSIXct("2020-01-31", "America/New_York") %>%
  add_days(c(-2, -1, 0, 1)) %>%
  add_hours(4:1)

x
#> [1] "2020-01-29 04:00:00 EST" "2020-01-30 03:00:00 EST"
#> [3] "2020-01-31 02:00:00 EST" "2020-02-01 01:00:00 EST"
```

[`add_with_rollback()`](https://lubridate.tidyverse.org/reference/mplus.html)
defaults to preserving the time of day. Invalid dates are rolled back to
the previous valid *day*, but the time of day of `x` is kept:

``` r
add_with_rollback(x, months(1))
#> [1] "2020-02-29 04:00:00 EST" "2020-02-29 03:00:00 EST"
#> [3] "2020-02-29 02:00:00 EST" "2020-03-01 01:00:00 EST"
```

I think this is dangerous for the same reason I advised not using
`"overflow"` in the Date section above. You can easily lose the relative
ordering within `x`.

``` r
order(x)
#> [1] 1 2 3 4

order(add_with_rollback(x, months(1)))
#> [1] 3 2 1 4
```

[`add_with_rollback()`](https://lubridate.tidyverse.org/reference/mplus.html)
has a `preserve_hms` argument, but this can’t help here, as it just sets
the time of day to midnight and the ordering is still lost.

``` r
add_with_rollback(x, months(1), preserve_hms = FALSE)
#> [1] "2020-02-29 04:00:00 EST" "2020-02-29 00:00:00 EST"
#> [3] "2020-02-29 00:00:00 EST" "2020-03-01 01:00:00 EST"

order(add_with_rollback(x, months(1), preserve_hms = FALSE))
#> [1] 2 3 1 4
```

With clock, using `"previous"` and `"next"` will use the previous and
next valid *moment in time*. With POSIXct (which clock assumes to have
second precision), this chooses the previous and next valid second. This
is the only way to preserve relative ordering in `x`, which is another
reason I advocate for using these options.

``` r
add_months(x, 1, invalid = "previous")
#> [1] "2020-02-29 04:00:00 EST" "2020-02-29 23:59:59 EST"
#> [3] "2020-02-29 23:59:59 EST" "2020-03-01 01:00:00 EST"

add_months(x, 1, invalid = "next")
#> [1] "2020-02-29 04:00:00 EST" "2020-03-01 00:00:00 EST"
#> [3] "2020-03-01 00:00:00 EST" "2020-03-01 01:00:00 EST"

order(add_months(x, 1, invalid = "previous"))
#> [1] 1 2 3 4
order(add_months(x, 1, invalid = "next"))
#> [1] 1 2 3 4
```

If you really want lubridate’s
[`add_with_rollback()`](https://lubridate.tidyverse.org/reference/mplus.html)
behavior of retaining the time of day, you can use `"previous-day"` and
`"next-day"`, which only adjust the day component, leaving the time of
day intact. Again, I don’t recommend this.

``` r
add_months(x, 1, invalid = "previous-day")
#> [1] "2020-02-29 04:00:00 EST" "2020-02-29 03:00:00 EST"
#> [3] "2020-02-29 02:00:00 EST" "2020-03-01 01:00:00 EST"

add_months(x, 1, invalid = "next-day")
#> [1] "2020-02-29 04:00:00 EST" "2020-03-01 03:00:00 EST"
#> [3] "2020-03-01 02:00:00 EST" "2020-03-01 01:00:00 EST"
```

### Daylight saving time

In general, there are two types of daylight saving time issues that
might come up with dealing with date-times:

- Nonexistent times due to daylight saving time gaps

- Ambiguous times due to daylight saving time fallbacks

#### Nonexistent time

On March 8th, 2020 in New York (the America/New_York time zone), the
clocks jumped forward from `01:59:59` to `03:00:00`. This created a
daylight saving time gap, resulting in a *nonexistent* 2 o’clock hour.
These daylight saving time gaps happen once a year in a large number of
time zones around the world. Luckily, they often happen at irregular
times, like 2 AM, so they often don’t interfere with your analyses. That
said, it is entirely possible that you could find yourself landing in
one of these gaps, so it is important to know how to deal with them.

``` r
# 1 day before the gap
x <- date_time_parse(
  "March 7, 2020 02:30:00", "America/New_York", 
  format = "%B %d, %Y %H:%M:%S"
)
x <- add_minutes(x, c(-45, 0, 45))
x
#> [1] "2020-03-07 01:45:00 EST" "2020-03-07 02:30:00 EST"
#> [3] "2020-03-07 03:15:00 EST"
```

Let’s try adding 1 day to these times with lubridate. As you might
expect, you get an `NA`.

``` r
x + days(1)
#> [1] "2020-03-08 01:45:00 EST" NA                       
#> [3] "2020-03-08 03:15:00 EDT"
```

Unlike with invalid dates, there aren’t any tools like
[`add_with_rollback()`](https://lubridate.tidyverse.org/reference/mplus.html)
to help you out here. You’re stuck with the `NA`. But what could you do?
Possibly:

- Roll forward to the next valid moment in time

- Roll backward to the previous valid moment in time

- Shift forward by the size of the gap

- Shift backward by the size of the gap

- Return `NA`

clock allows you to do all of these things. By default, nonexistent
times result in an error.

``` r
add_days(x, 1)
#> Error in `as_zoned_time()`:
#> ! Nonexistent time due to daylight saving time at location 2.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.
```

Solve these issues with a *nonexistent time resolution strategy*:

``` r
# 02:30:00 -> 03:00:00
add_days(x, 1, nonexistent = "roll-forward")
#> [1] "2020-03-08 01:45:00 EST" "2020-03-08 03:00:00 EDT"
#> [3] "2020-03-08 03:15:00 EDT"

# 02:30:00 -> 01:59:59
add_days(x, 1, nonexistent = "roll-backward")
#> [1] "2020-03-08 01:45:00 EST" "2020-03-08 01:59:59 EST"
#> [3] "2020-03-08 03:15:00 EDT"

# 02:30:00 + gap of 1 hr = 03:30:00
add_days(x, 1, nonexistent = "shift-forward")
#> [1] "2020-03-08 01:45:00 EST" "2020-03-08 03:30:00 EDT"
#> [3] "2020-03-08 03:15:00 EDT"

# 02:30:00 - gap of 1 hr = 01:30:00
add_days(x, 1, nonexistent = "shift-backward")
#> [1] "2020-03-08 01:45:00 EST" "2020-03-08 01:30:00 EST"
#> [3] "2020-03-08 03:15:00 EDT"

# lubridate behavior
add_days(x, 1, nonexistent = "NA")
#> [1] "2020-03-08 01:45:00 EST" NA                       
#> [3] "2020-03-08 03:15:00 EDT"
```

I recommend using `"roll-forward"` and `"roll-backward"`, as they are
guaranteed to retain the relative ordering of `x`. Shifting, while
sometimes useful, will not always retain this:

``` r
order(x)
#> [1] 1 2 3

order(add_days(x, 1, nonexistent = "shift-forward"))
#> [1] 1 3 2
```

When doing arithmetic with clock’s high-level API, you’ll only encounter
these nonexistent time issues when adding “large” units of time, like
days, months, or years. Adding hours, minutes, or seconds will first
convert to UTC, add the units of time there, and then convert back to
your original time zone, which never creates any problematic times. This
might sound a bit strange, but it is generally what you want when
working with these smaller units of time.

``` r
# 1 second before the gap
x <- date_time_parse(
  "March 8, 2020 01:59:59", "America/New_York", 
  format = "%B %d, %Y %H:%M:%S"
)

add_seconds(x, 1)
#> [1] "2020-03-08 03:00:00 EDT"
```

This is essentially equivalent to using
[`dseconds()`](https://lubridate.tidyverse.org/reference/duration.html)
from lubridate. Note that adding
[`seconds()`](https://lubridate.tidyverse.org/reference/period.html)
would “land” you in the gap, but
[`dseconds()`](https://lubridate.tidyverse.org/reference/duration.html)
jumps right over it.

``` r
x + seconds(1)
#> [1] NA

x + dseconds(1)
#> [1] "2020-03-08 03:00:00 EDT"
```

If for any reason you want the `+ seconds()` behavior, you can manually
take control by converting to naive-time, adding the units of time
there, and then converting back to POSIXct.

``` r
x %>%
  as_naive_time() %>%
  add_seconds(1) %>%
  as.POSIXct(date_time_zone(x))
#> Error in `as_zoned_time()`:
#> ! Nonexistent time due to daylight saving time at location 1.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.

x %>%
  as_naive_time() %>%
  add_seconds(1) %>%
  as.POSIXct(date_time_zone(x), nonexistent = "roll-forward")
#> [1] "2020-03-08 03:00:00 EDT"
```

#### Ambiguous time

On November 1st, 2020 in New York (the America/New_York time zone), the
clocks fell backwards from `01:59:59 EDT` to `01:00:00 EST`. This
created a daylight saving time fallback, resulting in two 1 o’clock
hours. Without prior knowledge about whether the daylight time or
standard time 1 o’clock should be used, times that fall in the two 1
o’clock hours are considered *ambiguous*.

Like with nonexistent times, adding hours, minutes, or seconds with the
high-level API won’t generate ambiguous time issues, as it always adds a
fixed duration of seconds to the underlying numeric representation of
your date-time. This gives us an easy way to show the two 1 o’clocks:

``` r
x <- date_time_parse("2020-11-01 00:59:59", "America/New_York")

# The first 1 o'clock
add_seconds(x, 1)
#> [1] "2020-11-01 01:00:00 EDT"

# The second 1 o'clock
add_seconds(x, 3601)
#> [1] "2020-11-01 01:00:00 EST"
```

If we instead worked with units of months, it would be possible to
generate an ambiguous time:

``` r
# 1 month before the two ambiguous times
x <- date_time_parse("2020-10-01 01:30:00", "America/New_York")

# Which of the two 1 o'clocks are we talking about? It is ambiguous!
add_months(x, 1)
#> Error in `as_zoned_time()`:
#> ! Ambiguous time due to daylight saving time at location 1.
#> ℹ Resolve ambiguous time issues by specifying the `ambiguous` argument.
```

You can handle these by setting an *ambiguous time resolution strategy*:

``` r
# The 1 o'clock that is earliest in the timeline
add_months(x, 1, ambiguous = "earliest")
#> [1] "2020-11-01 01:30:00 EDT"

# The 1 o'clock that is latest in the timeline
add_months(x, 1, ambiguous = "latest")
#> [1] "2020-11-01 01:30:00 EST"

# Ambiguous times should return NA
add_months(x, 1, ambiguous = "NA")
#> [1] NA
```

The lubridate behavior here is a bit tricky to explain. In some
examples, it might seem to give a more obvious answer:

``` r
# 1 month after the fallback
x_after <- x + months(2)

# Start before the fallback, add time, return the earliest ambiguous time
x
#> [1] "2020-10-01 01:30:00 EDT"
x + months(1)
#> [1] "2020-11-01 01:30:00 EDT"

# Start after the fallback, subtract time, return the latest ambiguous time
x_after
#> [1] "2020-12-01 01:30:00 EST"
x_after - months(1)
#> [1] "2020-11-01 01:30:00 EDT"
```

But with other examples, you can end up with confusing results:

``` r
x_year_before <- x - years(1) + months(1)
x_year_after <- x + years(1) + months(1)

# Start before the fallback, add time, return the earliest ambiguous time
x_year_before
#> [1] "2019-11-01 01:30:00 EDT"
x_year_before + years(1)
#> [1] "2020-11-01 01:30:00 EDT"

# Start after the fallback, subtract time, return the...earliest ambiguous time?
x_year_after
#> [1] "2021-11-01 01:30:00 EDT"
x_year_after - years(1)
#> [1] "2020-11-01 01:30:00 EDT"
```

When lubridate hits an ambiguous time, the UTC offset of the result
always matches the offset of the original input. Sometimes this makes
sense, but often when adding larger periods of time or updating multiple
components of a date-time, this can be confusing.

This isn’t to say that the lubridate behavior is wrong. I would argue
that it is just a bit too lenient. In clock, a slightly modified form of
this behavior exists that is a bit stricter. Consider what would happen
if you tried to floor these dates to the nearest hour:

``` r
x <- as.POSIXct("2020-11-01 00:30:00", "America/New_York")
x <- add_hours(x, 0:5)
x
#> [1] "2020-11-01 00:30:00 EDT" "2020-11-01 01:30:00 EDT"
#> [3] "2020-11-01 01:30:00 EST" "2020-11-01 02:30:00 EST"
#> [5] "2020-11-01 03:30:00 EST" "2020-11-01 04:30:00 EST"
```

Notice that if we converted two of these dates to naive-time, and then
tried to convert them back to POSIXct, they would have been considered
ambiguous.

``` r
x %>%
  as_naive_time() %>%
  as.POSIXct(date_time_zone(x))
#> Error in `as_zoned_time()`:
#> ! Ambiguous time due to daylight saving time at location 2.
#> ℹ Resolve ambiguous time issues by specifying the `ambiguous` argument.
```

[`date_floor()`](https://clock.r-lib.org/reference/date-and-date-time-rounding.md)
works by flooring while in naive-time, but somehow this still produces
the results you probably expected, with the two 1 o’clock hours being
kept in separate groups:

``` r
date_floor(x, "hour")
#> [1] "2020-11-01 00:00:00 EDT" "2020-11-01 01:00:00 EDT"
#> [3] "2020-11-01 01:00:00 EST" "2020-11-01 02:00:00 EST"
#> [5] "2020-11-01 03:00:00 EST" "2020-11-01 04:00:00 EST"
```

If you look carefully at the documentation for the POSIXct method of
[`date_floor()`](https://clock.r-lib.org/reference/date-and-date-time-rounding.md),
you’ll notice that `ambiguous` actually defaults to `x`. When ambiguous
times occur and your `ambiguous` argument is set to a date-time, clock
will attempt to use information from that date-time to determine how to
resolve the ambiguity. To resolve ambiguity using a date-time
`ambiguous` value, the following two conditions must be met:

- If `ambiguous` was converted to naive-time, then back to POSIXct, it
  must be considered ambiguous.

- The transition point of the ambiguous naive-time implied by
  `ambiguous` and the naive-time that results from the operation you are
  performing must be the same.

In the above example, both of these bullet points are true (the
transition point of interest is `2020-11-01 01:00:00-05:00`, the first
valid moment in time after the fallback), which allows
[`date_floor()`](https://clock.r-lib.org/reference/date-and-date-time-rounding.md)
to resolve the ambiguity “automatically” without any user intervention.

As a final example, let’s change the `origin` that we floor relative to
to be 1am EDT, and then let’s floor by two hours. This should generate
hour groups of `[1, 3), [3, 5), ...` .

``` r
origin <- date_floor(x[2], "hour")
origin
#> [1] "2020-11-01 01:00:00 EDT"

date_floor(x, "hour", n = 2, origin = origin)
#> Error in `as_zoned_time()`:
#> ! Ambiguous time due to daylight saving time at location 4.
#> ℹ Resolve ambiguous time issues by specifying the `ambiguous` argument.
```

It seems that we have an ambiguous time issue at location 4, the
`02:30:00` time. Flooring this to every two hours with an origin of 1
o’clock places it in the `01:00:00` group, but trying to convert that
back to POSIXct fails. Unlike when flooring by a single hour, here we
have an original time that would *not* have been considered ambiguous if
converted to naive-time and back (bullet point 1 above). This implies
that we cannot use information from `ambiguous` to resolve this new
ambiguity.

To solve this, ideally we’d be able to manually fix the ambiguity that
`02:30:00` introduced while maintaining the automatic ambiguity
resolution behavior for the `01:30:00 EDT` and `01:30:00 EST`
date-times. To handle both of these issues at once, you can supply a
list for `ambiguous` where the first element is `x` and the second is an
ambiguous time resolution strategy to use when `x` isn’t enough to solve
the ambiguity.

``` r
# [23, 1 EDT), [1 EDT, 1 EST), [1 EST, 3), [3, 5)
date_floor(x, "hour", n = 2, origin = origin, ambiguous = list(x, "latest"))
#> [1] "2020-10-31 23:00:00 EDT" "2020-11-01 01:00:00 EDT"
#> [3] "2020-11-01 01:00:00 EST" "2020-11-01 01:00:00 EST"
#> [5] "2020-11-01 03:00:00 EST" "2020-11-01 03:00:00 EST"
```

You end up with a grouping that contains only a single hour of
information (1 EDT), but this is probably the best that we can do since
it maintains the rest of the expected grouping structure before and
after the fallback.

### Production

This new invalid date and daylight saving time behavior might sound
great to you, but you might be wondering about usage of clock in
production. What happens if
[`add_months()`](https://clock.r-lib.org/reference/clock-arithmetic.md)
worked in interactive development, but then you put your analysis into
production, gathered new data, and all of the sudden it started failing?

``` r
x <- as.Date(c("2020-03-30", "2020-03-31"))
x
#> [1] "2020-03-30" "2020-03-31"

# All good! Ship it!
add_months(x[1], 1) 
#> [1] "2020-04-30"

# Got new production data. Oh no!
add_months(x[1:2], 1)
#> Error in `invalid_resolve()`:
#> ! Invalid date found at location 2.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.
```

To balance the usefulness of clock in interactive development with the
strict requirements of production, you can set the `clock.strict` global
option to `TRUE` to turn `invalid`, `nonexistent`, and `ambiguous` from
optional arguments into required ones.

``` r
with_options(clock.strict = TRUE, .expr = {
  add_months(x[1], 1)
})
#> Error in `strict_validate_invalid()`:
#> ! The global option, `clock.strict`, is currently set to `TRUE`. In this mode, `invalid` must be set and cannot be left as `NULL`.
```

Forcing yourself to specify these arguments up front during interactive
development is a great way to explicitly document your assumptions about
these possible issues, while also guarding against future problems in
production.

### Naive-time

The previous section was entirely focused on issues that might arise
when working with POSIXct and time zones. If you don’t require the
complexity of time zones, you can use a naive-time. Naive-times are a
type of time point that have a yet-to-be-specified time zone. The other
type of time point in clock is a sys-time, which is interpreted as
having a UTC time zone attached to it. If you aren’t using time zones at
all, naive-times and sys-times are equivalent, and even use the same
date algorithms under the hood.

Unlike our ambiguous time examples from before, since naive-times don’t
have any implied time zone, there is no repeated 1 o’clock hour.

``` r
x <- naive_time_parse("2020-11-01T00:30:00")
x <- add_hours(x, 0:5)
x
#> <naive_time<second>[6]>
#> [1] "2020-11-01T00:30:00" "2020-11-01T01:30:00" "2020-11-01T02:30:00"
#> [4] "2020-11-01T03:30:00" "2020-11-01T04:30:00" "2020-11-01T05:30:00"
```

This makes arithmetic and flooring much simpler to reason about:

``` r
time_point_floor(x, "hour")
#> <naive_time<hour>[6]>
#> [1] "2020-11-01T00" "2020-11-01T01" "2020-11-01T02" "2020-11-01T03"
#> [5] "2020-11-01T04" "2020-11-01T05"

origin <- time_point_cast(x[2], "hour")
time_point_floor(x, "hour", n = 2, origin = origin)
#> <naive_time<hour>[6]>
#> [1] "2020-10-31T23" "2020-11-01T01" "2020-11-01T01" "2020-11-01T03"
#> [5] "2020-11-01T03" "2020-11-01T05"
```

You’ll notice that the result of flooring by hour is a time point with
*hour* precision. Flooring drops all information at more precise
precisions, and the result is represented in a more compact form.

The difference between naive-time and sys-time is only important when
you are working with time zones and zoned-times / POSIXct. Since
naive-time has a yet-to-be-specified time zone, converting to a
zoned-time simply specifies that zone. If possible, it keeps the printed
time the same, while changing the underlying duration.

``` r
x[1]
#> <naive_time<second>[1]>
#> [1] "2020-11-01T00:30:00"

as_zoned_time(x[1], "America/New_York")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2020-11-01T00:30:00-04:00"
```

Sys-time is interpreted as being UTC. Converting it to a zoned-time will
keep the underlying duration, but will change the printed time.

``` r
x_sys <- as_sys_time(x[1])
x_sys
#> <sys_time<second>[1]>
#> [1] "2020-11-01T00:30:00"

# Subtract 4 hours to print the America/New_York time
as_zoned_time(x_sys, "America/New_York")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2020-10-31T20:30:00-04:00"
```

Converting from sys-time to zoned-time won’t ever have any issues with
nonexistent or ambiguous times, but converting from naive-time might.
Remember that there were two 1 o’clock hours on this day in the
America/New_York time zone.

``` r
x[1:2]
#> <naive_time<second>[2]>
#> [1] "2020-11-01T00:30:00" "2020-11-01T01:30:00"

as_zoned_time(x[1:2], "America/New_York")
#> Error in `as_zoned_time()`:
#> ! Ambiguous time due to daylight saving time at location 2.
#> ℹ Resolve ambiguous time issues by specifying the `ambiguous` argument.

as_zoned_time(x[1:2], "America/New_York", ambiguous = "earliest")
#> <zoned_time<second><America/New_York>[2]>
#> [1] "2020-11-01T00:30:00-04:00" "2020-11-01T01:30:00-04:00"
```

In the low-level API, converting from naive-time to zoned-time is
actually the *only* place where the `nonexistent` and `ambiguous`
arguments are used. This limits how often you have to think about
daylight saving time, ideally helping you avoid as many subtle bugs as
possible. In the high-level API for POSIXct, these arguments are forced
to bubble up in more places. This trades the user benefit of being able
to work directly on R’s native date-time types for the side-effect of
potentially having to deal with these issues in more places.

### Calendar types

clock provides multiple new *calendar* types, which allow you to
represent calendar “dates” in various different ways.

The year-month-day calendar is the most common, it represents the
Gregorian calendar through a set of *components* like year, month, and
day.

``` r
year_month_day(2019, 1, 1:5)
#> <year_month_day<day>[5]>
#> [1] "2019-01-01" "2019-01-02" "2019-01-03" "2019-01-04" "2019-01-05"

year_month_day(2020, 1:12, "last")
#> <year_month_day<day>[12]>
#>  [1] "2020-01-31" "2020-02-29" "2020-03-31" "2020-04-30" "2020-05-31"
#>  [6] "2020-06-30" "2020-07-31" "2020-08-31" "2020-09-30" "2020-10-31"
#> [11] "2020-11-30" "2020-12-31"
```

There are also calendars for:

- year-month-weekday: An alternative way to specify a Gregorian date as
  year, month, and indexed day of the week (like the 3rd Sunday in
  March). Great for specifying holidays.

- year-day: An alternative way to specify a Gregorian date as year and
  day of the year. Great for extracting the day of the year, or grouping
  by the day of the year.

- year-quarter-day: Great for quarterly financial data, or if you have a
  fiscal year that starts in a different month from January.

- year-week-day: Allows you to specify a date using the year, week
  number, day of the week, and a `start` value representing the day that
  represents the start of the week.

- iso-year-week-day: Follows the ISO week standard. Great if you need to
  represent ISO weeks like `2020-W02`, or if you need to extract out the
  ISO year which might differ from the Gregorian year. Identical to
  year-week-day with `start` set to Monday.

Calendar types are great for instant retrieval of their underlying
components (year, month, day, etc), and for irregular calendrical
arithmetic with units such as years or months.

``` r
year_month_day(2019, 1, 2) + duration_months(1:3)
#> <year_month_day<day>[3]>
#> [1] "2019-02-02" "2019-03-02" "2019-04-02"
```

The structure of calendar types also enables a unique feature among
date-time libraries, directly representing invalid dates. While adding 1
month to an R Date of 2019-03-31 requires setting an invalid date
resolution strategy, the year-month-day type can actually handle this
directly.

``` r
ymd <- year_month_day(2019, 3, c(30, 31)) + duration_months(1)
ymd
#> <year_month_day<day>[2]>
#> [1] "2019-04-30" "2019-04-31"
```

There are a family of functions for working with invalid dates:
[`invalid_count()`](https://clock.r-lib.org/reference/clock-invalid.md),
[`invalid_any()`](https://clock.r-lib.org/reference/clock-invalid.md),
[`invalid_detect()`](https://clock.r-lib.org/reference/clock-invalid.md),
and
[`invalid_resolve()`](https://clock.r-lib.org/reference/clock-invalid.md).

``` r
invalid_count(ymd)
#> [1] 1

invalid_detect(ymd)
#> [1] FALSE  TRUE

invalid_resolve(ymd, invalid = "previous")
#> <year_month_day<day>[2]>
#> [1] "2019-04-30" "2019-04-30"
invalid_resolve(ymd, invalid = "next")
#> <year_month_day<day>[2]>
#> [1] "2019-04-30" "2019-05-01"
```

The *only* time in the entire low-level API that invalid dates must be
resolved is before converting to another type:

``` r
as.Date(ymd)
#> Error in `as_sys_time()`:
#> ! Can't convert `x` to another type because some dates are
#>   invalid.
#> ℹ The following locations are invalid: 2.
#> ℹ Resolve invalid dates with `invalid_resolve()`.
as_naive_time(ymd)
#> Error in `as_sys_time()`:
#> ! Can't convert `x` to another type because some dates are
#>   invalid.
#> ℹ The following locations are invalid: 2.
#> ℹ Resolve invalid dates with `invalid_resolve()`.
as_year_month_weekday(ymd)
#> Error in `as_sys_time()`:
#> ! Can't convert `x` to another type because some dates are
#>   invalid.
#> ℹ The following locations are invalid: 2.
#> ℹ Resolve invalid dates with `invalid_resolve()`.
```

In the high-level API for R’s native date and date-time types, the
`invalid` argument is forced to bubble up in more places. This trades
the user benefit of being able to work directly on R’s native date-time
types for the side-effect of potentially having to deal with this issue
in more places.

For another example of invalid dates, imagine that your office is
planning on having a group meeting every 1st, 3rd, and 5th Friday of
every month in 2019. What days are those?

``` r
grid <- expand.grid(
  index = c(1L, 3L, 5L),
  day = clock_weekdays$friday,
  month = 1:12,
  year = 2019
)

meetings <- year_month_weekday(grid$year, grid$month, grid$day, grid$index)

meetings
#> <year_month_weekday<day>[36]>
#>  [1] "2019-01-Fri[1]" "2019-01-Fri[3]" "2019-01-Fri[5]"
#>  [4] "2019-02-Fri[1]" "2019-02-Fri[3]" "2019-02-Fri[5]"
#>  [7] "2019-03-Fri[1]" "2019-03-Fri[3]" "2019-03-Fri[5]"
#> [10] "2019-04-Fri[1]" "2019-04-Fri[3]" "2019-04-Fri[5]"
#> [13] "2019-05-Fri[1]" "2019-05-Fri[3]" "2019-05-Fri[5]"
#> [16] "2019-06-Fri[1]" "2019-06-Fri[3]" "2019-06-Fri[5]"
#> [19] "2019-07-Fri[1]" "2019-07-Fri[3]" "2019-07-Fri[5]"
#> [22] "2019-08-Fri[1]" "2019-08-Fri[3]" "2019-08-Fri[5]"
#> [25] "2019-09-Fri[1]" "2019-09-Fri[3]" "2019-09-Fri[5]"
#> [28] "2019-10-Fri[1]" "2019-10-Fri[3]" "2019-10-Fri[5]"
#> [31] "2019-11-Fri[1]" "2019-11-Fri[3]" "2019-11-Fri[5]"
#> [34] "2019-12-Fri[1]" "2019-12-Fri[3]" "2019-12-Fri[5]"

meetings[invalid_detect(meetings)]
#> <year_month_weekday<day>[8]>
#> [1] "2019-01-Fri[5]" "2019-02-Fri[5]" "2019-04-Fri[5]" "2019-06-Fri[5]"
#> [5] "2019-07-Fri[5]" "2019-09-Fri[5]" "2019-10-Fri[5]" "2019-12-Fri[5]"
```

The year-month-weekday calendar allows us to quickly generate these
dates. We can see that we’ve generated 8 invalid dates - some months
don’t have a 5th Friday.

One option for handling these is to simply remove them. Less group
meetings!

``` r
meetings[!invalid_detect(meetings)]
#> <year_month_weekday<day>[28]>
#>  [1] "2019-01-Fri[1]" "2019-01-Fri[3]" "2019-02-Fri[1]"
#>  [4] "2019-02-Fri[3]" "2019-03-Fri[1]" "2019-03-Fri[3]"
#>  [7] "2019-03-Fri[5]" "2019-04-Fri[1]" "2019-04-Fri[3]"
#> [10] "2019-05-Fri[1]" "2019-05-Fri[3]" "2019-05-Fri[5]"
#> [13] "2019-06-Fri[1]" "2019-06-Fri[3]" "2019-07-Fri[1]"
#> [16] "2019-07-Fri[3]" "2019-08-Fri[1]" "2019-08-Fri[3]"
#> [19] "2019-08-Fri[5]" "2019-09-Fri[1]" "2019-09-Fri[3]"
#> [22] "2019-10-Fri[1]" "2019-10-Fri[3]" "2019-11-Fri[1]"
#> [25] "2019-11-Fri[3]" "2019-11-Fri[5]" "2019-12-Fri[1]"
#> [28] "2019-12-Fri[3]"
```

Or, you could resolve them by selecting the previous valid day:

``` r
meetings_valid <- invalid_resolve(meetings, invalid = "previous")

meetings_valid
#> <year_month_weekday<day>[36]>
#>  [1] "2019-01-Fri[1]" "2019-01-Fri[3]" "2019-01-Thu[5]"
#>  [4] "2019-02-Fri[1]" "2019-02-Fri[3]" "2019-02-Thu[4]"
#>  [7] "2019-03-Fri[1]" "2019-03-Fri[3]" "2019-03-Fri[5]"
#> [10] "2019-04-Fri[1]" "2019-04-Fri[3]" "2019-04-Tue[5]"
#> [13] "2019-05-Fri[1]" "2019-05-Fri[3]" "2019-05-Fri[5]"
#> [16] "2019-06-Fri[1]" "2019-06-Fri[3]" "2019-06-Sun[5]"
#> [19] "2019-07-Fri[1]" "2019-07-Fri[3]" "2019-07-Wed[5]"
#> [22] "2019-08-Fri[1]" "2019-08-Fri[3]" "2019-08-Fri[5]"
#> [25] "2019-09-Fri[1]" "2019-09-Fri[3]" "2019-09-Mon[5]"
#> [28] "2019-10-Fri[1]" "2019-10-Fri[3]" "2019-10-Thu[5]"
#> [31] "2019-11-Fri[1]" "2019-11-Fri[3]" "2019-11-Fri[5]"
#> [34] "2019-12-Fri[1]" "2019-12-Fri[3]" "2019-12-Tue[5]"
```

If you didn’t want to meet on Saturday or Sunday, you could convert to a
naive-time and *shift* any Saturdays or Sundays to the previous Friday.

``` r
meetings_naive <- as_naive_time(meetings_valid) 
meetings_wday <- as_weekday(meetings_naive)

meetings_wday
#> <weekday[36]>
#>  [1] Fri Fri Thu Fri Fri Thu Fri Fri Fri Fri Fri Tue Fri Fri Fri Fri
#> [17] Fri Sun Fri Fri Wed Fri Fri Fri Fri Fri Mon Fri Fri Thu Fri Fri
#> [33] Fri Fri Fri Tue

friday <- weekday(clock_weekdays$friday)
saturday <- weekday(clock_weekdays$saturday)
sunday <- weekday(clock_weekdays$sunday)

on_weekend <- meetings_wday == saturday | meetings_wday == sunday

meetings_naive[on_weekend] <- time_point_shift(
  meetings_naive[on_weekend], 
  target = friday,
  which = "previous"
)

as_year_month_weekday(meetings_naive)
#> <year_month_weekday<day>[36]>
#>  [1] "2019-01-Fri[1]" "2019-01-Fri[3]" "2019-01-Thu[5]"
#>  [4] "2019-02-Fri[1]" "2019-02-Fri[3]" "2019-02-Thu[4]"
#>  [7] "2019-03-Fri[1]" "2019-03-Fri[3]" "2019-03-Fri[5]"
#> [10] "2019-04-Fri[1]" "2019-04-Fri[3]" "2019-04-Tue[5]"
#> [13] "2019-05-Fri[1]" "2019-05-Fri[3]" "2019-05-Fri[5]"
#> [16] "2019-06-Fri[1]" "2019-06-Fri[3]" "2019-06-Fri[4]"
#> [19] "2019-07-Fri[1]" "2019-07-Fri[3]" "2019-07-Wed[5]"
#> [22] "2019-08-Fri[1]" "2019-08-Fri[3]" "2019-08-Fri[5]"
#> [25] "2019-09-Fri[1]" "2019-09-Fri[3]" "2019-09-Mon[5]"
#> [28] "2019-10-Fri[1]" "2019-10-Fri[3]" "2019-10-Thu[5]"
#> [31] "2019-11-Fri[1]" "2019-11-Fri[3]" "2019-11-Fri[5]"
#> [34] "2019-12-Fri[1]" "2019-12-Fri[3]" "2019-12-Tue[5]"
```

### Variable precision

#### Calendars

clock’s new types all have variable precision. Calendars, for example,
range in precision from year to nanosecond.

``` r
year_month_day(2019)
#> <year_month_day<year>[1]>
#> [1] "2019"

year_month_day(2019, 01, 01, 02, 30, 50, 123, subsecond_precision = "nanosecond")
#> <year_month_day<nanosecond>[1]>
#> [1] "2019-01-01T02:30:50.000000123"
```

A nice thing about this is that you get year-month and year-quarter
types for free.

``` r
year_month_day(2019, 1:5)
#> <year_month_day<month>[5]>
#> [1] "2019-01" "2019-02" "2019-03" "2019-04" "2019-05"

year_quarter_day(2020, 1:4)
#> <year_quarter_day<January><quarter>[4]>
#> [1] "2020-Q1" "2020-Q2" "2020-Q3" "2020-Q4"
```

clock takes the worldview that a calendar at a specific precision
represents a *range* of values. Rather than assuming that a month
precision year-month-day would convert directly to a day precision
naive-time at the first of the month, you have to be explicit about
exactly what day you are talking about by first promoting that month
precision calendar up to day precision.

``` r
ym <- year_month_day(2019, 1:3)

as_naive_time(ym)
#> Error:
#> ! Can't convert to a time point from a calendar with 'month' precision. A minimum of 'day' precision is required.

# You might want the first of the month
ym %>%
  set_day(1) %>%
  as_naive_time()
#> <naive_time<day>[3]>
#> [1] "2019-01-01" "2019-02-01" "2019-03-01"

# Or the last
ym %>%
  set_day("last") %>%
  as_naive_time()
#> <naive_time<day>[3]>
#> [1] "2019-01-31" "2019-02-28" "2019-03-31"
```

This range assumption is all throughout clock. For example, grouping a
day precision iso-year-week-day by weeks returns a week precision
calendar.

``` r
x <- iso_year_week_day(2019, 1:10, 2)
calendar_group(x, "week", n = 2)
#> <iso_year_week_day<week>[10]>
#>  [1] "2019-W01" "2019-W01" "2019-W03" "2019-W03" "2019-W05" "2019-W05"
#>  [7] "2019-W07" "2019-W07" "2019-W09" "2019-W09"
```

#### Time points

Time points, such as naive-time and sys-time, can vary in precision from
day to nanosecond.

``` r
as_naive_time(year_month_day(2019, 1, 2))
#> <naive_time<day>[1]>
#> [1] "2019-01-02"

as_sys_time(year_month_day(
  2019, 2, 3, 1, 30, 35, 123, 
  subsecond_precision = "millisecond"
))
#> <sys_time<millisecond>[1]>
#> [1] "2019-02-03T01:30:35.123"
```

Unlike calendars, a time point at say, second precision, is considered
equivalent to a time point at millisecond precision with millisecond
information set to 0.

``` r
sec <- as_sys_time(year_month_day(
  2019, 2, 3, 1, 30, 35
))

milli <- as_sys_time(year_month_day(
  2019, 2, 3, 1, 30, 35, 0, 
  subsecond_precision = "millisecond"
))

sec == milli
#> [1] TRUE
```

This allows time points to naturally promote upwards to a more precise
precision when doing arithmetic with them:

``` r
as_naive_time(year_month_day(2019, 1, 2)) + duration_nanoseconds(25)
#> <naive_time<nanosecond>[1]>
#> [1] "2019-01-02T00:00:00.000000025"
```
