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

#' @export
local_date <- function(year = NULL,
                       month = NULL,
                       day = NULL,
                       ...,
                       day_nonexistent = "last-time") {
  check_dots_empty()

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
  if (any(fields$day < 1L | fields$day > 31L, na.rm = TRUE)) {
    abort("`day` must be within [1, 31].")
  }

  days <- convert_year_month_day_to_days(
    fields$year,
    fields$month,
    fields$day,
    day_nonexistent
  )

  new_local_date(days)
}

new_local_date <- function(days = integer(),
                           ...,
                           names = NULL) {
  if (!is_integer(days)) {
    abort("`days` must be an integer.")
  }

  fields <- list(
    days = days
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
    days = fields$days,
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
  days <- field(x, "days")

  fields <- convert_days_to_year_month_day(days)

  year <- fields$year
  month <- fields$month
  day <- fields$day

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

#' @export
local_datetime <- function(year = NULL,
                           month = NULL,
                           day = NULL,
                           hour = NULL,
                           minute = NULL,
                           second = NULL,
                           ...,
                           day_nonexistent = "last-time") {
  check_dots_empty()

  fields <- list(
    year = year,
    month = month,
    day = day,
    hour = hour,
    minute = minute,
    second = second
  )

  size <- vec_size_common(!!!fields)
  fields <- vec_recycle_common(!!!fields, .size = size)

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

  if (is_null(hour)) {
    fields$hour <- rep(0L, size)
  } else if (is_null(year) || is_null(month) || is_null(day)) {
    abort("Can't specify `hour` without `year`, `month`, and `day`.")
  }

  if (is_null(minute)) {
    fields$minute <- rep(0L, size)
  } else if (is_null(year) || is_null(month) || is_null(day) || is_null(hour)) {
    abort("Can't specify `minute` without `year`, `month`, `day`, and `hour`.")
  }

  if (is_null(second)) {
    fields$second <- rep(0L, size)
  } else if (is_null(year) || is_null(month) || is_null(day) || is_null(hour) || is_null(minute)) {
    abort("Can't specify `second` without `year`, `month`, `day`, `hour`, and `minute`.")
  }

  fields <- lapply(fields, vec_cast, to = integer())

  days <- convert_year_month_day_to_days(
    fields$year,
    fields$month,
    fields$day,
    day_nonexistent
  )

  time_of_day <- convert_hour_minute_second_to_time_of_day(
    fields$hour,
    fields$minute,
    fields$second
  )

  new_local_datetime(days, time_of_day)
}

new_local_datetime <- function(days = integer(),
                               time_of_day = integer(),
                               ...,
                               names = NULL) {
  if (!is_integer(days)) {
    abort("`days` must be an integer.")
  }
  if (!is_integer(time_of_day)) {
    abort("`time_of_day` must be an integer.")
  }

  if (length(days) != length(time_of_day)) {
    abort("All elements to `new_local_datetime()` must have the same length.")
  }

  fields <- list(
    days = days,
    time_of_day = time_of_day
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
    days = fields$days,
    time_of_day = fields$time_of_day,
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
  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")

  ymd <- convert_days_to_year_month_day(days)
  hms <- convert_time_of_day_to_hour_minute_second(time_of_day)

  year <- ymd$year
  month <- ymd$month
  day <- ymd$day
  hour <- hms$hour
  minute <- hms$minute
  second <- hms$second

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
  if (is_Date(x)) {
    localize_date(x)
  } else if (is_POSIXct(x) || is_POSIXlt(x)) {
    localize_posixt(x)
  } else {
    stop_civil_unsupported_class(x)
  }
}

localize_date <- function(x) {
  names <- names(x)
  days <- date_to_days(x)

  new_local_date(days, names = names)
}

localize_posixt <- function(x) {
  x <- to_posixct(x)

  names <- names(x)
  seconds <- unstructure(x)
  zone <- get_zone(x)

  fields <- convert_seconds_to_days_and_time_of_day(seconds, zone)

  new_local_datetime_from_fields(fields, names)
}

#' @export
unlocalize <- function(x, ...) {
  restrict_local(x)
  UseMethod("unlocalize")
}

#' @export
unlocalize.civil_local_date <- function(x, ...) {
  check_dots_empty()
  days <- field(x, "days")
  days_to_date(days, names(x))
}

#' @export
unlocalize.civil_local_datetime <- function(x,
                                            zone,
                                            ...,
                                            dst_nonexistent = "roll-forward",
                                            dst_ambiguous = "earliest") {
  check_dots_empty()

  if (missing(zone)) {
    abort(paste0(
      "In `unlocalize()`, `zone` is missing. ",
      "This argument is required with 'local_datetime' input."
    ))
  }

  zone <- zone_standardize(zone)

  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")

  seconds <- convert_days_and_time_of_day_to_seconds(
    days = days,
    time_of_day = time_of_day,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  names(seconds) <- names(x)

  new_datetime(seconds, zone)
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

convert_days_to_year_month_day <- function(days) {
  convert_days_to_year_month_day_cpp(days)
}

convert_year_month_day_to_days <- function(year,
                                           month,
                                           day,
                                           day_nonexistent) {
  validate_day_nonexistent(day_nonexistent)
  convert_year_month_day_to_days_cpp(year, month, day, day_nonexistent)
}

convert_hour_minute_second_to_time_of_day <- function(hour, minute, second) {
  convert_hour_minute_second_to_time_of_day_cpp(hour, minute, second)
}

convert_time_of_day_to_hour_minute_second <- function(time_of_day) {
  convert_time_of_day_to_hour_minute_second_cpp(time_of_day)
}

convert_seconds_to_days_and_time_of_day <- function(seconds, zone) {
  convert_seconds_to_days_and_time_of_day_cpp(seconds, zone)
}

convert_days_and_time_of_day_to_seconds <- function(days,
                                                    time_of_day,
                                                    zone,
                                                    dst_nonexistent,
                                                    dst_ambiguous) {
  validate_dst_nonexistent(dst_nonexistent)
  validate_dst_ambiguous(dst_ambiguous)

  size_days <- length(days)
  size_dst_nonexistent <- vec_size(dst_nonexistent)
  size_dst_ambiguous <- vec_size(dst_ambiguous)

  # These may be vectorized (for arithmetic), but must be the same length
  # as x if they are - x is not recycled to their length
  if (size_dst_nonexistent != 1L && size_dst_nonexistent != size_days) {
    abort("`dst_nonexistent` must be size 1 or the same size as `x`.")
  }
  if (size_dst_ambiguous != 1L && size_dst_ambiguous != size_days) {
    abort("`dst_ambiguous` must be size 1 or the same size as `x`.")
  }

  convert_days_and_time_of_day_to_seconds_cpp(days, time_of_day, zone, dst_nonexistent, dst_ambiguous)
}
