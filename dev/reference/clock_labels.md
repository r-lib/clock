# Create or retrieve date related labels

When parsing and formatting dates, you often need to know how weekdays
of the week and months are represented as text. These functions allow
you to either create your own labels, or look them up from a standard
set of language specific labels. The standard list is derived from ICU
(<https://unicode-org.github.io/icu/>) via the stringi package.

- `clock_labels_lookup()` looks up a set of labels from a given language
  code.

- `clock_labels_languages()` lists the language codes that are accepted.

- `clock_labels()` lets you create your own set of labels. Use this if
  the currently supported languages don't meet your needs.

## Usage

``` r
clock_labels(
  month,
  month_abbrev = month,
  weekday,
  weekday_abbrev = weekday,
  am_pm
)

clock_labels_lookup(language)

clock_labels_languages()
```

## Arguments

- month, month_abbrev:

  `[character(12)]`

  Full and abbreviated month names. Starts with January.

- weekday, weekday_abbrev:

  `[character(7)]`

  Full and abbreviated weekday names. Starts with Sunday.

- am_pm:

  `[character(2)]`

  Names used for AM and PM.

- language:

  `[character(1)]`

  A BCP 47 locale, generally constructed from a two or three digit
  language code. See `clock_labels_languages()` for a complete list of
  available locales.

## Value

A `"clock_labels"` object.

## Examples

``` r
clock_labels_lookup("en")
#> <clock_labels>
#> Weekdays: Sunday (Sun), Monday (Mon), Tuesday (Tue), Wednesday (Wed),
#>           Thursday (Thu), Friday (Fri), Saturday (Sat)
#> Months:   January (Jan), February (Feb), March (Mar), April (Apr), May
#>           (May), June (Jun), July (Jul), August (Aug),
#>           September (Sep), October (Oct), November (Nov),
#>           December (Dec)
#> AM/PM:    AM/PM
clock_labels_lookup("ko")
#> <clock_labels>
#> Weekdays: 일요일 (일), 월요일 (월), 화요일 (화), 수요일 (수), 목요일
#>           (목), 금요일 (금), 토요일 (토)
#> Months:   1월, 2월, 3월, 4월, 5월, 6월, 7월, 8월, 9월, 10월, 11월, 12월
#> AM/PM:    오전/오후
clock_labels_lookup("fr")
#> <clock_labels>
#> Weekdays: dimanche (dim.), lundi (lun.), mardi (mar.), mercredi (mer.),
#>           jeudi (jeu.), vendredi (ven.), samedi (sam.)
#> Months:   janvier (janv.), février (févr.), mars (mars), avril (avr.),
#>           mai (mai), juin (juin), juillet (juil.), août
#>           (août), septembre (sept.), octobre (oct.), novembre
#>           (nov.), décembre (déc.)
#> AM/PM:    AM/PM
```
