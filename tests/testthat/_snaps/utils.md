# POSIXt time zones are standardized as expected

    Code
      posixt_tzone_standardize(character())
    Warning <simpleWarning>
      POSIXt input had a corrupt time zone attribute of `character(0)`. Defaulting to the current zone by assuming the zone is `""`.
    Output
      [1] ""

---

    A POSIXt time zone should either be a character vector or `NULL`.

