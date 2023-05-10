# can't accidentally supply `zone` to reinterpret date-time in new zone

    Code
      as_date_time(new_datetime(0), zone = "America/New_York")
    Condition
      Error in `as_date_time()`:
      ! `...` must be empty.
      x Problematic argument:
      * zone = "America/New_York"

# can resolve nonexistent midnight issues for Date -> POSIXct

    Code
      (expect_error(as_date_time(x, zone), class = "clock_error_nonexistent_time"))
    Output
      <error/clock_error_nonexistent_time>
      Error in `as_zoned_time()`:
      ! Nonexistent time due to daylight saving time at location 1.
      i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# can resolve ambiguous midnight issues for Date -> POSIXct

    Code
      (expect_error(as_date_time(x, zone), class = "clock_error_ambiguous_time"))
    Output
      <error/clock_error_ambiguous_time>
      Error in `as_zoned_time()`:
      ! Ambiguous time due to daylight saving time at location 1.
      i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can handle nonexistent times resulting from grouping

    Code
      date_group(x, "hour", n = 2)
    Condition
      Error in `as_zoned_time()`:
      ! Nonexistent time due to daylight saving time at location 1.
      i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# can't group by finer precisions

    Code
      date_group(x, "nanosecond")
    Condition
      Error in `calendar_group()`:
      ! Can't group at a precision ("nanosecond") that is more precise than `x` ("second").

# can't group by non-year-month-day precisions

    Code
      date_group(x, "quarter")
    Condition
      Error in `calendar_group()`:
      ! `precision` must be a valid precision for a <year_month_day>, not "quarter".

# flooring can handle nonexistent times

    Code
      date_floor(x, "hour", n = 2)
    Condition
      Error in `as_zoned_time()`:
      ! Nonexistent time due to daylight saving time at location 2.
      i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# `origin` is floored to the precision of `precision` with a potential warning before rounding

    Code
      date_floor(x, "day", origin = origin)
    Condition
      Warning:
      `origin` has been floored from "second" precision to "day" precision to match `precision`. This floor has resulted in a loss of information.
    Output
      [1] "1970-01-01 EST" "1970-01-02 EST"

# `origin` is validated

    Code
      date_floor(x, "day", origin = 1)
    Condition
      Error in `date_floor()`:
      ! `origin` must be a <POSIXt>, not the number 1.

---

    Code
      date_floor(x, "day", origin = new_datetime(NA_real_, zone))
    Condition
      Error in `date_floor()`:
      ! `origin` can't contain missing values.
      i The following locations are missing: 1.

---

    Code
      date_floor(x, "day", origin = new_datetime(Inf, zone))
    Condition
      Error in `date_floor()`:
      ! `origin` can't be an infinite date.

---

    Code
      date_floor(x, "day", origin = new_datetime(c(0, 1), zone))
    Condition
      Error in `date_floor()`:
      ! `origin` must have size 1, not size 2.

---

    Code
      date_floor(x, "day", origin = new_datetime(0, ""))
    Condition
      Error in `date_floor()`:
      ! `origin` must have the same time zone as `x`.

---

    Code
      date_floor(x, "day", origin = new_datetime(0, "America/Los_Angeles"))
    Condition
      Error in `date_floor()`:
      ! `origin` must have the same time zone as `x`.

# default format is correct

    Code
      date_format(x)
    Output
      [1] "2019-01-01T00:00:00-05:00[America/New_York]"

# can format date-times

    Code
      vapply(X = formats, FUN = function(format) date_format(x, format = format),
      FUN.VALUE = character(1))
    Output
                              C: %C                         y: %y 
                            "C: 20"                       "y: 18" 
                              Y: %Y                         b: %b 
                          "Y: 2018"                      "b: Dec" 
                              h: %h                         B: %B 
                           "h: Dec"                 "B: December" 
                              m: %m                         d: %d 
                            "m: 12"                       "d: 31" 
                              a: %a                         A: %A 
                           "a: Mon"                   "A: Monday" 
                              w: %w                         g: %g 
                             "w: 1"                       "g: 19" 
                              G: %G                         V: %V 
                          "G: 2019"                       "V: 01" 
                              u: %u                         U: %U 
                             "u: 1"                       "U: 52" 
                              W: %W                         j: %j 
                            "W: 53"                      "j: 365" 
                              D: %D                         x: %x 
                      "D: 12/31/18"                 "x: 12/31/18" 
                              F: %F                         H: %H 
                    "F: 2018-12-31"                       "H: 23" 
                              I: %I                         M: %M 
                            "I: 11"                       "M: 59" 
                              S: %S                         p: %p 
                            "S: 59"                       "p: PM" 
                              R: %R                         T: %T 
                         "R: 23:59"                 "T: 23:59:59" 
                              X: %X                         r: %r 
                      "X: 23:59:59"              "r: 11:59:59 PM" 
                              c: %c                         %: %% 
      "c: Mon Dec 31 23:59:59 2018"                        "%: %" 
                              z: %z                       Ez: %Ez 
                         "z: -0500"                  "Ez: -05:00" 
                              Z: %Z 
              "Z: America/New_York" 

---

    Code
      vapply(X = formats, FUN = function(format) date_format(x, format = format,
        locale = clock_locale("fr")), FUN.VALUE = character(1))
    Output
                                C: %C                           y: %y 
                              "C: 20"                         "y: 18" 
                                Y: %Y                           b: %b 
                            "Y: 2018"                       "b: déc." 
                                h: %h                           B: %B 
                            "h: déc."                   "B: décembre" 
                                m: %m                           d: %d 
                              "m: 12"                         "d: 31" 
                                a: %a                           A: %A 
                            "a: lun."                      "A: lundi" 
                                w: %w                           g: %g 
                               "w: 1"                         "g: 19" 
                                G: %G                           V: %V 
                            "G: 2019"                         "V: 01" 
                                u: %u                           U: %U 
                               "u: 1"                         "U: 52" 
                                W: %W                           j: %j 
                              "W: 53"                        "j: 365" 
                                D: %D                           x: %x 
                        "D: 12/31/18"                   "x: 12/31/18" 
                                F: %F                           H: %H 
                      "F: 2018-12-31"                         "H: 23" 
                                I: %I                           M: %M 
                              "I: 11"                         "M: 59" 
                                S: %S                           p: %p 
                              "S: 59"                         "p: PM" 
                                R: %R                           T: %T 
                           "R: 23:59"                   "T: 23:59:59" 
                                X: %X                           r: %r 
                        "X: 23:59:59"                "r: 11:59:59 PM" 
                                c: %c                           %: %% 
      "c: lun. déc. 31 23:59:59 2018"                          "%: %" 
                                z: %z                         Ez: %Ez 
                           "z: -0500"                    "Ez: -05:00" 
                                Z: %Z 
                "Z: America/New_York" 

# `date_time_zone()` has a special error on Dates

    Code
      date_time_zone(new_date(0))
    Condition
      Error in `date_time_zone()`:
      ! `x` can't be a <Date>.
      i <Date> is considered a naive time with an unspecified time zone.
      i Time zones can only be get or set for date-times (<POSIXct> or <POSIXlt>).

# `date_time_zone()` validates `x`

    Code
      date_time_zone(1)
    Condition
      Error in `date_time_zone()`:
      ! `x` must be a <POSIXt>, not the number 1.

# `date_time_set_zone()` has a special error on Dates

    Code
      date_time_set_zone(new_date(), "UTC")
    Condition
      Error in `date_time_set_zone()`:
      ! `x` can't be a <Date>.
      i <Date> is considered a naive time with an unspecified time zone.
      i Time zones can only be get or set for date-times (<POSIXct> or <POSIXlt>).

# `date_time_set_zone()` validates `x`

    Code
      date_time_set_zone(1, "UTC")
    Condition
      Error in `date_time_set_zone()`:
      ! `x` must be a <POSIXt>, not the number 1.

# can resolve ambiguity and nonexistent times

    Code
      date_time_parse("1970-04-26 02:30:00", "America/New_York")
    Condition
      Error in `as_zoned_time()`:
      ! Nonexistent time due to daylight saving time at location 1.
      i Resolve nonexistent time issues by specifying the `nonexistent` argument.

---

    Code
      date_time_parse("1970-10-25 01:30:00", "America/New_York")
    Condition
      Error in `as_zoned_time()`:
      ! Ambiguous time due to daylight saving time at location 1.
      i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# failure to parse throws a warning

    Code
      date_time_parse("foo", "America/New_York")
    Condition
      Warning:
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      [1] NA

# throws warning on failed parses

    Code
      date_time_parse_complete("foo")
    Condition
      Warning:
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      [1] NA

# abbrev - throws warning on failed parses

    Code
      date_time_parse_abbrev("foo", "America/New_York")
    Condition
      Warning:
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      [1] NA

# `ambiguous = x` retains the offset of `x` if applicable

    Code
      date_shift(x, as_weekday(x), ambiguous = "error")
    Condition
      Error in `as_zoned_time()`:
      ! Ambiguous time due to daylight saving time at location 1.
      i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# `zone` is required

    Code
      date_time_build(2019)
    Condition
      Error in `date_time_build()`:
      ! `zone` must be supplied.

# can handle invalid dates

    Code
      date_time_build(2019, 1:12, 31, zone = zone)
    Condition
      Error in `invalid_resolve()`:
      ! Invalid date found at location 2.
      i Resolve invalid date issues by specifying the `invalid` argument.

# can handle nonexistent times

    Code
      date_time_build(1970, 4, 26, 2, 30, zone = zone)
    Condition
      Error in `as_zoned_time()`:
      ! Nonexistent time due to daylight saving time at location 1.
      i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# can handle ambiguous times

    Code
      date_time_build(1970, 10, 25, 1, 30, zone = zone)
    Condition
      Error in `as_zoned_time()`:
      ! Ambiguous time due to daylight saving time at location 1.
      i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# boundaries are handled right

    Code
      x$begin
    Output
      [1] "-32767-01-01 UTC"

---

    Code
      x$end
    Output
      [1] "32767-12-31 UTC"

# input must be a date-time

    Code
      date_time_info(1)
    Condition
      Error in `date_time_info()`:
      ! `x` must be a <POSIXt>, not the number 1.

# start: can't use invalid precisions

    Code
      date_start(date_time_build(2019, zone = "America/New_York"), "quarter")
    Condition
      Error in `calendar_start_end_checks()`:
      ! `precision` must be a valid precision for a <year_month_day>, not "quarter".

# can resolve nonexistent start issues

    Code
      (expect_error(date_start(x, "day"), class = "clock_error_nonexistent_time"))
    Output
      <error/clock_error_nonexistent_time>
      Error in `as_zoned_time()`:
      ! Nonexistent time due to daylight saving time at location 1.
      i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# can resolve ambiguous start issues

    Code
      (expect_error(date_start(x, "day"), class = "clock_error_ambiguous_time"))
    Output
      <error/clock_error_ambiguous_time>
      Error in `as_zoned_time()`:
      ! Ambiguous time due to daylight saving time at location 1.
      i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# end: can't use invalid precisions

    Code
      date_end(date_time_build(2019, zone = "America/New_York"), "quarter")
    Condition
      Error in `calendar_start_end_checks()`:
      ! `precision` must be a valid precision for a <year_month_day>, not "quarter".

# daily `by` uses naive-time around DST gaps

    Code
      date_seq(from, by = duration_days(1), total_size = 3)
    Condition
      Error in `as_zoned_time()`:
      ! Nonexistent time due to daylight saving time at location 2.
      i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# daily `by` uses naive-time around DST fallbacks

    Code
      date_seq(from, by = duration_days(1), total_size = 3)
    Condition
      Error in `as_zoned_time()`:
      ! Ambiguous time due to daylight saving time at location 2.
      i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# monthly / yearly `by` uses calendar -> naive-time around DST gaps

    Code
      date_seq(from, by = duration_months(1), total_size = 3)
    Condition
      Error in `as_zoned_time()`:
      ! Nonexistent time due to daylight saving time at location 2.
      i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# monthly / yearly `by` uses calendar -> naive-time around DST fallbacks

    Code
      date_seq(from, by = duration_years(1), total_size = 3)
    Condition
      Error in `as_zoned_time()`:
      ! Ambiguous time due to daylight saving time at location 2.
      i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can resolve invalid dates

    Code
      date_seq(from, to = to, by = duration_months(1))
    Condition
      Error in `invalid_resolve()`:
      ! Invalid date found at location 2.
      i Resolve invalid date issues by specifying the `invalid` argument.

# components of `to` more precise than `by` must match `from`

    Code
      date_seq(date_time_build(2019, 1, 1, 2, 3, 20, zone = zone), to = date_time_build(
        2019, 2, 2, 1, 3, 5, zone = zone), by = duration_minutes(1))
    Condition
      Error in `date_seq()`:
      ! All components of `from` and `to` more precise than "minute" must match.
      i `from` is "2019-01-01T07:03:20".
      i `to` is "2019-02-02T06:03:05".

---

    Code
      date_seq(date_time_build(2019, 1, 1, zone = zone), to = date_time_build(2019, 2,
        2, 2, zone = zone), by = duration_days(1))
    Condition
      Error in `date_seq()`:
      ! All components of `from` and `to` more precise than "day" must match.
      i `from` is "2019-01-01T00:00:00".
      i `to` is "2019-02-02T02:00:00".

---

    Code
      date_seq(date_time_build(2019, 1, 1, zone = zone), to = date_time_build(2019, 2,
        2, zone = zone), by = duration_months(1))
    Condition
      Error in `date_seq()`:
      ! All components of `from` and `to` more precise than "month" must match.
      i `from` is "2019-01-01T00:00:00".
      i `to` is "2019-02-02T00:00:00".

---

    Code
      date_seq(date_time_build(2019, 1, 1, zone = zone), to = date_time_build(2019, 2,
        1, 1, zone = zone), by = duration_months(1))
    Condition
      Error in `date_seq()`:
      ! All components of `from` and `to` more precise than "month" must match.
      i `from` is "2019-01-01T00:00:00".
      i `to` is "2019-02-01T01:00:00".

---

    Code
      date_seq(date_time_build(2019, 1, 1, zone = zone), to = date_time_build(2019, 1,
        2, zone = zone), by = duration_years(1))
    Condition
      Error in `date_seq()`:
      ! All components of `from` and `to` more precise than "year" must match.
      i `from` is "2019-01-01T00:00:00".
      i `to` is "2019-01-02T00:00:00".

# `to` must have same time zone as `by`

    Code
      date_seq(date_time_build(1970, zone = "UTC"), to = date_time_build(1970, zone = "America/New_York"),
      by = 1)
    Condition
      Error in `date_seq()`:
      ! `from` and `to` must have identical time zones.
      i `from` has zone "UTC".
      i `to` has zone "America/New_York".

# validates integerish `by`

    Code
      date_seq(new_datetime(1), by = 1.5, total_size = 1)
    Condition
      Error in `date_seq()`:
      ! Can't convert from `by` <double> to <integer> due to loss of precision.
      * Locations: 1

# validates `total_size` early

    Code
      date_seq(new_datetime(1), by = 1, total_size = 1.5)
    Condition
      Error in `date_seq()`:
      ! Can't convert from `total_size` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      date_seq(new_datetime(1), by = 1, total_size = NA)
    Condition
      Error in `date_seq()`:
      ! `total_size` can't contain missing values.
      i The following locations are missing: 1.

---

    Code
      date_seq(new_datetime(1), by = 1, total_size = -1)
    Condition
      Error in `date_seq()`:
      ! `total_size` can't be negative.

# `to` and `total_size` must not generate a non-fractional sequence

    Code
      date_seq(new_datetime(0), to = new_datetime(3), total_size = 3)
    Condition
      Error:
      ! The supplied output size does not result in a non-fractional sequence between `from` and `to`.

# requires exactly two optional arguments

    Code
      date_seq(new_datetime(1), by = 1)
    Condition
      Error in `date_seq()`:
      ! Must specify exactly two of:
      * `to`
      * `by`
      * `total_size`

---

    Code
      date_seq(new_datetime(1), total_size = 1)
    Condition
      Error in `date_seq()`:
      ! Must specify exactly two of:
      * `to`
      * `by`
      * `total_size`

---

    Code
      date_seq(new_datetime(1), to = new_datetime(1))
    Condition
      Error in `date_seq()`:
      ! Must specify exactly two of:
      * `to`
      * `by`
      * `total_size`

# requires `to` to be POSIXt

    Code
      date_seq(new_datetime(1), to = 1, by = 1)
    Condition
      Error in `date_seq()`:
      ! `to` must be a <POSIXt>, not the number 1.

# requires year, month, day, hour, minute, or second precision

    Code
      date_seq(new_datetime(1), to = new_datetime(2), by = duration_nanoseconds(1))
    Condition
      Error in `date_seq()`:
      ! `by` must have a precision of "year", "quarter", "month", "week", "day", "hour", "minute", or "second", not "nanosecond".

# checks empty dots

    Code
      date_seq(new_datetime(1), new_datetime(2))
    Condition
      Error in `date_seq()`:
      ! `...` must be empty.
      x Problematic argument:
      * ..1 = new_datetime(2)
      i Did you forget to name an argument?

# must use a valid POSIXt precision

    Code
      (expect_error(date_count_between(x, x, "millisecond")))
    Output
      <error/rlang_error>
      Error in `date_count_between()`:
      ! `precision` must be "year", "quarter", "month", "week", "day", "hour", "minute", or "second", not "millisecond".

# can't count between a POSIXt and a Date

    Code
      (expect_error(date_count_between(x, y, "year")))
    Output
      <error/rlang_error>
      Error in `date_count_between()`:
      ! `end` must be a <POSIXt>, not a <Date> object.

# <posixt> op <duration>

    Code
      vec_arith("+", new_datetime(0, zone), duration_milliseconds(1))
    Condition
      Error in `add_milliseconds()`:
      ! Can't perform this operation on a <POSIXct>.

---

    Code
      vec_arith("+", new_posixlt(0, zone), duration_milliseconds(1))
    Condition
      Error in `add_milliseconds()`:
      ! Can't perform this operation on a <POSIXlt>.

---

    Code
      vec_arith("*", new_datetime(0, zone), duration_years(1))
    Condition
      Error in `arith_posixt_and_duration()`:
      ! <datetime<America/New_York>> * <duration<year>> is not permitted

---

    Code
      vec_arith("*", new_posixlt(0, zone), duration_years(1))
    Condition
      Error in `arith_posixt_and_duration()`:
      ! <POSIXlt<America/New_York>> * <duration<year>> is not permitted

# <duration> op <posixt>

    Code
      vec_arith("-", duration_years(1), new_datetime(0, zone))
    Condition
      Error in `arith_duration_and_posixt()`:
      ! <duration<year>> - <datetime<America/New_York>> is not permitted
      Can't subtract a POSIXct/POSIXlt from a duration.

---

    Code
      vec_arith("-", duration_years(1), new_posixlt(0, zone))
    Condition
      Error in `arith_duration_and_posixt()`:
      ! <duration<year>> - <POSIXlt<America/New_York>> is not permitted
      Can't subtract a POSIXct/POSIXlt from a duration.

---

    Code
      vec_arith("+", duration_milliseconds(1), new_datetime(0, zone))
    Condition
      Error in `add_milliseconds()`:
      ! Can't perform this operation on a <POSIXct>.

---

    Code
      vec_arith("+", duration_milliseconds(1), new_posixlt(0, zone))
    Condition
      Error in `add_milliseconds()`:
      ! Can't perform this operation on a <POSIXlt>.

---

    Code
      vec_arith("*", duration_years(1), new_datetime(0, zone))
    Condition
      Error in `arith_duration_and_posixt()`:
      ! <duration<year>> * <datetime<America/New_York>> is not permitted

---

    Code
      vec_arith("*", duration_years(1), new_posixlt(0, zone))
    Condition
      Error in `arith_duration_and_posixt()`:
      ! <duration<year>> * <POSIXlt<America/New_York>> is not permitted

# `slide_index()` will error on naive-time based arithmetic and ambiguous times

    Code
      slider::slide_index(x, i, identity, .after = after)
    Condition
      Error in `as_zoned_time()`:
      ! Ambiguous time due to daylight saving time at location 1.
      i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# `slide_index()` will error on naive-time based arithmetic and nonexistent times

    Code
      slider::slide_index(x, i, identity, .after = after)
    Condition
      Error in `as_zoned_time()`:
      ! Nonexistent time due to daylight saving time at location 1.
      i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# `slide_index()` will error on calendrical arithmetic and ambiguous times

    Code
      slider::slide_index(x, i, identity, .after = after)
    Condition
      Error in `as_zoned_time()`:
      ! Ambiguous time due to daylight saving time at location 1.
      i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# `slide_index()` will error on calendrical arithmetic and nonexistent times

    Code
      slider::slide_index(x, i, identity, .after = after)
    Condition
      Error in `as_zoned_time()`:
      ! Nonexistent time due to daylight saving time at location 1.
      i Resolve nonexistent time issues by specifying the `nonexistent` argument.

