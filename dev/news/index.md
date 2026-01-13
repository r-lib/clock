# Changelog

## clock (development version)

- Avoid non-API `SET_ATTRIB()`.

## clock 0.7.3

CRAN release: 2025-03-21

- `%Y`, `%F`, `%G`, and `%c` now parse up to 4 *characters* by default,
  rather than 4 *digits*. This is more in line with the C++20
  specification and was a change made in the upstream `<date.h>` parser.
  Practically, this means that negative years such as `-1000-01-01` will
  no longer parse with `%Y-%m-%d`, and instead requires `%5Y-%m-%d` to
  capture the leading `-`
  ([\#387](https://github.com/r-lib/clock/issues/387)).

- tzdb \>=0.5.0 is required. Note that older versions of clock are not
  compatible with tzdb 0.5.0 and above, so if you are having issues
  (particularly with parsing) please make sure that both clock and tzdb
  are fully updated
  ([\#387](https://github.com/r-lib/clock/issues/387)).

- Fixed `-Wdeprecated-literal-operator` reported by clang
  ([\#386](https://github.com/r-lib/clock/issues/386),
  [@MichaelChirico](https://github.com/MichaelChirico)).

- R \>=4.0.0 is now required. This is consistent with the standards of
  the tidyverse.

## clock 0.7.2

CRAN release: 2025-01-21

- Added a [`diff()`](https://rdrr.io/r/base/diff.html) method for time
  points and calendars to ensure that durations are always returned,
  even in the empty result case
  ([\#364](https://github.com/r-lib/clock/issues/364)).

- Fixed an issue where clock would not compile on Centos 7 using
  gcc-5.4.0 due to a `constexpr` issue
  ([\#357](https://github.com/r-lib/clock/issues/357)).

- Fixed a test that failed due to
  [`seq.Date()`](https://rdrr.io/r/base/seq.Date.html) now returning
  integer storage in some cases in the development version of R.

## clock 0.7.1

CRAN release: 2024-07-18

- Removed usage of non-API `STRING_PTR()` in favor of `STRING_PTR_RO()`.

- Fixed a gcc warning reported by CRAN related to templated C++
  constructors ([\#371](https://github.com/r-lib/clock/issues/371)).

## clock 0.7.0

CRAN release: 2023-05-15

### New features

- New
  [`year_week_day()`](https://clock.r-lib.org/dev/reference/year_week_day.md)
  calendar for specifying a date using the year, the week number, and
  the day of the week, alongside a `start` value representing the day of
  the week that is considered the start of the week. Using
  `start = clock_weekdays$monday` is identical to the
  [`iso_year_week_day()`](https://clock.r-lib.org/dev/reference/iso_year_week_day.md)
  calendar, and using `start = clock_weekdays$sunday` is useful for
  representing the Epidemiological calendar used by the US CDC
  guidelines (similar to what is supported by
  [`lubridate::epiweek()`](https://lubridate.tidyverse.org/reference/week.html)
  and
  [`lubridate::epiyear()`](https://lubridate.tidyverse.org/reference/year.html))
  ([\#110](https://github.com/r-lib/clock/issues/110)).

- New
  [`date_spanning_seq()`](https://clock.r-lib.org/dev/reference/date_spanning_seq.md)
  for generating a regular sequence along the full span of a date or
  date-time vector (i.e. along `[min(x), max(x)]`). It is similar to
  `tidyr::full_seq()`, but is a bit simpler and currently has better
  handling of some edge cases. Additionally included in the low-level
  API are
  [`calendar_spanning_seq()`](https://clock.r-lib.org/dev/reference/calendar_spanning_seq.md),
  [`time_point_spanning_seq()`](https://clock.r-lib.org/dev/reference/time_point_spanning_seq.md),
  and
  [`duration_spanning_seq()`](https://clock.r-lib.org/dev/reference/duration_spanning_seq.md)
  ([\#279](https://github.com/r-lib/clock/issues/279)).

- New
  [`date_time_info()`](https://clock.r-lib.org/dev/reference/date_time_info.md)
  and
  [`zoned_time_info()`](https://clock.r-lib.org/dev/reference/zoned_time_info.md)
  low-level helpers for accessing the previous/next transition times,
  the offset from UTC, and the current time zone abbreviation
  ([\#295](https://github.com/r-lib/clock/issues/295)).

- [`calendar_leap_year()`](https://clock.r-lib.org/dev/reference/calendar_leap_year.md)
  now supports the year-quarter-day and iso-year-week-day calendars
  ([\#332](https://github.com/r-lib/clock/issues/332),
  [\#333](https://github.com/r-lib/clock/issues/333)).

### Breaking changes

- The storage mechanism for the duration, sys-time, naive-time, and
  zoned-time types has been altered to more correctly represent the full
  range of values allowed by the underlying C++ types. This means that
  if you have serialized a value of one of these types with an old
  version of clock, then it will no longer unserialize correctly going
  forward.

  Technically, rather than storing a variable number of integer vectors
  representing ticks, ticks of a day, and ticks of a second, we now
  always store values of these types within two double vectors,
  regardless of the precision. This simplifies the implementation and
  allows us to represent the full range of possible `int64_t` values
  ([\#331](https://github.com/r-lib/clock/issues/331)).

### Lifecycle changes

- [`date_zone()`](https://clock.r-lib.org/dev/reference/date-zone.md)
  and
  [`date_set_zone()`](https://clock.r-lib.org/dev/reference/date-zone.md)
  have been soft-deprecated in favor of
  [`date_time_zone()`](https://clock.r-lib.org/dev/reference/date-time-zone.md)
  and
  [`date_time_set_zone()`](https://clock.r-lib.org/dev/reference/date-time-zone.md)
  ([\#326](https://github.com/r-lib/clock/issues/326)).

### Minor changes and bug fixes

- clock now compiles significantly faster (on a 2018 Intel Mac, it used
  to take ~70 seconds for a full compilation, and now takes ~25 seconds)
  ([\#322](https://github.com/r-lib/clock/issues/322)).

- `%%` and `%/%` operators now return a missing value when the
  right-hand side is `0`. For `%/%`, this is consistent with
  `2L %/% 0L`, which returns a missing value, rather than with
  `2 %/% 0`, which returns `Inf`, since infinite durations are not
  supported ([\#349](https://github.com/r-lib/clock/issues/349)).

- [`seq()`](https://rdrr.io/r/base/seq.html) methods for durations and
  time points handle the empty sequence cases of `from > to && by > 0`
  and `from < to && by < 0` better when `from` and `to` are very far
  apart (i.e. when they would otherwise result in overflow if they were
  subtracted).

- [`zoned_time_zone()`](https://clock.r-lib.org/dev/reference/zoned-zone.md)
  and
  [`zoned_time_set_zone()`](https://clock.r-lib.org/dev/reference/zoned-zone.md)
  are no longer generic, and now only work for zoned-times.

- Documented clock’s current stance on leap seconds in the FAQ vignette
  (clock ignores them like POSIXct)
  ([\#309](https://github.com/r-lib/clock/issues/309)).

- Duration vectors now work as `.before` and `.after` arguments of
  [`slider::slide_index()`](https://slider.r-lib.org/reference/slide_index.html)
  and friends ([\#306](https://github.com/r-lib/clock/issues/306)).

- All `as_*()` generics exported by clock now include `...` in their
  signature to help with extensibility of converting to clock types.
  These are the only clock generics that are currently “blessed” as
  fully extensible ([\#348](https://github.com/r-lib/clock/issues/348)).

- [`as.character()`](https://rdrr.io/r/base/character.html) has been
  implemented for durations.

- Fixed
  [`vec_ptype_full()`](https://vctrs.r-lib.org/reference/vec_ptype_full.html)
  and
  [`vec_ptype_abbr()`](https://vctrs.r-lib.org/reference/vec_ptype_full.html)
  methods for sys-time and naive-time objects
  ([\#302](https://github.com/r-lib/clock/issues/302)).

- Many errors have been improved
  ([\#219](https://github.com/r-lib/clock/issues/219),
  [\#286](https://github.com/r-lib/clock/issues/286),
  [\#595](https://github.com/r-lib/clock/issues/595)).

- Renamed `locale.h` to `fill.h` to avoid clock’s `locale.h` being
  chosen over a system header of the same name on some CentOS machines
  ([\#310](https://github.com/r-lib/clock/issues/310)).

- Skipped a test on 32-bit architectures to work around a bug in base R
  ([\#312](https://github.com/r-lib/clock/issues/312)).

- R \>=3.5.0 is now required, which is in line with tidyverse standards.

- vctrs \>=0.6.1 and rlang \>=1.1.0 are now required.

## clock 0.6.1

CRAN release: 2022-07-18

- [`date_seq()`](https://clock.r-lib.org/dev/reference/date_seq.md) and
  the [`seq()`](https://rdrr.io/r/base/seq.html) methods for the
  calendar, time point, and duration types now allow `from > to` when
  `by > 0`. This now results in a size zero result rather than an error,
  which is more in line with
  [`rlang::seq2()`](https://rlang.r-lib.org/reference/seq2.html) and
  generally has more useful programmatic properties
  ([\#282](https://github.com/r-lib/clock/issues/282)).

- The sys-time method for
  [`as.POSIXct()`](https://rdrr.io/r/base/as.POSIXlt.html) now correctly
  promotes to a precision of at least seconds before attempting the
  conversion. This matches the behavior of the naive-time method
  ([\#278](https://github.com/r-lib/clock/issues/278)).

- Removed the dependency on ellipsis in favor of the equivalent
  functions in rlang
  ([\#288](https://github.com/r-lib/clock/issues/288)).

- Updated tests related to writing UTF-8 on Windows and testthat 3.1.2
  ([\#287](https://github.com/r-lib/clock/issues/287)).

- Updated all snapshot tests to use rlang 1.0.0
  ([\#285](https://github.com/r-lib/clock/issues/285)).

- tzdb \>=0.3.0 is now required to get access to the latest time zone
  database information (2022a).

- vctrs \>=0.4.1 and rlang \>=1.0.4 are now required
  ([\#297](https://github.com/r-lib/clock/issues/297)).

- cpp11 \>=0.4.2 is now required to ensure that a fix related to unwind
  protection is included.

- R \>=3.4.0 is now required. This is consistent with the standards of
  the tidyverse.

## clock 0.6.0

CRAN release: 2021-12-02

- New
  [`date_count_between()`](https://clock.r-lib.org/dev/reference/date_count_between.md),
  [`calendar_count_between()`](https://clock.r-lib.org/dev/reference/calendar-count-between.md),
  and
  [`time_point_count_between()`](https://clock.r-lib.org/dev/reference/time_point_count_between.md)
  for computing the number of units of time between two dates (i.e. the
  number of years, months, days, or seconds). This has a number of uses,
  like computing the age of an individual in years, or determining the
  number of weeks that have passed since the start of the year
  ([\#266](https://github.com/r-lib/clock/issues/266)).

- Modulus is now defined between a duration vector and an integer vector
  through `<duration> %% <integer>`. This returns a duration vector
  containing the remainder of the division
  ([\#273](https://github.com/r-lib/clock/issues/273)).

- Integer division is now defined for two duration objects through
  `<duration> %/% <duration>`. This always returns an integer vector, so
  be aware that using very precise duration objects (like nanoseconds)
  can easily generate a division result that is outside the range of an
  integer. In that case, an `NA` is returned with a warning.

## clock 0.5.0

CRAN release: 2021-10-29

- New
  [`date_time_parse_RFC_3339()`](https://clock.r-lib.org/dev/reference/date-time-parse.md)
  and
  [`sys_time_parse_RFC_3339()`](https://clock.r-lib.org/dev/reference/sys-parsing.md)
  for parsing date-time strings in the [RFC
  3339](https://datatracker.ietf.org/doc/html/rfc3339) format. This
  format is a subset of ISO 8601 representing the most common date-time
  formats seen in internet protocols, and is particularly useful for
  parsing date-time strings returned by an API. The default format
  parses strings like `"2019-01-01T01:02:03Z"` but can be adjusted to
  parse a numeric offset from UTC with the `offset` argument, which can
  parse strings like `"2019-01-01T01:02:03-04:30"`
  ([\#254](https://github.com/r-lib/clock/issues/254)).

- To align more with RFC 3339 and ISO 8601 standards, the default
  formats used in many of the date formatting and parsing functions have
  been slightly altered. The following changes have been made:

  - Date-times (POSIXct):

    - [`date_format()`](https://clock.r-lib.org/dev/reference/date_format.md)
      now prints a `T` between the date and time.

    - [`date_time_parse_complete()`](https://clock.r-lib.org/dev/reference/date-time-parse.md)
      now expects a `T` between the date and time by default.

  - Sys-times:

    - [`format()`](https://rdrr.io/r/base/format.html) and
      [`as.character()`](https://rdrr.io/r/base/character.html) now
      print a `T` between the date and time.

    - [`sys_time_parse()`](https://clock.r-lib.org/dev/reference/sys-parsing.md)
      now expects a `T` between the date and time by default.

  - Naive-times:

    - [`format()`](https://rdrr.io/r/base/format.html) and
      [`as.character()`](https://rdrr.io/r/base/character.html) now
      print a `T` between the date and time.

    - [`naive_time_parse()`](https://clock.r-lib.org/dev/reference/naive_time_parse.md)
      now expects a `T` between the date and time by default.

  - Zoned-times:

    - [`format()`](https://rdrr.io/r/base/format.html) and
      [`as.character()`](https://rdrr.io/r/base/character.html) now
      print a `T` between the date and time.

    - [`zoned_time_parse_complete()`](https://clock.r-lib.org/dev/reference/zoned-parsing.md)
      now expects a `T` between the date and time by default.

  - Calendars:

    - [`format()`](https://rdrr.io/r/base/format.html) and
      [`as.character()`](https://rdrr.io/r/base/character.html) now
      print a `T` between the date and time.

    - [`year_month_day_parse()`](https://clock.r-lib.org/dev/reference/year_month_day_parse.md)
      now expects a `T` between the date and time by default.

- Further improved documentation of undefined behavior resulting from
  attempting to parse sub-daily components of a string that is intended
  to be parsed into a Date
  ([\#258](https://github.com/r-lib/clock/issues/258)).

- Bumped required minimum version of tzdb to 0.2.0 to get access to the
  latest time zone database information (2021e) and to fix a Unicode bug
  on Windows.

## clock 0.4.1

CRAN release: 2021-09-21

- Updated a test related to upcoming changes in testthat.

## clock 0.4.0

CRAN release: 2021-07-22

- New
  [`date_start()`](https://clock.r-lib.org/dev/reference/date-and-date-time-boundary.md)
  and
  [`date_end()`](https://clock.r-lib.org/dev/reference/date-and-date-time-boundary.md)
  for computing the date at the start or end of a particular
  `precision`, such as the “end of the month” or the “start of the
  year”. These are powered by
  [`calendar_start()`](https://clock.r-lib.org/dev/reference/calendar-boundary.md)
  and
  [`calendar_end()`](https://clock.r-lib.org/dev/reference/calendar-boundary.md),
  which allow for even more flexible calendar-specific boundary
  generation, such as the “last moment in the fiscal quarter”
  ([\#232](https://github.com/r-lib/clock/issues/232)).

- New
  [`invalid_remove()`](https://clock.r-lib.org/dev/reference/clock-invalid.md)
  for removing invalid dates. This is just a wrapper around
  `x[!invalid_detect(x)]`, but works nicely with the pipe
  ([\#229](https://github.com/r-lib/clock/issues/229)).

- All clock types now support
  [`is.nan()`](https://rdrr.io/r/base/is.finite.html),
  [`is.finite()`](https://rdrr.io/r/base/is.finite.html), and
  [`is.infinite()`](https://rdrr.io/r/base/is.finite.html).
  Additionally, duration types now support
  [`abs()`](https://rdrr.io/r/base/MathFun.html) and
  [`sign()`](https://rdrr.io/r/base/sign.html)
  ([\#235](https://github.com/r-lib/clock/issues/235)).

- tzdb 0.1.2 is now required, which fixes compilation issues on
  RHEL7/Centos ([\#234](https://github.com/r-lib/clock/issues/234)).

## clock 0.3.1

CRAN release: 2021-06-28

- Parsing into a date-time type that is coarser than the original string
  is now considered ambiguous and undefined behavior. For example,
  parsing a string with fractional seconds using `date_time_parse(x)` or
  `naive_time_parse(x, precision = "second")` is no longer considered
  correct. Instead, if you only require second precision from such a
  string, parse the full string, with fractional seconds, into a clock
  type that can handle them, then round to seconds using whatever
  rounding convention is required for your use case, such as
  [`time_point_floor()`](https://clock.r-lib.org/dev/reference/time-point-rounding.md)
  ([\#230](https://github.com/r-lib/clock/issues/230)).

  For example:

      x <- c("2019-01-01 00:00:59.123", "2019-01-01 00:00:59.556")

      x <- naive_time_parse(x, precision = "millisecond")
      x
      #> <time_point<naive><millisecond>[2]>
      #> [1] "2019-01-01 00:00:59.123" "2019-01-01 00:00:59.556"

      x <- time_point_round(x, "second")
      x
      #> <time_point<naive><second>[2]>
      #> [1] "2019-01-01 00:00:59" "2019-01-01 00:01:00"

      as_date_time(x, "America/New_York")
      #> [1] "2019-01-01 00:00:59 EST" "2019-01-01 00:01:00 EST"

- Preemptively updated tests related to upcoming changes in testthat
  ([\#236](https://github.com/r-lib/clock/issues/236)).

## clock 0.3.0

CRAN release: 2021-04-22

- New [`date_seq()`](https://clock.r-lib.org/dev/reference/date_seq.md)
  for generating date and date-time sequences
  ([\#218](https://github.com/r-lib/clock/issues/218)).

- clock now uses the tzdb package to access the date library’s API. This
  means that the experimental API that was to be used for vroom has been
  removed in favor of using the one exposed in tzdb.

- `zone_database_names()` and `zone_database_version()` have been
  removed in favor of re-exporting
  [`tzdb_names()`](https://tzdb.r-lib.org/reference/tzdb_names.html) and
  [`tzdb_version()`](https://tzdb.r-lib.org/reference/tzdb_version.html)
  from the tzdb package.

## clock 0.2.0

CRAN release: 2021-04-12

- clock now interprets R’s Date class as *naive-time* rather than
  *sys-time*. This means that it no longer assumes that Date has an
  implied time zone of UTC
  ([\#203](https://github.com/r-lib/clock/issues/203)). This generally
  aligns better with how users think Date should work. This resulted in
  the following changes:

  - [`date_zone()`](https://clock.r-lib.org/dev/reference/date-zone.md)
    now errors with Date input, as naive-times do not have a specified
    time zone.

  - [`date_parse()`](https://clock.r-lib.org/dev/reference/date_parse.md)
    now parses into a naive-time, rather than a sys-time, before
    converting to Date. This means that `%z` and `%Z` are now completely
    ignored.

  - The Date method for
    [`date_format()`](https://clock.r-lib.org/dev/reference/date_format.md)
    now uses the naive-time
    [`format()`](https://rdrr.io/r/base/format.html) method rather than
    the zoned-time one. This means that `%z` and `%Z` are no longer
    valid format commands.

  - The zoned-time method for
    [`as.Date()`](https://rdrr.io/r/base/as.Date.html) now converts to
    Date through an intermediate naive-time, rather than a sys-time.
    This means that the printed date will always be retained, which is
    generally what is expected.

  - The Date method for
    [`as_zoned_time()`](https://clock.r-lib.org/dev/reference/as_zoned_time.md)
    now converts to zoned-time through an intermediate naive-time,
    rather than a sys-time. This means that the printed date will always
    attempt to be retained, if possible, which is generally what is
    expected. In the rare case that daylight saving time makes a direct
    conversion impossible, `nonexistent` and `ambiguous` can be used to
    resolve any issues.

- New [`as_date()`](https://clock.r-lib.org/dev/reference/as_date.md)
  and
  [`as_date_time()`](https://clock.r-lib.org/dev/reference/as_date_time.md)
  for converting to Date and POSIXct respectively. Unlike
  [`as.Date()`](https://rdrr.io/r/base/as.Date.html) and
  [`as.POSIXct()`](https://rdrr.io/r/base/as.POSIXlt.html), these
  functions always treat Date as a naive-time type, which results in
  more consistent and intuitive conversions. Note that
  [`as_date()`](https://clock.r-lib.org/dev/reference/as_date.md) does
  conflict with
  [`lubridate::as_date()`](https://lubridate.tidyverse.org/reference/as_date.html),
  and the lubridate version handles Dates differently
  ([\#209](https://github.com/r-lib/clock/issues/209)).

- Added two new convenient helpers
  ([\#197](https://github.com/r-lib/clock/issues/197)):

  - [`date_today()`](https://clock.r-lib.org/dev/reference/date-today.md)
    for getting the current date (Date)

  - [`date_now()`](https://clock.r-lib.org/dev/reference/date-today.md)
    for getting the current date-time (POSIXct)

- Fixed a bug where converting from a time point to a Date or POSIXct
  could round incorrectly
  ([\#205](https://github.com/r-lib/clock/issues/205)).

- Errors resulting from invalid dates or nonexistent/ambiguous times are
  now a little nicer to read through the usage of an info bullet
  ([\#200](https://github.com/r-lib/clock/issues/200)).

- Formatting a naive-time with `%Z` or `%z` now warns that there were
  format failures ([\#204](https://github.com/r-lib/clock/issues/204)).

- Fixed a Solaris ambiguous behavior issue from calling `pow(int, int)`.

- Linking against cpp11 0.2.7 is now required to fix a rare memory leak
  issue.

- Exposed an extremely experimental and limited C++ API for vroom
  ([\#322](https://github.com/r-lib/clock/issues/322)).

## clock 0.1.0

CRAN release: 2021-03-31

- Added a `NEWS.md` file to track changes to the package.
