# Parsing: date

`date_parse()` parses strings into a Date.

The default `format` used is `"%Y-%m-%d"`. This matches the default
result from calling [`print()`](https://rdrr.io/r/base/print.html) or
[`format()`](https://rdrr.io/r/base/format.html) on a Date.

## Usage

``` r
date_parse(x, ..., format = NULL, locale = clock_locale())
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

- locale:

  `[clock_locale]`

  A locale object created from
  [`clock_locale()`](https://clock.r-lib.org/reference/clock_locale.md).

## Value

A Date.

## Details

*`date_parse()` ignores both the `%z` and `%Z` commands,* as clock
treats Date as a *naive* type, with a yet-to-be-specified time zone.

Parsing strings with sub-daily components, such as hours, minutes, or
seconds, should generally be done with
[`date_time_parse()`](https://clock.r-lib.org/reference/date-time-parse.md).
If you only need the date components from a string with sub-daily
components, choose one of the following:

- If the date components are at the front of the string, and you don't
  want the time components to affect the date in any way, you can use
  `date_parse()` to parse only the date components. For example,
  `date_parse("2019-01-05 00:01:02", format = "%Y-%m-%d")` will parse
  through `05` and then stop.

- If you want the time components to influence the date, then parse the
  full string with
  [`date_time_parse()`](https://clock.r-lib.org/reference/date-time-parse.md),
  round to day precision with a rounding function like
  [`date_round()`](https://clock.r-lib.org/reference/date-and-date-time-rounding.md),
  and cast to date with
  [`as_date()`](https://clock.r-lib.org/reference/as_date.md).

Attempting to directly parse all components of a sub-daily string into a
Date is ambiguous and undefined, and is unlikely to work as you might
expect. For example,
`date_parse("2019-01-05 00:01:02", format = "%Y-%m-%d %H:%M:%S")` is not
officially supported, even if it works in some cases.

## Examples

``` r
date_parse("2020-01-01")
#> [1] "2020-01-01"

date_parse(
  "January 5, 2020",
  format = "%B %d, %Y"
)
#> [1] "2020-01-05"

# With a different locale
date_parse(
  "janvier 5, 2020",
  format = "%B %d, %Y",
  locale = clock_locale("fr")
)
#> [1] "2020-01-05"

# A neat feature of `date_parse()` is the ability to parse
# the ISO year-week-day format
date_parse("2020-W01-2", format = "%G-W%V-%u")
#> [1] "2019-12-31"

# ---------------------------------------------------------------------------
# Sub-daily components

# If you have a string with sub-daily components, but only require the date,
# first parse them as date-times to fully parse the sub-daily components,
# then round using whatever convention is required for your use case before
# converting to date.
x <- c("2019-01-01 11", "2019-01-01 12")

x <- date_time_parse(x, zone = "UTC", format = "%Y-%m-%d %H")
x
#> [1] "2019-01-01 11:00:00 UTC" "2019-01-01 12:00:00 UTC"

date_floor(x, "day")
#> [1] "2019-01-01 UTC" "2019-01-01 UTC"
date_round(x, "day")
#> [1] "2019-01-01 UTC" "2019-01-02 UTC"

as_date(date_round(x, "day"))
#> [1] "2019-01-01" "2019-01-02"
```
