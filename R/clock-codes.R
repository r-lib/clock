#' Integer codes
#'
#' @description
#'
#' Objects with useful mappings from month names and weekday names
#' to integer codes.
#'
#' ## Month codes (`clock_months`)
#'
#' - `january == 1`
#' - `february == 2`
#' - `march == 3`
#' - `april == 4`
#' - `may == 5`
#' - `june == 6`
#' - `july == 7`
#' - `august == 8`
#' - `september == 9`
#' - `october == 10`
#' - `november == 11`
#' - `december == 12`
#'
#' ## Weekday codes (`clock_weekdays`)
#'
#' - `sunday == 1`
#' - `monday == 2`
#' - `tuesday == 3`
#' - `wednesday == 4`
#' - `thursday == 5`
#' - `friday == 6`
#' - `saturday == 7`
#'
#' ## ISO weekday codes (`clock_iso_weekdays`)
#'
#' - `monday == 1`
#' - `tuesday == 2`
#' - `wednesday == 3`
#' - `thursday == 4`
#' - `friday == 5`
#' - `saturday == 6`
#' - `sunday == 7`
#'
#' @name clock-codes
#'
#' @examples
#' weekday(clock_weekdays$wednesday)
#'
#' year_month_weekday(2019, clock_months$april, clock_weekdays$monday, 1:4)
#'
#' year_week_day(2020, 52, start = clock_weekdays$monday)
#'
#' iso_year_week_day(2020, 52, clock_iso_weekdays$thursday)
NULL

#' @format - `clock_months`: An environment containing month codes.
#' @rdname clock-codes
"clock_months"

#' @format - `clock_weekdays`: An environment containing weekday codes.
#' @rdname clock-codes
"clock_weekdays"

#' @format - `clock_iso_weekdays`: An environment containing ISO weekday codes.
#' @rdname clock-codes
"clock_iso_weekdays"
