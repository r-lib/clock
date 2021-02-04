# can resolve ambiguous issues - character

    Ambiguous time due to daylight savings at location 1. Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can resolve ambiguous issues - zoned-time

    Ambiguous time due to daylight savings at location 2. Resolve ambiguous time issues by specifying the `ambiguous` argument.

---

    Ambiguous time due to daylight savings at location 1. Resolve ambiguous time issues by specifying the `ambiguous` argument.

# can resolve nonexistent issues

    Nonexistent time due to daylight savings at location 1. Resolve nonexistent time issues by specifying the `nonexistent` argument.

# `ambiguous` is validated

    `ambiguous` must be a character vector, a zoned-time, a POSIXct, or a list.

---

    'foo' is not a recognized `ambiguous` option.

---

    `ambiguous` must have length 1, or 0.

---

    A zoned-time or POSIXct `ambiguous` must have the same zone as `zone`.

---

    A list `ambiguous` must have length 2.

---

    The first element of a list `ambiguous` must be a zoned-time or POSIXt.

---

    The second element of a list `ambiguous` must be a character vector.

---

    'foo' is not a recognized `ambiguous` option.

# `nonexistent` is validated

    `nonexistent` must be a character vector.

---

    'foo' is not a recognized `nonexistent` option.

---

    `nonexistent` must have length 1, or 0.

# zone is validated

    'foo' is not a known time zone.

---

    Invalid input type, expected 'character' actual 'double'

