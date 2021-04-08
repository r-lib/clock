# clock (development version)

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

# clock 0.1.0

* Added a `NEWS.md` file to track changes to the package.