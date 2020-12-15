.onLoad <- function(libname, pkgname) {
  tzdata <- system.file("tzdata", package = "civil", mustWork = TRUE)
  civil_set_install(tzdata)

  vctrs::s3_register("pillar::pillar_shaft", "clock_time_point", pillar_shaft.clock_time_point)
}
