# can't round across common precision boundary

    Code
      duration_ceiling(duration_weeks(), "month")
    Condition
      Error in `duration_ceiling()`:
      ! Can't ceiling from a chronological precision (week) to a calendrical precision (month).

---

    Code
      duration_floor(duration_seconds(), "year")
    Condition
      Error in `duration_floor()`:
      ! Can't floor from a chronological precision (second) to a calendrical precision (year).

# input is validated

    Code
      duration_floor(1, "year")
    Condition
      Error in `duration_floor()`:
      ! `x` must be a <clock_duration>, not the number 1.

---

    Code
      duration_floor(duration_seconds(1), "foo")
    Condition
      Error in `duration_floor()`:
      ! `precision` must be one of "year", "quarter", "month", "week", "day", "hour", "minute", "second", "millisecond", "microsecond", or "nanosecond", not "foo".

---

    Code
      duration_floor(duration_seconds(1), "day", n = -1)
    Condition
      Error in `duration_floor()`:
      ! `n` must be a whole number larger than or equal to 0, not the number -1.

# seq() validates from

    Code
      seq(duration_years(1:2))
    Condition
      Error in `seq()`:
      ! `from` must have size 1, not size 2.

---

    Code
      seq(duration_years(NA_integer_))
    Condition
      Error in `seq()`:
      ! `from` can't contain missing values.
      i The following locations are missing: 1.

# seq() validates length.out / along.with exclusiveness

    Code
      seq(duration_years(1L), length.out = 1, along.with = 2)
    Condition
      Error in `seq()`:
      ! Can only specify one of `length.out` and `along.with`.

# seq() only takes two optional args

    Code
      seq(x, to = duration_years(1), by = 1, length.out = 1)
    Condition
      Error in `seq()`:
      ! Must specify exactly two of:
      - `to`
      - `by`
      - Either `length.out` or `along.with`

---

    Code
      seq(x, to = duration_years(1), by = 1, along.with = 1)
    Condition
      Error in `seq()`:
      ! Must specify exactly two of:
      - `to`
      - `by`
      - Either `length.out` or `along.with`

# seq() requires two optional args

    Code
      seq(x, to = duration_years(1))
    Condition
      Error in `seq()`:
      ! Must specify exactly two of:
      - `to`
      - `by`
      - Either `length.out` or `along.with`

---

    Code
      seq(x, by = 1)
    Condition
      Error in `seq()`:
      ! Must specify exactly two of:
      - `to`
      - `by`
      - Either `length.out` or `along.with`

---

    Code
      seq(x, length.out = 1)
    Condition
      Error in `seq()`:
      ! Must specify exactly two of:
      - `to`
      - `by`
      - Either `length.out` or `along.with`

---

    Code
      seq(x, along.with = 1)
    Condition
      Error in `seq()`:
      ! Must specify exactly two of:
      - `to`
      - `by`
      - Either `length.out` or `along.with`

# seq() validates `to`

    Code
      seq(duration_years(1L), to = duration_years(1:2), by = 1)
    Condition
      Error in `seq()`:
      ! `to` must have size 1, not size 2.

---

    Code
      seq(duration_years(1L), to = 1, by = 1)
    Condition
      Error in `seq()`:
      ! Can't convert `to` <double> to match type of `from` <duration<year>>.

---

    Code
      seq(duration_years(1L), to = duration_days(1), by = 1)
    Condition
      Error in `seq()`:
      ! Can't convert `to` <duration<day>> to match type of `from` <duration<year>>.
      Can't cast between calendrical durations and chronological durations.

---

    Code
      seq(duration_years(1L), to = duration_years(NA_integer_), by = 1)
    Condition
      Error in `seq()`:
      ! `to` can't contain missing values.
      i The following locations are missing: 1.

# seq() validates `by`

    Code
      seq(duration_years(1L), to = duration_years(1L), by = 1:2)
    Condition
      Error in `seq()`:
      ! `by` must have size 1, not size 2.

---

    Code
      seq(duration_years(1L), to = duration_years(1L), by = NA_integer_)
    Condition
      Error in `seq()`:
      ! `by` can't contain missing values.
      i The following locations are missing: 1.

---

    Code
      seq(duration_years(1L), to = duration_years(1L), by = 0)
    Condition
      Error in `seq()`:
      ! `by` can't be `0`.

---

    Code
      seq(duration_years(1L), to = duration_years(1L), by = duration_years(0))
    Condition
      Error in `seq()`:
      ! `by` can't be `0`.

---

    Code
      seq(duration_years(1L), to = duration_years(1L), by = "x")
    Condition
      Error in `seq()`:
      ! Can't convert `by` <character> to <integer>.

# `by` must be castable to the type of `from`

    Code
      seq(duration_years(0), to = duration_years(1), by = duration_months(1))
    Condition
      Error in `seq()`:
      ! Can't convert `by` <duration<month>> to <duration<year>>.
      Can't cast to a less precise precision.

---

    Code
      seq(duration_years(0), to = duration_years(1), by = duration_days(1))
    Condition
      Error in `seq()`:
      ! Can't convert `by` <duration<day>> to <duration<year>>.
      Can't cast between calendrical durations and chronological durations.

---

    Code
      seq(duration_days(0), to = duration_days(1), by = duration_years(1))
    Condition
      Error in `seq()`:
      ! Can't convert `by` <duration<year>> to <duration<day>>.
      Can't cast between calendrical durations and chronological durations.

# seq() validates `length.out`

    Code
      seq(duration_years(1L), to = duration_years(1L), length.out = 1:2)
    Condition
      Error in `seq()`:
      ! `length.out` must have size 1, not size 2.

---

    Code
      seq(duration_years(1L), to = duration_years(1L), length.out = NA_integer_)
    Condition
      Error in `seq()`:
      ! `length.out` can't contain missing values.
      i The following locations are missing: 1.

---

    Code
      seq(duration_years(1L), to = duration_years(1L), length.out = -1)
    Condition
      Error in `seq()`:
      ! `length.out` can't be negative.

---

    Code
      seq(duration_years(1L), to = duration_years(1L), length.out = "x")
    Condition
      Error in `seq()`:
      ! Can't convert `length.out` <character> to <integer>.

# seq() validates dots

    Code
      seq(duration_years(1), duration_years(1), 1, 1, 1, 1)
    Condition
      Error in `seq()`:
      ! `...` must be empty.
      x Problematic argument:
      * ..1 = 1
      i Did you forget to name an argument?

# seq() enforces non-fractional results

    Code
      seq(duration_years(1L), to = duration_years(2L), length.out = 3)
    Condition
      Error:
      ! The supplied output size does not result in a non-fractional sequence between `from` and `to`.

---

    Code
      seq(duration_years(1L), to = duration_years(2L), along.with = 1:3)
    Condition
      Error:
      ! The supplied output size does not result in a non-fractional sequence between `from` and `to`.

# `to` is always cast to `from`

    Code
      seq(duration_days(0), to = duration_years(5), by = 2)
    Condition
      Error in `seq()`:
      ! Can't convert `to` <duration<year>> to match type of `from` <duration<day>>.
      Can't cast between calendrical durations and chronological durations.

---

    Code
      seq(duration_years(0), to = duration_months(5), by = 2)
    Condition
      Error in `seq()`:
      ! Can't convert `to` <duration<month>> to match type of `from` <duration<year>>.
      Can't cast to a less precise precision.

# validates the input

    Code
      duration_spanning_seq(1)
    Condition
      Error in `duration_spanning_seq()`:
      ! `x` must be a <clock_duration>, not the number 1.

# can't add chronological and calendrical durations

    Code
      add_seconds(duration_years(1), 1)
    Condition
      Error in `duration_arith()`:
      ! Can't combine `x` <duration<year>> and `y` <duration<second>>.
      Can't combine calendrical durations with chronological durations.

---

    Code
      add_years(duration_seconds(1), 1)
    Condition
      Error in `duration_arith()`:
      ! Can't combine `x` <duration<second>> and `y` <duration<year>>.
      Can't combine calendrical durations with chronological durations.

# can't convert calendrical duration to time point

    Code
      as_sys_time(duration_years(0))
    Condition
      Error in `as_sys_time()`:
      ! Can't combine `x` <duration<year>> and <duration<day>>.
      Can't combine calendrical durations with chronological durations.

---

    Code
      as_naive_time(duration_years(0))
    Condition
      Error in `as_sys_time()`:
      ! Can't combine `x` <duration<year>> and <duration<day>>.
      Can't combine calendrical durations with chronological durations.

# precision: can only be called on durations

    Code
      duration_precision(sys_days(0))
    Condition
      Error in `duration_precision()`:
      ! `x` must be a <clock_duration>, not a <clock_sys_time> object.

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

# minimums are right

    Code
      clock_minimum(duration_years())
    Output
      <duration<year>[1]>
      [1] -2147483648
    Code
      clock_minimum(duration_quarters())
    Output
      <duration<quarter>[1]>
      [1] -2147483648
    Code
      clock_minimum(duration_months())
    Output
      <duration<month>[1]>
      [1] -2147483648
    Code
      clock_minimum(duration_weeks())
    Output
      <duration<week>[1]>
      [1] -2147483648
    Code
      clock_minimum(duration_days())
    Output
      <duration<day>[1]>
      [1] -2147483648
    Code
      clock_minimum(duration_hours())
    Output
      <duration<hour>[1]>
      [1] -9223372036854775808
    Code
      clock_minimum(duration_minutes())
    Output
      <duration<minute>[1]>
      [1] -9223372036854775808
    Code
      clock_minimum(duration_seconds())
    Output
      <duration<second>[1]>
      [1] -9223372036854775808
    Code
      clock_minimum(duration_milliseconds())
    Output
      <duration<millisecond>[1]>
      [1] -9223372036854775808
    Code
      clock_minimum(duration_microseconds())
    Output
      <duration<microsecond>[1]>
      [1] -9223372036854775808
    Code
      clock_minimum(duration_nanoseconds())
    Output
      <duration<nanosecond>[1]>
      [1] -9223372036854775808

# maximums are right

    Code
      clock_maximum(duration_years())
    Output
      <duration<year>[1]>
      [1] 2147483647
    Code
      clock_maximum(duration_quarters())
    Output
      <duration<quarter>[1]>
      [1] 2147483647
    Code
      clock_maximum(duration_months())
    Output
      <duration<month>[1]>
      [1] 2147483647
    Code
      clock_maximum(duration_weeks())
    Output
      <duration<week>[1]>
      [1] 2147483647
    Code
      clock_maximum(duration_days())
    Output
      <duration<day>[1]>
      [1] 2147483647
    Code
      clock_maximum(duration_hours())
    Output
      <duration<hour>[1]>
      [1] 9223372036854775807
    Code
      clock_maximum(duration_minutes())
    Output
      <duration<minute>[1]>
      [1] 9223372036854775807
    Code
      clock_maximum(duration_seconds())
    Output
      <duration<second>[1]>
      [1] 9223372036854775807
    Code
      clock_maximum(duration_milliseconds())
    Output
      <duration<millisecond>[1]>
      [1] 9223372036854775807
    Code
      clock_maximum(duration_microseconds())
    Output
      <duration<microsecond>[1]>
      [1] 9223372036854775807
    Code
      clock_maximum(duration_nanoseconds())
    Output
      <duration<nanosecond>[1]>
      [1] 9223372036854775807

# `NA` duration prints as expected

    Code
      duration_years(NA)
    Output
      <duration<year>[1]>
      [1] NA
    Code
      duration_quarters(NA)
    Output
      <duration<quarter>[1]>
      [1] NA
    Code
      duration_months(NA)
    Output
      <duration<month>[1]>
      [1] NA
    Code
      duration_weeks(NA)
    Output
      <duration<week>[1]>
      [1] NA
    Code
      duration_days(NA)
    Output
      <duration<day>[1]>
      [1] NA
    Code
      duration_hours(NA)
    Output
      <duration<hour>[1]>
      [1] NA
    Code
      duration_minutes(NA)
    Output
      <duration<minute>[1]>
      [1] NA
    Code
      duration_seconds(NA)
    Output
      <duration<second>[1]>
      [1] NA
    Code
      duration_milliseconds(NA)
    Output
      <duration<millisecond>[1]>
      [1] NA
    Code
      duration_microseconds(NA)
    Output
      <duration<microsecond>[1]>
      [1] NA
    Code
      duration_nanoseconds(NA)
    Output
      <duration<nanosecond>[1]>
      [1] NA

