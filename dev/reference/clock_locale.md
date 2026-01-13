# Create a clock locale

A clock locale contains the information required to format and parse
dates. The defaults have been chosen to match US English. A clock locale
object can be provided to
[`format()`](https://rdrr.io/r/base/format.html) methods or parse
functions (like
[`year_month_day_parse()`](https://clock.r-lib.org/dev/reference/year_month_day_parse.md))
to override the defaults.

## Usage

``` r
clock_locale(labels = "en", decimal_mark = ".")
```

## Arguments

- labels:

  `[clock_labels / character(1)]`

  Character representations of localized weekday names, month names, and
  AM/PM names. Either the language code as string (passed on to
  [`clock_labels_lookup()`](https://clock.r-lib.org/dev/reference/clock_labels.md)),
  or an object created by
  [`clock_labels()`](https://clock.r-lib.org/dev/reference/clock_labels.md).

- decimal_mark:

  `[character(1)]`

  Symbol used for the decimal place when formatting sub-second
  date-times. Either `","` or `"."`.

## Value

A `"clock_locale"` object.

## Examples

``` r
clock_locale()
#> <clock_locale>
#> Decimal Mark: .
#> <clock_labels>
#> Weekdays: Sunday (Sun), Monday (Mon), Tuesday (Tue), Wednesday (Wed),
#>           Thursday (Thu), Friday (Fri), Saturday (Sat)
#> Months:   January (Jan), February (Feb), March (Mar), April (Apr), May
#>           (May), June (Jun), July (Jul), August (Aug),
#>           September (Sep), October (Oct), November (Nov),
#>           December (Dec)
#> AM/PM:    AM/PM
clock_locale(labels = "fr")
#> <clock_locale>
#> Decimal Mark: .
#> <clock_labels>
#> Weekdays: dimanche (dim.), lundi (lun.), mardi (mar.), mercredi (mer.),
#>           jeudi (jeu.), vendredi (ven.), samedi (sam.)
#> Months:   janvier (janv.), février (févr.), mars (mars), avril (avr.),
#>           mai (mai), juin (juin), juillet (juil.), août
#>           (août), septembre (sept.), octobre (oct.), novembre
#>           (nov.), décembre (déc.)
#> AM/PM:    AM/PM
```
