# can create a default locale object

    Code
      locale
    Output
      <clock_locale>
      Decimal Mark: .
      <clock_labels>
      Weekdays: Sunday (Sun), Monday (Mon), Tuesday (Tue), Wednesday (Wed), Thursday
                (Thu), Friday (Fri), Saturday (Sat)
      Months:   January (Jan), February (Feb), March (Mar), April (Apr), May (May),
                June (Jun), July (Jul), August (Aug), September (Sep),
                October (Oct), November (Nov), December (Dec)
      AM/PM:    AM/PM

# must use a valid clock-labels object

    Code
      clock_locale(1)
    Condition
      Error in `clock_locale()`:
      ! `labels` must be a <clock_labels>, not the number 1.

# must use a valid decimal-mark

    Code
      clock_locale(decimal_mark = "f")
    Condition
      Error in `clock_locale()`:
      ! `decimal_mark` must be one of "." or ",", not "f".

# can change the decimal-mark

    Code
      x
    Output
      <clock_locale>
      Decimal Mark: ,
      <clock_labels>
      Weekdays: Sunday (Sun), Monday (Mon), Tuesday (Tue), Wednesday (Wed), Thursday
                (Thu), Friday (Fri), Saturday (Sat)
      Months:   January (Jan), February (Feb), March (Mar), April (Apr), May (May),
                June (Jun), July (Jul), August (Aug), September (Sep),
                October (Oct), November (Nov), December (Dec)
      AM/PM:    AM/PM

