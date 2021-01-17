.onLoad <- function(libname, pkgname) {
  sysname <- tolower(Sys.info()[["sysname"]])

  if (identical(sysname, "windows")) {
    # Use shipped text version of the tzdb
    tzdata <- system.file("tzdata", package = "clock", mustWork = TRUE)
    clock_set_install(tzdata)
  } else {
    # Use shipped binary version of the tzdb
    tz_dir <- system.file("zoneinfo", package = "clock", mustWork = TRUE)
    clock_set_tz_dir(tz_dir)
  }

  vctrs::s3_register("pillar::pillar_shaft", "clock_calendar", pillar_shaft.clock_calendar)
  vctrs::s3_register("pillar::pillar_shaft", "clock_time_point", pillar_shaft.clock_time_point)
  vctrs::s3_register("pillar::pillar_shaft", "clock_zoned_time", pillar_shaft.clock_zoned_time)
}
