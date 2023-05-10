# @export .onLoad()
scale_type.clock_year_week_day <- function(x) {
  c("year_week_day", "continuous")
}

# ------------------------------------------------------------------------------

#' @export
#' @rdname calendar-scales-position
scale_x_year_week_day <- function(...,
                                  name = ggplot2::waiver(),
                                  breaks = ggplot2::waiver(),
                                  date_breaks = ggplot2::waiver(),
                                  minor_breaks = ggplot2::waiver(),
                                  date_minor_breaks = ggplot2::waiver(),
                                  n.breaks = NULL,
                                  labels = ggplot2::waiver(),
                                  limits = NULL,
                                  oob = scales::censor,
                                  expand = ggplot2::waiver(),
                                  guide = ggplot2::waiver(),
                                  position = "bottom") {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  scale <- year_week_day_scale(
    aesthetics = ggplot2_x_aes(),
    palette = identity,
    name = name,
    breaks = breaks,
    date_breaks = date_breaks,
    minor_breaks = minor_breaks,
    date_minor_breaks = date_minor_breaks,
    n.breaks = n.breaks,
    labels = labels,
    limits = limits,
    oob = oob,
    expand = expand,
    guide = guide,
    position = position,
    super = the$ScaleContinuousPositionYearWeekDay
  )

  scale
}

#' @export
#' @rdname calendar-scales-position
scale_y_year_week_day <- function(...,
                                  name = ggplot2::waiver(),
                                  breaks = ggplot2::waiver(),
                                  date_breaks = ggplot2::waiver(),
                                  minor_breaks = ggplot2::waiver(),
                                  date_minor_breaks = ggplot2::waiver(),
                                  n.breaks = NULL,
                                  labels = ggplot2::waiver(),
                                  limits = NULL,
                                  oob = scales::censor,
                                  expand = ggplot2::waiver(),
                                  guide = ggplot2::waiver(),
                                  position = "left") {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  scale <- year_week_day_scale(
    aesthetics = ggplot2_y_aes(),
    palette = identity,
    name = name,
    breaks = breaks,
    date_breaks = date_breaks,
    minor_breaks = minor_breaks,
    date_minor_breaks = date_minor_breaks,
    n.breaks = n.breaks,
    labels = labels,
    limits = limits,
    oob = oob,
    expand = expand,
    guide = guide,
    position = position,
    super = the$ScaleContinuousPositionYearWeekDay
  )

  scale
}

#' @export
#' @rdname calendar-scales-color
scale_colour_year_week_day <- function(...,
                                       low = "#132B43",
                                       high = "#56B1F7",
                                       na.value = "grey50",
                                       guide = "colourbar") {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  year_week_day_scale(
    aesthetics = "colour",
    palette = scales::seq_gradient_pal(low, high),
    na.value = na.value,
    guide = guide
  )
}

#' @export
#' @rdname calendar-scales-color
scale_color_year_week_day <- scale_colour_year_week_day

#' @export
#' @rdname calendar-scales-color
scale_fill_year_week_day <- function(...,
                                     low = "#132B43",
                                     high = "#56B1F7",
                                     na.value = "grey50",
                                     guide = "colourbar") {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  year_week_day_scale(
    aesthetics = "fill",
    palette = scales::seq_gradient_pal(low, high),
    na.value = na.value,
    guide = guide
  )
}

#' @export
#' @rdname calendar-scales-alpha
scale_alpha_year_week_day <- function(..., range = c(0.1, 1)) {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  year_week_day_scale(
    aesthetics = "alpha",
    palette = scales::rescale_pal(range)
  )
}

#' @export
#' @rdname calendar-scales-size
scale_size_year_week_day <- function(..., range = c(1, 6)) {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  year_week_day_scale(
    aesthetics = "size",
    palette = scales::area_pal(range)
  )
}

# ------------------------------------------------------------------------------

year_week_day_scale <- function(aesthetics,
                                palette,
                                ...,
                                name = ggplot2::waiver(),
                                breaks = ggplot2::waiver(),
                                date_breaks = ggplot2::waiver(),
                                minor_breaks = ggplot2::waiver(),
                                date_minor_breaks = ggplot2::waiver(),
                                n.breaks = NULL,
                                labels = ggplot2::waiver(),
                                limits = NULL,
                                oob = scales::censor,
                                expand = ggplot2::waiver(),
                                na.value = NA_real_,
                                guide = ggplot2::waiver(),
                                position = "bottom",
                                super = the$ScaleContinuousYearWeekDay) {
  check_dots_empty0(...)

  if (!is.waive(date_breaks)) {
    breaks <- breaks_duration_year_week_day(date_breaks)
  }
  if (!is.waive(date_minor_breaks)) {
    minor_breaks <- breaks_duration_year_week_day(date_minor_breaks)
  }

  # Insert a fake `trans` that gets updated by the ggproto `$transform()` method
  # which is called before anything else
  trans <- year_week_day_trans(epoch = NULL)

  ggplot2::continuous_scale(
    aesthetics = aesthetics,
    scale_name = "year_week_day",
    palette = palette,
    name = name,
    breaks = breaks,
    minor_breaks = minor_breaks,
    n.breaks = n.breaks,
    labels = labels,
    limits = limits,
    oob = oob,
    expand = expand,
    na.value = na.value,
    trans = trans,
    guide = guide,
    position = position,
    super = super
  )
}

year_week_day_trans <- function(epoch = NULL) {
  # year-week-day transforms are complicated by the fact that year-week-day
  # (correctly) doesn't support math on week precision inputs. But in this
  # case we go ahead and assume the user provided fully valid dates, allowing
  # us to build in week precision math by converting to and from sys-time.
  scales::trans_new(
    name = "year_week_day",
    transform = year_week_day_trans_transform_fn(epoch),
    inverse = year_week_day_trans_inverse_fn(epoch),
    breaks = year_week_day_trans_breaks(epoch),
    domain = calendar_trans_domain(epoch)
  )
}

year_week_day_epoch <- function(precision, start) {
  precision <- precision_to_string(precision)
  calendar_widen(year_week_day(1970, start = start), precision)
}

# ------------------------------------------------------------------------------

year_week_day_trans_transform_fn <- function(epoch) {
  force(epoch)

  function(x) {
    check_trans_epoch(epoch)
    precision <- calendar_precision_attribute(epoch)

    if (precision == PRECISION_YEAR) {
      calendar_trans_transform_fn(epoch)(x)
    } else if (precision == PRECISION_WEEK) {
      as.integer(year_week_day_as_weeks(x, epoch))
    } else {
      abort("Unknown precision.", .internal = TRUE)
    }
  }
}

year_week_day_trans_inverse_fn <- function(epoch) {
  force(epoch)

  function(x) {
    check_trans_epoch(epoch)
    precision <- calendar_precision_attribute(epoch)

    if (precision == PRECISION_YEAR) {
      calendar_trans_inverse_fn(epoch)(x)
    } else if (precision == PRECISION_WEEK) {
      x <- round_limits(x)
      year_week_day_add_weeks(epoch, x)
    } else {
      abort("Unknown precision.", .internal = TRUE)
    }
  }
}

year_week_day_trans_breaks <- function(epoch) {
  force(epoch)

  function(x, n = 5L) {
    check_trans_epoch(epoch)
    precision <- calendar_precision_attribute(epoch)

    if (precision == PRECISION_YEAR) {
      calendar_trans_breaks(x, n = n)
    } else if (precision == PRECISION_WEEK) {
      x <- year_week_day_as_weeks(x, epoch)
      x <- calendar_trans_breaks(x, n = n)
      year_week_day_from_weeks(x, epoch)
    } else {
      abort("Unknown precision.", .internal = TRUE)
    }
  }
}

# ------------------------------------------------------------------------------

# These helpers add support for:
# - Week based math
# - Conversion to and from `duration<week>` based on some epoch
#
# They assume that `x` has no invalid dates.
#
# These should never be exported from clock itself, as they blur the lines
# between calendar and time-points. But they are nice here for plotting
# convenience, as otherwise you wouldn't be able to plot a year-week calendar.

year_week_day_add_weeks <- function(x, n) {
  start <- week_start(x)

  x <- set_day(x, 1)

  # Better be sure about no invalid weeks here! Not safe!
  x <- as_sys_time(x)

  x <- add_weeks(x, n)

  x <- as_year_week_day(x, start = start)

  x <- calendar_narrow(x, "week")

  x
}

year_week_day_as_weeks <- function(x, epoch) {
  x <- set_day(x, 1)
  epoch <- set_day(epoch, 1)

  # Better be sure about no invalid weeks here! Not safe!
  x <- as_sys_time(x)
  epoch <- as_sys_time(epoch)

  out <- x - epoch

  duration_cast(out, "week")
}

year_week_day_from_weeks <- function(x, epoch) {
  start <- week_start(epoch)

  epoch <- set_day(epoch, 1L)
  epoch <- as_sys_time(epoch)

  out <- epoch + x

  out <- as_year_week_day(out, start = start)

  calendar_narrow(out, "week")
}

# ------------------------------------------------------------------------------

breaks_duration_year_week_day <- function(by) {
  force(by)

  function(x) {
    breaks <- breaks_duration(by)

    precision <- calendar_precision_attribute(x)
    start <- week_start(x)
    epoch <- year_week_day_epoch(precision, start)

    x <- year_week_day_as_weeks(x, epoch)
    x <- breaks(x)
    x <- year_week_day_from_weeks(x, epoch)

    x
  }
}

# ------------------------------------------------------------------------------

make_ScaleContinuousYearWeekDay <- function() {
  # Forced to wrap in a generator function so we can keep ggplot2 in Suggests.
  # Initialized on load.
  ggplot2::ggproto(
    "ScaleContinuousYearWeekDay",
    ggplot2::ScaleContinuous,
    epoch = NULL,
    transform = year_week_day_transform
  )
}

make_ScaleContinuousPositionYearWeekDay <- function() {
  # Forced to wrap in a generator function so we can keep ggplot2 in Suggests.
  # Initialized on load.
  ggplot2::ggproto(
    "ScaleContinuousPositionYearWeekDay",
    ggplot2::ScaleContinuousPosition,
    epoch = NULL,
    transform = year_week_day_transform
  )
}

year_week_day_transform <- function(self, x) {
  # Refresh the `$trans` object with the actual data `epoch`.
  # Same trick as `ScaleContinuousDatetime` for the time zone.
  precision <- calendar_precision_attribute(x)
  start <- week_start(x)

  if (precision > PRECISION_WEEK) {
    cli::cli_abort("{.cls year_week_day} inputs can only be {.str year} or {.str week} precision.")
  }

  if (is_null(self$epoch)) {
    self$epoch <- year_week_day_epoch(precision, start)
    self$trans <- year_week_day_trans(self$epoch)
  } else if (calendar_precision_attribute(self$epoch) != precision) {
    cli::cli_abort("All {.cls year_week_day} inputs must have the same precision.")
  } else if (week_start(self$epoch) != start) {
    cli::cli_abort("All {.cls year_week_day} inputs must have the same start.")
  }

  ggplot2::ggproto_parent(ggplot2::ScaleContinuousPosition, self)$transform(x)
}
