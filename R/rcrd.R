new_civil_rcrd <- function(fields, ..., names = NULL, class = NULL) {
  if (length(fields) == 0L) {
    abort("`fields` must be a list of length 1 or greater.")
  }

  size <- length(fields[[1]])
  validate_names(names, size)

  new_rcrd(
    fields = fields,
    ...,
    `civil_rcrd:::names` = names,
    class = c(class, "civil_rcrd")
  )
}

is_civil_rcrd <- function(x) {
  inherits(x, "civil_rcrd")
}

# ------------------------------------------------------------------------------

#' @export
names.civil_rcrd <- function(x) {
  attr(x, "civil_rcrd:::names", exact = TRUE)
}

#' @export
`names<-.civil_rcrd` <- function(x, value) {
  attrib <- attributes(x)

  # Remove names
  if (is.null(value)) {
    attrib[["civil_rcrd:::names"]] <- NULL
    attributes(x) <- attrib

    return(x)
  }

  size <- vec_size(x)
  value <- as_names(value, size)

  attrib[["civil_rcrd:::names"]] <- value
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

proxy_civil_rcrd <- function(x) {
  out <- unclass(x)
  out[["civil_rcrd:::names"]] <- names(x)
  out <- new_data_frame(out)
  out
}

restore_civil_rcrd_fields <- function(x) {
  x[["civil_rcrd:::names"]] <- NULL
  names <- names(x)
  x <- unstructure(x)
  names(x) <- names
  x
}

restore_civil_rcrd_names <- function(x) {
  x[["civil_rcrd:::names"]]
}


