# ------------------------------------------------------------------------------

#' @export
names.clock_rcrd <- function(x) {
  .Call(`_clock_clock_rcrd_names`, x)
}

#' @export
`names<-.clock_rcrd` <- function(x, value) {
  .Call(`_clock_clock_rcrd_set_names`, x, value)
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

#' @export
min.clock_rcrd <- function(x, ..., na.rm = FALSE) {
  check_bool(na.rm)

  if (vec_is_empty(x) || (na.rm && vec_all_missing(x))) {
    clock_maximum(x)
  } else {
    NextMethod()
  }
}

#' @export
max.clock_rcrd <- function(x, ..., na.rm = FALSE) {
  check_bool(na.rm)

  if (vec_is_empty(x) || (na.rm && vec_all_missing(x))) {
    clock_minimum(x)
  } else {
    NextMethod()
  }
}

#' @export
range.clock_rcrd <- function(x, ..., na.rm = FALSE) {
  check_bool(na.rm)

  if (vec_is_empty(x) || (na.rm && vec_all_missing(x))) {
    vec_c(clock_maximum(x), clock_minimum(x))
  } else {
    NextMethod()
  }
}

vec_all_missing <- function(x) {
  vec_any_missing(x) && all(vec_detect_missing(x))
}

# ------------------------------------------------------------------------------

#' @export
vec_math.clock_rcrd <- function(.fn, .x, ...) {
  switch(
    .fn,
    is.nan = clock_rcrd_is_nan(.x),
    is.finite = clock_rcrd_is_finite(.x),
    is.infinite = clock_rcrd_is_infinite(.x),
    NextMethod()
  )
}

clock_rcrd_is_nan <- function(x) {
  vec_rep(FALSE, vec_size(x))
}

clock_rcrd_is_finite <- function(x) {
  !vec_detect_missing(x)
}

clock_rcrd_is_infinite <- function(x) {
  vec_rep(FALSE, vec_size(x))
}
