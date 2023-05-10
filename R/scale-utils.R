check_trans_epoch <- function(epoch) {
  if (!is_null(epoch)) {
    return(invisible(NULL))
  }
  abort("`epoch` can't be `NULL` at this time.", .internal = TRUE)
}

# ------------------------------------------------------------------------------

calendar_trans_transform_fn <- function(epoch) {
  force(epoch)

  function(x) {
    check_trans_epoch(epoch)
    # `x` is original units, output is integer
    out <- as.integer(x - epoch)
    out
  }
}

calendar_trans_inverse_fn <- function(epoch) {
  force(epoch)

  function(x) {
    check_trans_epoch(epoch)
    precision <- calendar_precision_attribute(epoch)

    x <- round_limits(x)

    # `x` is integer / double, output is original units
    out <- duration_helper(x, precision) + epoch

    out
  }
}

round_limits <- function(x) {
  if (is.double(x) && length(x) == 2L && any(x != floor(x), na.rm = TRUE)) {
    # This handles when limits are expanded by a multiplicative factor.
    # For continuous scales a 5% expansion is applied by default.
    # Limits of `c(NA_real_, NA_real_)` are sometimes passed through here so
    # the `na.rm = TRUE` is required.
    # AFAIK this isn't triggered at any other time.
    c(floor(x[1L]), ceiling(x[2L]))
  } else {
    x
  }
}

calendar_trans_breaks <- function(x, n = 5) {
  from <- x[[1L]]
  to <- x[[2L]]
  seq_expanded_n(from, to, n = n)
}

calendar_trans_domain <- function(epoch) {
  if (is_null(epoch)) {
    NULL
  } else {
    vec_c(clock_minimum(epoch), clock_maximum(epoch))
  }
}

# ------------------------------------------------------------------------------

breaks_duration <- function(by) {
  force(by)

  function(x) {
    from <- x[[1L]]
    to <- x[[2L]]
    seq_expanded(from, to, by)
  }
}

seq_expanded_n <- function(from, to, ..., n = 5L) {
  # Constructs an equally spaced sequence that is roughly along `from -> to`.
  # If it can't create an equally spaced sequence of `n` pieces, it expands
  # `to` until it can. Keeping `from` the same is important because it ensure
  # that major and minor breaks stay aligned.

  check_dots_empty0(...)
  check_number_whole(n)

  # Avoid problematic `n` values
  n <- pmax(n, 2L)

  # Division is based on number of ranges, not number of breaks
  # i.e. [2, 10), [10, 18) is 2 ranges, 3 breaks
  n <- n - 1L

  difference <- to - from
  precision <- duration_precision_attribute(difference)

  if (difference < duration_helper(n, precision)) {
    # Force `by` to at least be `1`
    difference <- duration_helper(n, precision)
  }

  # If there is any remainder, i.e. `difference %% n != 0L`, then that will
  # be handled by `seq_expanded()` to ensure that the sequence covers the full
  # range
  by <- difference %/% n

  seq_expanded(from, to, by)
}

seq_expanded <- function(from, to, by) {
  # Figure out how much to add to `to` to ensure that the sequence generates a
  # "full" sequence. We always add to `to` so that major and minor breaks are
  # aligned on `from` so that you can use a multiple of `date_minor_breaks` as
  # the `date_breaks` value and get a sensible result, like:
  # `date_breaks = duration_months(24), date_minor_breaks = duration_months(6)`
  difference <- (to - from)

  precision <- duration_precision_attribute(difference)

  # `by` is always cast to the precision of the inputs!
  by <- duration_collect_by(by, precision)

  # How far "over" `by` are we?
  over <- difference %% by

  if (over > duration_helper(0L, precision)) {
    # How much do we need to divide between `from` and `to` to get a regular seq?
    extra <- by - over
    to <- to + extra
  }

  seq(from, to = to, by = by)
}

# ------------------------------------------------------------------------------

ggplot2_x_aes <- function() {
  # ggplot2:::ggplot_global$x_aes
  c(
    "x",
    "xmin",
    "xmax",
    "xend",
    "xintercept",
    "xmin_final",
    "xmax_final",
    "xlower",
    "xmiddle",
    "xupper",
    "x0"
  )
}

ggplot2_y_aes <- function() {
  # ggplot2:::ggplot_global$y_aes
  c(
    "y",
    "ymin",
    "ymax",
    "yend",
    "yintercept",
    "ymin_final",
    "ymax_final",
    "lower",
    "middle",
    "upper",
    "y0"
  )
}

# ------------------------------------------------------------------------------

is.waive <- function(x) {
  # ggplot2:::is.waive()
  inherits(x, "waiver")
}
