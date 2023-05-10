# can't use invalid `date_breaks` or `date_minor_breaks`

    Code
      ggplot2::ggplot_build(p)
    Condition
      Error in `duration_collect_by()`:
      ! Can't convert `by` <duration<day>> to <duration<month>>.
      Can't cast between calendrical durations and chronological durations.

---

    Code
      ggplot2::ggplot_build(p)
    Condition
      Error in `duration_collect_by()`:
      ! Can't convert `by` <duration<day>> to <duration<month>>.
      Can't cast between calendrical durations and chronological durations.

# can't use with unsupported precision

    Code
      ggplot2::ggplot_build(p)
    Condition
      Error in `transform()`:
      ! <year_month_day> inputs can only be "year" or "month" precision.

# can't mix precisions

    Code
      ggplot2::ggplot_build(p)
    Condition
      Error in `transform()`:
      ! All <year_month_day> inputs must have the same precision.

