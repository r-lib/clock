% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/gregorian-year-month-weekday.R
\name{year-month-weekday-count-between}
\alias{year-month-weekday-count-between}
\alias{calendar_count_between.clock_year_month_weekday}
\title{Counting: year-month-weekday}
\usage{
\method{calendar_count_between}{clock_year_month_weekday}(start, end, precision, ..., n = 1L)
}
\arguments{
\item{start, end}{\verb{[clock_year_month_weekday]}

A pair of year-month-weekday vectors. These will be recycled to their
common size.}

\item{precision}{\verb{[character(1)]}

One of:
\itemize{
\item \code{"year"}
\item \code{"quarter"}
\item \code{"month"}
}}

\item{...}{These dots are for future extensions and must be empty.}

\item{n}{\verb{[positive integer(1)]}

A single positive integer specifying a multiple of \code{precision} to use.}
}
\value{
An integer representing the number of \code{precision} units between
\code{start} and \code{end}.
}
\description{
This is a year-month-weekday method for the \code{\link[=calendar_count_between]{calendar_count_between()}}
generic. It counts the number of \code{precision} units between \code{start} and \code{end}
(i.e., the number of years or months).
}
\details{
Remember that year-month-weekday is not comparable when it is \code{"day"}
precision or finer, so this method is only defined for \code{"year"} and
\code{"month"} precision year-month-weekday objects.

\code{"quarter"} is equivalent to \code{"month"} precision with \code{n} set to \code{n * 3L}.
}
\examples{
# Compute the number of months between two dates
x <- year_month_weekday(2001, 2)
y <- year_month_weekday(2021, c(1, 3))

calendar_count_between(x, y, "month")

# Remember that day precision or finer year-month-weekday objects
# are not comparable, so this won't work
x <- year_month_weekday(2001, 2, 1, 1)
try(calendar_count_between(x, x, "month"))
}
