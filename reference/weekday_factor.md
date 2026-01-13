# Convert a weekday to an ordered factor

`weekday_factor()` converts a weekday object to an ordered factor. This
can be useful in combination with ggplot2, or for modeling.

## Usage

``` r
weekday_factor(x, ..., labels = "en", abbreviate = TRUE, encoding = "western")
```

## Arguments

- x:

  `[weekday]`

  A weekday vector.

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
x <- weekday(1:7)

# Default to Sunday -> Saturday
weekday_factor(x)
#> [1] Sun Mon Tue Wed Thu Fri Sat
#> Levels: Sun < Mon < Tue < Wed < Thu < Fri < Sat

# ISO encoding is Monday -> Sunday
weekday_factor(x, encoding = "iso")
#> [1] Sun Mon Tue Wed Thu Fri Sat
#> Levels: Mon < Tue < Wed < Thu < Fri < Sat < Sun

# With full names
weekday_factor(x, abbreviate = FALSE)
#> [1] Sunday    Monday    Tuesday   Wednesday Thursday  Friday   
#> [7] Saturday 
#> 7 Levels: Sunday < Monday < Tuesday < Wednesday < ... < Saturday

# Or a different language
weekday_factor(x, labels = "fr")
#> [1] dim. lun. mar. mer. jeu. ven. sam.
#> Levels: dim. < lun. < mar. < mer. < jeu. < ven. < sam.
```
