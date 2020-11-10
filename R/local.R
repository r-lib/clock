new_local <- function(fields, ..., names = NULL, class = NULL) {
  if (length(fields) == 0L) {
    abort("`fields` must be a list of length 1 or greater.")
  }

  size <- length(fields[[1]])
  validate_names(names, size)

  new_rcrd(
    fields = fields,
    ...,
    `civil_local:::names` = names,
    class = c(class, "civil_local")
  )
}

#' @export
names.civil_local <- function(x) {
  attr(x, "civil_local:::names", exact = TRUE)
}

#' @export
`names<-.civil_local` <- function(x, value) {
  attrib <- attributes(x)

  # Remove names
  if (is.null(value)) {
    attrib[["civil_local:::names"]] <- NULL
    attributes(x) <- attrib

    return(x)
  }

  size <- vec_size(x)
  value <- as_names(value, size)

  attrib[["civil_local:::names"]] <- value
  attributes(x) <- attrib

  x
}

is_local <- function(x) {
  inherits(x, "civil_local")
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

proxy_civil_local <- function(x) {
  out <- unclass(x)
  out[["civil_local:::names"]] <- names(x)
  out <- new_data_frame(out)
  out
}
restore_civil_local_fields <- function(x) {
  x[["civil_local:::names"]] <- NULL
  names <- names(x)
  x <- unstructure(x)
  names(x) <- names
  x
}
restore_civil_local_names <- function(x) {
  x[["civil_local:::names"]]
}



