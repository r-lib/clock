#include "rcrd.h"
#include "utils.h"
#include "enums.h"

static inline void validate_names(SEXP names, r_ssize size);

SEXP
new_clock_rcrd_from_fields(SEXP fields, SEXP names, SEXP classes) {
  if (TYPEOF(fields) != VECSXP) {
    clock_abort("`fields` must be a list.");
  }
  if (TYPEOF(classes) != STRSXP) {
    clock_abort("`classes` must be a character vector.");
  }

  fields = PROTECT(r_clone_referenced(fields));

  // Clear all attributes except for `names`, as we often
  // pass in other duration or time point objects as `fields`
  SEXP field_names = Rf_getAttrib(fields, R_NamesSymbol);
  SET_ATTRIB(fields, r_null);
  Rf_setAttrib(fields, R_NamesSymbol, field_names);

  const r_ssize n_fields = Rf_xlength(fields);
  if (n_fields == 0L) {
    clock_abort("There must be at least 1 field.");
  }

  const SEXP* p_fields = r_list_deref_const(fields);

  const SEXP field = p_fields[0];
  if (TYPEOF(field) != INTSXP) {
    clock_abort("All clock_rcrd types have integer fields.");
  }
  const r_ssize size = Rf_xlength(field);

  for (r_ssize i = 1; i < n_fields; ++i) {
    const SEXP field = p_fields[i];
    if (TYPEOF(field) != INTSXP) {
      clock_abort("All clock_rcrd types have integer fields.");
    }
    if (Rf_xlength(field) != size) {
      clock_abort("All fields must have the same size.");
    }
  }

  Rf_setAttrib(fields, R_ClassSymbol, classes);

  if (names != r_null) {
    validate_names(names, size);
    Rf_setAttrib(fields, syms_clock_rcrd_names, names);
  }

  UNPROTECT(1);
  return fields;
}

static
inline
void
validate_names(SEXP names, r_ssize size) {
  if (TYPEOF(names) != STRSXP) {
    clock_abort("Names must be a character vector.");
  }

  const r_ssize names_size = Rf_xlength(names);

  if (names_size != size) {
    clock_abort("Names must have length %i, not %i.", size, names_size);
  }

  const SEXP* p_names = r_chr_deref_const(names);

  for (r_ssize i = 0; i < size; ++i) {
    const SEXP name = p_names[i];
    if (name == r_chr_na) {
      clock_abort("Names cannot be `NA`.");
    }
  }
}

// -----------------------------------------------------------------------------

SEXP
clock_rcrd_restore(SEXP x, SEXP to, SEXP classes) {
  const r_ssize size = Rf_xlength(x);

  SEXP field_names = Rf_getAttrib(x, R_NamesSymbol);
  const SEXP* p_field_names = r_chr_deref_const(field_names);

  const SEXP last_field_name = p_field_names[size - 1];
  const char* last_field_name_char = CHAR(last_field_name);

  // Check if the last field name matches `clock_rcrd:::names`
  const bool has_names = !strcmp(last_field_name_char, cpp_strings_clock_rcrd_names.c_str());

  if (!has_names) {
    // No names, so restore new clock_rcrd without any names
    return new_clock_rcrd_from_fields(x, r_null, classes);
  }

  const SEXP* p_x = r_list_deref_const(x);

  // Extract and possibly repair the names
  SEXP names = p_x[size - 1];
  names = PROTECT(r_repair_na_names(names));

  const r_ssize new_size = size - 1;
  SEXP new_field_names = PROTECT(Rf_allocVector(STRSXP, new_size));
  SEXP new_x = PROTECT(Rf_allocVector(VECSXP, new_size));

  for (r_ssize i = 0; i < new_size; ++i) {
    SET_STRING_ELT(new_field_names, i, p_field_names[i]);
    SET_VECTOR_ELT(new_x, i, p_x[i]);
  }

  Rf_setAttrib(new_x, R_NamesSymbol, new_field_names);

  SEXP out = new_clock_rcrd_from_fields(new_x, names, classes);

  UNPROTECT(3);
  return out;
}
