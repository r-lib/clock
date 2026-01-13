# Convert a date or date-time to a weekday factor

`date_weekday_factor()` converts a date or date-time to an ordered
factor with levels representing the weekday. This can be useful in
combination with ggplot2, or for modeling.

## Usage

``` r
date_weekday_factor(
  x,
  ...,
  labels = "en",
  abbreviate = TRUE,
  encoding = "western"
)
```

## Arguments

- x:

  `[Date / POSIXct / POSIXlt]`

  A date or date-time vector.

- ...:

  These dots are for future extensions and must be empty.

- labels:

  `[clock_labels / character(1)]`

  Character representations of localized weekday names, month names, and
  AM/PM names. Either the language code as string (passed on to
  [`clock_labels_lookup()`](https://clock.r-lib.org/dev/reference/clock_labels.md)),
  or an object created by
  [`clock_labels()`](https://clock.r-lib.org/dev/reference/clock_labels.md).

- abbreviate:

  `[logical(1)]`

  If `TRUE`, the abbreviated weekday names from `labels` will be used.

  If `FALSE`, the full weekday names from `labels` will be used.

- encoding:

  `[character(1)]`

  One of:

  - `"western"`: Encode the weekdays as an ordered factor with levels
    from Sunday -\> Saturday.

  - `"iso"`: Encode the weekdays as an ordered factor with levels from
    Monday -\> Sunday.

## Value

An ordered factor representing the weekdays.

## Examples

``` r
x <- as.Date("2019-01-01") + 0:6

# Default to Sunday -> Saturday
date_weekday_factor(x)
#> [1] Tue Wed Thu Fri Sat Sun Mon
#> Levels: Sun < Mon < Tue < Wed < Thu < Fri < Sat

# ISO encoding is Monday -> Sunday
date_weekday_factor(x, encoding = "iso")
#> [1] Tue Wed Thu Fri Sat Sun Mon
#> Levels: Mon < Tue < Wed < Thu < Fri < Sat < Sun

# With full names
date_weekday_factor(x, abbreviate = FALSE)
#> [1] Tuesday   Wednesday Thursday  Friday    Saturday  Sunday   
#> [7] Monday   
#> 7 Levels: Sunday < Monday < Tuesday < Wednesday < ... < Saturday

# Or a different language
date_weekday_factor(x, labels = "fr")
#> [1] mar. mer. jeu. ven. sam. dim. lun.
#> Levels: dim. < lun. < mar. < mer. < jeu. < ven. < sam.
```
