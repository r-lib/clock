
clock_init_limits_init <- function(env) {
  assign("clock_year_max", clock_get_year_max(), envir = env)
  assign("clock_year_min", clock_get_year_min(), envir = env)
}
