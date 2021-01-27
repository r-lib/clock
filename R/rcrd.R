# ------------------------------------------------------------------------------

clock_rcrd_names <- function(x) {
  attr(x, "clock_rcrd:::names", exact = TRUE)
}

clock_rcrd_set_names <- function(x, value) {
  attrib <- attributes(x)

  # Remove names
  if (is.null(value)) {
    attrib[["clock_rcrd:::names"]] <- NULL
    attributes(x) <- attrib

    return(x)
  }

  size <- vec_size(x)
  value <- as_names(value, size)

  attrib[["clock_rcrd:::names"]] <- value
  attributes(x) <- attrib

  x
}

as_names <- function(x, size) {
  x <- unstructure(x)

  if (!is_character(x)) {
    x <- as.character(x)
  }

  validate_names(x, size)

  x
}

validate_names <- function(names, size) {
  if (is_null(names)) {
    return(invisible(names))
  }

  if (!is_character(names)) {
    abort("Names must be a character vector.")
  }

  if (length(names) != size) {
    abort(paste0("Names must have length ", size, " not ", length(names), "."))
  }

  if (any(is.na(names))) {
    abort("Names cannot be `NA`.")
  }

  invisible(names)
}

# ------------------------------------------------------------------------------

#' @export
names.clock_rcrd <- function(x) {
  attr(x, "clock_rcrd:::names", exact = TRUE)
}

#' @export
`names<-.clock_rcrd` <- function(x, value) {
  clock_rcrd_set_names(x, value)
}

# ------------------------------------------------------------------------------

# - `[.vctrs_rcrd` accidentally allows subsetting fields through `...`
#   https://github.com/r-lib/vctrs/issues/1295

#' @export
`[.clock_rcrd` <- function(x, i) {
  i <- maybe_missing(i, default = TRUE)
  vec_slice(x, i)
}

# - `[[.vctrs_rcrd` doesn't drop names because names aren't supported for rcrds
# - `[[.vctrs_rcrd` allows selections of size >1
#   https://github.com/r-lib/vctrs/issues/1294

#' @export
`[[.clock_rcrd` <- function(x, i) {
  size <- vec_size(x)
  names <- names(x)

  i <- vec_as_location2(i, n = size, names = names, arg = "i")

  # Unname - `[[` never returns input with names
  x <- unname(x)

  vec_slice(x, i)
}

# ------------------------------------------------------------------------------

clock_rcrd_proxy_equal <- function(x) {
  new_data_frame(x)
}
