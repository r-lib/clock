% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/week-year-week-day.R
\name{year-week-day-narrow}
\alias{year-week-day-narrow}
\alias{calendar_narrow.clock_year_week_day}
\title{Narrow: year-week-day}
\usage{
\method{calendar_narrow}{clock_year_week_day}(x, precision)
}
\arguments{
\item{x}{\verb{[clock_year_week_day]}

A year-week-day vector.}

\item{precision}{\verb{[character(1)]}

One of:
\itemize{
\item \code{"year"}
\item \code{"week"}
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
\code{x} narrowed to the supplied \code{precision}.
}
\description{
This is a year-week-day method for the \code{\link[=calendar_narrow]{calendar_narrow()}} generic. It
narrows a year-week-day vector to the specified \code{precision}.
}
\examples{
# Day precision
x <- year_week_day(2019, 1, 5)
x

# Narrowed to week precision
calendar_narrow(x, "week")
}
