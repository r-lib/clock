.onLoad <- function(libname, pkgname) {
  tzdata <- system.file("tzdata", package = "civil", mustWork = TRUE)
  civil_set_install(tzdata)

  vctrs::s3_register("pillar::pillar_shaft", "civil_rcrd", pillar_shaft.civil_rcrd)
  vctrs::s3_register("pillar::pillar_shaft", "civil_zoned_gregorian_datetime", pillar_shaft.civil_zoned_gregorian_datetime)
  vctrs::s3_register("pillar::pillar_shaft", "civil_zoned_gregorian_nano_datetime", pillar_shaft.civil_zoned_gregorian_nano_datetime)
}
