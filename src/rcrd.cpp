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

  const SEXP* p_names = STRING_PTR_RO(names);

  for (r_ssize i = 0; i < size; ++i) {
    const SEXP name = p_names[i];
    if (name == r_chr_na) {
      clock_abort("Names cannot be `NA`.");
    }
  }
}
