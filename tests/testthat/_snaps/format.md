# can format with locale specific weekday

    [1] "2019-dimanche"

---

    [1] "2019-dim."

# can format with locale specific month name

    [1] "2019-janvier"

---

    [1] "2019-janv."

# can format with locale specific AM/PM name

    [1] "vm." "nm."

# can format with different decimal mark

    [1] "2019-01-01 00:00:00,000000000"

# decimal mark customization passes through with %T and %X

    [1] "00:00:00,000000000"

---

    [1] "00:00:00,000000000"

# locale passes through to default calendar print method

    [1] "2019-01-dimanche[1] 00:00:00"

