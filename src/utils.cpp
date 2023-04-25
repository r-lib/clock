#include "utils.h"

SEXP strings = NULL;
SEXP strings_empty = NULL;
SEXP strings_vctrs_vctr = NULL;
SEXP strings_vctrs_rcrd = NULL;
SEXP strings_clock_rcrd = NULL;
SEXP strings_clock_time_point = NULL;
SEXP strings_clock_sys_time = NULL;
SEXP strings_clock_naive_time = NULL;
SEXP strings_clock_zoned_time = NULL;
SEXP strings_clock_duration = NULL;
SEXP strings_clock_calendar = NULL;
SEXP strings_clock_year_month_day = NULL;
SEXP strings_clock_year_month_weekday = NULL;
SEXP strings_clock_year_day = NULL;
SEXP strings_clock_year_week_day = NULL;
SEXP strings_clock_iso_year_week_day = NULL;
SEXP strings_clock_year_quarter_day = NULL;
SEXP strings_data_frame = NULL;

SEXP syms_precision = NULL;
SEXP syms_start = NULL;
SEXP syms_clock = NULL;
SEXP syms_zone = NULL;
SEXP syms_set_names = NULL;

SEXP classes_duration = NULL;
SEXP classes_sys_time = NULL;
SEXP classes_naive_time = NULL;
SEXP classes_zoned_time = NULL;
SEXP classes_year_month_day = NULL;
SEXP classes_year_month_weekday = NULL;
SEXP classes_year_day = NULL;
SEXP classes_year_week_day = NULL;
SEXP classes_iso_year_week_day = NULL;
SEXP classes_year_quarter_day = NULL;
SEXP classes_data_frame = NULL;

SEXP ints_empty = NULL;

[[cpp11::register]]
SEXP
clock_init_utils() {
  strings = Rf_allocVector(STRSXP, 17);
  R_PreserveObject(strings);
  MARK_NOT_MUTABLE(strings);

  strings_empty = Rf_mkChar("");
  SET_STRING_ELT(strings, 0, strings_empty);

  strings_vctrs_vctr = Rf_mkChar("vctrs_vctr");
  SET_STRING_ELT(strings, 1, strings_vctrs_vctr);

  strings_vctrs_rcrd = Rf_mkChar("vctrs_rcrd");
  SET_STRING_ELT(strings, 2, strings_vctrs_rcrd);

  strings_clock_rcrd = Rf_mkChar("clock_rcrd");
  SET_STRING_ELT(strings, 3, strings_clock_rcrd);

  strings_clock_time_point = Rf_mkChar("clock_time_point");
  SET_STRING_ELT(strings, 4, strings_clock_time_point);

  strings_clock_sys_time = Rf_mkChar("clock_sys_time");
  SET_STRING_ELT(strings, 5, strings_clock_sys_time);

  strings_clock_naive_time = Rf_mkChar("clock_naive_time");
  SET_STRING_ELT(strings, 6, strings_clock_naive_time);

  strings_clock_zoned_time = Rf_mkChar("clock_zoned_time");
  SET_STRING_ELT(strings, 7, strings_clock_zoned_time);

  strings_clock_duration = Rf_mkChar("clock_duration");
  SET_STRING_ELT(strings, 8, strings_clock_duration);

  strings_clock_calendar = Rf_mkChar("clock_calendar");
  SET_STRING_ELT(strings, 9, strings_clock_calendar);

  strings_clock_year_month_day = Rf_mkChar("clock_year_month_day");
  SET_STRING_ELT(strings, 10, strings_clock_year_month_day);

  strings_clock_year_month_weekday = Rf_mkChar("clock_year_month_weekday");
  SET_STRING_ELT(strings, 11, strings_clock_year_month_weekday);

  strings_clock_year_day = Rf_mkChar("clock_year_day");
  SET_STRING_ELT(strings, 12, strings_clock_year_day);

  strings_clock_year_week_day = Rf_mkChar("clock_year_week_day");
  SET_STRING_ELT(strings, 13, strings_clock_year_week_day);

  strings_clock_iso_year_week_day = Rf_mkChar("clock_iso_year_week_day");
  SET_STRING_ELT(strings, 14, strings_clock_iso_year_week_day);

  strings_clock_year_quarter_day = Rf_mkChar("clock_year_quarter_day");
  SET_STRING_ELT(strings, 15, strings_clock_year_quarter_day);

  strings_data_frame = Rf_mkChar("data.frame");
  SET_STRING_ELT(strings, 16, strings_data_frame);


  syms_precision = Rf_install("precision");
  syms_start = Rf_install("start");
  syms_clock = Rf_install("clock");
  syms_zone = Rf_install("zone");
  syms_set_names = Rf_install("names<-");


  classes_duration = Rf_allocVector(STRSXP, 4);
  R_PreserveObject(classes_duration);
  MARK_NOT_MUTABLE(classes_duration);
  SET_STRING_ELT(classes_duration, 0, strings_clock_duration);
  SET_STRING_ELT(classes_duration, 1, strings_clock_rcrd);
  SET_STRING_ELT(classes_duration, 2, strings_vctrs_rcrd);
  SET_STRING_ELT(classes_duration, 3, strings_vctrs_vctr);

  classes_sys_time = Rf_allocVector(STRSXP, 5);
  R_PreserveObject(classes_sys_time);
  MARK_NOT_MUTABLE(classes_sys_time);
  SET_STRING_ELT(classes_sys_time, 0, strings_clock_sys_time);
  SET_STRING_ELT(classes_sys_time, 1, strings_clock_time_point);
  SET_STRING_ELT(classes_sys_time, 2, strings_clock_rcrd);
  SET_STRING_ELT(classes_sys_time, 3, strings_vctrs_rcrd);
  SET_STRING_ELT(classes_sys_time, 4, strings_vctrs_vctr);

  classes_naive_time = Rf_allocVector(STRSXP, 5);
  R_PreserveObject(classes_naive_time);
  MARK_NOT_MUTABLE(classes_naive_time);
  SET_STRING_ELT(classes_naive_time, 0, strings_clock_naive_time);
  SET_STRING_ELT(classes_naive_time, 1, strings_clock_time_point);
  SET_STRING_ELT(classes_naive_time, 2, strings_clock_rcrd);
  SET_STRING_ELT(classes_naive_time, 3, strings_vctrs_rcrd);
  SET_STRING_ELT(classes_naive_time, 4, strings_vctrs_vctr);

  classes_zoned_time = Rf_allocVector(STRSXP, 4);
  R_PreserveObject(classes_zoned_time);
  MARK_NOT_MUTABLE(classes_zoned_time);
  SET_STRING_ELT(classes_zoned_time, 0, strings_clock_zoned_time);
  SET_STRING_ELT(classes_zoned_time, 1, strings_clock_rcrd);
  SET_STRING_ELT(classes_zoned_time, 2, strings_vctrs_rcrd);
  SET_STRING_ELT(classes_zoned_time, 3, strings_vctrs_vctr);

  classes_year_month_day = Rf_allocVector(STRSXP, 5);
  R_PreserveObject(classes_year_month_day);
  MARK_NOT_MUTABLE(classes_year_month_day);
  SET_STRING_ELT(classes_year_month_day, 0, strings_clock_year_month_day);
  SET_STRING_ELT(classes_year_month_day, 1, strings_clock_calendar);
  SET_STRING_ELT(classes_year_month_day, 2, strings_clock_rcrd);
  SET_STRING_ELT(classes_year_month_day, 3, strings_vctrs_rcrd);
  SET_STRING_ELT(classes_year_month_day, 4, strings_vctrs_vctr);

  classes_year_month_weekday = Rf_allocVector(STRSXP, 5);
  R_PreserveObject(classes_year_month_weekday);
  MARK_NOT_MUTABLE(classes_year_month_weekday);
  SET_STRING_ELT(classes_year_month_weekday, 0, strings_clock_year_month_weekday);
  SET_STRING_ELT(classes_year_month_weekday, 1, strings_clock_calendar);
  SET_STRING_ELT(classes_year_month_weekday, 2, strings_clock_rcrd);
  SET_STRING_ELT(classes_year_month_weekday, 3, strings_vctrs_rcrd);
  SET_STRING_ELT(classes_year_month_weekday, 4, strings_vctrs_vctr);

  classes_year_day = Rf_allocVector(STRSXP, 5);
  R_PreserveObject(classes_year_day);
  MARK_NOT_MUTABLE(classes_year_day);
  SET_STRING_ELT(classes_year_day, 0, strings_clock_year_day);
  SET_STRING_ELT(classes_year_day, 1, strings_clock_calendar);
  SET_STRING_ELT(classes_year_day, 2, strings_clock_rcrd);
  SET_STRING_ELT(classes_year_day, 3, strings_vctrs_rcrd);
  SET_STRING_ELT(classes_year_day, 4, strings_vctrs_vctr);

  classes_year_week_day = Rf_allocVector(STRSXP, 5);
  R_PreserveObject(classes_year_week_day);
  MARK_NOT_MUTABLE(classes_year_week_day);
  SET_STRING_ELT(classes_year_week_day, 0, strings_clock_year_week_day);
  SET_STRING_ELT(classes_year_week_day, 1, strings_clock_calendar);
  SET_STRING_ELT(classes_year_week_day, 2, strings_clock_rcrd);
  SET_STRING_ELT(classes_year_week_day, 3, strings_vctrs_rcrd);
  SET_STRING_ELT(classes_year_week_day, 4, strings_vctrs_vctr);

  classes_iso_year_week_day = Rf_allocVector(STRSXP, 5);
  R_PreserveObject(classes_iso_year_week_day);
  MARK_NOT_MUTABLE(classes_iso_year_week_day);
  SET_STRING_ELT(classes_iso_year_week_day, 0, strings_clock_iso_year_week_day);
  SET_STRING_ELT(classes_iso_year_week_day, 1, strings_clock_calendar);
  SET_STRING_ELT(classes_iso_year_week_day, 2, strings_clock_rcrd);
  SET_STRING_ELT(classes_iso_year_week_day, 3, strings_vctrs_rcrd);
  SET_STRING_ELT(classes_iso_year_week_day, 4, strings_vctrs_vctr);

  classes_year_quarter_day = Rf_allocVector(STRSXP, 5);
  R_PreserveObject(classes_year_quarter_day);
  MARK_NOT_MUTABLE(classes_year_quarter_day);
  SET_STRING_ELT(classes_year_quarter_day, 0, strings_clock_year_quarter_day);
  SET_STRING_ELT(classes_year_quarter_day, 1, strings_clock_calendar);
  SET_STRING_ELT(classes_year_quarter_day, 2, strings_clock_rcrd);
  SET_STRING_ELT(classes_year_quarter_day, 3, strings_vctrs_rcrd);
  SET_STRING_ELT(classes_year_quarter_day, 4, strings_vctrs_vctr);

  classes_data_frame = Rf_allocVector(STRSXP, 1);
  R_PreserveObject(classes_data_frame);
  MARK_NOT_MUTABLE(classes_data_frame);
  SET_STRING_ELT(classes_data_frame, 0, strings_data_frame);


  ints_empty = Rf_allocVector(INTSXP, 0);
  R_PreserveObject(ints_empty);
  MARK_NOT_MUTABLE(ints_empty);

  return r_null;
}
