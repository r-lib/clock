# Parsing: sys-time

There are two parsers into a sys-time, `sys_time_parse()` and
`sys_time_parse_RFC_3339()`.

### sys_time_parse()

`sys_time_parse()` is useful when you have date-time strings like
`"2020-01-01T01:04:30"` that you know should be interpreted as UTC, or
like `"2020-01-01T01:04:30-04:00"` with a UTC offset but no zone name.
If you find yourself in the latter situation, then parsing this string
as a sys-time using the `%Ez` command to capture the offset is probably
your best option. If you know that this string should be interpreted in
a specific time zone, parse as a sys-time to get the UTC equivalent,
then use
[`as_zoned_time()`](https://clock.r-lib.org/dev/reference/as_zoned_time.md).

The default options assume that `x` should be parsed at second
precision, using a `format` string of `"%Y-%m-%dT%H:%M:%S"`. This
matches the default result from calling
[`format()`](https://rdrr.io/r/base/format.html) on a sys-time.

`sys_time_parse()` is nearly equivalent to
[`naive_time_parse()`](https://clock.r-lib.org/dev/reference/naive_time_parse.md),
except for the fact that the `%z` command is actually used. Using `%z`
assumes that the rest of the date-time string should be interpreted as a
naive-time, which is then shifted by the UTC offset found in `%z`. The
returned time can then be validly interpreted as UTC.

*`sys_time_parse()` ignores the `%Z` command.*

### sys_time_parse_RFC_3339()

`sys_time_parse_RFC_3339()` is a wrapper around `sys_time_parse()` that
is intended to parse the extremely common date-time format outlined by
[RFC 3339](https://datatracker.ietf.org/doc/html/rfc3339). This document
outlines a profile of the ISO 8601 format that is even more restrictive.

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

For this function, the `precision` must be at least `"second"`.

## Usage

``` r
sys_time_parse(
  x,
  ...,
  format = NULL,
  precision = "second",
  locale = clock_locale()
)

sys_time_parse_RFC_3339(
  x,
  ...,
  separator = "T",
  offset = "Z",
  precision = "second"
)
```

## Arguments

- x:

  `[character]`

  A character vector to parse.

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

- precision:

  `[character(1)]`

  A precision for the resulting time point. One of:

  - `"day"`

  - `"hour"`

  - `"minute"`

  - `"second"`

  - `"millisecond"`

  - `"microsecond"`

  - `"nanosecond"`

  Setting the `precision` determines how much information `%S` attempts
  to parse.

- locale:

  `[clock_locale]`

  A locale object created from
  [`clock_locale()`](https://clock.r-lib.org/dev/reference/clock_locale.md).

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

A sys-time.

## Details

If your date-time strings contain a full time zone name and a UTC
offset, use
[`zoned_time_parse_complete()`](https://clock.r-lib.org/dev/reference/zoned-parsing.md).
If they contain a time zone abbreviation, use
[`zoned_time_parse_abbrev()`](https://clock.r-lib.org/dev/reference/zoned-parsing.md).

If your date-time strings don't contain an offset from UTC and you
aren't sure if they should be treated as UTC or not, you might consider
using
[`naive_time_parse()`](https://clock.r-lib.org/dev/reference/naive_time_parse.md),
since the resulting naive-time doesn't come with an assumption of a UTC
time zone.

## Full Precision Parsing

It is highly recommended to parse all of the information in the
date-time string into a type at least as precise as the string. For
example, if your string has fractional seconds, but you only require
seconds, specify a sub-second `precision`, then round to seconds
manually using whatever convention is appropriate for your use case.
Parsing such a string directly into a second precision result is
ambiguous and undefined, and is unlikely to work as you might expect.

## Examples

``` r
sys_time_parse("2020-01-01T05:06:07")
#> <sys_time<second>[1]>
#> [1] "2020-01-01T05:06:07"

# Day precision
sys_time_parse("2020-01-01", precision = "day")
#> <sys_time<day>[1]>
#> [1] "2020-01-01"

# Nanosecond precision, but using a day based format
sys_time_parse("2020-01-01", format = "%Y-%m-%d", precision = "nanosecond")
#> <sys_time<nanosecond>[1]>
#> [1] "2020-01-01T00:00:00.000000000"

# Multiple format strings are allowed for heterogeneous times
sys_time_parse(
  c("2019-01-01", "2019/1/1"),
  format = c("%Y/%m/%d", "%Y-%m-%d"),
  precision = "day"
)
#> <sys_time<day>[2]>
#> [1] "2019-01-01" "2019-01-01"

# The `%z` command shifts the date-time by subtracting the UTC offset so
# that the returned sys-time can be interpreted as UTC
sys_time_parse(
  "2020-01-01 02:00:00 -0400",
  format = "%Y-%m-%d %H:%M:%S %z"
)
#> <sys_time<second>[1]>
#> [1] "2020-01-01T06:00:00"

# Remember that the `%Z` command is ignored entirely!
sys_time_parse("2020-01-01 America/New_York", format = "%Y-%m-%d %Z")
#> <sys_time<second>[1]>
#> [1] "2020-01-01T00:00:00"

# ---------------------------------------------------------------------------
# RFC 3339

# Typical UTC format
x <- "2019-01-01T00:01:02Z"
sys_time_parse_RFC_3339(x)
#> <sys_time<second>[1]>
#> [1] "2019-01-01T00:01:02"

# With a UTC offset containing a `:`
x <- "2019-01-01T00:01:02+02:30"
sys_time_parse_RFC_3339(x, offset = "%Ez")
#> <sys_time<second>[1]>
#> [1] "2018-12-31T21:31:02"

# With a space between the date and time and no `:` in the offset
x <- "2019-01-01 00:01:02+0230"
sys_time_parse_RFC_3339(x, separator = " ", offset = "%z")
#> <sys_time<second>[1]>
#> [1] "2018-12-31T21:31:02"
```
