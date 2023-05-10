skip_if_not_installed("ggplot2")
skip_if_not_installed("vdiffr")

# test_that("works with 0 data points", {
#   df <- data_frame(
#     x = year_week_day(integer(), integer()),
#     y = double()
#   )
#
#   p <- ggplot2::ggplot(df, ggplot2::aes(x, y)) +
#     ggplot2::geom_line()
#
#   # Fails because `ggplot2:::ScalesList$transform_df()` early exists on
#   # empty data frames rather than training with them
#   expect_no_error(ggplot2::ggplot_build(p))
# })

test_that("works with 1 data point", {
  df <- data_frame(
    x = year_week_day(2019, 1),
    y = 1
  )

  p <- ggplot2::ggplot(df, ggplot2::aes(x, y)) +
    ggplot2::geom_point()

  vdiffr::expect_doppelganger("1 data point", {
    p
  })
})

test_that("works with a vector of data", {
  df <- data_frame(
    x = year_week_day(2019, 1:5),
    y = 1:5
  )

  p <- ggplot2::ggplot(df, ggplot2::aes(x, y)) +
    ggplot2::geom_line()

  vdiffr::expect_doppelganger("vector", {
    p
  })
})

test_that("works with changing the `date_breaks`", {
  df <- data_frame(
    x = year_week_day_add_weeks(year_week_day(2019, 1), 1:20),
    y = 1:20
  )

  # Note that minor breaks should look nice if you use a multiple
  # of them as the major breaks
  p <- ggplot2::ggplot(df, ggplot2::aes(x, y)) +
    ggplot2::geom_line() +
    scale_x_year_week_day(
      date_breaks = duration_weeks(4),
      date_minor_breaks = duration_weeks(2)
    )

  vdiffr::expect_doppelganger("date_breaks", {
    p
  })
})

test_that("can't use invalid `date_breaks` or `date_minor_breaks`", {
  df <- data_frame(
    x = year_week_day(2019, 1:5),
    y = 1:5
  )

  p <- ggplot2::ggplot(df, ggplot2::aes(x, y)) +
    ggplot2::geom_line() +
    scale_x_year_week_day(date_breaks = duration_days(1))

  expect_snapshot(error = TRUE, {
    ggplot2::ggplot_build(p)
  })

  p <- ggplot2::ggplot(df, ggplot2::aes(x, y)) +
    ggplot2::geom_line() +
    scale_x_year_week_day(date_minor_breaks = duration_days(1))

  expect_snapshot(error = TRUE, {
    ggplot2::ggplot_build(p)
  })
})

test_that("can't use with unsupported precision", {
  df <- data_frame(
    x = year_week_day(2019, 1, 2),
    y = 1
  )

  p <- ggplot2::ggplot(df, ggplot2::aes(x, y)) +
    ggplot2::geom_line()

  expect_snapshot(error = TRUE, {
    ggplot2::ggplot_build(p)
  })
})

test_that("can't mix precisions", {
  df <- data_frame(
    x = year_week_day(2019, 1),
    y = 1
  )
  df2 <- data_frame(
    x = year_week_day(2019),
    y = 1
  )

  p <- ggplot2::ggplot() +
    ggplot2::geom_line(data = df, mapping = ggplot2::aes(x, y)) +
    ggplot2::geom_line(data = df2, mapping = ggplot2::aes(x, y))

  expect_snapshot(error = TRUE, {
    ggplot2::ggplot_build(p)
  })
})

test_that("works with year precision", {
  df <- data_frame(
    x = year_week_day(2020:2030),
    y = 1:11
  )

  p <- ggplot2::ggplot(df, ggplot2::aes(x, y)) +
    ggplot2::geom_line()

  vdiffr::expect_doppelganger("year precision", {
    p
  })
})

test_that("less common scales have basic support", {
  date <- year_week_day(2019, 1:5)
  height <- c(10, 12, 15, 12, 14)
  weight <- c(50, 55, 65, 50, 70)

  df <- data_frame(
    date = date,
    height = height,
    weight = weight
  )

  vdiffr::expect_doppelganger("y scale", {
    ggplot2::ggplot(df, ggplot2::aes(x = height, y = date)) +
      ggplot2::geom_point()
  })
  vdiffr::expect_doppelganger("size scale", {
    ggplot2::ggplot(df, ggplot2::aes(height, weight, size = date)) +
      ggplot2::geom_point()
  })
  vdiffr::expect_doppelganger("color scale", {
    ggplot2::ggplot(df, ggplot2::aes(height, weight, color = date)) +
      ggplot2::geom_point()
  })
  vdiffr::expect_doppelganger("colour scale", {
    ggplot2::ggplot(df, ggplot2::aes(height, weight, colour = date)) +
      ggplot2::geom_point()
  })
  vdiffr::expect_doppelganger("fill scale", {
    ggplot2::ggplot(df, ggplot2::aes(height, weight, fill = date)) +
      ggplot2::geom_col()
  })
  vdiffr::expect_doppelganger("alpha scale", {
    ggplot2::ggplot(df, ggplot2::aes(height, weight, alpha = date)) +
      ggplot2::geom_point()
  })
})
