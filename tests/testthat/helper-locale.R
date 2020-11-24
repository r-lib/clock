local_c_locale <- function(env = parent.frame()) {
  skip_if_not_installed("withr")

  withr::local_locale(
    .new = list(LC_TIME = "C"),
    .local_envir = env
  )
}
