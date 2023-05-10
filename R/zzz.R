# nocov start

.onLoad <- function(libname, pkgname) {
  tzdb::tzdb_initialize()

  clock_init_utils()

  clock_ns <- topenv(environment())

  # Initializers must run after initializing C++ utils and setting tzdata
  clock_init_duration_utils(clock_ns)
  clock_init_limits_init(clock_ns)
  clock_init_year_month_day_utils(clock_ns)
  clock_init_year_month_weekday_utils(clock_ns)
  clock_init_iso_year_week_day_utils(clock_ns)
  clock_init_year_day_utils(clock_ns)
  clock_init_sys_time_utils(clock_ns)
  clock_init_naive_time_utils(clock_ns)
  clock_init_zoned_time_utils(clock_ns)
  clock_init_weekday_utils(clock_ns)

  on_package_load("ggplot2", {
    the$ScaleContinuousYearMonthDay <- make_ScaleContinuousYearMonthDay()
    the$ScaleContinuousPositionYearMonthDay <- make_ScaleContinuousPositionYearMonthDay()

    the$ScaleContinuousYearQuarterDay <- make_ScaleContinuousYearQuarterDay()
    the$ScaleContinuousPositionYearQuarterDay <- make_ScaleContinuousPositionYearQuarterDay()

    the$ScaleContinuousYearWeekDay <- make_ScaleContinuousYearWeekDay()
    the$ScaleContinuousPositionYearWeekDay <- make_ScaleContinuousPositionYearWeekDay()
  })

  vctrs::s3_register("pillar::pillar_shaft", "clock_calendar", pillar_shaft.clock_calendar)
  vctrs::s3_register("pillar::pillar_shaft", "clock_time_point", pillar_shaft.clock_time_point)
  vctrs::s3_register("pillar::pillar_shaft", "clock_zoned_time", pillar_shaft.clock_zoned_time)

  vctrs::s3_register("slider::slider_plus", "Date.clock_duration", slider_plus.Date.clock_duration)
  vctrs::s3_register("slider::slider_plus", "POSIXct.clock_duration", slider_plus.POSIXct.clock_duration)
  vctrs::s3_register("slider::slider_plus", "POSIXlt.clock_duration", slider_plus.POSIXlt.clock_duration)

  vctrs::s3_register("slider::slider_minus", "Date.clock_duration", slider_minus.Date.clock_duration)
  vctrs::s3_register("slider::slider_minus", "POSIXct.clock_duration", slider_minus.POSIXct.clock_duration)
  vctrs::s3_register("slider::slider_minus", "POSIXlt.clock_duration", slider_minus.POSIXlt.clock_duration)

  vctrs::s3_register("ggplot2::scale_type", "clock_year_month_day", scale_type.clock_year_month_day)
  vctrs::s3_register("ggplot2::scale_type", "clock_year_quarter_day", scale_type.clock_year_quarter_day)
  vctrs::s3_register("ggplot2::scale_type", "clock_year_week_day", scale_type.clock_year_week_day)
}

# nocov end
