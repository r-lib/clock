# Parsing: date-time

There are four parsers for parsing strings into POSIXct date-times,
`date_time_parse()`, `date_time_parse_complete()`,
`date_time_parse_abbrev()`, and `date_time_parse_RFC_3339()`.

### date_time_parse()

`date_time_parse()` is useful for strings like `"2019-01-01 00:00:00"`,
where the UTC offset and full time zone name are not present in the
string. The string is first parsed as a naive-time without any time zone
assumptions, and is then converted to a POSIXct with the supplied
`zone`.

Because converting from naive-time to POSIXct may result in nonexistent
or ambiguous times due to daylight saving time, these must be resolved
explicitly with the `nonexistent` and `ambiguous` arguments.

`date_time_parse()` completely ignores the `%z` and `%Z` commands. The
only time zone specific information that is used is the `zone`.

The default `format` used is `"%Y-%m-%d %H:%M:%S"`. This matches the
default result from calling
[`format()`](https://rdrr.io/r/base/format.html) on a POSIXct date-time.

### date_time_parse_complete()

`date_time_parse_complete()` is a parser for *complete* date-time
strings, like `"2019-01-01T00:00:00-05:00[America/New_York]"`. A
complete date-time string has both the time zone offset and full time
zone name in the string, which is the only way for the string itself to
contain all of the information required to unambiguously construct a
zoned-time. Because of this, `date_time_parse_complete()` requires both
the `%z` and `%Z` commands to be supplied in the `format` string.

The default `format` used is `"%Y-%m-%dT%H:%M:%S%Ez[%Z]"`. This matches
the default result from calling
[`date_format()`](https://clock.r-lib.org/reference/date_format.md) on a
POSIXct date-time. Additionally, this format matches the de-facto
standard extension to RFC 3339 for creating completely unambiguous
date-times.

### date_time_parse_abbrev()

`date_time_parse_abbrev()` is a parser for date-time strings containing
only a time zone abbreviation, like `"2019-01-01 00:00:00 EST"`. The
time zone abbreviation is not enough to identify the full time zone name
that the date-time belongs to, so the full time zone name must be
supplied as the `zone` argument. However, the time zone abbreviation can
help with resolving ambiguity around daylight saving time fallbacks.

For `date_time_parse_abbrev()`, `%Z` must be supplied and is interpreted
as the time zone abbreviation rather than the full time zone name.

If used, the `%z` command must parse correctly, but its value will be
completely ignored.

The default `format` used is `"%Y-%m-%d %H:%M:%S %Z"`. This matches the
default result from calling
[`print()`](https://rdrr.io/r/base/print.html) or `format(usetz = TRUE)`
on a POSIXct date-time.

### date_time_parse_RFC_3339()

`date_time_parse_RFC_3339()` is a parser for date-time strings in the
extremely common date-time format outlined by [RFC
3339](https://datatracker.ietf.org/doc/html/rfc3339). This document
outlines a profile of the ISO 8601 format that is even more restrictive,
but corresponds to the most common formats that are likely to be used in
internet protocols (i.e. through APIs).

In particular, this function is intended to parse the following three
formats:

    2019-01-01T00:00:00Z
    2019-01-01T00:00:00+0430
    2019-01-01T00:00:00+04:30

This function defaults to parsing the first of these formats by using a
format string of `"%Y-%m-%dT%H:%M:%SZ"`.

If your date-time strings use offsets from UTC rather than `"Z"`, then
set `offset` to one of the following:

- `"%z"` if the offset is of the form `"+0430"`.

- `"%Ez"` if the offset is of the form `"+04:30"`.

The RFC 3339 standard allows for replacing the `"T"` with a `"t"` or a
space (`" "`). Set `separator` to adjust this as needed.

The date-times returned by this function will always be in the UTC time
zone.

## Usage

``` r
date_time_parse(
  x,
  zone,
  ...,
  format = NULL,
  locale = clock_locale(),
  nonexistent = NULL,
  ambiguous = NULL
)

date_time_parse_complete(x, ..., format = NULL, locale = clock_locale())

date_time_parse_abbrev(x, zone, ..., format = NULL, locale = clock_locale())

date_time_parse_RFC_3339(x, ..., separator = "T", offset = "Z")
```

## Arguments

- x:

  `[character]`

  A character vector to parse.

- zone:

  `[character(1)]`

  A full time zone name.

- ...:

  These dots are for future extensions and must be empty.

- format:

  `[character / NULL]`

  A format string. A combination of the following commands, or `NULL`,
  in which case a default format string is used.

  A vector of multiple format strings can be supplied. They will be
  tried in the order they are provided.

  **Year**

  - `%C`: The century as a decimal number. The modified command `%NC`
    where `N` is a positive decimal integer specifies the maximum number
    of characters to read. If not specified, the default is `2`. Leading
    zeroes are permitted but not required.

  - `%y`: The last two decimal digits of the year. If the century is not
    otherwise specified (e.g. with `%C`), values in the range
    `[69 - 99]` are presumed to refer to the years `[1969 - 1999]`, and
    values in the range `[00 - 68]` are presumed to refer to the years
    `[2000 - 2068]`. The modified command `%Ny`, where `N` is a positive
    decimal integer, specifies the maximum number of characters to read.
    If not specified, the default is `2`. Leading zeroes are permitted
    but not required.

  - `%Y`: The year as a decimal number. The modified command `%NY` where
    `N` is a positive decimal integer specifies the maximum number of
    characters to read. If not specified, the default is `4`. Leading
    zeroes are permitted but not required.

  **Month**

  - `%b`, `%B`, `%h`: The `locale`'s full or abbreviated
    case-insensitive month name.

  - `%m`: The month as a decimal number. January is `1`. The modified
    command `%Nm` where `N` is a positive decimal integer specifies the
    maximum number of characters to read. If not specified, the default
    is `2`. Leading zeroes are permitted but not required.

  **Day**

  - `%d`, `%e`: The day of the month as a decimal number. The modified
    command `%Nd` where `N` is a positive decimal integer specifies the
    maximum number of characters to read. If not specified, the default
    is `2`. Leading zeroes are permitted but not required.

  **Day of the week**

  - `%a`, `%A`: The `locale`'s full or abbreviated case-insensitive
    weekday name.

  - `%w`: The weekday as a decimal number (`0-6`), where Sunday is `0`.
    The modified command `%Nw` where `N` is a positive decimal integer
    specifies the maximum number of characters to read. If not
    specified, the default is `1`. Leading zeroes are permitted but not
    required.

  **ISO 8601 week-based year**

  - `%g`: The last two decimal digits of the ISO week-based year. The
    modified command `%Ng` where `N` is a positive decimal integer
    specifies the maximum number of characters to read. If not
    specified, the default is `2`. Leading zeroes are permitted but not
    required.

  - `%G`: The ISO week-based year as a decimal number. The modified
    command `%NG` where `N` is a positive decimal integer specifies the
    maximum number of characters to read. If not specified, the default
    is `4`. Leading zeroes are permitted but not required.

  - `%V`: The ISO week-based week number as a decimal number. The
    modified command `%NV` where `N` is a positive decimal integer
    specifies the maximum number of characters to read. If not
    specified, the default is `2`. Leading zeroes are permitted but not
    required.

  - `%u`: The ISO weekday as a decimal number (`1-7`), where Monday is
    `1`. The modified command `%Nu` where `N` is a positive decimal
    integer specifies the maximum number of characters to read. If not
    specified, the default is `1`. Leading zeroes are permitted but not
    required.

  **Week of the year**

  - `%U`: The week number of the year as a decimal number. The first
    Sunday of the year is the first day of week `01`. Days of the same
    year prior to that are in week `00`. The modified command `%NU`
    where `N` is a positive decimal integer specifies the maximum number
    of characters to read. If not specified, the default is `2`. Leading
    zeroes are permitted but not required.

  - `%W`: The week number of the year as a decimal number. The first
    Monday of the year is the first day of week `01`. Days of the same
    year prior to that are in week `00`. The modified command `%NW`
    where `N` is a positive decimal integer specifies the maximum number
    of characters to read. If not specified, the default is `2`. Leading
    zeroes are permitted but not required.

  **Day of the year**

  - `%j`: The day of the year as a decimal number. January 1 is `1`. The
    modified command `%Nj` where `N` is a positive decimal integer
    specifies the maximum number of characters to read. If not
    specified, the default is `3`. Leading zeroes are permitted but not
    required.

  **Date**

  - `%D`, `%x`: Equivalent to `%m/%d/%y`.

  - `%F`: Equivalent to `%Y-%m-%d`. If modified with a width (like
    `%NF`), the width is applied to only `%Y`.

  **Time of day**

  - `%H`: The hour (24-hour clock) as a decimal number. The modified
    command `%NH` where `N` is a positive decimal integer specifies the
    maximum number of characters to read. If not specified, the default
    is `2`. Leading zeroes are permitted but not required.

  - `%I`: The hour (12-hour clock) as a decimal number. The modified
    command `%NI` where `N` is a positive decimal integer specifies the
    maximum number of characters to read. If not specified, the default
    is `2`. Leading zeroes are permitted but not required.

  - `%M`: The minutes as a decimal number. The modified command `%NM`
    where `N` is a positive decimal integer specifies the maximum number
    of characters to read. If not specified, the default is `2`. Leading
    zeroes are permitted but not required.

  - `%S`: The seconds as a decimal number. Leading zeroes are permitted
    but not required. If encountered, the `locale` determines the
    decimal point character. Generally, the maximum number of characters
    to read is determined by the precision that you are parsing at. For
    example, a precision of `"second"` would read a maximum of 2
    characters, while a precision of `"millisecond"` would read a
    maximum of 6 (2 for the values before the decimal point, 1 for the
    decimal point, and 3 for the values after it). The modified command
    `%NS`, where `N` is a positive decimal integer, can be used to
    exactly specify the maximum number of characters to read. This is
    only useful if you happen to have seconds with more than 1 leading
    zero.

  - `%p`: The `locale`'s equivalent of the AM/PM designations associated
    with a 12-hour clock. The command `%I` must precede `%p` in the
    format string.

  - `%R`: Equivalent to `%H:%M`.

  - `%T`, `%X`: Equivalent to `%H:%M:%S`.

  - `%r`: Equivalent to `%I:%M:%S %p`.

  **Time zone**

  - `%z`: The offset from UTC in the format `[+|-]hh[mm]`. For example
    `-0430` refers to 4 hours 30 minutes behind UTC. And `04` refers to
    4 hours ahead of UTC. The modified command `%Ez` parses a `:`
    between the hours and minutes and leading zeroes on the hour field
    are optional: `[+|-]h[h][:mm]`. For example `-04:30` refers to 4
    hours 30 minutes behind UTC. And `4` refers to 4 hours ahead of UTC.

  - `%Z`: The full time zone name or the time zone abbreviation,
    depending on the function being used. A single word is parsed. This
    word can only contain characters that are alphanumeric, or one of
    `'_'`, `'/'`, `'-'` or `'+'`.

  **Miscellaneous**

  - `%c`: A date and time representation. Equivalent to
    `%a %b %d %H:%M:%S %Y`.

  - `%%`: A `%` character.

  - `%n`: Matches one white space character. `%n`, `%t`, and a space can
    be combined to match a wide range of white-space patterns. For
    example `"%n "` matches one or more white space characters, and
    `"%n%t%t"` matches one to three white space characters.

  - `%t`: Matches zero or one white space characters.

- locale:

  `[clock_locale]`

  A locale object created from
  [`clock_locale()`](https://clock.r-lib.org/reference/clock_locale.md).

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

- separator:

  `[character(1)]`

  The separator between the date and time components of the string. One
  of:

  - `"T"`

  - `"t"`

  - `" "`

- offset:

  `[character(1)]`

  The format of the offset from UTC contained in the string. One of:

  - `"Z"`

  - `"z"`

  - `"%z"` to parse a numeric offset of the form `"+0430"`

  - `"%Ez"` to parse a numeric offset of the form `"+04:30"`

## Value

A POSIXct.

## Details

If `date_time_parse_complete()` is given input that is length zero, all
`NA`s, or completely fails to parse, then no time zone will be able to
be determined. In that case, the result will use `"UTC"`.

If you have strings with sub-second components, then these date-time
parsers are not appropriate for you. Remember that clock treats POSIXct
as a second precision type, so parsing a string with fractional seconds
directly into a POSIXct is ambiguous and undefined. Instead, fully parse
the string, including its fractional seconds, into a clock type that can
handle it, such as a naive-time with
[`naive_time_parse()`](https://clock.r-lib.org/reference/naive_time_parse.md),
then round to seconds with whatever rounding convention is appropriate
for your use case, such as
[`time_point_floor()`](https://clock.r-lib.org/reference/time-point-rounding.md),
and finally convert that to POSIXct with
[`as_date_time()`](https://clock.r-lib.org/reference/as_date_time.md).
This gives you complete control over how the fractional seconds are
handled when converting to POSIXct.

## Examples

``` r
# Parse with a known `zone`, even though that information isn't in the string
date_time_parse("2020-01-01 05:06:07", "America/New_York")
#> [1] "2020-01-01 05:06:07 EST"

# Same time as above, except this is a completely unambiguous parse that
# doesn't require a `zone` argument, because the zone name and offset are
# both present in the string
date_time_parse_complete("2020-01-01T05:06:07-05:00[America/New_York]")
#> [1] "2020-01-01 05:06:07 EST"

# Only day components
date_time_parse("2020-01-01", "America/New_York", format = "%Y-%m-%d")
#> [1] "2020-01-01 EST"

# `date_time_parse()` may have issues with ambiguous times due to daylight
# saving time fallbacks. For example, there were two 1'oclock hours here:
x <- date_time_parse("1970-10-25 00:59:59", "America/New_York")

# First (earliest) 1'oclock hour
add_seconds(x, 1)
#> [1] "1970-10-25 01:00:00 EDT"
# Second (latest) 1'oclock hour
add_seconds(x, 3601)
#> [1] "1970-10-25 01:00:00 EST"

# If you try to parse this ambiguous time directly, you'll get an error:
ambiguous_time <- "1970-10-25 01:00:00"
try(date_time_parse(ambiguous_time, "America/New_York"))
#> Error in as_zoned_time(x, zone = tz, nonexistent = nonexistent, ambiguous = ambiguous) : 
#>   Ambiguous time due to daylight saving time at location 1.
#> ℹ Resolve ambiguous time issues by specifying the `ambiguous` argument.

# Resolve it by specifying whether you'd like to use the
# `earliest` or `latest` of the two possible times
date_time_parse(ambiguous_time, "America/New_York", ambiguous = "earliest")
#> [1] "1970-10-25 01:00:00 EDT"
date_time_parse(ambiguous_time, "America/New_York", ambiguous = "latest")
#> [1] "1970-10-25 01:00:00 EST"

# `date_time_parse_complete()` doesn't have these issues, as it requires
# that the offset and zone name are both in the string, which resolves
# the ambiguity
complete_times <- c(
  "1970-10-25T01:00:00-04:00[America/New_York]",
  "1970-10-25T01:00:00-05:00[America/New_York]"
)
date_time_parse_complete(complete_times)
#> [1] "1970-10-25 01:00:00 EDT" "1970-10-25 01:00:00 EST"

# `date_time_parse_abbrev()` also doesn't have these issues, since it
# uses the time zone abbreviation name to resolve the ambiguity
abbrev_times <- c(
  "1970-10-25 01:00:00 EDT",
  "1970-10-25 01:00:00 EST"
)
date_time_parse_abbrev(abbrev_times, "America/New_York")
#> [1] "1970-10-25 01:00:00 EDT" "1970-10-25 01:00:00 EST"

# ---------------------------------------------------------------------------
# RFC 3339

# Typical UTC format
x <- "2019-01-01T00:01:02Z"
date_time_parse_RFC_3339(x)
#> [1] "2019-01-01 00:01:02 UTC"

# With a UTC offset containing a `:`
x <- "2019-01-01T00:01:02+02:30"
date_time_parse_RFC_3339(x, offset = "%Ez")
#> [1] "2018-12-31 21:31:02 UTC"

# With a space between the date and time and no `:` in the offset
x <- "2019-01-01 00:01:02+0230"
date_time_parse_RFC_3339(x, separator = " ", offset = "%z")
#> [1] "2018-12-31 21:31:02 UTC"

# ---------------------------------------------------------------------------
# Sub-second components

# If you have a string with sub-second components, but only require up to
# seconds, first parse them into a clock type that can handle sub-seconds to
# fully capture that information, then round using whatever convention is
# required for your use case before converting to a date-time.
x <- c("2019-01-01T00:00:01.1", "2019-01-01T00:00:01.78")

x <- naive_time_parse(x, precision = "millisecond")
x
#> <naive_time<millisecond>[2]>
#> [1] "2019-01-01T00:00:01.100" "2019-01-01T00:00:01.780"

time_point_floor(x, "second")
#> <naive_time<second>[2]>
#> [1] "2019-01-01T00:00:01" "2019-01-01T00:00:01"
time_point_round(x, "second")
#> <naive_time<second>[2]>
#> [1] "2019-01-01T00:00:01" "2019-01-01T00:00:02"

as_date_time(time_point_round(x, "second"), "America/New_York")
#> [1] "2019-01-01 00:00:01 EST" "2019-01-01 00:00:02 EST"
```
