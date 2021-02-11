clock_months <- list(
  january = 1L,
  february = 2L,
  march = 3L,
  april = 4L,
  may = 5L,
  june = 6L,
  july = 7L,
  august = 8L,
  september = 9L,
  october = 10L,
  november = 11L,
  december = 12L
)
clock_months <- as.environment(clock_months)
lockEnvironment(clock_months, bindings = TRUE)
usethis::use_data(clock_months, internal = FALSE, overwrite = TRUE)


clock_weekdays <- list(
  sunday = 1L,
  monday = 2L,
  tuesday = 3L,
  wednesday = 4L,
  thursday = 5L,
  friday = 6L,
  saturday = 7L
)
clock_weekdays <- as.environment(clock_weekdays)
lockEnvironment(clock_weekdays, bindings = TRUE)
usethis::use_data(clock_weekdays, internal = FALSE, overwrite = TRUE)


clock_iso_weekdays <- list(
  monday = 1L,
  tuesday = 2L,
  wednesday = 3L,
  thursday = 4L,
  friday = 5L,
  saturday = 6L,
  sunday = 7L
)
clock_iso_weekdays <- as.environment(clock_iso_weekdays)
lockEnvironment(clock_iso_weekdays, bindings = TRUE)
usethis::use_data(clock_iso_weekdays, internal = FALSE, overwrite = TRUE)
