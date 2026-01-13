# Support for vctrs arithmetic

Support for vctrs arithmetic

## Usage

``` r
# S3 method for class 'clock_year_day'
vec_arith(op, x, y, ...)

# S3 method for class 'clock_year_month_day'
vec_arith(op, x, y, ...)

# S3 method for class 'clock_year_month_weekday'
vec_arith(op, x, y, ...)

# S3 method for class 'clock_iso_year_week_day'
vec_arith(op, x, y, ...)

# S3 method for class 'clock_naive_time'
vec_arith(op, x, y, ...)

# S3 method for class 'clock_year_quarter_day'
vec_arith(op, x, y, ...)

# S3 method for class 'clock_sys_time'
vec_arith(op, x, y, ...)

# S3 method for class 'clock_year_week_day'
vec_arith(op, x, y, ...)

# S3 method for class 'clock_weekday'
vec_arith(op, x, y, ...)
```

## Arguments

- op:

  An arithmetic operator as a string

- x, y:

  A pair of vectors. For `!`, unary `+` and unary `-`, `y` will be a
  sentinel object of class `MISSING`, as created by `MISSING()`.

- ...:

  These dots are for future extensions and must be empty.

## Value

The result of the arithmetic operation.

## Examples

``` r
vctrs::vec_arith("+", year_month_day(2019), 1)
#> <year_month_day<year>[1]>
#> [1] "2020"
```
