# printing zoned-date-time

    <time_point<second><year_month_day><UTC>[1]>
    [1] "2019-05-01 00:00:00+00:00"

---

    <time_point<second><year_month_day><America/New_York>[1]>
    [1] "2019-05-01 00:00:00-04:00"

---

    <time_point<second><year_month_day><UTC>[3]>
    [1] "2019-05-01 00:00:03+00:00" "2020-05-01 00:00:04+00:00"
    [3] NA                         

# printing zoned-date-nano-time

    <time_point<nanosecond><year_month_day><UTC>[1]>
    [1] "2019-05-01 00:00:00.000000005+00:00"

---

    <time_point<nanosecond><year_month_day><America/New_York>[1]>
    [1] "2019-05-01 00:00:00.000000005-04:00"

---

    <time_point<nanosecond><year_month_day><UTC>[3]>
    [1] "2019-05-01 00:00:03.000000005+00:00" "2020-05-01 00:00:04.000000006+00:00"
    [3] NA                                   

# printing zoned-date-time - ambiguous time

    <time_point<second><year_month_day><America/New_York>[2]>
    [1] "1970-10-25 01:30:00-04:00" "1970-10-25 01:30:00-05:00"

# printing zoned-date-nano-time - ambiguous time

    <time_point<nanosecond><year_month_day><America/New_York>[2]>
    [1] "1970-10-25 01:30:00.000000005-04:00" "1970-10-25 01:30:00.000000005-05:00"

# format for zoned-date-time has the zone name by default

    [1] "1970-10-25 01:30:00-04:00[America/New_York]"

---

    [1] "1970-10-25 01:30:00-04:00[EDT]"

---

    [1] "1970-10-25 01:30:00-04:00"

# format for zoned-date-nano-time has the zone name by default

    [1] "1970-10-25 01:30:00.000005000-04:00[America/New_York]"

---

    [1] "1970-10-25 01:30:00.000005000-04:00[EDT]"

---

    [1] "1970-10-25 01:30:00.000005000-04:00"

# printing in data frames uses zone name - zoned-date-time

                                                x
    1 1970-10-25 01:30:00-04:00[America/New_York]

# printing in data frames uses zone name - zoned-date-nano-time

                                                          x
    1 1970-10-25 01:30:00.000005000-04:00[America/New_York]

# printing in tibble columns is nice and doesn't use zone name - zoned-date-time

      x                               
      <tp<sec><ymd><America/New_York>>
    1 2019-01-01 00:00:00-05:00       
    2 NA                              

# printing in tibble columns is nice and doesn't use zone name - zoned-date-nano-time

      x                                  
      <tp<nano><ymd><America/New_York>>  
    1 2019-01-01 00:00:00.000000000-05:00
    2 NA                                 

