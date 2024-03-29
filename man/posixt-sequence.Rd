% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/posixt.R
\name{posixt-sequence}
\alias{posixt-sequence}
\alias{date_seq.POSIXt}
\title{Sequences: date-time}
\usage{
\method{date_seq}{POSIXt}(
  from,
  ...,
  to = NULL,
  by = NULL,
  total_size = NULL,
  invalid = NULL,
  nonexistent = NULL,
  ambiguous = NULL
)
}
\arguments{
\item{from}{\verb{[POSIXct(1) / POSIXlt(1)]}

A date-time to start the sequence from.}

\item{...}{These dots are for future extensions and must be empty.}

\item{to}{\verb{[POSIXct(1) / POSIXlt(1) / NULL]}

A date-time to stop the sequence at.

\code{to} is only included in the result if the resulting sequence divides
the distance between \code{from} and \code{to} exactly.

If \code{to} is supplied along with \code{by}, all components of \code{to} more precise
than the precision of \code{by} must match \code{from} exactly. For example, if \code{by = duration_months(1)}, the day, hour, minute, and second components of \code{to}
must match the corresponding components of \code{from}. This ensures that the
generated sequence is, at a minimum, a weakly monotonic sequence of
date-times.

The time zone of \code{to} must match the time zone of \code{from} exactly.}

\item{by}{\verb{[integer(1) / clock_duration(1) / NULL]}

The unit to increment the sequence by.

If \code{by} is an integer, it is equivalent to \code{duration_seconds(by)}.

If \code{by} is a duration, it is allowed to have a precision of:
\itemize{
\item year
\item quarter
\item month
\item week
\item day
\item hour
\item minute
\item second
}}

\item{total_size}{\verb{[positive integer(1) / NULL]}

The size of the resulting sequence.

If specified alongside \code{to}, this must generate a non-fractional sequence
between \code{from} and \code{to}.}

\item{invalid}{\verb{[character(1) / NULL]}

One of the following invalid date resolution strategies:
\itemize{
\item \code{"previous"}: The previous valid instant in time.
\item \code{"previous-day"}: The previous valid day in time, keeping the time of
day.
\item \code{"next"}: The next valid instant in time.
\item \code{"next-day"}: The next valid day in time, keeping the time of day.
\item \code{"overflow"}: Overflow by the number of days that the input is invalid
by. Time of day is dropped.
\item \code{"overflow-day"}: Overflow by the number of days that the input is
invalid by. Time of day is kept.
\item \code{"NA"}: Replace invalid dates with \code{NA}.
\item \code{"error"}: Error on invalid dates.
}

Using either \code{"previous"} or \code{"next"} is generally recommended, as these
two strategies maintain the \emph{relative ordering} between elements of the
input.

If \code{NULL}, defaults to \code{"error"}.

If \code{getOption("clock.strict")} is \code{TRUE}, \code{invalid} must be supplied and
cannot be \code{NULL}. This is a convenient way to make production code robust
to invalid dates.}

\item{nonexistent}{\verb{[character / NULL]}

One of the following nonexistent time resolution strategies, allowed to be
either length 1, or the same length as the input:
\itemize{
\item \code{"roll-forward"}: The next valid instant in time.
\item \code{"roll-backward"}: The previous valid instant in time.
\item \code{"shift-forward"}: Shift the nonexistent time forward by the size of
the daylight saving time gap.
\item \verb{"shift-backward}: Shift the nonexistent time backward by the size of
the daylight saving time gap.
\item \code{"NA"}: Replace nonexistent times with \code{NA}.
\item \code{"error"}: Error on nonexistent times.
}

Using either \code{"roll-forward"} or \code{"roll-backward"} is generally
recommended over shifting, as these two strategies maintain the
\emph{relative ordering} between elements of the input.

If \code{NULL}, defaults to \code{"error"}.

If \code{getOption("clock.strict")} is \code{TRUE}, \code{nonexistent} must be supplied
and cannot be \code{NULL}. This is a convenient way to make production code
robust to nonexistent times.}

\item{ambiguous}{\verb{[character / zoned_time / POSIXct / list(2) / NULL]}

One of the following ambiguous time resolution strategies, allowed to be
either length 1, or the same length as the input:
\itemize{
\item \code{"earliest"}: Of the two possible times, choose the earliest one.
\item \code{"latest"}: Of the two possible times, choose the latest one.
\item \code{"NA"}: Replace ambiguous times with \code{NA}.
\item \code{"error"}: Error on ambiguous times.
}

Alternatively, \code{ambiguous} is allowed to be a zoned_time (or POSIXct) that
is either length 1, or the same length as the input. If an ambiguous time
is encountered, the zoned_time is consulted. If the zoned_time corresponds
to a naive_time that is also ambiguous \emph{and} uses the same daylight saving
time transition point as the original ambiguous time, then the offset of
the zoned_time is used to resolve the ambiguity. If the ambiguity cannot be
resolved by consulting the zoned_time, then this method falls back to
\code{NULL}.

Finally, \code{ambiguous} is allowed to be a list of size 2, where the first
element of the list is a zoned_time (as described above), and the second
element of the list is an ambiguous time resolution strategy to use when
the ambiguous time cannot be resolved by consulting the zoned_time.
Specifying a zoned_time on its own is identical to \verb{list(<zoned_time>, NULL)}.

If \code{NULL}, defaults to \code{"error"}.

If \code{getOption("clock.strict")} is \code{TRUE}, \code{ambiguous} must be supplied and
cannot be \code{NULL}. Additionally, \code{ambiguous} cannot be specified as a
zoned_time on its own, as this implies \code{NULL} for ambiguous times that the
zoned_time cannot resolve. Instead, it must be specified as a list
alongside an ambiguous time resolution strategy as described above. This is
a convenient way to make production code robust to ambiguous times.}
}
\value{
A date-time vector.
}
\description{
This is a POSIXct method for the \code{\link[=date_seq]{date_seq()}} generic.

\code{date_seq()} generates a date-time (POSIXct) sequence.

When calling \code{date_seq()}, exactly two of the following must be specified:
\itemize{
\item \code{to}
\item \code{by}
\item \code{total_size}
}
}
\section{Sequence Generation}{


Different methods are used to generate the sequences, depending on the
precision implied by \code{by}. They are intended to generate the most intuitive
sequences, especially around daylight saving time gaps and fallbacks.

See the examples for more details.
\subsection{Calendrical based sequences:}{

These convert to a naive-time, then to a year-month-day, generate
the sequence, then convert back to a date-time.
\itemize{
\item \code{by = duration_years()}
\item \code{by = duration_quarters()}
\item \code{by = duration_months()}
}
}

\subsection{Naive-time based sequences:}{

These convert to a naive-time, generate the sequence, then
convert back to a date-time.
\itemize{
\item \code{by = duration_weeks()}
\item \code{by = duration_days()}
}
}

\subsection{Sys-time based sequences:}{

These convert to a sys-time, generate the sequence, then
convert back to a date-time.
\itemize{
\item \code{by = duration_hours()}
\item \code{by = duration_minutes()}
\item \code{by = duration_seconds()}
}
}
}

\examples{
zone <- "America/New_York"

from <- date_time_build(2019, 1, zone = zone)
to <- date_time_build(2019, 1, second = 50, zone = zone)

# Defaults to second precision sequence
date_seq(from, to = to, by = 7)

to <- date_time_build(2019, 1, 5, zone = zone)

# Use durations to change to alternative precisions
date_seq(from, to = to, by = duration_days(1))
date_seq(from, to = to, by = duration_hours(10))
date_seq(from, by = duration_minutes(-2), total_size = 3)

# Note that components of `to` more precise than the precision of `by`
# must match `from` exactly. For example, this is not well defined:
from <- date_time_build(2019, 1, 1, 0, 1, 30, zone = zone)
to <- date_time_build(2019, 1, 1, 5, 2, 20, zone = zone)
try(date_seq(from, to = to, by = duration_hours(1)))

# The minute and second components of `to` must match `from`
to <- date_time_build(2019, 1, 1, 5, 1, 30, zone = zone)
date_seq(from, to = to, by = duration_hours(1))

# ---------------------------------------------------------------------------

# Invalid dates must be resolved with the `invalid` argument
from <- date_time_build(2019, 1, 31, zone = zone)
to <- date_time_build(2019, 12, 31, zone = zone)

try(date_seq(from, to = to, by = duration_months(1)))
date_seq(from, to = to, by = duration_months(1), invalid = "previous-day")

# Compare this to the base R result, which is often a source of confusion
seq(from, to = to, by = "1 month")

# This is equivalent to the overflow invalid resolution strategy
date_seq(from, to = to, by = duration_months(1), invalid = "overflow")

# ---------------------------------------------------------------------------

# This date-time is 2 days before a daylight saving time gap that occurred
# on 2021-03-14 between 01:59:59 -> 03:00:00
from <- as.POSIXct("2021-03-12 02:30:00", "America/New_York")

# So creating a daily sequence lands us in that daylight saving time gap,
# creating a nonexistent time
try(date_seq(from, by = duration_days(1), total_size = 5))

# Resolve the nonexistent time with `nonexistent`. Note that this importantly
# allows times after the gap to retain the `02:30:00` time.
date_seq(from, by = duration_days(1), total_size = 5, nonexistent = "roll-forward")

# Compare this to the base R behavior, where the hour is adjusted from 2->3
# as you cross the daylight saving time gap, and is never restored. This is
# equivalent to always using sys-time (rather than naive-time, like clock
# uses for daily sequences).
seq(from, by = "1 day", length.out = 5)

# You can replicate this behavior by generating a second precision sequence
# of 86,400 seconds. Seconds always add in sys-time.
date_seq(from, by = duration_seconds(86400), total_size = 5)

# ---------------------------------------------------------------------------

# Usage of `to` and `total_size` must generate a non-fractional sequence
# between `from` and `to`
from <- date_time_build(2019, 1, 1, 0, 0, 0, zone = "America/New_York")
to <- date_time_build(2019, 1, 1, 0, 0, 3, zone = "America/New_York")

# These are fine
date_seq(from, to = to, total_size = 2)
date_seq(from, to = to, total_size = 4)

# But this is not!
try(date_seq(from, to = to, total_size = 3))
}
