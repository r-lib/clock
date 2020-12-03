#' @export
fiscal_year_quarter_day <- function(year,
                                    quarter = 1L,
                                    day = 1L,
                                    ...,
                                    fiscal_start = 1L,
                                    day_nonexistent = "last-time") {
  check_dots_empty()

  args <- list(year = year, quarter = quarter, day = day)
  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  fiscal_start <- cast_fiscal_start(fiscal_start)

  days <- convert_fiscal_year_quarter_day_to_local_days(
    year = args$year,
    quarter = args$quarter,
    day = args$day,
    fiscal_start = fiscal_start,
    day_nonexistent = day_nonexistent
  )

  new_fiscal_year_quarter_day(days, fiscal_start)
}

new_fiscal_year_quarter_day <- function(days = integer(),
                                        fiscal_start = 1L,
                                        ...,
                                        names = NULL) {
  if (!is_integer(days)) {
    abort("`days` must be an integer.")
  }
  if (!is_number(fiscal_start)) {
    abort("`fiscal_start` must be a single number.")
  }

  fields <- list(
    days = days
  )

  new_naive_fiscal(
    fields,
    fiscal_start = fiscal_start,
    ...,
    names = names,
    class = "civil_naive_fiscal_year_quarter_day"
  )
}

new_fiscal_year_quarter_day_from_fields <- function(fields, fiscal_start, names = NULL) {
  new_fiscal_year_quarter_day(
    days = fields$days,
    fiscal_start = fiscal_start,
    names = names
  )
}

#' @export
vec_proxy.civil_naive_fiscal_year_quarter_day <- function(x, ...) {
  proxy_civil_rcrd(x)
}

#' @export
vec_restore.civil_naive_fiscal_year_quarter_day <- function(x, to, ...) {
  fields <- restore_civil_rcrd_fields(x)
  names <- restore_civil_rcrd_names(x)
  fiscal_start <- get_fiscal_start(to)
  new_fiscal_year_quarter_day_from_fields(fields, fiscal_start, names)
}

#' @export
vec_proxy_equal.civil_naive_fiscal_year_quarter_day <- function(x, ...) {
  proxy_equal_civil_rcrd(x)
}

#' @export
format.civil_naive_fiscal_year_quarter_day <- function(x, ...) {
  # TODO: Support format = fmt_fiscal_year_quarter_day() and the "%q" token and
  # pass directly to a formatter
  days <- field(x, "days")
  fiscal_start <- get_fiscal_start(x)

  fields <- convert_local_days_to_fiscal_year_quarter_day(days, fiscal_start)
  year <- fields$year
  quarter <- fields$quarter
  day <- fields$day

  year <- sprintf("%04i", year)
  quarter <- sprintf("%i", quarter)
  day <- sprintf("%02i", day)

  out <- paste0(year, "-Q", quarter, "-", day)

  out[is.na(x)] <- NA_character_
  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.civil_naive_fiscal_year_quarter_day <- function(x, ...) {
  fiscal_start <- get_fiscal_start(x)
  fiscal_start <- pretty_fiscal_start(fiscal_start)
  paste0("fiscal_year_quarter_day<", fiscal_start, ">")
}

#' @export
vec_ptype_abbr.civil_naive_fiscal_year_quarter_day <- function(x, ...) {
  fiscal_start <- get_fiscal_start(x)
  fiscal_start <- pretty_fiscal_start(fiscal_start, abbreviate = TRUE)
  paste0("fiscal_yqd<", fiscal_start, ">")
}

is_fiscal_year_quarter_day <- function(x) {
  inherits(x, "civil_naive_fiscal_year_quarter_day")
}
