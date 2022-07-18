# can't round across common precision boundary

    Can't ceiling from a chronological precision (week) to a calendrical precision (month).

---

    Can't floor from a chronological precision (second) to a calendrical precision (year).

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

    Can't convert `by` <character> to <integer>.

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

    `...` must be empty.
    x Problematic argument:
    * ..1 = 1
    i Did you forget to name an argument?

# seq() enforces non-fractional results

    The supplied output size does not result in a non-fractional sequence between `from` and `to`.

---

    The supplied output size does not result in a non-fractional sequence between `from` and `to`.

# `to` is always cast to `from`

    Can't convert `to` <duration<year>> to match type of `from` <duration<day>>.
    Can't cast between calendrical durations and chronological durations.

---

    Can't convert `to` <duration<month>> to match type of `from` <duration<year>>.
    Can't cast to a less precise precision.

# can't add chronological and calendrical durations

    Can't combine `x` <duration<year>> and `y` <duration<second>>.
    Can't combine calendrical durations with chronological durations.

---

    Can't combine `x` <duration<second>> and `y` <duration<year>>.
    Can't combine calendrical durations with chronological durations.

# can't convert calendrical duration to time point

    Can't combine `x` <duration<year>> and <duration<day>>.
    Can't combine calendrical durations with chronological durations.

---

    Can't combine `x` <duration<year>> and <duration<day>>.
    Can't combine calendrical durations with chronological durations.

# precision: can only be called on durations

    `x` must be a 'clock_duration'.

# `<duration> / <duration>` is not allowed

    Code
      (expect_error(duration_years(1) / duration_years(2)))
    Output
      <error/vctrs_error_incompatible_op>
      Error in `vec_arith()`:
      ! <duration<year>> / <duration<year>> is not permitted
      Durations only support integer division. Did you want `%/%`?

# `<duration> %/% <duration>` results in NA for OOB values

    Code
      out <- (numerator + one) %/% denominator
    Condition
      Warning:
      Conversion to integer is outside the range of an integer. `NA` values have been introduced, beginning at location 1.

---

    Code
      out <- (-numerator - one) %/% denominator
    Condition
      Warning:
      Conversion to integer is outside the range of an integer. `NA` values have been introduced, beginning at location 1.

# `<duration> %% <numeric>` casts the numeric to integer

    Code
      (expect_error(duration_hours(5) %% 2.5))
    Output
      <error/vctrs_error_cast_lossy>
      Error in `duration_scalar_arith()`:
      ! Can't convert from `y` <double> to <integer> due to loss of precision.
      * Locations: 1

