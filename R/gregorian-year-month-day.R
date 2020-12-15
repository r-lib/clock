#' @export
year_month_day <- function(year,
                           month = 1L,
                           day = 1L,
                           ...,
                           day_nonexistent = "last-time") {
  check_dots_empty()

  args <- list(year = year, month = month, day = day)
  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  days <- convert_year_month_day_to_calendar_days(
    args$year,
    args$month,
    args$day,
    day_nonexistent
  )

  new_year_month_day(days)
}

#' @export
new_year_month_day <- function(days = integer(), ..., class = NULL) {
  new_gregorian(days, ..., class = c(class, "clock_year_month_day"))
}

#' @export
format.clock_year_month_day <- function(x, ..., format = "%Y-%m-%d") {
  # TODO: Formalize this
  seconds_of_day <- zeros_along(x)
  format_naive_second_point(x, seconds_of_day, format)
}

#' @export
vec_ptype_full.clock_year_month_day <- function(x, ...) {
  "year_month_day"
}

#' @export
vec_ptype_abbr.clock_year_month_day <- function(x, ...) {
  "ymd"
}

#' @export
is_year_month_day <- function(x) {
  inherits(x, "clock_year_month_day")
}
