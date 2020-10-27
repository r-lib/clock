.onLoad <- function(libname, pkgname) {
  civil_init()

  tzdata <- system.file("tzdata", package = "civil", mustWork = TRUE)
  civil_set_install(tzdata)
}
