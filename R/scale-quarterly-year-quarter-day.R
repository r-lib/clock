# @export .onLoad()
scale_type.clock_year_quarter_day <- function(x) {
  c("year_quarter_day", "continuous")
}

# ------------------------------------------------------------------------------

#' @export
#' @rdname calendar-scales-position
scale_x_year_quarter_day <- function(...,
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

  scale <- year_quarter_day_scale(
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
    super = the$ScaleContinuousPositionYearQuarterDay
  )

  scale
}

#' @export
#' @rdname calendar-scales-position
scale_y_year_quarter_day <- function(...,
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

  scale <- year_quarter_day_scale(
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
    super = the$ScaleContinuousPositionYearQuarterDay
  )

  scale
}

#' @export
#' @rdname calendar-scales-color
scale_colour_year_quarter_day <- function(...,
                                          low = "#132B43",
                                          high = "#56B1F7",
                                          na.value = "grey50",
                                          guide = "colourbar") {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  year_quarter_day_scale(
    aesthetics = "colour",
    palette = scales::seq_gradient_pal(low, high),
    na.value = na.value,
    guide = guide
  )
}

#' @export
#' @rdname calendar-scales-color
scale_color_year_quarter_day <- scale_colour_year_quarter_day

#' @export
#' @rdname calendar-scales-color
scale_fill_year_quarter_day <- function(...,
                                        low = "#132B43",
                                        high = "#56B1F7",
                                        na.value = "grey50",
                                        guide = "colourbar") {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  year_quarter_day_scale(
    aesthetics = "fill",
    palette = scales::seq_gradient_pal(low, high),
    na.value = na.value,
    guide = guide
  )
}

#' @export
#' @rdname calendar-scales-alpha
scale_alpha_year_quarter_day <- function(..., range = c(0.1, 1)) {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  year_quarter_day_scale(
    aesthetics = "alpha",
    palette = scales::rescale_pal(range)
  )
}

#' @export
#' @rdname calendar-scales-size
scale_size_year_quarter_day <- function(..., range = c(1, 6)) {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  year_quarter_day_scale(
    aesthetics = "size",
    palette = scales::area_pal(range)
  )
}

# ------------------------------------------------------------------------------

year_quarter_day_scale <- function(aesthetics,
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
                                   super = the$ScaleContinuousYearQuarterDay) {
  check_dots_empty0(...)

  if (!is.waive(date_breaks)) {
    breaks <- breaks_duration(date_breaks)
  }
  if (!is.waive(date_minor_breaks)) {
    minor_breaks <- breaks_duration(date_minor_breaks)
  }

  # Insert a fake `trans` that gets updated by the ggproto `$transform()` method
  # which is called before anything else
  trans <- year_quarter_day_trans(epoch = NULL)

  ggplot2::continuous_scale(
    aesthetics = aesthetics,
    scale_name = "year_quarter_day",
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

year_quarter_day_trans <- function(epoch = NULL) {
  scales::trans_new(
    name = "year_quarter_day",
    transform = calendar_trans_transform_fn(epoch),
    inverse = calendar_trans_inverse_fn(epoch),
    breaks = calendar_trans_breaks,
    domain = calendar_trans_domain(epoch)
  )
}

year_quarter_day_epoch <- function(precision, start) {
  precision <- precision_to_string(precision)
  calendar_widen(year_quarter_day(1970, start = start), precision)
}

# ------------------------------------------------------------------------------

make_ScaleContinuousYearQuarterDay <- function() {
  # Forced to wrap in a generator function so we can keep ggplot2 in Suggests.
  # Initialized on load.
  ggplot2::ggproto(
    "ScaleContinuousYearQuarterDay",
    ggplot2::ScaleContinuous,
    epoch = NULL,
    transform = year_quarter_day_transform
  )
}

make_ScaleContinuousPositionYearQuarterDay <- function() {
  # Forced to wrap in a generator function so we can keep ggplot2 in Suggests.
  # Initialized on load.
  ggplot2::ggproto(
    "ScaleContinuousPositionYearQuarterDay",
    ggplot2::ScaleContinuousPosition,
    epoch = NULL,
    transform = year_quarter_day_transform
  )
}

year_quarter_day_transform <- function(self, x) {
  # Refresh the `$trans` object with the actual data `epoch`.
  # Same trick as `ScaleContinuousDatetime` for the time zone.
  precision <- calendar_precision_attribute(x)
  start <- quarterly_start(x)

  if (precision > PRECISION_QUARTER) {
    cli::cli_abort("{.cls year_quarter_day} inputs can only be {.str year} or {.str quarter} precision.")
  }

  if (is_null(self$epoch)) {
    self$epoch <- year_quarter_day_epoch(precision, start)
    self$trans <- year_quarter_day_trans(self$epoch)
  } else if (calendar_precision_attribute(self$epoch) != precision) {
    cli::cli_abort("All {.cls year_quarter_day} inputs must have the same precision.")
  } else if (quarterly_start(self$epoch) != start) {
    cli::cli_abort("All {.cls year_quarter_day} inputs must have the same start.")
  }

  ggplot2::ggproto_parent(ggplot2::ScaleContinuousPosition, self)$transform(x)
}
