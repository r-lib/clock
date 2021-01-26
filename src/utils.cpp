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

SEXP syms_precision = NULL;
SEXP syms_clock = NULL;
SEXP syms_zone = NULL;
SEXP syms_clock_rcrd_names = NULL;

SEXP classes_duration = NULL;
SEXP classes_sys_time = NULL;
SEXP classes_naive_time = NULL;
SEXP classes_zoned_time = NULL;

[[cpp11::register]]
SEXP
clock_init_utils() {
  strings = Rf_allocVector(STRSXP, 9);
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


  syms_precision = Rf_install("precision");
  syms_clock = Rf_install("clock");
  syms_zone = Rf_install("zone");
  syms_clock_rcrd_names = Rf_install("clock_rcrd:::names");


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

  return r_null;
}
