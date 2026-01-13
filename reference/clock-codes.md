# Integer codes

Objects with useful mappings from month names and weekday names to
integer codes.

### Month codes (`clock_months`)

- `january == 1`

- `february == 2`

- `march == 3`

- `april == 4`

- `may == 5`

- `june == 6`

- `july == 7`

- `august == 8`

- `september == 9`

- `october == 10`

- `november == 11`

- `december == 12`

### Weekday codes (`clock_weekdays`)

- `sunday == 1`

- `monday == 2`

- `tuesday == 3`

- `wednesday == 4`

- `thursday == 5`

- `friday == 6`

- `saturday == 7`

### ISO weekday codes (`clock_iso_weekdays`)

- `monday == 1`

- `tuesday == 2`

- `wednesday == 3`

- `thursday == 4`

- `friday == 5`

- `saturday == 6`

- `sunday == 7`

## Usage

``` r
clock_months

clock_weekdays

clock_iso_weekdays
```

## Format

- `clock_months`: An environment containing month codes.

&nbsp;

- `clock_weekdays`: An environment containing weekday codes.

&nbsp;

- `clock_iso_weekdays`: An environment containing ISO weekday codes.

## Examples

``` r
weekday(clock_weekdays$wednesday)
#> <weekday[1]>
#> [1] Wed

year_month_weekday(2019, clock_months$april, clock_weekdays$monday, 1:4)
#> <year_month_weekday<day>[4]>
#> [1] "2019-04-Mon[1]" "2019-04-Mon[2]" "2019-04-Mon[3]" "2019-04-Mon[4]"

year_week_day(2020, 52, start = clock_weekdays$monday)
#> <year_week_day<Monday><week>[1]>
#> [1] "2020-W52"

iso_year_week_day(2020, 52, clock_iso_weekdays$thursday)
#> <iso_year_week_day<day>[1]>
#> [1] "2020-W52-4"
```
