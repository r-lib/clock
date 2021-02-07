# can handle nonexistent times resulting from grouping

    Nonexistent time due to daylight saving time at location 1. Resolve nonexistent time issues by specifying the `nonexistent` argument.

# can't group by finer precisions

    Can't group at a precision (nanosecond) that is more precise than `x` (second).

# can't group by non-year-month-day precisions

    `precision` must be a valid precision for a 'year_month_day'.

# flooring can handle nonexistent times

    Nonexistent time due to daylight saving time at location 2. Resolve nonexistent time issues by specifying the `nonexistent` argument.

# `origin` is floored to the precision of `precision` with a potential warning before rounding

    Code
      date_floor(x, "day", origin = origin)
    Warning <clock_warning_invalid_rounding_origin>
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

