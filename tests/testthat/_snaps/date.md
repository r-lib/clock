# invalid dates must be resolved when converting to a Date

    Conversion from a calendar requires that all dates are valid. Resolve invalid dates by calling `invalid_resolve()`.

# can resolve nonexistent midnight issues

    Nonexistent time due to daylight saving time at location 1.
    i Resolve nonexistent time issues by specifying the `nonexistent` argument.

# can resolve ambiguous midnight issues

    Ambiguous time due to daylight saving time at location 1.
    i Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can't group by finer precisions

    Can't group at a precision (hour) that is more precise than `x` (day).

---

    Can't group at a precision (nanosecond) that is more precise than `x` (day).

# can't group by non-year-month-day precisions

    `precision` must be a valid precision for a 'year_month_day'.

# can only floor by week/day

    Can't floor to a more precise precision.

---

    `precision` must be at least 'day' precision.

# `origin` is validated

    `origin` must be a 'Date'.

---

    `origin` must not be `NA` or an infinite date.

---

    `origin` must not be `NA` or an infinite date.

---

    `origin` must have length 1.

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

    Invalid date found at location 2.
    i Resolve invalid date issues by specifying the `invalid` argument.

# start: can't use invalid precisions

    `precision` must be a valid precision for a 'year_month_day'.

# end: can't use invalid precisions

    `precision` must be a valid precision for a 'year_month_day'.

# can resolve invalid dates

    Invalid date found at location 2.
    i Resolve invalid date issues by specifying the `invalid` argument.

# components of `to` more precise than `by` must match `from`

    All components of `from` and `to` more precise than 'month' must match.

---

    All components of `from` and `to` more precise than 'year' must match.

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

# requires `to` to be Date

    If supplied, `to` must be a <Date>.

# requires year, month, or day precision

    `by` must have a precision of 'year', 'quarter', 'month', 'week', or 'day'.

# checks empty dots

    `...` must be empty.
    x Problematic argument:
    * ..1 = new_date(2)
    i Did you forget to name an argument?

# must use a valid Date precision

    Code
      (expect_error(date_count_between(x, x, "hour")))
    Output
      <error/rlang_error>
      Error in `date_count_between_impl()`:
      ! `precision` must be one of: 'year', 'quarter', 'month', 'week', 'day'.

# can't count between a Date and a POSIXt

    Code
      (expect_error(date_count_between(x, y, "year")))
    Output
      <error/rlang_error>
      Error in `date_count_between()`:
      ! `end` must be a <Date>.

# cannot get the zone of a Date

    Can't get the zone of a 'Date'.

# cannot set the zone of a Date

    Can't set the zone of a 'Date'.

# <date> op <duration>

    no applicable method for 'add_hours' applied to an object of class "Date"

---

    <date> * <duration<year>> is not permitted

# <duration> op <date>

    <duration<year>> - <date> is not permitted
    Can't subtract a Date from a duration.

---

    no applicable method for 'add_hours' applied to an object of class "Date"

---

    <duration<year>> * <date> is not permitted

