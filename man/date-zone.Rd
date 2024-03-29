% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/clock-deprecated.R
\name{date-zone}
\alias{date-zone}
\alias{date_zone}
\alias{date_set_zone}
\title{Get or set the time zone}
\usage{
date_zone(x)

date_set_zone(x, zone)
}
\arguments{
\item{x}{\verb{[POSIXct / POSIXlt]}

A date-time vector.}

\item{zone}{\verb{[character(1)]}

A valid time zone to switch to.}
}
\description{
\ifelse{html}{\href{https://lifecycle.r-lib.org/articles/stages.html#deprecated}{\figure{lifecycle-deprecated.svg}{options: alt='[Deprecated]'}}}{\strong{[Deprecated]}}
\itemize{
\item \code{date_zone()} is deprecated in favor of \code{\link[=date_time_zone]{date_time_zone()}}.
\item \code{date_set_zone()} is deprecated in favor of \code{\link[=date_time_set_zone]{date_time_set_zone()}}.
}
}
\keyword{internal}
