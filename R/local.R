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

# TODO: Uncertain if we need this
local_date <- function(year = NULL, month = NULL, day = NULL) {
  size <- vec_size_common(year = year, month = month, day = day)
  fields <- vec_recycle_common(year = year, month = month, day = day, .size = size)

  if (is_null(year)) {
    fields$year <- integer()
  }

  if (is_null(month)) {
    fields$month <- rep(1L, size)
  } else if (is_null(year)) {
    abort("Can't specify `month` without `year`.")
  }

  if (is_null(day)) {
    fields$day <- rep(1L, size)
  } else if (is_null(year) || is_null(month)) {
    abort("Can't specify `day` without `year` and `month`.")
  }

  fields <- lapply(fields, vec_cast, to = integer())

  if (any(fields$year < 0L | fields$year > 9999L, na.rm = TRUE)) {
    abort("`year` must be within [0, 9999].")
  }
  if (any(fields$month < 1L | fields$month > 12L, na.rm = TRUE)) {
    abort("`month` must be within [1, 12].")
  }
  if (any(fields$day < 1L | fields$day > 32L, na.rm = TRUE)) {
    abort("`day` must be within [1, 32].")
  }

  na <- is.na(fields$year) | is.na(fields$month) | is.na(fields$day)

  if (any(na)) {
    fields$year[na] <- NA_integer_
    fields$month[na] <- NA_integer_
    fields$day[na] <- NA_integer_
  }

  new_local_date_from_fields(
    fields = fields,
    names = NULL
  )
}

new_local_date <- function(year = integer(),
                           month = integer(),
                           day = integer(),
                           ...,
                           names = NULL) {
  if (!is_integer(year)) {
    abort("`year` must be an integer.")
  }
  if (!is_integer(month)) {
    abort("`month` must be an integer.")
  }
  if (!is_integer(day)) {
    abort("`day` must be an integer.")
  }

  size <- length(year)

  ok <-
    (size == length(month)) &&
    (size == length(day))

  if (!ok) {
    abort("All elements to `new_local_date()` must have the same length.")
  }

  fields <- list(
    year = year,
    month = month,
    day = day
  )

  new_local(
    fields,
    ...,
    names = names,
    class = "civil_local_date"
  )
}

new_local_date_from_fields <- function(fields, names) {
  new_local_date(
    year = fields$year,
    month = fields$month,
    day = fields$day,
    names = names
  )
}

#' @export
vec_proxy.civil_local_date <- function(x, ...) {
  proxy_civil_local(x)
}

#' @export
vec_restore.civil_local_date <- function(x, to, ...) {
  fields <- restore_civil_local_fields(x)
  names <- restore_civil_local_names(x)
  new_local_date_from_fields(fields, names)
}

#' @export
format.civil_local_date <- function(x, ...) {
  year <- field(x, "year")
  month <- field(x, "month")
  day <- field(x, "day")

  year <- format_year(year)
  month <- format_month(month)
  day <- format_day(day)

  out <- glue(
    "<",
    year, "-", month, "-", day,
    ">"
  )

  out[is.na(x)] <- NA_character_

  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.civil_local_date <- function(x, ...) {
  "local_date"
}

is_local_date <- function(x) {
  inherits(x, "civil_local_date")
}

# ------------------------------------------------------------------------------

new_local_datetime <- function(year = integer(),
                               month = integer(),
                               day = integer(),
                               hour = integer(),
                               minute = integer(),
                               second = integer(),
                               ...,
                               names = NULL) {
  if (!is_integer(year)) {
    abort("`year` must be an integer.")
  }
  if (!is_integer(month)) {
    abort("`month` must be an integer.")
  }
  if (!is_integer(day)) {
    abort("`day` must be an integer.")
  }
  if (!is_integer(hour)) {
    abort("`hour` must be an integer.")
  }
  if (!is_integer(minute)) {
    abort("`minute` must be an integer.")
  }
  if (!is_integer(second)) {
    abort("`second` must be an integer.")
  }

  size <- length(year)

  ok <-
    (size == length(month)) &&
    (size == length(day)) &&
    (size == length(hour)) &&
    (size == length(minute)) &&
    (size == length(second))

  if (!ok) {
    abort("All elements to `new_local_datetime()` must have the same length.")
  }

  fields <- list(
    year = year,
    month = month,
    day = day,
    hour = hour,
    minute = minute,
    second = second
  )

  new_local(
    fields,
    ...,
    names = names,
    class = "civil_local_datetime"
  )
}

new_local_datetime_from_fields <- function(fields, names) {
  new_local_datetime(
    year = fields$year,
    month = fields$month,
    day = fields$day,
    hour = fields$hour,
    minute = fields$minute,
    second = fields$second,
    names = names
  )
}

#' @export
vec_proxy.civil_local_datetime <- function(x, ...) {
  proxy_civil_local(x)
}

#' @export
vec_restore.civil_local_datetime <- function(x, to, ...) {
  fields <- restore_civil_local_fields(x)
  names <- restore_civil_local_names(x)
  new_local_datetime_from_fields(fields, names)
}

#' @export
format.civil_local_datetime <- function(x, ...) {
  year <- field(x, "year")
  month <- field(x, "month")
  day <- field(x, "day")
  hour <- field(x, "hour")
  minute <- field(x, "minute")
  second <- field(x, "second")

  year <- format_year(year)
  month <- format_month(month)
  day <- format_day(day)
  hour <- format_hour(hour)
  minute <- format_minute(minute)
  second <- format_second(second)

  out <- glue(
    "<",
    year, "-", month, "-", day,
    " ",
    hour, ":", minute, ":", second,
    ">"
  )

  out[is.na(x)] <- NA_character_

  names(out) <- names(x)

  out
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
  date <- is_Date(x)
  x <- to_posixct(x)

  fields <- localize_posixct_cpp(x)
  names <- names(x)

  if (date) {
    new_local_date_from_fields(fields, names)
  } else {
    new_local_datetime_from_fields(fields, names)
  }
}

#' @export
unlocalize <- function(x, ...) {
  restrict_local(x)
  UseMethod("unlocalize")
}

#' @export
unlocalize.civil_local_date <- function(x,
                                        ...,
                                        day_nonexistent = "last-time") {
  check_dots_empty()
  validate_day_nonexistent(day_nonexistent)
  unlocalize_date_cpp(x, day_nonexistent)
}

#' @export
unlocalize.civil_local_datetime <- function(x,
                                            zone,
                                            ...,
                                            day_nonexistent = "last-time",
                                            dst_nonexistent = "roll-forward",
                                            dst_ambiguous = "earliest") {
  check_dots_empty()

  if (missing(zone)) {
    abort(paste0(
      "In `unlocalize()`, `zone` is missing. ",
      "This argument is required with 'local_datetime' input."
    ))
  }

  validate_day_nonexistent(day_nonexistent)
  validate_dst_nonexistent(dst_nonexistent)
  validate_dst_ambiguous(dst_ambiguous)

  unlocalize_datetime_cpp(x, zone, day_nonexistent, dst_nonexistent, dst_ambiguous)
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

format_year <- function(x) {
  sprintf("%04i", x)
}
format_month <- function(x) {
  sprintf("%02i", x)
}
format_day <- function(x) {
  sprintf("%02i", x)
}
format_hour <- function(x) {
  sprintf("%02i", x)
}
format_minute <- function(x) {
  sprintf("%02i", x)
}
format_second <- function(x) {
  sprintf("%02i", x)
}
