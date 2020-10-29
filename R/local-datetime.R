new_local_datetime <- function(x = double()) {
  if (!is_double(x)) {
    abort("`x` must be a double.")
  }

  new_vctr(
    x,
    class = "civil_local_datetime",
    inherit_base_type = FALSE
  )
}

#' @export
format.civil_local_datetime <- function(x, ...) {
  x <- new_datetime(x, "UTC")
  x <- format(x, format = "%Y-%m-%d %H:%M:%S")

  # Avoid size 0 -> size 1 recycling
  if (length(x) > 0L) {
    x <- paste0("<", x, ">")
  }

  x
}

#' @export
vec_ptype_full.civil_local_datetime <- function(x, ...) {
  "local_datetime"
}

# ------------------------------------------------------------------------------

#' @export
localize <- function(x) {
  UseMethod("localize")
}

#' @export
localize.Date <- function(x) {
  out <- unstructure(x) * 86400
  names(out) <- names(x)
  new_local_datetime(out)
}

#' @export
localize.POSIXct <- function(x) {
  x <- posixct_standardize(x)
  # convert to local seconds
  out <- localize_posixct_cpp(x)
  new_local_datetime(out)
}

#' @export
localize.POSIXlt <- function(x) {
  x <- as.POSIXct.POSIXlt(x)
  localize(x)
}

# ------------------------------------------------------------------------------

#' @export
unlocalize <- function(x, zone, ...) {
  UseMethod("unlocalize")
}

#' @export
unlocalize.civil_local_datetime <- function(x,
                                            zone,
                                            ...,
                                            dst_resolver = default_dst_resolver()) {
  check_dots_empty()
  validate_dst_resolver(dst_resolver)
  unlocalize_cpp(x, zone, dst_resolver)
}
