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
  !vec_equal_na(x)
}

clock_rcrd_is_infinite <- function(x) {
  vec_rep(FALSE, vec_size(x))
}
