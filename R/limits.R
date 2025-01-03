# Internal generic
clock_minimum <- function(x) {
  UseMethod("clock_minimum")
}

# Internal generic
clock_maximum <- function(x) {
  UseMethod("clock_maximum")
}

clock_init_limits_init <- function(env) {
  assign(
    "clock_calendar_year_maximum",
    clock_get_calendar_year_maximum(),
    envir = env
  )
  assign(
    "clock_calendar_year_minimum",
    clock_get_calendar_year_minimum(),
    envir = env
  )
}
