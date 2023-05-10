#' Position scales: calendar
#'
#' @description
#' `r lifecycle::badge("experimental")`
#'
#' Position scales for use with ggplot2.
#'
#' - `scale_x_year_month_day()` and `scale_y_year_month_day()` are only valid on
#'   year and month precision inputs.
#'
#' - `scale_x_year_quarter_day()` and `scale_y_year_quarter_day()` are only
#'   valid on year and quarter precision inputs.
#'
#' - `scale_x_year_week_day()` and `scale_y_year_week_day()` are only
#'   valid on year and week precision inputs.
#'
#' For day precision and finer, we currently still recommend using Date and
#' POSIXct with [ggplot2::scale_x_date()] and [ggplot2::scale_x_datetime()].
#'
#' @inheritParams rlang::args_dots_empty
#' @inheritParams ggplot2::scale_x_continuous
#'
#' @param breaks One of:
#'
#'   - `NULL` for no breaks.
#'
#'   - `ggplot2::waiver()` for default breaks, or to use breaks specified by
#'     `date_breaks`.
#'
#'   - A calendar type of the same type as the input giving positions of breaks.
#'
#'   - A function that takes the limits as input and returns breaks as output.
#'
#' @param date_breaks A single duration object giving the distance between the
#'   breaks, like `duration_months(1)` or `duration_weeks(2)`. If both `breaks`
#'   and `date_breaks` are specified, `date_breaks` wins.
#'
#' @param minor_breaks Same as `breaks`, but applied to minor breaks.
#'
#' @param date_minor_breaks Same as `date_breaks`, but applied to minor breaks.
#'
#' @param date_labels For year-month-day only, a string giving the formatting
#'   specification for the labels, such as `"%B %Y"`. The full list of format
#'   tokens is available at [format.clock_zoned_time()]. Note that you should
#'   only use month and year specific codes.
#'
#' @param date_locale For year-month-day only, the locale used when
#'   `date_labels` is also specified.
#'
#' @name calendar-scales-position
#'
#' @examplesIf rlang::is_installed("ggplot2") && rlang::is_installed("scales")
#' library(ggplot2)
#' library(vctrs)
#'
#' # ---------------------------------------------------------------------------
#' # Monthly data
#'
#' set.seed(1234)
#'
#' from <- year_month_day(2019, 1)
#'
#' df <- vec_rbind(
#'   data_frame(
#'     g = "stock 1",
#'     date = from + duration_months(cumsum(sample(1:2, size = 100, replace = TRUE))),
#'     price = cumsum(1 + rnorm(100))
#'   ),
#'   data_frame(
#'     g = "stock 2",
#'     date = from + duration_months(cumsum(sample(1:2, size = 100, replace = TRUE))),
#'     price = cumsum(1 + rnorm(100))
#'   )
#' )
#'
#' # Defaults automatically know you have monthly data
#' ggplot(df, aes(date, price, group = g, color = g)) +
#'   geom_line()
#'
#' # Fully customize as needed
#' ggplot(df, aes(date, price, group = g, color = g)) +
#'   geom_line() +
#'   scale_x_year_month_day(
#'     date_breaks = duration_months(24),
#'     date_minor_breaks = duration_months(6),
#'     date_labels = "%Y"
#'   )
#'
#' ggplot(df, aes(date, price, group = g, color = g)) +
#'   geom_line() +
#'   scale_x_year_month_day(
#'     date_labels = "%B\n%Y",
#'     date_locale = clock_locale("fr")
#'   )
#'
#' # ---------------------------------------------------------------------------
#' # Quarterly data
#'
#' set.seed(1234)
#'
#' from1 <- year_quarter_day(2019, 1)
#' from2 <- year_quarter_day(2000, 2)
#'
#' df <- vec_rbind(
#'   data_frame(
#'     g = "stock 1",
#'     date = from1 + duration_quarters(cumsum(sample(1:5, size = 50, replace = TRUE))),
#'     price = cumsum(1 + rnorm(50))
#'   ),
#'   data_frame(
#'     g = "stock 2",
#'     date = from2 + duration_quarters(cumsum(sample(1:5, size = 50, replace = TRUE))),
#'     price = cumsum(1 + rnorm(50))
#'   )
#' )
#'
#' ggplot(df, aes(date, price, group = g, color = g)) +
#'   geom_line()
#'
#' # Zooming with `coord_cartesian()`
#' ggplot(df, aes(date, price, group = g, color = g)) +
#'   geom_line() +
#'   coord_cartesian(xlim = year_quarter_day(c(2020, 2040), 1))
#'
#' # ---------------------------------------------------------------------------
#' # Weekly data
#'
#' set.seed(123)
#'
#' # A monday
#' x <- naive_time_parse("2018-12-31", precision = "day")
#' x <- x + duration_weeks(sort(sample(100, size = 50)))
#'
#' # ISO calendar
#' x <- as_year_week_day(x, start = clock_weekdays$monday)
#' x <- calendar_narrow(x, "week")
#'
#' df <- data_frame(
#'   date = x,
#'   value = cumsum(rnorm(50, mean = .2))
#' )
#'
#' ggplot(df, aes(date, value)) +
#'   geom_line() +
#'   scale_x_year_week_day()
#'
#' ggplot(df, aes(date, value)) +
#'   geom_line() +
#'   scale_x_year_week_day(
#'     date_breaks = duration_weeks(16),
#'     date_minor_breaks = duration_weeks(2)
#'   )
NULL

#' Gradient color scales: calendar
#'
#' @description
#' `r lifecycle::badge("experimental")`
#'
#' Gradient color scales for use with ggplot2.
#'
#' - `scale_color_year_month_day()` and `scale_fill_year_month_day()` are only
#'   valid on year and month precision inputs.
#'
#' - `scale_color_year_quarter_day()` and `scale_fill_year_quarter_day()` are
#'   only valid on year and quarter precision inputs.
#'
#' - `scale_color_year_week_day()` and `scale_fill_year_week_day()` are only
#'   valid on year and week precision inputs.
#'
#' For day precision and finer, we currently still recommend using Date and
#' POSIXct with [ggplot2::scale_color_date()] and
#' [ggplot2::scale_color_datetime()].
#'
#' @inheritParams rlang::args_dots_empty
#' @inheritParams ggplot2::scale_color_date
#'
#' @name calendar-scales-color
#'
#' @examplesIf rlang::is_installed("ggplot2") && rlang::is_installed("scales")
#' library(ggplot2)
#' library(vctrs)
#'
#' # ---------------------------------------------------------------------------
#' # Color
#'
#' set.seed(123)
#'
#' date <- year_quarter_day(2019, 1) + duration_quarters(1:100)
#' height <- ifelse(
#'   date < year_quarter_day(2033, 1),
#'   sample(30:50, size = 50, replace = TRUE),
#'   sample(40:60, size = 50, replace = TRUE)
#' )
#' weight <- ifelse(
#'   date < year_quarter_day(2028, 1),
#'   sample(100:150, size = 50, replace = TRUE),
#'   sample(100:180, size = 50, replace = TRUE)
#' )
#'
#' df <- data_frame(
#'   date = date,
#'   height = height,
#'   weight = weight
#' )
#'
#' ggplot(df, aes(height, weight, color = date)) +
#'   geom_point() +
#'   scale_colour_year_quarter_day(
#'     low = "red",
#'     high = "blue"
#'   )
#'
#' # ---------------------------------------------------------------------------
#' # Fill
#'
#' economics$date <- as_year_month_day(economics$date)
#' economics$date <- calendar_narrow(economics$date, "month")
#'
#' ggplot(economics, aes(x = date, y = unemploy, fill = date)) +
#'   geom_col()
NULL

#' Alpha transparency scales: calendar
#'
#' @description
#' `r lifecycle::badge("experimental")`
#'
#' Alpha transparency scales for use with ggplot2.
#'
#' - `scale_alpha_year_month_day()` is only valid on year and month precision
#'   inputs.
#'
#' - `scale_alpha_year_quarter_day()` is only valid on year and quarter
#'   precision inputs.
#'
#' - `scale_alpha_year_week_day()` is only valid on year and week precision
#'   inputs.
#'
#' For day precision and finer, we currently still recommend using Date and
#' POSIXct with [ggplot2::scale_alpha_date()] and
#' [ggplot2::scale_alpha_datetime()].
#'
#' @inheritParams rlang::args_dots_empty
#' @inheritParams ggplot2::scale_alpha_date
#'
#' @name calendar-scales-alpha
#'
#' @examplesIf rlang::is_installed("ggplot2") && rlang::is_installed("scales")
#' library(ggplot2)
#' library(vctrs)
#'
#' set.seed(123)
#'
#' date <- year_quarter_day(2019, 1) + duration_quarters(1:100)
#' height <- ifelse(
#'   date < year_quarter_day(2033, 1),
#'   sample(30:50, size = 50, replace = TRUE),
#'   sample(40:60, size = 50, replace = TRUE)
#' )
#' weight <- ifelse(
#'   date < year_quarter_day(2028, 1),
#'   sample(100:150, size = 50, replace = TRUE),
#'   sample(100:180, size = 50, replace = TRUE)
#' )
#'
#' df <- data_frame(
#'   date = date,
#'   height = height,
#'   weight = weight
#' )
#'
#' ggplot(df, aes(height, weight, alpha = date)) +
#'   geom_point()
NULL

#' Size scales: calendar
#'
#' @description
#' `r lifecycle::badge("experimental")`
#'
#' Size scales for use with ggplot2.
#'
#' - `scale_size_year_month_day()` is only valid on year and month precision
#'   inputs.
#'
#' - `scale_size_year_quarter_day()` is only valid on year and quarter
#'   precision inputs.
#'
#' - `scale_size_year_week_day()` is only valid on year and week precision
#'   inputs.
#'
#' For day precision and finer, we currently still recommend using Date and
#' POSIXct with [ggplot2::scale_size_date()] and
#' [ggplot2::scale_size_datetime()].
#'
#' @inheritParams rlang::args_dots_empty
#' @inheritParams ggplot2::scale_size_date
#'
#' @name calendar-scales-size
#'
#' @examplesIf rlang::is_installed("ggplot2") && rlang::is_installed("scales")
#' library(ggplot2)
#' library(vctrs)
#'
#' set.seed(123)
#'
#' date <- year_quarter_day(2019, 1) + duration_quarters(1:100)
#' height <- ifelse(
#'   date < year_quarter_day(2033, 1),
#'   sample(30:50, size = 50, replace = TRUE),
#'   sample(40:60, size = 50, replace = TRUE)
#' )
#' weight <- ifelse(
#'   date < year_quarter_day(2028, 1),
#'   sample(100:150, size = 50, replace = TRUE),
#'   sample(100:180, size = 50, replace = TRUE)
#' )
#'
#' df <- data_frame(
#'   date = date,
#'   height = height,
#'   weight = weight
#' )
#'
#' ggplot(df, aes(height, weight, size = date)) +
#'   geom_point() +
#'   scale_size_year_quarter_day(range = c(1, 10))
NULL
