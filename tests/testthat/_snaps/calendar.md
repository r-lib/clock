# normal print method works

    Code
      x
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03" "2019-04" "2019-05"

# can limit with `max`

    Code
      print(x, max = 2)
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02"
      Reached `max` or `getOption('max.print')`. Omitted 3 values.

---

    Code
      print(x, max = 4)
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03" "2019-04"
      Reached `max` or `getOption('max.print')`. Omitted 1 value.

---

    Code
      print(x, max = 5)
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03" "2019-04" "2019-05"

---

    Code
      print(x, max = 6)
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03" "2019-04" "2019-05"

# `max` defaults to `getOption('max.print')` but can be overridden

    Code
      x
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03"
      Reached `max` or `getOption('max.print')`. Omitted 2 values.

---

    Code
      print(x, max = 4)
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03" "2019-04"
      Reached `max` or `getOption('max.print')`. Omitted 1 value.

---

    Code
      print(x, max = 5)
    Output
      <year_month_day<month>[5]>
      [1] "2019-01" "2019-02" "2019-03" "2019-04" "2019-05"

# `max` is validated

    `max` must be a positive number.

---

    `max` must be a single number, or `NULL`.

---

    `max` must be a single number, or `NULL`.

---

    Can't convert `max` <character> to <integer>.

# group: `precision` is validated

    `precision` not recognized.

# group: `precision` must be calendar specific

    `precision` must be a valid precision for a 'year_month_day'.

# group: `precision` can't be wider than `x`

    Can't group at a precision (second) that is more precise than `x` (day).

# group: can't group a subsecond precision `x` at another subsecond precision

    Can't group a subsecond precision `x` (nanosecond) by another subsecond precision (microsecond).

# narrow: `precision` is validated

    `precision` not recognized.

# narrow: `precision` must be calendar specific

    `precision` must be a valid precision for a 'year_month_day'.

# narrow: `precision` can't be wider than `x`

    Can't narrow to a precision (second) that is wider than `x` (day).

# narrow: can't narrow a subsecond precision `x` to another subsecond precision

    Can't narrow a subsecond precision `x` (nanosecond) to another subsecond precision (microsecond).

# widen: `precision` is validated

    `precision` not recognized.

# widen: `precision` must be calendar specific

    `precision` must be a valid precision for a 'year_month_day'.

# widen: `precision` can't be narrower than `x`

    Can't widen to a precision (month) that is narrower than `x` (day).

# widen: can't widen a subsecond precision `x` to another subsecond precision

    Can't widen a subsecond precision `x` (millisecond) to another subsecond precision (microsecond).

# precision: can only be called on calendars

    no applicable method for 'calendar_precision' applied to an object of class "c('clock_sys_time', 'clock_time_point', 'clock_rcrd', 'vctrs_rcrd', 'vctrs_vctr')"

