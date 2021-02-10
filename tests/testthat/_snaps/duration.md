# can't round across common precision boundary

    Can't ceiling from 'week' precision to 'month' precision.

---

    Can't floor from 'second' precision to 'year' precision.

# seq() validates from

    `from` must have size 1, not size 2.

---

    `from` can't be `NA`.

# seq() validates length.out / along.with exclusiveness

    Can only specify one of `length.out` and `along.with`.

# seq() only takes two optional args

    Must specify exactly two of:
    - `to`
    - `by`
    - Either `length.out` or `along.with`

---

    Must specify exactly two of:
    - `to`
    - `by`
    - Either `length.out` or `along.with`

# seq() requires two optional args

    Must specify exactly two of:
    - `to`
    - `by`
    - Either `length.out` or `along.with`

---

    Must specify exactly two of:
    - `to`
    - `by`
    - Either `length.out` or `along.with`

---

    Must specify exactly two of:
    - `to`
    - `by`
    - Either `length.out` or `along.with`

---

    Must specify exactly two of:
    - `to`
    - `by`
    - Either `length.out` or `along.with`

# seq() validates `to`

    `to` must have size 1, not size 2.

---

    Can't convert `to` <double> to match type of `from` <duration<year>>.

---

    Can't convert `to` <duration<day>> to match type of `from` <duration<year>>.
    Can't cast between calendrical durations and chronological durations.

---

    `to` can't be `NA`.

# seq() validates `by`

    `by` must have size 1, not size 2.

---

    `by` can't be `NA`.

---

    `by` can't be `0`.

---

    `by` can't be `0`.

---

    Can't convert `n` <character> to <integer>.

# `by` must be castable to the type of `from`

    Can't convert `by` <duration<month>> to <duration<year>>.
    Can't cast to a less precise precision.

---

    Can't convert `by` <duration<day>> to <duration<year>>.
    Can't cast between calendrical durations and chronological durations.

---

    Can't convert `by` <duration<year>> to <duration<day>>.
    Can't cast between calendrical durations and chronological durations.

# seq() validates `length.out`

    `length.out` must have size 1, not size 2.

---

    `length.out` can't be `NA`.

---

    `length.out` can't be negative.

---

    Can't convert `length.out` <character> to <integer>.

# seq() validates dots

    `...` is not empty.
    
    We detected these problematic arguments:
    * `..1`
    
    These dots only exist to allow future extensions and should be empty.
    Did you misspecify an argument?

# seq() validates from/to/by signs

    When `from` is less than `to`, `by` must be positive.

---

    When `from` is greater than `to`, `by` must be negative.

# seq() enforces non-fractional results

    Usage of `length.out` or `along.with` must generate a non-fractional sequence between `from` and `to`.

---

    Usage of `length.out` or `along.with` must generate a non-fractional sequence between `from` and `to`.

# `to` is always cast to `from`

    Can't convert `to` <duration<year>> to match type of `from` <duration<day>>.
    Can't cast between calendrical durations and chronological durations.

---

    Can't convert `to` <duration<month>> to match type of `from` <duration<year>>.
    Can't cast to a less precise precision.

