
#' Time zone database
#'
#' @description
#' - `zone_database_version()` returns the version of the time zone database
#' currently in use by clock.
#'
#' - `zone_database_names()` returns the time zone names found in the database.
#'
#' @return
#' - `zone_database_version()` returns a single string of the database version.
#'
#' - `zone_database_names()` returns a character vector of zone names.
#'
#' @name zone-database
#'
#' @examples
#' zone_database_version()
#' zone_database_names()
NULL

#' @rdname zone-database
#' @export
zone_database_version <- function() {
  zone_database_version_cpp()
}

#' @rdname zone-database
#' @export
zone_database_names <- function() {
  zone_database_names_cpp()
}
