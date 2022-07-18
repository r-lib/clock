# can't accidentally supply `zone` to reinterpret date-time in new zone

    `...` must be empty.
    x Problematic argument:
    * zone = "America/New_York"

# can resolve nonexistent midnight issues for Date -> POSIXct

    Nonexistent time due to daylight saving time at location 1.
    i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# can resolve ambiguous midnight issues for Date -> POSIXct

    Ambiguous time due to daylight saving time at location 1.
    i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can handle nonexistent times resulting from grouping

    Nonexistent time due to daylight saving time at location 1.
    i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# can't group by finer precisions

    Can't group at a precision (nanosecond) that is more precise than `x` (second).

# can't group by non-year-month-day precisions

    `precision` must be a valid precision for a 'year_month_day'.

# flooring can handle nonexistent times

    Nonexistent time due to daylight saving time at location 2.
    i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# `origin` is floored to the precision of `precision` with a potential warning before rounding

    Code
      date_floor(x, "day", origin = origin)
    Condition
      Warning:
      `origin` has been floored from 'second' precision to 'day' precision to match `precision`. This floor has lost information.
    Output
      [1] "1970-01-01 EST" "1970-01-02 EST"

# `origin` is validated

    `origin` must be a 'POSIXt'.

---

    `origin` must not be `NA` or an infinite date.

---

    `origin` must not be `NA` or an infinite date.

---

    `origin` must have length 1.

---

    `origin` must have the same time zone as `x`.

---

    `origin` must have the same time zone as `x`.

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

# can resolve ambiguity and nonexistent times

    Nonexistent time due to daylight saving time at location 1.
    i Resolve nonexistent time issues by specifying the `nonexistent` argument.

---

    Ambiguous time due to daylight saving time at location 1.
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

    Ambiguous time due to daylight saving time at location 1.
    i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# `zone` is required

    `zone` is a required argument to `date_time_build()`.

# can handle invalid dates

    Invalid date found at location 2.
    i Resolve invalid date issues by specifying the `invalid` argument.

# can handle nonexistent times

    Nonexistent time due to daylight saving time at location 1.
    i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# can handle ambiguous times

    Ambiguous time due to daylight saving time at location 1.
    i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# start: can't use invalid precisions

    `precision` must be a valid precision for a 'year_month_day'.

# can resolve nonexistent start issues

    Nonexistent time due to daylight saving time at location 1.
    i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# can resolve ambiguous start issues

    Ambiguous time due to daylight saving time at location 1.
    i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# end: can't use invalid precisions

    `precision` must be a valid precision for a 'year_month_day'.

# daily `by` uses naive-time around DST gaps

    Nonexistent time due to daylight saving time at location 2.
    i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# daily `by` uses naive-time around DST fallbacks

    Ambiguous time due to daylight saving time at location 2.
    i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# monthly / yearly `by` uses calendar -> naive-time around DST gaps

    Nonexistent time due to daylight saving time at location 2.
    i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# monthly / yearly `by` uses calendar -> naive-time around DST fallbacks

    Ambiguous time due to daylight saving time at location 2.
    i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can resolve invalid dates

    Invalid date found at location 2.
    i Resolve invalid date issues by specifying the `invalid` argument.

# components of `to` more precise than `by` must match `from`

    All components of `from` and `to` more precise than 'minute' must match.

---

    All components of `from` and `to` more precise than 'day' must match.

---

    All components of `from` and `to` more precise than 'month' must match.

---

    All components of `from` and `to` more precise than 'month' must match.

---

    All components of `from` and `to` more precise than 'year' must match.

# `to` must have same time zone as `by`

    `from` and `to` must have identical time zones.

# validates integerish `by`

    Can't convert `by` <double> to <integer>.

# validates `total_size` early

    Can't convert `total_size` <double> to <integer>.

---

    `total_size` can't be `NA`.

---

    `total_size` can't be negative.

# `to` and `total_size` must not generate a non-fractional sequence

    The supplied output size does not result in a non-fractional sequence between `from` and `to`.

# requires exactly two optional arguments

    Must specify exactly two of:
    - `to`
    - `by`
    - `total_size`

---

    Must specify exactly two of:
    - `to`
    - `by`
    - `total_size`

---

    Must specify exactly two of:
    - `to`
    - `by`
    - `total_size`

# requires `to` to be POSIXt

    If supplied, `to` must be a <POSIXct> or <POSIXlt>.

# requires year, month, day, hour, minute, or second precision

    `by` must have a precision of 'year', 'quarter', 'month', 'week', 'day', 'hour', 'minute', or 'second'.

# checks empty dots

    `...` must be empty.
    x Problematic argument:
    * ..1 = new_datetime(2)
    i Did you forget to name an argument?

# must use a valid POSIXt precision

    Code
      (expect_error(date_count_between(x, x, "millisecond")))
    Output
      <error/rlang_error>
      Error in `date_count_between_impl()`:
      ! `precision` must be one of: 'year', 'quarter', 'month', 'week', 'day', 'hour', 'minute', 'second'.

# can't count between a POSIXt and a Date

    Code
      (expect_error(date_count_between(x, y, "year")))
    Output
      <error/rlang_error>
      Error in `date_count_between()`:
      ! `end` must be a <POSIXt>.

# <posixt> op <duration>

    no applicable method for 'add_milliseconds' applied to an object of class "c('POSIXct', 'POSIXt')"

---

    no applicable method for 'add_milliseconds' applied to an object of class "c('POSIXlt', 'POSIXt')"

---

    <datetime<America/New_York>> * <duration<year>> is not permitted

---

    <POSIXlt<America/New_York>> * <duration<year>> is not permitted

# <duration> op <posixt>

    <duration<year>> - <datetime<America/New_York>> is not permitted
    Can't subtract a POSIXct/POSIXlt from a duration.

---

    <duration<year>> - <POSIXlt<America/New_York>> is not permitted
    Can't subtract a POSIXct/POSIXlt from a duration.

---

    no applicable method for 'add_milliseconds' applied to an object of class "c('POSIXct', 'POSIXt')"

---

    no applicable method for 'add_milliseconds' applied to an object of class "c('POSIXlt', 'POSIXt')"

---

    <duration<year>> * <datetime<America/New_York>> is not permitted

---

    <duration<year>> * <POSIXlt<America/New_York>> is not permitted

