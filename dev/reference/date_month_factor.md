# Convert a date or date-time to an ordered factor of month names

`date_month_factor()` extracts the month values from a date or date-time
and converts them to an ordered factor of month names. This can be
useful in combination with ggplot2, or for modeling.

## Usage

``` r
date_month_factor(x, ..., labels = "en", abbreviate = FALSE)
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

  If `TRUE`, the abbreviated month names from `labels` will be used.

  If `FALSE`, the full month names from `labels` will be used.

## Value

An ordered factor representing the months.

## Examples

``` r
x <- add_months(as.Date("2019-01-01"), 0:11)

date_month_factor(x)
#>  [1] January   February  March     April     May       June     
#>  [7] July      August    September October   November  December 
#> 12 Levels: January < February < March < April < May < ... < December
date_month_factor(x, abbreviate = TRUE)
#>  [1] Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
#> 12 Levels: Jan < Feb < Mar < Apr < May < Jun < Jul < Aug < ... < Dec
date_month_factor(x, labels = "fr")
#>  [1] janvier   février   mars      avril     mai       juin     
#>  [7] juillet   août      septembre octobre   novembre  décembre 
#> 12 Levels: janvier < février < mars < avril < mai < ... < décembre
```
