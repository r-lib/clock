% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/time-point.R
\name{time-point-rounding}
\alias{time-point-rounding}
\alias{time_point_floor}
\alias{time_point_ceiling}
\alias{time_point_round}
\title{Time point rounding}
\usage{
time_point_floor(x, precision, ..., n = 1L, origin = NULL)

time_point_ceiling(x, precision, ..., n = 1L, origin = NULL)

time_point_round(x, precision, ..., n = 1L, origin = NULL)
}
\arguments{
\item{x}{\verb{[clock_sys_time / clock_naive_time]}

A sys-time or naive-time.}

\item{precision}{\verb{[character(1)]}

A time point precision. One of:
\itemize{
\item \code{"day"}
\item \code{"hour"}
\item \code{"minute"}
\item \code{"second"}
\item \code{"millisecond"}
\item \code{"microsecond"}
\item \code{"nanosecond"}
}}

\item{...}{These dots are for future extensions and must be empty.}

\item{n}{\verb{[positive integer(1)]}

A positive integer specifying the multiple of \code{precision} to use.}

\item{origin}{\verb{[clock_sys_time(1) / clock_naive_time(1) / NULL]}

An origin to begin counting from. Mostly useful when \code{n > 1} and you
want to control how the rounding groups are created.

If \code{x} is a sys-time, \code{origin} must be a sys-time.

If \code{x} is a naive-time, \code{origin} must be a naive-time.

The precision of \code{origin} must be equally precise as or less
precise than \code{precision}.

If \code{NULL}, a default origin of midnight on 1970-01-01 is used.}
}
\value{
\code{x} rounded to the new \code{precision}.
}
\description{
\itemize{
\item \code{time_point_floor()} rounds a sys-time or naive-time down to a multiple of
the specified \code{precision}.
\item \code{time_point_ceiling()} rounds a sys-time or naive-time up to a multiple of
the specified \code{precision}.
\item \code{time_point_round()} rounds up or down depending on what is closer,
rounding up on ties.
}

Rounding time points is mainly useful for rounding sub-daily time points
up to daily time points.

It can also be useful for flooring by a set number of days (like 20) with
respect to some origin. By default, the origin is 1970-01-01 00:00:00.

If you want to group by components, such as "day of the month", rather than
by "n days", see \code{\link[=calendar_group]{calendar_group()}}.
}
\section{Boundary Handling}{


To understand how flooring and ceiling work, you need to know how they
create their intervals for rounding.
\itemize{
\item \code{time_point_floor()} constructs intervals of \code{[lower, upper)} that
bound each element of \code{x}, then always chooses the \emph{left-hand side}.
\item \code{time_point_ceiling()} constructs intervals of \code{(lower, upper]} that
bound each element of \code{x}, then always chooses the \emph{right-hand side}.
}

As an easy example, consider 2020-01-02 00:00:05.

To floor this to the nearest day, the following interval is constructed,
and the left-hand side is returned at day precision:

\code{[2020-01-02 00:00:00, 2020-01-03 00:00:00)}

To ceiling this to the nearest day, the following interval
is constructed, and the right-hand side is returned at day precision:

\code{(2020-01-02 00:00:00, 2020-01-03 00:00:00]}

Here is another example, this time with a time point on a boundary,
2020-01-02 00:00:00.

To floor this to the nearest day, the following interval is constructed,
and the left-hand side is returned at day precision:

\code{[2020-01-02 00:00:00, 2020-01-03 00:00:00)}

To ceiling this to the nearest day, the following interval
is constructed, and the right-hand side is returned at day precision:

\code{(2020-01-01 00:00:00, 2020-01-02 00:00:00]}

Notice that, regardless of whether you are doing a floor or ceiling, if
the input falls on a boundary then it will be returned as is.
}

\examples{
library(magrittr)

x <- as_naive_time(year_month_day(2019, 01, 01))
x <- add_days(x, 0:40)
head(x)

# Floor by sets of 20 days
# The implicit origin to start the 20 day counter is 1970-01-01
time_point_floor(x, "day", n = 20)

# You can easily customize the origin by supplying a new one
# as the `origin` argument
origin <- year_month_day(2019, 01, 01) \%>\%
  as_naive_time()

time_point_floor(x, "day", n = 20, origin = origin)

# For times on the boundary, floor and ceiling both return the input
# at the new precision. Notice how the first element is on the boundary,
# and the second is 1 second after the boundary.
y <- as_naive_time(year_month_day(2020, 01, 02, 00, 00, c(00, 01)))
time_point_floor(y, "day")
time_point_ceiling(y, "day")
}
