.onLoad <- function(libname, pkgname) {
  tzdata <- system.file("tzdata", package = "civil", mustWork = TRUE)
  clock_set_install(tzdata)

  vctrs::s3_register("pillar::pillar_shaft", "clock_calendar", pillar_shaft.clock_calendar)
  vctrs::s3_register("pillar::pillar_shaft", "clock_time_point", pillar_shaft.clock_time_point)
}
