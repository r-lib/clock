% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/gregorian-year-month-day.R
\name{year-month-day-widen}
\alias{year-month-day-widen}
\alias{calendar_widen.clock_year_month_day}
\title{Widen: year-month-day}
\usage{
\method{calendar_widen}{clock_year_month_day}(x, precision)
}
\arguments{
\item{x}{\verb{[clock_year_month_day]}

A year-month-day vector.}

\item{precision}{\verb{[character(1)]}

One of:
\itemize{
\item \code{"year"}
\item \code{"month"}
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
\code{x} widened to the supplied \code{precision}.
}
\description{
This is a year-month-day method for the \code{\link[=calendar_widen]{calendar_widen()}} generic. It
widens a year-month-day vector to the specified \code{precision}.
}
\examples{
# Month precision
x <- year_month_day(2019, 1)
x

# Widen to day precision
calendar_widen(x, "day")

# Or second precision
sec <- calendar_widen(x, "second")
sec

# Second precision can be widened to subsecond precision
milli <- calendar_widen(sec, "millisecond")
micro <- calendar_widen(sec, "microsecond")
milli
micro

# But once you have "locked in" a subsecond precision, it can't
# be widened again
try(calendar_widen(milli, "microsecond"))
}
