% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/quarterly-year-quarter-day.R
\name{year-quarter-day-boundary}
\alias{year-quarter-day-boundary}
\alias{calendar_start.clock_year_quarter_day}
\alias{calendar_end.clock_year_quarter_day}
\title{Boundaries: year-quarter-day}
\usage{
\method{calendar_start}{clock_year_quarter_day}(x, precision)

\method{calendar_end}{clock_year_quarter_day}(x, precision)
}
\arguments{
\item{x}{\verb{[clock_year_quarter_day]}

A year-quarter-day vector.}

\item{precision}{\verb{[character(1)]}

One of:
\itemize{
\item \code{"year"}
\item \code{"quarter"}
\item \code{"day"}
\item \code{"hour"}
\item \code{"minute"}
\item \code{"second"}
\item \code{"millisecond"}
\item \code{"microsecond"}
\item \code{"nanosecond"}
}}
}
\value{
\code{x} at the same precision, but with some components altered to be
at the boundary value.
}
\description{
This is a year-quarter-day method for the \code{\link[=calendar_start]{calendar_start()}} and
\code{\link[=calendar_end]{calendar_end()}} generics. They adjust components of a calendar to the
start or end of a specified \code{precision}.
}
\examples{
x <- year_quarter_day(2019:2020, 2:3, 5, 6, 7, 8, start = clock_months$march)
x

# Compute the last moment of the fiscal quarter
calendar_end(x, "quarter")

# Compare that to just setting the day to `"last"`,
# which doesn't affect the other components
set_day(x, "last")

# Compute the start of the fiscal year
calendar_start(x, "year")

as_date(calendar_start(x, "year"))
}
