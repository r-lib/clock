library(stringi)

locs <- stri_locale_list()
base <- unique(stri_split_fixed(locs, "_", n = 2, simplify = TRUE)[, 1])

locale_info <- function(x) {

  full <- stri_datetime_symbols(x, context = "format", width = "wide")
  abbr <- stri_datetime_symbols(x, context = "format", width = "abbreviated")

  clock_mapping(
    mon = full$Month,
    mon_ab = abbr$Month,
    day = full$Weekday,
    day_ab = abbr$Weekday,
    am_pm = full$AmPm
  )
}

clock_mappings <- lapply(base, locale_info)
names(clock_mappings) <- base

usethis::use_data(clock_mappings, internal = TRUE, overwrite = TRUE)
