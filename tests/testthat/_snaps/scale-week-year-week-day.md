# can't use invalid `date_breaks` or `date_minor_breaks`

    Code
      ggplot2::ggplot_build(p)
    Condition
      Error in `duration_collect_by()`:
      ! Can't convert `by` <duration<day>> to <duration<week>>.
      Can't cast to a less precise precision.

---

    Code
      ggplot2::ggplot_build(p)
    Condition
      Error in `duration_collect_by()`:
      ! Can't convert `by` <duration<day>> to <duration<week>>.
      Can't cast to a less precise precision.

# can't use with unsupported precision

    Code
      ggplot2::ggplot_build(p)
    Condition
      Error in `transform()`:
      ! <year_week_day> inputs can only be "year" or "week" precision.

# can't mix precisions

    Code
      ggplot2::ggplot_build(p)
    Condition
      Error in `transform()`:
      ! All <year_week_day> inputs must have the same precision.

