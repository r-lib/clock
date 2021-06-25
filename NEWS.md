# clock 0.3.1

* Parsing into a date-time type that is coarser than the original string is now
  considered ambiguous and undefined behavior. For example, parsing a string
  with fractional seconds using `date_time_parse(x)` or
  `naive_time_parse(x, precision = "second")` is no longer considered correct.
  Instead, if you only require second precision from such a string, parse the
  full string, with fractional seconds, into a clock type that can handle them,
  then round to seconds using whatever rounding convention is required for your
  use case, such as `time_point_floor()` (#230).
  
  For example:
  
  ```
  x <- c("2019-01-01 00:00:59.123", "2019-01-01 00:00:59.556")
  
  x <- naive_time_parse(x, precision = "millisecond")
  x
  #> <time_point<naive><millisecond>[2]>
  #> [1] "2019-01-01 00:00:59.123" "2019-01-01 00:00:59.556"
  
  x <- time_point_round(x, "second")
  x
  #> <time_point<naive><second>[2]>
  #> [1] "2019-01-01 00:00:59" "2019-01-01 00:01:00"
  
  as_date_time(x, "America/New_York")
  #> [1] "2019-01-01 00:00:59 EST" "2019-01-01 00:01:00 EST"
  ```
  
* Preemptively updated tests related to upcoming changes in testthat (#236).

# clock 0.3.0

* New `date_seq()` for generating date and date-time sequences (#218).

* clock now uses the tzdb package to access the date library's API. This
  means that the experimental API that was to be used for vroom has been
  removed in favor of using the one exposed in tzdb.
  
* `zone_database_names()` and `zone_database_version()` have been removed in
  favor of re-exporting `tzdb_names()` and `tzdb_version()` from the tzdb
  package.

# clock 0.2.0

* clock now interprets R's Date class as _naive-time_ rather than _sys-time_.
  This means that it no longer assumes that Date has an implied time zone of
  UTC (#203). This generally aligns better with how users think Date should
  work. This resulted in the following changes:
  
  * `date_zone()` now errors with Date input, as naive-times do not have a
    specified time zone.
    
  * `date_parse()` now parses into a naive-time, rather than a sys-time, before
    converting to Date. This means that `%z` and `%Z` are now completely
    ignored.
    
  * The Date method for `date_format()` now uses the naive-time `format()`
    method rather than the zoned-time one. This means that `%z` and `%Z` are
    no longer valid format commands.
    
  * The zoned-time method for `as.Date()` now converts to Date through an
    intermediate naive-time, rather than a sys-time. This means that the
    printed date will always be retained, which is generally what is expected.
    
  * The Date method for `as_zoned_time()` now converts to zoned-time through
    an intermediate naive-time, rather than a sys-time. This means that the
    printed date will always attempt to be retained, if possible, which is
    generally what is expected. In the rare case that daylight saving time makes
    a direct conversion impossible, `nonexistent` and `ambiguous` can be used
    to resolve any issues.

* New `as_date()` and `as_date_time()` for converting to Date and POSIXct
  respectively. Unlike `as.Date()` and `as.POSIXct()`, these functions always
  treat Date as a naive-time type, which results in more consistent and
  intuitive conversions. Note that `as_date()` does conflict with
  `lubridate::as_date()`, and the lubridate version handles Dates differently
  (#209).

* Added two new convenient helpers (#197):

  * `date_today()` for getting the current date (Date)
  
  * `date_now()` for getting the current date-time (POSIXct)

* Fixed a bug where converting from a time point to a Date or POSIXct could
  round incorrectly (#205).

* Errors resulting from invalid dates or nonexistent/ambiguous times are now
  a little nicer to read through the usage of an info bullet (#200).
  
* Formatting a naive-time with `%Z` or `%z` now warns that there were
  format failures (#204).

* Fixed a Solaris ambiguous behavior issue from calling `pow(int, int)`.

* Linking against cpp11 0.2.7 is now required to fix a rare memory leak issue.

* Exposed an extremely experimental and limited C++ API for vroom (#322).

# clock 0.1.0

* Added a `NEWS.md` file to track changes to the package.
