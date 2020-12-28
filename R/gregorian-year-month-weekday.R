#' @export
year_month_weekday <- function(year, month = 1L, weekday = 1L, index = 1L) {
  if (is_last(index)) {
    index <- -1L
    last <- TRUE
  } else {
    last <- FALSE
  }

  args <- list(year = year, month = month, weekday = weekday, index = index)
  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  fields <- collect_year_month_weekday_fields(
    args$year,
    args$month,
    args$weekday,
    args$index,
    last
  )

  new_year_month_weekday_from_fields(fields)
}

#' @export
new_year_month_weekday <- function(year = integer(),
                                   month = integer(),
                                   weekday = integer(),
                                   index = integer(),
                                   ...,
                                   names = NULL,
                                   class = NULL) {
  if (!is_integer(year)) {
    abort("`year` must be an integer vector.")
  }
  if (!is_integer(month)) {
    abort("`month` must be an integer vector.")
  }
  if (!is_integer(weekday)) {
    abort("`weekday` must be an integer vector.")
  }
  if (!is_integer(index)) {
    abort("`index` must be an integer vector.")
  }

  fields <- list(
    year = year,
    month = month,
    weekday = weekday,
    index = index
  )

  new_gregorian(
    fields = fields,
    ...,
    names = names,
    class = c(class, "clock_year_month_weekday")
  )
}

new_year_month_weekday_from_fields <- function(fields, names = NULL) {
  new_year_month_weekday(fields$year, fields$month, fields$weekday, fields$index, names = names)
}

#' @export
format.clock_year_month_weekday <- function(x, ..., locale = default_date_locale()) {
  if (!is_date_locale(locale)) {
    abort("`locale` must be a date locale.")
  }

  year <- field_year(x)
  month <- field_month(x)
  weekday <- field_weekday(x)
  index <- field_index(x)

  day <- locale$date_names$day

  out <- format_year_month_weekday(
    year,
    month,
    weekday,
    index,
    day
  )

  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.clock_year_month_weekday <- function(x, ...) {
  count <- invalid_count(x)
  paste0("year_month_weekday[invalid=", count, "]")
}

#' @export
vec_ptype_abbr.clock_year_month_weekday <- function(x, ...) {
  count <- invalid_count(x)
  paste0("ymw[i=", count, "]")
}

#' @export
is_year_month_weekday <- function(x) {
  inherits(x, "clock_year_month_weekday")
}

#' @export
calendar_is_complete.clock_year_month_weekday <- function(x) {
  TRUE
}

#' @export
invalid_detect.clock_year_month_weekday <- function(x) {
  invalid_detect_year_month_weekday(field_year(x), field_month(x), field_weekday(x), field_index(x))
}

#' @export
invalid_any.clock_year_month_weekday <- function(x) {
  invalid_any_year_month_weekday(field_year(x), field_month(x), field_weekday(x), field_index(x))
}

#' @export
invalid_count.clock_year_month_weekday <- function(x) {
  invalid_count_year_month_weekday(field_year(x), field_month(x), field_weekday(x), field_index(x))
}

#' @export
invalid_resolve.clock_year_month_weekday <- function(x, invalid = "last-day") {
  if (!invalid_any(x)) {
    return(x)
  }

  fields <- invalid_resolve_year_month_weekday(
    field_year(x),
    field_month(x),
    field_weekday(x),
    field_index(x),
    invalid
  )

  new_year_month_weekday_from_fields(fields, names = names(x))
}
