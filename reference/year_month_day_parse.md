# Parsing: year-month-day

`year_month_day_parse()` parses strings into a year-month-day.

The default options assume `x` should be parsed at day precision, using
a `format` string of `"%Y-%m-%d"`.

If a more precise precision than day is used, then time components will
also be parsed. The default format separates date and time components by
a `"T"` and the time components by a `":"`. For example, setting the
precision to `"second"` will use a default format of
`"%Y-%m-%dT%H:%M:%S"`. This is aligned with the
[`format()`](https://rdrr.io/r/base/format.html) method for
year-month-day, and with the RFC 3339 standard.

## Usage

``` r
year_month_day_parse(
  x,
  ...,
  format = NULL,
  precision = "day",
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

  A precision for the resulting year-month-day. One of:

  - `"year"`

  - `"month"`

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

  A locale object created by
  [`clock_locale()`](https://clock.r-lib.org/reference/clock_locale.md).

## Value

A year-month-day calendar vector. If a parsing fails, `NA` is returned.

## Details

`year_month_day_parse()` completely ignores the `%z` and `%Z` commands.

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
x <- "2019-01-01"

# Default parses at day precision
year_month_day_parse(x)
#> <year_month_day<day>[1]>
#> [1] "2019-01-01"

# Can parse at less precise precisions too
year_month_day_parse(x, precision = "month")
#> <year_month_day<month>[1]>
#> [1] "2019-01"
year_month_day_parse(x, precision = "year")
#> <year_month_day<year>[1]>
#> [1] "2019"

# Even invalid dates can be round-tripped through format<->parse calls
invalid <- year_month_day(2019, 2, 30)
year_month_day_parse(format(invalid))
#> <year_month_day<day>[1]>
#> [1] "2019-02-30"

# Can parse with time of day
year_month_day_parse(
  "2019-01-30T02:30:00.123456789",
  precision = "nanosecond"
)
#> <year_month_day<nanosecond>[1]>
#> [1] "2019-01-30T02:30:00.123456789"

# Can parse using multiple format strings, which will be tried
# in the order they are provided
x <- c("2019-01-01", "2020-01-01", "2021/2/3")
formats <- c("%Y-%m-%d", "%Y/%m/%d")
year_month_day_parse(x, format = formats)
#> <year_month_day<day>[3]>
#> [1] "2019-01-01" "2020-01-01" "2021-02-03"

# Can parse using other format tokens as well
year_month_day_parse(
  "January, 2019",
  format = "%B, %Y",
  precision = "month"
)
#> <year_month_day<month>[1]>
#> [1] "2019-01"

# Parsing a French year-month-day
year_month_day_parse(
  "octobre 1, 2000",
  format = "%B %d, %Y",
  locale = clock_locale("fr")
)
#> <year_month_day<day>[1]>
#> [1] "2000-10-01"
```
