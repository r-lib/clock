# printing zoned-datetime

    <zoned_datetime<UTC>[1]>
    [1] "2019-05-01 00:00:00+00:00"

---

    <zoned_datetime<America/New_York>[1]>
    [1] "2019-05-01 00:00:00-04:00"

---

    <zoned_datetime<UTC>[3]>
    [1] "2019-05-01 00:00:03+00:00" "2020-05-01 00:00:04+00:00"
    [3] NA                         

# printing zoned-datetime - ambiguous time

    <zoned_datetime<America/New_York>[2]>
    [1] "1970-10-25 01:30:00-04:00" "1970-10-25 01:30:00-05:00"

# format for zoned-datetime has the zone name by default

    [1] "1970-10-25 01:30:00-04:00[America/New_York]"

---

    [1] "1970-10-25 01:30:00-04:00[EDT]"

---

    [1] "1970-10-25 01:30:00-04:00"

# printing in data frames uses zone name

                                                x
    1 1970-10-25 01:30:00-04:00[America/New_York]

# printing in tibble columns is nice and doesn't use zone name

      x                             
      <zoned_dttm<America/New_York>>
    1 2019-01-01 00:00:00-05:00     
    2 NA                            

