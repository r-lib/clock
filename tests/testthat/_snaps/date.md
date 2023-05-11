# invalid dates must be resolved when converting to a Date

    Code
      as.Date(year_month_day(2019, 2, 31))
    Condition
      Error in `as_sys_time()`:
      ! Can't convert `x` to another type because some dates are invalid.
      i The following locations are invalid: 1.
      i Resolve invalid dates with `invalid_resolve()`.

# can resolve nonexistent midnight issues

    Code
      (expect_error(as_zoned_time(x, zone), class = "clock_error_nonexistent_time"))
    Output
      <error/clock_error_nonexistent_time>
      Error in `as_zoned_time()`:
      ! Nonexistent time due to daylight saving time at location 1.
      i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# can resolve ambiguous midnight issues

    Code
      (expect_error(as_zoned_time(x, zone), class = "clock_error_ambiguous_time"))
    Output
      <error/clock_error_ambiguous_time>
      Error in `as_zoned_time()`:
      ! Ambiguous time due to daylight saving time at location 1.
      i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can't group by finer precisions

    Code
      date_group(x, "hour")
    Condition
      Error in `calendar_group()`:
      ! Can't group at a precision ("hour") that is more precise than `x` ("day").

---

    Code
      date_group(x, "nanosecond")
    Condition
      Error in `calendar_group()`:
      ! Can't group at a precision ("nanosecond") that is more precise than `x` ("day").

# can't group by non-year-month-day precisions

    Code
      date_group(x, "quarter")
    Condition
      Error in `calendar_group()`:
      ! `precision` must be a valid precision for a <year_month_day>, not "quarter".

# can only floor by week/day

    Code
      date_floor(as.Date("2019-01-01"), "hour")
    Condition
      Error in `duration_rounder()`:
      ! Can't floor to a more precise precision.

---

    Code
      date_floor(as.Date("2019-01-01"), "month")
    Condition
      Error in `time_point_rounder()`:
      ! `precision` must be one of "day", "hour", "minute", "second", "millisecond", "microsecond", or "nanosecond", not "month".
      i Did you mean "minute"?

# `origin` is validated

    Code
      date_floor(x, "day", origin = 1)
    Condition
      Error in `date_floor()`:
      ! `origin` must be a <Date>, not the number 1.

---

    Code
      date_floor(x, "day", origin = new_date(NA_real_))
    Condition
      Error in `date_floor()`:
      ! `origin` can't contain missing values.
      i The following locations are missing: 1.

---

    Code
      date_floor(x, "day", origin = new_date(Inf))
    Condition
      Error in `date_floor()`:
      ! `origin` can't be an infinite date.

---

    Code
      date_floor(x, "day", origin = new_date(c(0, 1)))
    Condition
      Error in `date_floor()`:
      ! `origin` must have size 1, not size 2.

# can format dates

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
                    "F: 2018-12-31"                       "H: 00" 
                              I: %I                         M: %M 
                            "I: 12"                       "M: 00" 
                              S: %S                         p: %p 
                            "S: 00"                       "p: AM" 
                              R: %R                         T: %T 
                         "R: 00:00"                 "T: 00:00:00" 
                              X: %X                         r: %r 
                      "X: 00:00:00"              "r: 12:00:00 AM" 
                              c: %c                         %: %% 
      "c: Mon Dec 31 00:00:00 2018"                        "%: %" 

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
                      "F: 2018-12-31"                         "H: 00" 
                                I: %I                           M: %M 
                              "I: 12"                         "M: 00" 
                                S: %S                           p: %p 
                              "S: 00"                         "p: AM" 
                                R: %R                           T: %T 
                           "R: 00:00"                   "T: 00:00:00" 
                                X: %X                           r: %r 
                        "X: 00:00:00"                "r: 12:00:00 AM" 
                                c: %c                           %: %% 
      "c: lun. déc. 31 00:00:00 2018"                          "%: %" 

# formatting Dates with `%z` or `%Z` returns NA with a warning

    Code
      date_format(x, format = "%z")
    Condition
      Warning:
      Failed to format 1 string at location 1. Returning `NA` at that location.
    Output
      [1] NA

---

    Code
      date_format(x, format = "%Z")
    Condition
      Warning:
      Failed to format 1 string at location 1. Returning `NA` at that location.
    Output
      [1] NA

# failure to parse throws a warning

    Code
      date_parse("foo")
    Condition
      Warning:
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      [1] NA

# can handle invalid dates

    Code
      date_build(2019, 1:12, 31)
    Condition
      Error in `invalid_resolve()`:
      ! Invalid date found at location 2.
      i Resolve invalid date issues by specifying the `invalid` argument.

# start: can't use invalid precisions

    Code
      date_start(date_build(2019), "quarter")
    Condition
      Error in `calendar_start_end_checks()`:
      ! `precision` must be a valid precision for a <year_month_day>, not "quarter".

# end: can't use invalid precisions

    Code
      date_end(date_build(2019), "quarter")
    Condition
      Error in `calendar_start_end_checks()`:
      ! `precision` must be a valid precision for a <year_month_day>, not "quarter".

# can resolve invalid dates

    Code
      date_seq(from, to = to, by = duration_months(1))
    Condition
      Error in `invalid_resolve()`:
      ! Invalid date found at location 2.
      i Resolve invalid date issues by specifying the `invalid` argument.

# components of `to` more precise than `by` must match `from`

    Code
      date_seq(date_build(2019, 1, 1), to = date_build(2019, 2, 2), by = duration_months(
        1))
    Condition
      Error in `date_seq()`:
      ! All components of `from` and `to` more precise than "month" must match.
      i `from` is "2019-01-01".
      i `to` is "2019-02-02".

---

    Code
      date_seq(date_build(2019, 1, 1), to = date_build(2019, 3, 1), by = duration_years(
        1))
    Condition
      Error in `date_seq()`:
      ! All components of `from` and `to` more precise than "year" must match.
      i `from` is "2019-01-01".
      i `to` is "2019-03-01".

# validates integerish `by`

    Code
      date_seq(new_date(1), by = 1.5, total_size = 1)
    Condition
      Error in `date_seq()`:
      ! Can't convert from `by` <double> to <integer> due to loss of precision.
      * Locations: 1

# validates `total_size` early

    Code
      date_seq(new_date(1), by = 1, total_size = 1.5)
    Condition
      Error in `date_seq()`:
      ! Can't convert from `total_size` <double> to <integer> due to loss of precision.
      * Locations: 1

---

    Code
      date_seq(new_date(1), by = 1, total_size = NA)
    Condition
      Error in `date_seq()`:
      ! `total_size` can't contain missing values.
      i The following locations are missing: 1.

---

    Code
      date_seq(new_date(1), by = 1, total_size = -1)
    Condition
      Error in `date_seq()`:
      ! `total_size` can't be negative.

# `to` and `total_size` must not generate a non-fractional sequence

    Code
      date_seq(new_date(0), to = new_date(3), total_size = 3)
    Condition
      Error:
      ! The supplied output size does not result in a non-fractional sequence between `from` and `to`.

# requires exactly two optional arguments

    Code
      date_seq(new_date(1), by = 1)
    Condition
      Error in `date_seq()`:
      ! Must specify exactly two of:
      * `to`
      * `by`
      * `total_size`

---

    Code
      date_seq(new_date(1), total_size = 1)
    Condition
      Error in `date_seq()`:
      ! Must specify exactly two of:
      * `to`
      * `by`
      * `total_size`

---

    Code
      date_seq(new_date(1), to = new_date(1))
    Condition
      Error in `date_seq()`:
      ! Must specify exactly two of:
      * `to`
      * `by`
      * `total_size`

# requires `to` to be Date

    Code
      date_seq(new_date(1), to = 1, by = 1)
    Condition
      Error in `date_seq()`:
      ! `to` must be a <Date>, not the number 1.

# requires year, month, or day precision

    Code
      date_seq(new_date(1), to = new_date(2), by = duration_nanoseconds(1))
    Condition
      Error in `date_seq()`:
      ! `by` must have a precision of "year", "quarter", "month", "week", or "day", not "nanosecond".

# checks empty dots

    Code
      date_seq(new_date(1), new_date(2))
    Condition
      Error in `date_seq()`:
      ! `...` must be empty.
      x Problematic argument:
      * ..1 = new_date(2)
      i Did you forget to name an argument?

# must use a valid Date precision

    Code
      (expect_error(date_count_between(x, x, "hour")))
    Output
      <error/rlang_error>
      Error in `date_count_between()`:
      ! `precision` must be "year", "quarter", "month", "week", or "day", not "hour".

# can't count between a Date and a POSIXt

    Code
      (expect_error(date_count_between(x, y, "year")))
    Output
      <error/rlang_error>
      Error in `date_count_between()`:
      ! `end` must be a <Date>, not a <POSIXct> object.

# <date> op <duration>

    Code
      vec_arith("+", new_date(0), duration_hours(1))
    Condition
      Error in `add_hours()`:
      ! Can't perform this operation on a <Date>.

---

    Code
      vec_arith("*", new_date(0), duration_years(1))
    Condition
      Error in `arith_date_and_duration()`:
      ! <date> * <duration<year>> is not permitted

# <duration> op <date>

    Code
      vec_arith("-", duration_years(1), new_date(0))
    Condition
      Error in `arith_duration_and_date()`:
      ! <duration<year>> - <date> is not permitted
      Can't subtract a Date from a duration.

---

    Code
      vec_arith("+", duration_hours(1), new_date(0))
    Condition
      Error in `add_hours()`:
      ! Can't perform this operation on a <Date>.

---

    Code
      vec_arith("*", duration_years(1), new_date(0))
    Condition
      Error in `arith_duration_and_date()`:
      ! <duration<year>> * <date> is not permitted

# `slide_index()` will error on calendrical arithmetic and invalid dates

    Code
      slider::slide_index(x, i, identity, .after = after)
    Condition
      Error in `invalid_resolve()`:
      ! Invalid date found at location 2.
      i Resolve invalid date issues by specifying the `invalid` argument.

