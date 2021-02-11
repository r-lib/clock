clock_codes <- list(
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
  december = 12L,

  sunday = 1L,
  monday = 2L,
  tuesday = 3L,
  wednesday = 4L,
  thursday = 5L,
  friday = 6L,
  saturday = 7L,

  iso = list(
    monday = 1L,
    tuesday = 2L,
    wednesday = 3L,
    thursday = 4L,
    friday = 5L,
    saturday = 6L,
    sunday = 7L
  )
)

clock_codes <- as.environment(clock_codes)

lockEnvironment(clock_codes, bindings = TRUE)

usethis::use_data(clock_codes, internal = FALSE, overwrite = TRUE)
