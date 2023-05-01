# POSIXt time zones are standardized as expected

    Code
      posixt_tzone_standardize(character())
    Condition
      Warning in `posixt_tzone_standardize()`:
      POSIXt input had a corrupt time zone attribute of `character(0)`. Defaulting to the current zone by assuming the zone is `""`.
    Output
      [1] ""

---

    Code
      posixt_tzone_standardize(1)
    Condition
      Error in `posixt_tzone_standardize()`:
      ! A POSIXt time zone should either be a character vector or `NULL`.

