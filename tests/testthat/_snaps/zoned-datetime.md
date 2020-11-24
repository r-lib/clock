# printing zoned-datetime

    <civil_datetime<UTC>[1]>
    [1] "2019-05-01T00:00:00+00:00"

---

    <civil_datetime<America/New_York>[1]>
    [1] "2019-05-01T00:00:00-04:00"

---

    <civil_datetime<UTC>[3]>
    [1] "2019-05-01T00:00:03+00:00" "2020-05-01T00:00:04+00:00"
    [3] NA                         

# printing zoned-datetime - ambiguous time

    <civil_datetime<America/New_York>[2]>
    [1] "1970-10-25T01:30:00-04:00" "1970-10-25T01:30:00-05:00"

# format for zoned-datetime is extended by default

    [1] "1970-10-25T01:30:00-04:00[America/New_York]"

---

    [1] "1970-10-25T01:30:00-04:00[EDT]"

---

    [1] "1970-10-25T01:30:00-04:00"

# printing in data frames uses extended format

                                                x
    1 1970-10-25T01:30:00-04:00[America/New_York]

