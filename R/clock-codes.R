#' Integer codes
#'
#' @description
#'
#' A locked environment with useful mappings from month names and weekday names
#' to integer codes.
#'
#' ## Month codes
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
#' ## Weekday codes
#'
#' - `sunday == 1`
#' - `monday == 2`
#' - `tuesday == 3`
#' - `wednesday == 4`
#' - `thursday == 5`
#' - `friday == 6`
#' - `saturday == 7`
#'
#' ## ISO weekday codes
#'
#' In `clock_codes$iso`:
#'
#' - `monday == 1`
#' - `tuesday == 2`
#' - `wednesday == 3`
#' - `thursday == 4`
#' - `friday == 5`
#' - `saturday == 6`
#' - `sunday == 7`
#'
#' @format An environment containing mappings from month names and weekday names
#'   to integer codes.
#'
#' @examples
#' weekday(clock_codes$wednesday)
#'
#' year_month_weekday(2019, clock_codes$april, clock_codes$monday, 1:4)
#'
#' iso_year_week_day(2020, 52, clock_codes$iso$thursday)
"clock_codes"
