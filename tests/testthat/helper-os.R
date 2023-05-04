skip_if_not_on_os <- function(os) {
  os <- match.arg(os, choices = c("windows", "mac", "linux", "solaris"))
  system <- system_os()

  message <- switch(
    os,
    windows = if (system != "windows") "Not on Windows",
    mac = if (system != "darwin") "Not on Mac",
    linux = if (system != "linux") "Not on Linux",
    solaris = if (system != "sunos") "Not on Solaris"
  )

  if (is.null(message)) {
    invisible(TRUE)
  } else {
    skip(message)
  }
}

# `testthat:::system_os()`
system_os <- function() {
  tolower(Sys.info()[["sysname"]])
}
