# Formatting: zoned-time

This is a zoned-time method for the
[`format()`](https://rdrr.io/r/base/format.html) generic.

This function allows you to format a zoned-time using a flexible
`format` string.

If `format` is `NULL`, a default format of `"%Y-%m-%dT%H:%M:%S%Ez[%Z]"`
is used. This matches the default format that
[`zoned_time_parse_complete()`](https://clock.r-lib.org/dev/reference/zoned-parsing.md)
parses. Additionally, this format matches the de-facto standard
extension to RFC 3339 for creating completely unambiguous date-times.

## Usage

``` r
# S3 method for class 'clock_zoned_time'
format(x, ..., format = NULL, locale = clock_locale(), abbreviate_zone = FALSE)
```

## Arguments

- x:

  `[clock_zoned_time]`

  A zoned-time.

- ...:

  `[dots]`

  Not used, but no error will be thrown if not empty to remain
  compatible with usage of the
  [`format()`](https://rdrr.io/r/base/format.html) generic.

- format:

  `[character(1) / NULL]`

  If `NULL`, a default format is used, which depends on the type of the
  input.

  Otherwise, a format string which is a combination of:

  **Year**

  - `%C`: The year divided by 100 using floored division. If the result
    is a single decimal digit, it is prefixed with `0`.

  - `%y`: The last two decimal digits of the year. If the result is a
    single digit it is prefixed by `0`.

  - `%Y`: The year as a decimal number. If the result is less than four
    digits it is left-padded with `0` to four digits.

  **Month**

  - `%b`, `%h`: The `locale`'s abbreviated month name.

  - `%B`: The `locale`'s full month name.

  - `%m`: The month as a decimal number. January is `01`. If the result
    is a single digit, it is prefixed with `0`.

  **Day**

  - `%d`: The day of month as a decimal number. If the result is a
    single decimal digit, it is prefixed with `0`.

  **Day of the week**

  - `%a`: The `locale`'s abbreviated weekday name.

  - `%A`: The `locale`'s full weekday name.

  - `%w`: The weekday as a decimal number (`0-6`), where Sunday is `0`.

  **ISO 8601 week-based year**

  - `%g`: The last two decimal digits of the ISO week-based year. If the
    result is a single digit it is prefixed by `0`.

  - `%G`: The ISO week-based year as a decimal number. If the result is
    less than four digits it is left-padded with `0` to four digits.

  - `%V`: The ISO week-based week number as a decimal number. If the
    result is a single digit, it is prefixed with `0`.

  - `%u`: The ISO weekday as a decimal number (`1-7`), where Monday is
    `1`.

  **Week of the year**

  - `%U`: The week number of the year as a decimal number. The first
    Sunday of the year is the first day of week `01`. Days of the same
    year prior to that are in week `00`. If the result is a single
    digit, it is prefixed with `0`.

  - `%W`: The week number of the year as a decimal number. The first
    Monday of the year is the first day of week `01`. Days of the same
    year prior to that are in week `00`. If the result is a single
    digit, it is prefixed with `0`.

  **Day of the year**

  - `%j`: The day of the year as a decimal number. January 1 is `001`.
    If the result is less than three digits, it is left-padded with `0`
    to three digits.

  **Date**

  - `%D`, `%x`: Equivalent to `%m/%d/%y`.

  - `%F`: Equivalent to `%Y-%m-%d`.

  **Time of day**

  - `%H`: The hour (24-hour clock) as a decimal number. If the result is
    a single digit, it is prefixed with `0`.

  - `%I`: The hour (12-hour clock) as a decimal number. If the result is
    a single digit, it is prefixed with `0`.

  - `%M`: The minute as a decimal number. If the result is a single
    digit, it is prefixed with `0`.

  - `%S`: Seconds as a decimal number. Fractional seconds are printed at
    the precision of the input. The character for the decimal point is
    localized according to `locale`.

  - `%p`: The `locale`'s equivalent of the AM/PM designations associated
    with a 12-hour clock.

  - `%R`: Equivalent to `%H:%M`.

  - `%T`, `%X`: Equivalent to `%H:%M:%S`.

  - `%r`: Nearly equivalent to `%I:%M:%S %p`, but seconds are always
    printed at second precision.

  **Time zone**

  - `%z`: The offset from UTC in the ISO 8601 format. For example
    `-0430` refers to 4 hours 30 minutes behind UTC. If the offset is
    zero, `+0000` is used. The modified command `%Ez` inserts a `:`
    between the hour and minutes, like `-04:30`.

  - `%Z`: The full time zone name. If `abbreviate_zone` is `TRUE`, the
    time zone abbreviation.

  **Miscellaneous**

  - `%c`: A date and time representation. Similar to, but not exactly
    the same as, `%a %b %d %H:%M:%S %Y`.

  - `%%`: A `%` character.

  - `%n`: A newline character.

  - `%t`: A horizontal-tab character.

- locale:

  `[clock_locale]`

  A locale object created from
  [`clock_locale()`](https://clock.r-lib.org/dev/reference/clock_locale.md).

- abbreviate_zone:

  `[logical(1)]`

  If `TRUE`, `%Z` returns an abbreviated time zone name.

  If `FALSE`, `%Z` returns the full time zone name.

## Value

A character vector of the formatted input.

## Examples

``` r
x <- year_month_day(2019, 1, 1)
x <- as_zoned_time(as_naive_time(x), "America/New_York")

format(x)
#> [1] "2019-01-01T00:00:00-05:00[America/New_York]"
format(x, format = "%B %d, %Y")
#> [1] "January 01, 2019"
format(x, format = "%B %d, %Y", locale = clock_locale("fr"))
#> [1] "janvier 01, 2019"
```
