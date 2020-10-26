library(here)
library(fs)
library(glue)

# Notes about this updater:
#
# The `tz.cpp` file that is downloaded into `src/tz.cpp` will need to be updated
# to pass R CMD Check. All calls to `std::cerr` will have to be commented out.
#
# The rest of the date API is downloaded into `inst/include/date/`
# and is header only.

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
# Update `inst/include/date/`

dir_date_include_date <- path(dir_date, "include", "date")
dir_pkg_inst_include_date <- here("inst", "include", "date")

dir_copy(dir_date_include_date, dir_pkg_inst_include_date, overwrite = TRUE)

# ------------------------------------------------------------------------------
# Update `src/tz.cpp`
# Important - this file will need to be tweaked!

file_date_src_tz <- path(dir_date, "src", "tz.cpp")
file_pkg_src_tz <- path("src", "tz.cpp")

file_copy(file_date_src_tz, file_pkg_src_tz, overwrite = TRUE)

# ------------------------------------------------------------------------------

unlink(dir_temp, recursive = TRUE, force = TRUE)
