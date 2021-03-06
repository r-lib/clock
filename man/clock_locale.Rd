% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/clock-locale.R
\name{clock_locale}
\alias{clock_locale}
\title{Create a clock locale}
\usage{
clock_locale(labels = "en", decimal_mark = ".")
}
\arguments{
\item{labels}{\verb{[clock_labels / character(1)]}

Character representations of localized weekday names, month names, and
AM/PM names. Either the language code as string (passed on to
\code{\link[=clock_labels_lookup]{clock_labels_lookup()}}), or an object created by \code{\link[=clock_labels]{clock_labels()}}.}

\item{decimal_mark}{\verb{[character(1)]}

Symbol used for the decimal place when formatting sub-second date-times.
Either \code{","} or \code{"."}.}
}
\value{
A \code{"clock_locale"} object.
}
\description{
A clock locale contains the information required to format and parse dates.
The defaults have been chosen to match US English. A clock locale object can
be provided to \code{format()} methods or parse functions (like
\code{\link[=year_month_day_parse]{year_month_day_parse()}}) to override the defaults.
}
\examples{
clock_locale()
clock_locale(labels = "fr")
}
