# Convert a calendar to an ordered factor of month names

`calendar_month_factor()` extracts the month values from a calendar and
converts them to an ordered factor of month names. This can be useful in
combination with ggplot2, or for modeling.

This function is only relevant for calendar types that use a month
field, i.e.
[`year_month_day()`](https://clock.r-lib.org/reference/year_month_day.md)
and
[`year_month_weekday()`](https://clock.r-lib.org/reference/year_month_weekday.md).
The calendar type must have at least month precision.

## Usage

``` r
calendar_month_factor(x, ..., labels = "en", abbreviate = FALSE)
```

## Arguments

- x:

  `[calendar]`

  A calendar vector.

- ...:

  These dots are for future extensions and must be empty.

- labels:

  `[clock_labels / character(1)]`

  Character representations of localized weekday names, month names, and
  AM/PM names. Either the language code as string (passed on to
  [`clock_labels_lookup()`](https://clock.r-lib.org/reference/clock_labels.md)),
  or an object created by
  [`clock_labels()`](https://clock.r-lib.org/reference/clock_labels.md).

- abbreviate:

  `[logical(1)]`

  If `TRUE`, the abbreviated month names from `labels` will be used.

  If `FALSE`, the full month names from `labels` will be used.

## Value

An ordered factor representing the months.

## Examples

``` r
x <- year_month_day(2019, 1:12)

calendar_month_factor(x)
#>  [1] January   February  March     April     May       June     
#>  [7] July      August    September October   November  December 
#> 12 Levels: January < February < March < April < May < ... < December
calendar_month_factor(x, abbreviate = TRUE)
#>  [1] Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
#> 12 Levels: Jan < Feb < Mar < Apr < May < Jun < Jul < Aug < ... < Dec
calendar_month_factor(x, labels = "fr")
#>  [1] janvier   février   mars      avril     mai       juin     
#>  [7] juillet   août      septembre octobre   novembre  décembre 
#> 12 Levels: janvier < février < mars < avril < mai < ... < décembre
```
