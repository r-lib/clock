# printing naive_date_time()

    <time_point<second><year_month_day>[1]>
    [1] "2019-01-01 00:00:05"

---

    <time_point<second><year_month_day>[3]>
    [1] "2019-01-01 00:00:05" "2020-01-01 00:00:05" NA                   

# printing in tibble columns is nice - date time

      x                  
      <tp<sec><ymd>>     
    1 2019-01-01 00:00:00
    2 NA                 

# printing naive_date_nanotime()

    <time_point<nanosecond><year_month_day>[1]>
    [1] "2019-01-01 00:00:05.000000006"

---

    <time_point<nanosecond><year_month_day>[3]>
    [1] "2019-01-01 00:00:05.000000006" "2020-01-01 00:00:05.000000006"
    [3] NA                             

# printing in tibble columns is nice - date nanotime

      x                            
      <tp<nano><ymd>>              
    1 2019-01-01 00:00:00.000000000
    2 NA                           

