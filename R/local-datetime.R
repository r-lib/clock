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

is_local_datetime <- function(x) {
  inherits(x, "civil_local_datetime")
}

# ------------------------------------------------------------------------------

#' @export
localize <- function(x) {
  x <- to_posixct(x)
  out <- localize_posixct_cpp(x)
  new_local_datetime(out)
}

# ------------------------------------------------------------------------------

#' @export
unlocalize <- function(x,
                       zone,
                       ...,
                       dst_nonexistent = "next",
                       dst_ambiguous = "earliest") {
  check_dots_empty()

  if (!is_local_datetime(x)) {
    abort("`x` must be a local datetime object to unlocalize.")
  }

  validate_dst_nonexistent(dst_nonexistent)
  validate_dst_ambiguous(dst_ambiguous)

  unlocalize_cpp(x, zone, dst_nonexistent, dst_ambiguous)
}
