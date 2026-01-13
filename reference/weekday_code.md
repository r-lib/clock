# Extract underlying weekday codes

`weekday_code()` extracts out the integer code for the weekday.

## Usage

``` r
weekday_code(x, ..., encoding = "western")
```

## Arguments

- x:

  `[weekday]`

  A weekday vector.

- ...:

  These dots are for future extensions and must be empty.

- encoding:

  `[character(1)]`

  One of:

  - `"western"`: Encode weekdays assuming `1 == Sunday` and
    `7 == Saturday`.

  - `"iso"`: Encode weekdays assuming `1 == Monday` and `7 == Sunday`.
    This is in line with the ISO standard.

## Value

An integer vector of codes.

## Examples

``` r
# Here we supply a western encoding to `weekday()`
x <- weekday(1:7)
x
#> <weekday[7]>
#> [1] Sun Mon Tue Wed Thu Fri Sat

# We can extract out the codes using different encodings
weekday_code(x, encoding = "western")
#> [1] 1 2 3 4 5 6 7
weekday_code(x, encoding = "iso")
#> [1] 7 1 2 3 4 5 6
```
