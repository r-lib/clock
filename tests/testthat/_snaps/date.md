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
    c: lun. déc. 31 00:00:00 2018
    %: %

# formatting Dates with `%z` or `%Z` returns NA with a warning

    Code
      date_format(x, format = "%z")
    Warning <clock_warning_format_failures>
      Failed to format 1 string at location 1. Returning `NA` at that location.
    Output
      [1] NA

---

    Code
      date_format(x, format = "%Z")
    Warning <clock_warning_format_failures>
      Failed to format 1 string at location 1. Returning `NA` at that location.
    Output
      [1] NA

# failure to parse throws a warning

    Code
      date_parse("foo")
    Warning <clock_warning_parse_failures>
      Failed to parse 1 string at location 1. Returning `NA` at that location.
    Output
      [1] NA

# can handle invalid dates

    Invalid date found at location 2.
    i Resolve invalid date issues by specifying the `invalid` argument.

# can resolve invalid dates

    Invalid date found at location 2.
    i Resolve invalid date issues by specifying the `invalid` argument.

# components of `to` more precise than `by` must match `from`

    All components of `from` and `to` more precise than 'month' must match.

---

    All components of `from` and `to` more precise than 'year' must match.

# validates integerish `by`

    Can't convert from `by` <double> to <integer> due to loss of precision.
    * Locations: 1

# validates `total_size` early

    Can't convert from `total_size` <double> to <integer> due to loss of precision.
    * Locations: 1

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

    `...` is not empty.
    
    We detected these problematic arguments:
    * `..1`
    
    These dots only exist to allow future extensions and should be empty.
    Did you misspecify an argument?

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

