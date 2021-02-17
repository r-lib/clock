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

    `labels` must be a 'clock_labels' object.

# format - `abbreviate` is validated

    `abbreviate` must be `TRUE` or `FALSE`.

---

    `abbreviate` must be `TRUE` or `FALSE`.

---

    `abbreviate` must be `TRUE` or `FALSE`.

# validates `x`

    `x` must be a 'clock_weekday'.

# weekday_code - `encoding` is validated

    `encoding` must be one of "western" or "iso".

---

    `encoding` must be one of "western" or "iso".

---

    `encoding` must be one of "western" or "iso".

# `x` is validated

    `x` must be a 'clock_weekday' object.

# `labels` is validated

    `labels` must be a 'clock_labels' object.

# `encoding` is validated

    `encoding` must be one of "western" or "iso".

---

    `encoding` must be one of "western" or "iso".

---

    `encoding` must be one of "western" or "iso".

# `abbreviate` is validated

    `abbreviate` must be `TRUE` or `FALSE`.

---

    `abbreviate` must be `TRUE` or `FALSE`.

---

    `abbreviate` must be `TRUE` or `FALSE`.

# can't compare or order weekdays (#153)

    Can't compare or order values of the 'clock_weekday' type, as this type does not specify a 'first' day of the week.

---

    Can't compare or order values of the 'clock_weekday' type, as this type does not specify a 'first' day of the week.

---

    Can't compare or order values of the 'clock_weekday' type, as this type does not specify a 'first' day of the week.

---

    Can't compare or order values of the 'clock_weekday' type, as this type does not specify a 'first' day of the week.

