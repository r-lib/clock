.onLoad <- function(libname, pkgname) {
  tzdata <- system.file("tzdata", package = "civil", mustWork = TRUE)
  civil_set_install(tzdata)
}
