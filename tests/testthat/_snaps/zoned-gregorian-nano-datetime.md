# printing zoned-nano-datetime

    <zoned_nano_datetime<UTC>[1]>
    [1] "2019-05-01 00:00:00.000000005+00:00"

---

    <zoned_nano_datetime<America/New_York>[1]>
    [1] "2019-05-01 00:00:00.000000005-04:00"

---

    <zoned_nano_datetime<UTC>[3]>
    [1] "2019-05-01 00:00:03.000000005+00:00" "2020-05-01 00:00:04.000000006+00:00"
    [3] NA                                   

# printing zoned-nano-datetime - ambiguous time

    <zoned_nano_datetime<America/New_York>[2]>
    [1] "1970-10-25 01:30:00.000000005-04:00" "1970-10-25 01:30:00.000000005-05:00"

# format for zoned-nano-datetime has the zone name by default

    [1] "1970-10-25 01:30:00.000005000-04:00[America/New_York]"

---

    [1] "1970-10-25 01:30:00.000005000-04:00[EDT]"

---

    [1] "1970-10-25 01:30:00.000005000-04:00"

# printing in data frames uses zone name

                                                          x
    1 1970-10-25 01:30:00.000005000-04:00[America/New_York]

# printing in tibble columns is nice and doesn't use zone name

      x                                  
      <zoned_nano_dttm<America/New_York>>
    1 2019-01-01 00:00:00.000000000-05:00
    2 NA                                 

