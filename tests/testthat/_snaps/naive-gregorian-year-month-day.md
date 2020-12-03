# printing year-month-day

    <year_month_day[1]>
    [1] "2019-01-01"

---

    <year_month_day[3]>
    [1] "2019-01-01" "2020-01-01" NA          

# printing empty year-month-day doesn't print `character()`

    <year_month_day[0]>

# printing in tibble columns is nice

      x         
      <ymd>     
    1 2019-01-01
    2 NA        

