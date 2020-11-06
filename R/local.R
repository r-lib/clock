new_local <- function(fields, ..., names = NULL, class = NULL) {
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

  value <- as.character(value)
  value <- unstructure(value)

  if (vec_size(x) != vec_size(value)) {
    abort("Names must have the same length as `x`.")
  }
  if (any(is.na(value))) {
    abort("Names must not be `NA`.")
  }

  attrib[["civil_local:::names"]] <- value
  attributes(x) <- attrib

  x
}

is_local <- function(x) {
  inherits(x, "civil_local")
}

# ------------------------------------------------------------------------------

new_local_date <- function(year = integer(),
                           month = integer(),
                           day = integer()) {
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

  names <- names(year)

  if (!is.null(names)) {
    names(year) <- NULL
  }

  fields <- list(
    year = year,
    month = month,
    day = day
  )

  new_local(
    fields,
    names = names,
    class = "civil_local_date"
  )
}

new_local_date_from_fields <- function(fields) {
  new_local_date(
    year = fields$year,
    month = fields$month,
    day = fields$day
  )
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
                               zone = NULL) {
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
  if (!is_null(zone) && !is_string(zone)) {
    abort("`zone` must be `NULL` or a single character.")
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

  names <- names(year)

  if (!is.null(names)) {
    names(year) <- NULL
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
    zone = zone,
    names = names,
    class = "civil_local_datetime"
  )
}

new_local_datetime_from_fields <- function(fields, zone) {
  new_local_datetime(
    year = fields$year,
    month = fields$month,
    day = fields$day,
    hour = fields$hour,
    minute = fields$minute,
    second = fields$second,
    zone = zone
  )
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

  out
}

#' @export
vec_ptype_full.civil_local_datetime <- function(x, ...) {
  "local_datetime"
}

#' @export
obj_print_header.civil_local_datetime <- function(x, ...) {
  zone <- local_datetime_zone(x)
  zone <- zone_printable(zone)

  if (is_null(zone)) {
    cat_line("<", vec_ptype_full(x), "[", vec_size(x), "]>")
  } else {
    cat_line("<", vec_ptype_full(x), "[zone: ", zone, "][", vec_size(x), "]>")
  }

  invisible(x)
}

is_local_datetime <- function(x) {
  inherits(x, "civil_local_datetime")
}

local_datetime_zone <- function(x) {
  attr(x, "zone", exact = TRUE)
}

zone_printable <- function(zone) {
  if (identical(zone, "")) {
    "local"
  } else {
    zone
  }
}

# ------------------------------------------------------------------------------

#' @export
localize <- function(x) {
  date <- is_Date(x)
  x <- to_posixct(x)
  fields <- localize_posixct_cpp(x)

  if (date) {
    new_local_date_from_fields(fields)
  } else {
    new_local_datetime_from_fields(fields, get_zone(x))
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
                                            ...,
                                            zone = NULL,
                                            day_nonexistent = "last-time",
                                            dst_nonexistent = "roll-forward",
                                            dst_ambiguous = "earliest") {
  check_dots_empty()

  zone <- resolve_zone(zone, x)

  validate_day_nonexistent(day_nonexistent)
  validate_dst_nonexistent(dst_nonexistent)
  validate_dst_ambiguous(dst_ambiguous)

  unlocalize_datetime_cpp(x, zone, day_nonexistent, dst_nonexistent, dst_ambiguous)
}

# ------------------------------------------------------------------------------

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

resolve_zone <- function(zone, x) {
  if (!is_null(zone)) {
    return(zone)
  }

  zone <- local_datetime_zone(x)

  if (is_null(zone)) {
    abort("`zone` is `NULL`, but `x` has no implicit time zone.")
  }

  zone
}
