# Formatting: date

This is a Date method for the
[`date_format()`](https://clock.r-lib.org/dev/reference/date_format.md)
generic.

[`date_format()`](https://clock.r-lib.org/dev/reference/date_format.md)
formats a date (Date) using a `format` string.

If `format` is `NULL`, a default format of `"%Y-%m-%d"` is used.

## Usage

``` r
# S3 method for class 'Date'
date_format(x, ..., format = NULL, locale = clock_locale())
```

## Arguments

- x:

  `[Date]`

  A date vector.

- ...:

  These dots are for future extensions and must be empty.

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

## Value

A character vector of the formatted input.

## Details

Because a Date is considered to be a *naive* type in clock, meaning that
it currently has no implied time zone, using the `%z` or `%Z` format
commands is not allowed and will result in `NA`.

## Examples

``` r
x <- as.Date("2019-01-01")

# Default
date_format(x)
#> [1] "2019-01-01"

date_format(x, format = "year: %Y, month: %m, day: %d")
#> [1] "year: 2019, month: 01, day: 01"

# With different locales
date_format(x, format = "%A, %B %d, %Y")
#> [1] "Tuesday, January 01, 2019"
date_format(x, format = "%A, %B %d, %Y", locale = clock_locale("fr"))
#> [1] "mardi, janvier 01, 2019"
```
