# Parsing: naive-time

`naive_time_parse()` is a parser into a naive-time.

`naive_time_parse()` is useful when you have date-time strings like
`"2020-01-01T01:04:30"`. If there is no attached UTC offset or time zone
name, then parsing this string as a naive-time is your best option. If
you know that this string should be interpreted in a specific time zone,
parse as a naive-time, then use
[`as_zoned_time()`](https://clock.r-lib.org/reference/as_zoned_time.md).

The default options assume that `x` should be parsed at second
precision, using a `format` string of `"%Y-%m-%dT%H:%M:%S"`. This
matches the default result from calling
[`format()`](https://rdrr.io/r/base/format.html) on a naive-time.

*`naive_time_parse()` ignores both the `%z` and `%Z` commands.*

If your date-time strings contain a full time zone name and a UTC
offset, use
[`zoned_time_parse_complete()`](https://clock.r-lib.org/reference/zoned-parsing.md).
If they contain a time zone abbreviation, use
[`zoned_time_parse_abbrev()`](https://clock.r-lib.org/reference/zoned-parsing.md).

If your date-time strings contain a UTC offset, but not a full time zone
name, use
[`sys_time_parse()`](https://clock.r-lib.org/reference/sys-parsing.md).

## Usage

``` r
naive_time_parse(
  x,
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
  [`clock_locale()`](https://clock.r-lib.org/reference/clock_locale.md).

## Value

A naive-time.

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
naive_time_parse("2020-01-01T05:06:07")
#> <naive_time<second>[1]>
#> [1] "2020-01-01T05:06:07"

# Day precision
naive_time_parse("2020-01-01", precision = "day")
#> <naive_time<day>[1]>
#> [1] "2020-01-01"

# Nanosecond precision, but using a day based format
naive_time_parse("2020-01-01", format = "%Y-%m-%d", precision = "nanosecond")
#> <naive_time<nanosecond>[1]>
#> [1] "2020-01-01T00:00:00.000000000"

# Remember that the `%z` and `%Z` commands are ignored entirely!
naive_time_parse(
  "2020-01-01 -4000 America/New_York",
  format = "%Y-%m-%d %z %Z"
)
#> <naive_time<second>[1]>
#> [1] "2020-01-01T00:00:00"

# ---------------------------------------------------------------------------
# Fractional seconds and POSIXct

# If you have a string with fractional seconds and want to convert it to
# a POSIXct, remember that clock treats POSIXct as a second precision type.
# Ideally, you'd use a clock type that can support fractional seconds, but
# if you really want to parse it into a POSIXct, the correct way to do so
# is to parse the full fractional time point with the correct `precision`,
# then round to seconds using whatever convention you require, and finally
# convert that to POSIXct.
x <- c("2020-01-01T00:00:00.123", "2020-01-01T00:00:00.555")

# First, parse string with full precision
x <- naive_time_parse(x, precision = "millisecond")
x
#> <naive_time<millisecond>[2]>
#> [1] "2020-01-01T00:00:00.123" "2020-01-01T00:00:00.555"

# Then round to second with a floor, ceiling, or round to nearest
time_point_floor(x, "second")
#> <naive_time<second>[2]>
#> [1] "2020-01-01T00:00:00" "2020-01-01T00:00:00"
time_point_round(x, "second")
#> <naive_time<second>[2]>
#> [1] "2020-01-01T00:00:00" "2020-01-01T00:00:01"

# Finally, convert to POSIXct
as_date_time(time_point_round(x, "second"), zone = "UTC")
#> [1] "2020-01-01 00:00:00 UTC" "2020-01-01 00:00:01 UTC"
```
