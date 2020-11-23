.onLoad <- function(libname, pkgname) {
  sysname <- tolower(Sys.info()[["sysname"]])

  if (identical(sysname, "windows")) {
    # Use shipped text version of the tzdb
    tzdata <- system.file("tzdata", package = "civil", mustWork = TRUE)
    civil_set_install(tzdata)
  } else {
    # Use shipped binary version of the tzdb
    tz_dir <- system.file("zoneinfo", package = "civil", mustWork = TRUE)
    civil_set_tz_dir(tz_dir)
  }
}
