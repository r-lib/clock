# format - can format() a weekday

    Code
      format(weekday(1:7))
    Output
      [1] "Sun" "Mon" "Tue" "Wed" "Thu" "Fri" "Sat"

# format - can use full names

    Code
      format(weekday(1:7), abbreviate = FALSE)
    Output
      [1] "Sunday"    "Monday"    "Tuesday"   "Wednesday" "Thursday"  "Friday"   
      [7] "Saturday" 

# format - can use a different locale

    Code
      format(weekday(1:7), labels = "fr", abbreviate = FALSE)
    Output
      [1] "dimanche" "lundi"    "mardi"    "mercredi" "jeudi"    "vendredi" "samedi"  

# format - `labels` is validated

    Code
      format(weekday(1), labels = 1)
    Condition
      Error in `format()`:
      ! `labels` must be a <clock_labels>, not the number 1.

# format - `abbreviate` is validated

    Code
      format(weekday(1), abbreviate = "foo")
    Condition
      Error in `format()`:
      ! `abbreviate` must be `TRUE` or `FALSE`, not the string "foo".

---

    Code
      format(weekday(1), abbreviate = 1)
    Condition
      Error in `format()`:
      ! `abbreviate` must be `TRUE` or `FALSE`, not the number 1.

---

    Code
      format(weekday(1), abbreviate = c(TRUE, FALSE))
    Condition
      Error in `format()`:
      ! `abbreviate` must be `TRUE` or `FALSE`, not a logical vector.

# validates `x`

    Code
      weekday_code(1)
    Condition
      Error in `weekday_code()`:
      ! `x` must be a <clock_weekday>, not the number 1.

# weekday_code - `encoding` is validated

    Code
      weekday_code(weekday(1), encoding = "foo")
    Condition
      Error in `weekday_code()`:
      ! `encoding` must be one of "western" or "iso", not "foo".

---

    Code
      weekday_code(weekday(1), encoding = 1)
    Condition
      Error in `weekday_code()`:
      ! `encoding` must be a string or character vector.

# `x` is validated

    Code
      weekday_factor(1)
    Condition
      Error in `weekday_factor()`:
      ! `x` must be a <clock_weekday>, not the number 1.

# `labels` is validated

    Code
      weekday_factor(weekday(1), labels = 1)
    Condition
      Error in `weekday_factor()`:
      ! `labels` must be a <clock_labels>, not the number 1.

# `encoding` is validated

    Code
      weekday_factor(weekday(1), encoding = "foo")
    Condition
      Error in `weekday_factor()`:
      ! `encoding` must be one of "western" or "iso", not "foo".

---

    Code
      weekday_factor(weekday(1), encoding = 1)
    Condition
      Error in `weekday_factor()`:
      ! `encoding` must be a string or character vector.

# `abbreviate` is validated

    Code
      weekday_factor(weekday(1), abbreviate = "foo")
    Condition
      Error in `weekday_factor()`:
      ! `abbreviate` must be `TRUE` or `FALSE`, not the string "foo".

---

    Code
      weekday_factor(weekday(1), abbreviate = 1)
    Condition
      Error in `weekday_factor()`:
      ! `abbreviate` must be `TRUE` or `FALSE`, not the number 1.

---

    Code
      weekday_factor(weekday(1), abbreviate = c(TRUE, FALSE))
    Condition
      Error in `weekday_factor()`:
      ! `abbreviate` must be `TRUE` or `FALSE`, not a logical vector.

# can't compare or order weekdays (#153)

    Code
      weekday(1) < weekday(2)
    Condition
      Error in `vec_proxy_compare()`:
      ! Can't compare or order values of the <clock_weekday> type, as this type does not specify a "first" day of the week.

---

    Code
      min(weekday(1))
    Condition
      Error in `vec_proxy_compare()`:
      ! Can't compare or order values of the <clock_weekday> type, as this type does not specify a "first" day of the week.

---

    Code
      xtfrm(weekday(1:2))
    Condition
      Error in `vec_proxy_compare()`:
      ! Can't compare or order values of the <clock_weekday> type, as this type does not specify a "first" day of the week.

---

    Code
      vec_order(weekday(1:2))
    Condition
      Error in `vec_proxy_compare()`:
      ! Can't compare or order values of the <clock_weekday> type, as this type does not specify a "first" day of the week.

