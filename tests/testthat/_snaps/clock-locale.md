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

    `labels` must be a 'clock_labels' object.

# must use a valid decimal-mark

    `decimal_mark` must be either ',' or '.'.

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

