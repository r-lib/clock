# @export .onLoad()
scale_type.clock_year_month_day <- function(x) {
  c("year_month_day", "continuous")
}

# ------------------------------------------------------------------------------

#' @export
#' @rdname calendar-scales-position
scale_x_year_month_day <- function(...,
                                   name = ggplot2::waiver(),
                                   breaks = ggplot2::waiver(),
                                   date_breaks = ggplot2::waiver(),
                                   minor_breaks = ggplot2::waiver(),
                                   date_minor_breaks = ggplot2::waiver(),
                                   n.breaks = NULL,
                                   labels = ggplot2::waiver(),
                                   date_labels = ggplot2::waiver(),
                                   date_locale = clock_locale(),
                                   limits = NULL,
                                   oob = scales::censor,
                                   expand = ggplot2::waiver(),
                                   guide = ggplot2::waiver(),
                                   position = "bottom") {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  scale <- year_month_day_scale(
    aesthetics = ggplot2_x_aes(),
    palette = identity,
    name = name,
    breaks = breaks,
    date_breaks = date_breaks,
    minor_breaks = minor_breaks,
    date_minor_breaks = date_minor_breaks,
    n.breaks = n.breaks,
    labels = labels,
    date_labels = date_labels,
    date_locale = date_locale,
    limits = limits,
    oob = oob,
    expand = expand,
    guide = guide,
    position = position,
    super = the$ScaleContinuousPositionYearMonthDay
  )

  scale
}

#' @export
#' @rdname calendar-scales-position
scale_y_year_month_day <- function(...,
                                   name = ggplot2::waiver(),
                                   breaks = ggplot2::waiver(),
                                   date_breaks = ggplot2::waiver(),
                                   minor_breaks = ggplot2::waiver(),
                                   date_minor_breaks = ggplot2::waiver(),
                                   n.breaks = NULL,
                                   labels = ggplot2::waiver(),
                                   date_labels = ggplot2::waiver(),
                                   date_locale = clock_locale(),
                                   limits = NULL,
                                   oob = scales::censor,
                                   expand = ggplot2::waiver(),
                                   guide = ggplot2::waiver(),
                                   position = "left") {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  scale <- year_month_day_scale(
    aesthetics = ggplot2_y_aes(),
    palette = identity,
    name = name,
    breaks = breaks,
    date_breaks = date_breaks,
    minor_breaks = minor_breaks,
    date_minor_breaks = date_minor_breaks,
    n.breaks = n.breaks,
    labels = labels,
    date_labels = date_labels,
    date_locale = date_locale,
    limits = limits,
    oob = oob,
    expand = expand,
    guide = guide,
    position = position,
    super = the$ScaleContinuousPositionYearMonthDay
  )

  scale
}

#' @export
#' @rdname calendar-scales-color
scale_colour_year_month_day <- function(...,
                                        low = "#132B43",
                                        high = "#56B1F7",
                                        na.value = "grey50",
                                        guide = "colourbar") {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  year_month_day_scale(
    aesthetics = "colour",
    palette = scales::seq_gradient_pal(low, high),
    na.value = na.value,
    guide = guide
  )
}

#' @export
#' @rdname calendar-scales-color
scale_color_year_month_day <- scale_colour_year_month_day

#' @export
#' @rdname calendar-scales-color
scale_fill_year_month_day <- function(...,
                                      low = "#132B43",
                                      high = "#56B1F7",
                                      na.value = "grey50",
                                      guide = "colourbar") {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  year_month_day_scale(
    aesthetics = "fill",
    palette = scales::seq_gradient_pal(low, high),
    na.value = na.value,
    guide = guide
  )
}

#' @export
#' @rdname calendar-scales-alpha
scale_alpha_year_month_day <- function(..., range = c(0.1, 1)) {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  year_month_day_scale(
    aesthetics = "alpha",
    palette = scales::rescale_pal(range)
  )
}

#' @export
#' @rdname calendar-scales-size
scale_size_year_month_day <- function(..., range = c(1, 6)) {
  check_dots_empty0(...)
  check_installed("scales")
  check_installed("ggplot2")

  year_month_day_scale(
    aesthetics = "size",
    palette = scales::area_pal(range)
  )
}

# ------------------------------------------------------------------------------

year_month_day_scale <- function(aesthetics,
                                 palette,
                                 ...,
                                 name = ggplot2::waiver(),
                                 breaks = ggplot2::waiver(),
                                 date_breaks = ggplot2::waiver(),
                                 minor_breaks = ggplot2::waiver(),
                                 date_minor_breaks = ggplot2::waiver(),
                                 n.breaks = NULL,
                                 labels = ggplot2::waiver(),
                                 date_labels = ggplot2::waiver(),
                                 date_locale = clock_locale(),
                                 limits = NULL,
                                 oob = scales::censor,
                                 expand = ggplot2::waiver(),
                                 na.value = NA_real_,
                                 guide = ggplot2::waiver(),
                                 position = "bottom",
                                 super = the$ScaleContinuousYearMonthDay) {
  check_dots_empty0(...)

  if (!is.waive(date_breaks)) {
    breaks <- breaks_duration(date_breaks)
  }
  if (!is.waive(date_minor_breaks)) {
    minor_breaks <- breaks_duration(date_minor_breaks)
  }
  if (!is.waive(date_labels)) {
    labels <- function(x) {
      year_month_day_trans_format(x, format = date_labels, locale = date_locale)
    }
  }

  # Insert a fake `trans` that gets updated by the ggproto `$transform()` method
  # which is called before anything else
  trans <- year_month_day_trans(epoch = NULL)

  ggplot2::continuous_scale(
    aesthetics = aesthetics,
    scale_name = "year_month_day",
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

year_month_day_trans <- function(epoch = NULL) {
  scales::trans_new(
    name = "year_month_day",
    transform = calendar_trans_transform_fn(epoch),
    inverse = calendar_trans_inverse_fn(epoch),
    breaks = calendar_trans_breaks,
    domain = calendar_trans_domain(epoch),
    format = year_month_day_trans_format
  )
}

year_month_day_epoch <- function(precision) {
  precision <- precision_to_string(precision)
  calendar_widen(year_month_day(1970), precision)
}

# ------------------------------------------------------------------------------

year_month_day_trans_format <- function(x,
                                        format = NULL,
                                        locale = clock_locale()) {
  if (is_null(format)) {
    precision <- calendar_precision_attribute(x)
    format <- year_month_day_trans_format_default(precision)
  }

  # Assumes year or month precision
  x <- calendar_widen(x, "day")
  x <- as_sys_time(x)

  format(x, format = format, locale = locale)
}

year_month_day_trans_format_default <- function(precision) {
  if (precision == PRECISION_YEAR) {
    "%Y"
  } else if (precision == PRECISION_MONTH) {
    "%b %Y"
  } else {
    abort("Unsupported `precision`.", .internal = TRUE)
  }
}

# ------------------------------------------------------------------------------

make_ScaleContinuousYearMonthDay <- function() {
  # Forced to wrap in a generator function so we can keep ggplot2 in Suggests.
  # Initialized on load.
  ggplot2::ggproto(
    "ScaleContinuousYearMonthDay",
    ggplot2::ScaleContinuous,
    epoch = NULL,
    transform = year_month_day_transform
  )
}

make_ScaleContinuousPositionYearMonthDay <- function() {
  # Forced to wrap in a generator function so we can keep ggplot2 in Suggests.
  # Initialized on load.
  ggplot2::ggproto(
    "ScaleContinuousPositionYearMonthDay",
    ggplot2::ScaleContinuousPosition,
    epoch = NULL,
    transform = year_month_day_transform
  )
}

year_month_day_transform <- function(self, x) {
  # Refresh the `$trans` object with the actual data `epoch`.
  # Same trick as `ScaleContinuousDatetime` for the time zone.
  precision <- calendar_precision_attribute(x)

  if (precision > PRECISION_MONTH) {
    cli::cli_abort("{.cls year_month_day} inputs can only be {.str year} or {.str month} precision.")
  }

  if (is.null(self$epoch)) {
    self$epoch <- year_month_day_epoch(precision)
    self$trans <- year_month_day_trans(self$epoch)
  } else if (calendar_precision_attribute(self$epoch) != precision) {
    cli::cli_abort("All {.cls year_month_day} inputs must have the same precision.")
  }

  ggplot2::ggproto_parent(ggplot2::ScaleContinuousPosition, self)$transform(x)
}
