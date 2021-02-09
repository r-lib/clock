# format - can format() a weekday

    Code
      format(weekday(0:6))
    Output
      [1] "Sun" "Mon" "Tue" "Wed" "Thu" "Fri" "Sat"

# format - can use full names

    Code
      format(weekday(0:6), abbreviate = FALSE)
    Output
      [1] "Sunday"    "Monday"    "Tuesday"   "Wednesday" "Thursday"  "Friday"   
      [7] "Saturday" 

# format - can use a different locale

    Code
      format(weekday(0:6), labels = "fr", abbreviate = FALSE)
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

# `x` is validated

    `x` must be a 'clock_weekday' object.

# `labels` is validated

    `labels` must be a 'clock_labels' object.

# `encoding` is validated

    `encoding` must be one of "c" or "iso".

---

    `encoding` must be one of "c" or "iso".

---

    `encoding` must be one of "c" or "iso".

# `abbreviate` is validated

    `abbreviate` must be `TRUE` or `FALSE`.

---

    `abbreviate` must be `TRUE` or `FALSE`.

---

    `abbreviate` must be `TRUE` or `FALSE`.

