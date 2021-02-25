# nocov start

.onLoad <- function(libname, pkgname) {
  clock_init_utils()

  tzdata <- tzdb::tzdb_path(type = "text")
  clock_set_install(tzdata)

  clock_ns <- topenv(environment())

  # Initializers must run after initializing C++ utils and setting tzdata
  clock_init_year_month_day_utils(clock_ns)

  vctrs::s3_register("pillar::pillar_shaft", "clock_calendar", pillar_shaft.clock_calendar)
  vctrs::s3_register("pillar::pillar_shaft", "clock_time_point", pillar_shaft.clock_time_point)
  vctrs::s3_register("pillar::pillar_shaft", "clock_zoned_time", pillar_shaft.clock_zoned_time)
}

# nocov end
