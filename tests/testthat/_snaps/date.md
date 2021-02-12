# invalid dates must be resolved when converting to a Date

    Conversion from a calendar requires that all dates are valid. Resolve invalid dates by calling `invalid_resolve()`.

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

    C: 20
    y: 18
    Y: 2018
    b: Dec
    h: Dec
    B: December
    m: 12
    d: 31
    a: Mon
    A: Monday
    w: 1
    g: 19
    G: 2019
    V: 01
    u: 1
    U: 52
    W: 53
    j: 365
    D: 12/31/18
    x: 12/31/18
    F: 2018-12-31
    H: 00
    I: 12
    M: 00
    S: 00
    p: AM
    R: 00:00
    T: 00:00:00
    X: 00:00:00
    r: 12:00:00 AM
    z: +0000
    Ez: +00:00
    Z: UTC
    c: Mon Dec 31 00:00:00 2018
    %: %

---

    C: 20
    y: 18
    Y: 2018
    b: déc.
    h: déc.
    B: décembre
    m: 12
    d: 31
    a: lun.
    A: lundi
    w: 1
    g: 19
    G: 2019
    V: 01
    u: 1
    U: 52
    W: 53
    j: 365
    D: 12/31/18
    x: 12/31/18
    F: 2018-12-31
    H: 00
    I: 12
    M: 00
    S: 00
    p: AM
    R: 00:00
    T: 00:00:00
    X: 00:00:00
    r: 12:00:00 AM
    z: +0000
    Ez: +00:00
    Z: UTC
    c: lun. déc. 31 00:00:00 2018
    %: %

# can't set the zone of a Date

    'Date' objects are required to be UTC.

# failure to parse throws a warning

    Code
      date_parse("foo")
    Warning <clock_warning_parse_failures>
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      [1] NA

# can handle invalid dates

    Invalid date found at location 2. Resolve invalid date issues by specifying the `invalid` argument.

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

