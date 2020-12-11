floor_days_to_year_month_precision <- function(days) {
  floor_days_to_year_month_precision_cpp(days)
}

floor_days_to_year_quarter_precision <- function(days, fiscal_start) {
  floor_days_to_year_quarter_precision_cpp(days, fiscal_start)
}

floor_days_to_iso_year_weeknum_precision <- function(days) {
  floor_days_to_iso_year_weeknum_precision_cpp(days)
}
