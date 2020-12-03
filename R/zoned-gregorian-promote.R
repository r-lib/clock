
LEVEL_ZONED_GREGORIAN_UTC_DATE = 0L
LEVEL_ZONED_GREGORIAN_POSIXLT = 1L
LEVEL_ZONED_GREGORIAN_POSIXCT = 2L
LEVEL_ZONED_GREGORIAN_DATETIME = 3L
LEVEL_ZONED_GREGORIAN_NANO_DATETIME = 4L

zoned_gregorian_level <- function(x) {
  if (is_Date(x)) {
    LEVEL_ZONED_GREGORIAN_UTC_DATE
  } else if (is_POSIXlt(x)) {
    LEVEL_ZONED_GREGORIAN_POSIXLT
  } else if (is_POSIXct(x)) {
    LEVEL_ZONED_GREGORIAN_POSIXCT
  } else if (is_zoned_datetime(x)) {
    LEVEL_ZONED_GREGORIAN_DATETIME
  } else if (is_zoned_nano_datetime(x)) {
    LEVEL_ZONED_GREGORIAN_NANO_DATETIME
  } else {
    stop_civil_unsupported_class(x)
  }
}

# ------------------------------------------------------------------------------

# Note: Promotes POSIXlt to POSIXct

promote_at_least_posixct <- function(x) {
  if (is_at_least_posixct(x)) {
    x
  } else {
    to_posixct(x)
  }
}

is_at_least_posixct <- function(x) {
  zoned_gregorian_level(x) >= LEVEL_ZONED_GREGORIAN_POSIXCT
}

# ------------------------------------------------------------------------------

promote_at_least_zoned_datetime <- function(x) {
  if (is_at_least_zoned_datetime(x)) {
    x
  } else {
    as_zoned_datetime(x)
  }
}

is_at_least_zoned_datetime <- function(x) {
  zoned_gregorian_level(x) >= LEVEL_ZONED_GREGORIAN_DATETIME
}

# ------------------------------------------------------------------------------

promote_at_least_zoned_nano_datetime <- function(x) {
  if (is_at_least_zoned_nano_datetime(x)) {
    x
  } else {
    as_zoned_nano_datetime(x)
  }
}

is_at_least_zoned_nano_datetime <- function(x) {
  zoned_gregorian_level(x) >= LEVEL_ZONED_GREGORIAN_NANO_DATETIME
}
