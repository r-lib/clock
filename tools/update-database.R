library(here)
library(fs)

# Notes about this updater:
#
# Don't forget to update the version below.
#
# This updater will download all the required information and will place it
# in the right place. The next few paragraphs describe what it downloads in
# case you have to do it manually for some reason.
#
# The time zone database is downloaded from: https://www.iana.org/time-zones.
# If you are going to do it manually for some reason, choose the latest
# version, and grab the one with JUST the data, in the format like
# `tzdata2019c.tar.gz`. Uncompress it, and then take the contents of the
# folder and move them into `inst/tzdata/`.
#
# On Windows, we also need a `windowsZone.xml` mapping file.
# It can be found here if you need to download it manually:
# https://raw.githubusercontent.com/unicode-org/cldr/master/common/supplemental/windowsZones.xml.
# Download this file and place it in `inst/tzdata` as `windowsZones.xml`.

# Update the version!
version <- "2021a"

# ------------------------------------------------------------------------------
# Download the time zone database

file <- paste0("tzdata", version, ".tar.gz")

path_base <- "https://data.iana.org/time-zones/releases/"
path_version <- path(path_base, file)

dir_temp <- tempdir()
path_temp <- tempfile(fileext = ".tar.gz", tmpdir = dir_temp)

download.file(path_version, path_temp)

untar(path_temp, exdir = here("inst", "tzdata"))

unlink(path_temp)
unlink(dir_temp, recursive = TRUE, force = TRUE)

# ------------------------------------------------------------------------------
# Download the Windows mapping file

windows_mapping_url <- "https://raw.githubusercontent.com/unicode-org/cldr/master/common/supplemental/windowsZones.xml"
path_dest_windows_mapping <- here("inst/tzdata/windowsZones.xml")

download.file(windows_mapping_url, path_dest_windows_mapping)
