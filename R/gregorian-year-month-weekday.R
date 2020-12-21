#' @export
year_month_weekday <- function(year,
                               month = 1L,
                               weekday = 1L,
                               index = 1L,
                               ...,
                               day_nonexistent = "last-time") {
  check_dots_empty()

  args <- list(year = year, month = month, weekday = weekday, index = index)
  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  days <- convert_year_month_weekday_index_to_calendar_days(
    args$year,
    args$month,
    args$weekday,
    args$index,
    day_nonexistent
  )

  new_year_month_weekday(days)
}

#' @export
new_year_month_weekday <- function(days = integer(), ..., class = NULL) {
  new_gregorian(days, ..., class = c(class, "clock_year_month_weekday"))
}

#' @export
format.clock_year_month_weekday <- function(x, ..., format = NULL, locale = default_date_locale()) {
  if (!is.null(format)) {
    out <- format_calendar_days(x, format, locale)
    return(out)
  }

  format <- "%Y-%m-%A"
  out <- format_calendar_days(x, format, locale)

  fields <- convert_calendar_days_to_year_month_weekday_index(x)
  index <- fields$index

  out <- paste0(out, "[", index, "]")

  out[is.na(x)] <- NA_character_
  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.clock_year_month_weekday <- function(x, ...) {
  "year_month_weekday"
}

#' @export
vec_ptype_abbr.clock_year_month_weekday <- function(x, ...) {
  "ymw"
}

#' @export
is_year_month_weekday <- function(x) {
  inherits(x, "clock_year_month_weekday")
}

weekday_map <- c(
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
)
