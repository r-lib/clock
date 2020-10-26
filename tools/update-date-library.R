library(here)
library(fs)
library(glue)

# Notes about this updater:
#
# The `tz.cpp` file that is downloaded into `src/tz.cpp` will need to be updated
# to pass R CMD Check. All calls to `std::cerr` will have to be commented out.
#
# The rest of the date API is downloaded into `src/date/` and is header only.

# ------------------------------------------------------------------------------
# cd into temp dir
# git clone the repo

dir_temp <- tempdir()
dir_dest <- dir_create(path(dir_temp, "dest/"))

url_repo <- "https://github.com/HowardHinnant/date"

cmd_cd <- glue(shQuote("cd"), " ", dir_dest)
cmd_clone <- glue(shQuote("git"), " clone ", url_repo)
cmd <- glue(cmd_cd, "; ", cmd_clone)

# cd into temp dir
# git clone the repo
system(cmd)

dir_date <- path(dir_dest, "date")

# ------------------------------------------------------------------------------
# Update headers and tz.cpp
# Will overwrite `src/tz.cpp` (which will need to be tweaked)
# and `src/date/`

dir_pkg_src <- here("src")

dir_date_include_date <- path(dir_date, "include", "date")
dir_pkg_src_date <- path(dir_pkg_src, "date")
dir_copy(dir_date_include_date, dir_pkg_src_date, overwrite = TRUE)

file_date_src_tz <- path(dir_date, "src", "tz.cpp")
file_pkg_src_tz <- path(dir_src, "tz.cpp")

file_copy(file_date_src_tz, file_pkg_src_tz, overwrite = TRUE)

# ------------------------------------------------------------------------------

unlink(dir_temp, recursive = TRUE, force = TRUE)
