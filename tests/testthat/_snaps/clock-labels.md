# can make custom labels

    Code
      labels
    Output
      <clock_labels>
      Weekdays: Su, Mo, Tu, We, Th, Fr, Sa
      Months:   January, February, March, April, May, June, July, August, September,
                October, November, December
      AM/PM:    A/P

# input is validated

    Code
      clock_labels(1)
    Condition
      Error in `clock_labels()`:
      ! `month` must be a character vector of length 12.

---

    Code
      clock_labels("x")
    Condition
      Error in `clock_labels()`:
      ! `month` must be a character vector of length 12.

---

    Code
      clock_labels(months, 1)
    Condition
      Error in `clock_labels()`:
      ! `month_abbrev` must be a character vector of length 12.

---

    Code
      clock_labels(months, "x")
    Condition
      Error in `clock_labels()`:
      ! `month_abbrev` must be a character vector of length 12.

---

    Code
      clock_labels(months, months, 1)
    Condition
      Error in `clock_labels()`:
      ! `weekday` must be a character vector of length 7.

---

    Code
      clock_labels(months, months, "x")
    Condition
      Error in `clock_labels()`:
      ! `weekday` must be a character vector of length 7.

---

    Code
      clock_labels(months, months, weekdays, 1)
    Condition
      Error in `clock_labels()`:
      ! `weekday_abbrev` must be a character vector of length 7.

---

    Code
      clock_labels(months, months, weekdays, "x")
    Condition
      Error in `clock_labels()`:
      ! `weekday_abbrev` must be a character vector of length 7.

---

    Code
      clock_labels(months, months, weekdays, weekdays, 1)
    Condition
      Error in `clock_labels()`:
      ! `am_pm` must be a character vector of length 2.

---

    Code
      clock_labels(months, months, weekdays, weekdays, "x")
    Condition
      Error in `clock_labels()`:
      ! `am_pm` must be a character vector of length 2.

# can lookup a language

    Code
      labels
    Output
      <clock_labels>
      Weekdays: dimanche (dim.), lundi (lun.), mardi (mar.), mercredi (mer.), jeudi
                (jeu.), vendredi (ven.), samedi (sam.)
      Months:   janvier (janv.), février (févr.), mars (mars), avril (avr.), mai (mai),
                juin (juin), juillet (juil.), août (août), septembre (sept.),
                octobre (oct.), novembre (nov.), décembre (déc.)
      AM/PM:    AM/PM

# must be a valid language code

    Code
      clock_labels_lookup(1)
    Condition
      Error in `clock_labels_lookup()`:
      ! `language` must be a single string, not the number 1.

---

    Code
      clock_labels_lookup("foo")
    Condition
      Error in `clock_labels_lookup()`:
      ! Can't find a locale for "foo".

# can list all the languages

    Code
      langs
    Output
        [1] "af"  "agq" "ak"  "am"  "ar"  "as"  "asa" "ast" "az"  "bas" "be"  "bem"
       [13] "bez" "bg"  "bm"  "bn"  "bo"  "br"  "brx" "bs"  "ca"  "ccp" "ce"  "cgg"
       [25] "chr" "ckb" "cs"  "cy"  "da"  "dav" "de"  "dje" "dsb" "dua" "dyo" "dz" 
       [37] "ebu" "ee"  "el"  "en"  "eo"  "es"  "et"  "eu"  "ewo" "fa"  "ff"  "fi" 
       [49] "fil" "fo"  "fr"  "fur" "fy"  "ga"  "gd"  "gl"  "gsw" "gu"  "guz" "gv" 
       [61] "ha"  "haw" "he"  "hi"  "hr"  "hsb" "hu"  "hy"  "id"  "ig"  "ii"  "is" 
       [73] "it"  "ja"  "jgo" "jmc" "ka"  "kab" "kam" "kde" "kea" "khq" "ki"  "kk" 
       [85] "kkj" "kl"  "kln" "km"  "kn"  "ko"  "kok" "ks"  "ksb" "ksf" "ksh" "kw" 
       [97] "ky"  "lag" "lb"  "lg"  "lkt" "ln"  "lo"  "lrc" "lt"  "lu"  "luo" "luy"
      [109] "lv"  "mas" "mer" "mfe" "mg"  "mgh" "mgo" "mk"  "ml"  "mn"  "mr"  "ms" 
      [121] "mt"  "mua" "my"  "mzn" "naq" "nb"  "nd"  "nds" "ne"  "nl"  "nmg" "nn" 
      [133] "nnh" "nus" "nyn" "om"  "or"  "os"  "pa"  "pl"  "ps"  "pt"  "qu"  "rm" 
      [145] "rn"  "ro"  "rof" "ru"  "rw"  "rwk" "sah" "saq" "sbp" "se"  "seh" "ses"
      [157] "sg"  "shi" "si"  "sk"  "sl"  "smn" "sn"  "so"  "sq"  "sr"  "sv"  "sw" 
      [169] "ta"  "te"  "teo" "tg"  "th"  "ti"  "to"  "tr"  "tt"  "twq" "tzm" "ug" 
      [181] "uk"  "ur"  "uz"  "vai" "vi"  "vun" "wae" "wo"  "xog" "yav" "yi"  "yo" 
      [193] "yue" "zgh" "zh"  "zu" 

