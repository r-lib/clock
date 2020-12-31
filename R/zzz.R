.onLoad <- function(libname, pkgname) {
  tzdata <- system.file("tzdata", package = "clock", mustWork = TRUE)
  clock_set_install(tzdata)

  vctrs::s3_register("pillar::pillar_shaft", "clock_rcrd", pillar_shaft.clock_rcrd)
}
