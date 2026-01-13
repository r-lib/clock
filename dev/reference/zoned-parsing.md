# Parsing: zoned-time

There are two parsers into a zoned-time, `zoned_time_parse_complete()`
and `zoned_time_parse_abbrev()`.

### zoned_time_parse_complete()

`zoned_time_parse_complete()` is a parser for *complete* date-time
strings, like `"2019-01-01T00:00:00-05:00[America/New_York]"`. A
complete date-time string has both the time zone offset and full time
zone name in the string, which is the only way for the string itself to
contain all of the information required to construct a zoned-time.
Because of this, `zoned_time_parse_complete()` requires both the `%z`
and `%Z` commands to be supplied in the `format` string.

The default options assume that `x` should be parsed at second
precision, using a `format` string of `"%Y-%m-%dT%H:%M:%S%Ez[%Z]"`. This
matches the default result from calling
[`format()`](https://rdrr.io/r/base/format.html) on a zoned-time.
Additionally, this format matches the de-facto standard extension to RFC
3339 for creating completely unambiguous date-times.

### zoned_time_parse_abbrev()

`zoned_time_parse_abbrev()` is a parser for date-time strings containing
only a time zone abbreviation, like `"2019-01-01 00:00:00 EST"`. The
time zone abbreviation is not enough to identify the full time zone name
that the date-time belongs to, so the full time zone name must be
supplied as the `zone` argument. However, the time zone abbreviation can
help with resolving ambiguity around daylight saving time fallbacks.

For `zoned_time_parse_abbrev()`, `%Z` must be supplied and is
interpreted as the time zone abbreviation rather than the full time zone
name.

If used, the `%z` command must parse correctly, but its value will be
completely ignored.

The default options assume that `x` should be parsed at second
precision, using a `format` string of `"%Y-%m-%d %H:%M:%S %Z"`. This
matches the default result from calling
[`print()`](https://rdrr.io/r/base/print.html) or `format(usetz = TRUE)`
on a POSIXct date-time.

## Usage

``` r
zoned_time_parse_complete(
  x,
  ...,
  format = NULL,
  precision = "second",
  locale = clock_locale()
)

zoned_time_parse_abbrev(
  x,
  zone,
  ...,
  format = NULL,
  precision = "second",
  locale = clock_locale()
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

  A precision for the resulting zoned-time. One of:

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

- zone:

  `[character(1)]`

  A full time zone name.

## Value

A zoned-time.

## Details

If `zoned_time_parse_complete()` is given input that is length zero, all
`NA`s, or completely fails to parse, then no time zone will be able to
be determined. In that case, the result will use `"UTC"`.

If your date-time strings contain time zone offsets (like `-04:00`), but
not the full time zone name, you might need
[`sys_time_parse()`](https://clock.r-lib.org/dev/reference/sys-parsing.md).

If your date-time strings don't contain time zone offsets or the full
time zone name, you might need to use
[`naive_time_parse()`](https://clock.r-lib.org/dev/reference/naive_time_parse.md).
From there, if you know the time zone that the date-times are supposed
to be in, you can convert to a zoned-time with
[`as_zoned_time()`](https://clock.r-lib.org/dev/reference/as_zoned_time.md).

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
library(magrittr)

zoned_time_parse_complete("2019-01-01T01:02:03-05:00[America/New_York]")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-01-01T01:02:03-05:00"

zoned_time_parse_complete(
  "January 21, 2019 -0500 America/New_York",
  format = "%B %d, %Y %z %Z"
)
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-01-21T00:00:00-05:00"

# Nanosecond precision
x <- "2019/12/31 01:05:05.123456700-05:00[America/New_York]"
zoned_time_parse_complete(
  x,
  format = "%Y/%m/%d %H:%M:%S%Ez[%Z]",
  precision = "nanosecond"
)
#> <zoned_time<nanosecond><America/New_York>[1]>
#> [1] "2019-12-31T01:05:05.123456700-05:00"

# The `%z` offset must correspond to the true offset that would be used
# if the input was parsed as a naive-time and then converted to a zoned-time
# with `as_zoned_time()`. For example, the time that was parsed above used an
# offset of `-05:00`. We can confirm that this is correct with:
year_month_day(2019, 1, 1, 1, 2, 3) %>%
  as_naive_time() %>%
  as_zoned_time("America/New_York")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-01-01T01:02:03-05:00"

# So the following would not parse correctly
zoned_time_parse_complete("2019-01-01T01:02:03-04:00[America/New_York]")
#> Warning: Failed to parse 1 string at location 1. Returning `NA` at that location.
#> <zoned_time<second><America/New_York>[1]>
#> [1] NA

# `%z` is useful for breaking ties in otherwise ambiguous times. For example,
# these two times are on either side of a daylight saving time fallback.
# Without the `%z` offset, you wouldn't be able to tell them apart!
x <- c(
  "1970-10-25T01:30:00-04:00[America/New_York]",
  "1970-10-25T01:30:00-05:00[America/New_York]"
)

zoned_time_parse_complete(x)
#> <zoned_time<second><America/New_York>[2]>
#> [1] "1970-10-25T01:30:00-04:00" "1970-10-25T01:30:00-05:00"

# If you have date-time strings with time zone abbreviations,
# `zoned_time_parse_abbrev()` should be able to help. The `zone` must be
# provided, because multiple countries may use the same time zone
# abbreviation. For example:
x <- "1970-01-01 02:30:30 IST"

# IST = India Standard Time
zoned_time_parse_abbrev(x, "Asia/Kolkata")
#> <zoned_time<second><Asia/Kolkata>[1]>
#> [1] "1970-01-01T02:30:30+05:30"

# IST = Israel Standard Time
zoned_time_parse_abbrev(x, "Asia/Jerusalem")
#> <zoned_time<second><Asia/Jerusalem>[1]>
#> [1] "1970-01-01T02:30:30+02:00"

# The time zone abbreviation is mainly useful for resolving ambiguity
# around daylight saving time fallbacks. Without the abbreviation, these
# date-times would be ambiguous.
x <- c(
  "1970-10-25 01:30:00 EDT",
  "1970-10-25 01:30:00 EST"
)
zoned_time_parse_abbrev(x, "America/New_York")
#> <zoned_time<second><America/New_York>[2]>
#> [1] "1970-10-25T01:30:00-04:00" "1970-10-25T01:30:00-05:00"
```
