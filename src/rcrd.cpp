#include "rcrd.h"
#include "utils.h"
#include "enums.h"

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

  SEXP field0 = p_fields[0];
  if (TYPEOF(field0) != INTSXP && TYPEOF(field0) != REALSXP) {
    clock_abort("All clock_rcrd types have integer or double fields.");
  }
  const r_ssize size = Rf_xlength(field0);

  for (r_ssize i = 1; i < n_fields; ++i) {
    const SEXP field = p_fields[i];
    if (TYPEOF(field) != INTSXP && TYPEOF(field) != REALSXP) {
      clock_abort("All clock_rcrd types have integer or double fields.");
    }
    if (Rf_xlength(field) != size) {
      clock_abort("All fields must have the same size.");
    }
  }

  Rf_setAttrib(fields, R_ClassSymbol, classes);

  SEXP field0_names = Rf_getAttrib(field0, R_NamesSymbol);

  if (names != field0_names) {
    // If names are "new" (based on pointer comparison), set them.
    // This can remove names if `names` is `NULL`.
    // We assume `names` coming through here are validated names, for performance.
    field0 = set_names_dispatch(field0, names);
    SET_VECTOR_ELT(fields, 0, field0);
  }

  UNPROTECT(1);
  return fields;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
clock_rcrd_proxy(SEXP x) {
  const r_ssize n_fields = Rf_xlength(x);
  const SEXP* p_x = r_list_deref_const(x);

  // clock-rcrds always have at least 1 field
  const r_ssize size = Rf_xlength(p_x[0]);

  SEXP out = PROTECT(Rf_allocVector(VECSXP, n_fields));
  Rf_setAttrib(out, R_NamesSymbol, Rf_getAttrib(x, R_NamesSymbol));
  r_init_data_frame(out, size);

  for (r_ssize i = 0; i < n_fields; ++i) {
    SET_VECTOR_ELT(out, i, p_x[i]);
  }

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

SEXP
clock_rcrd_restore(SEXP x, SEXP to, SEXP classes) {
  const r_ssize n_fields = Rf_xlength(x);
  const SEXP* p_x = r_list_deref_const(x);

  SEXP out = PROTECT(Rf_allocVector(VECSXP, n_fields));
  Rf_setAttrib(out, R_NamesSymbol, Rf_getAttrib(x, R_NamesSymbol));
  Rf_setAttrib(out, R_ClassSymbol, classes);

  for (r_ssize i = 0; i < n_fields; ++i) {
    SET_VECTOR_ELT(out, i, p_x[i]);
  }

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
clock_rcrd_names(SEXP x) {
  SEXP field0 = VECTOR_ELT(x, 0);
  return Rf_getAttrib(field0, R_NamesSymbol);
}

// -----------------------------------------------------------------------------

static inline void validate_names(SEXP names, r_ssize size);

[[cpp11::register]]
SEXP
clock_rcrd_set_names(SEXP x, SEXP names) {
  SEXP field0 = VECTOR_ELT(x, 0);
  SEXP field0_names = Rf_getAttrib(field0, R_NamesSymbol);

  if (field0_names == names) {
    // New and old were either `NULL`, or we are using the exact same chr SEXP
    return x;
  }

  x = PROTECT(r_clone_referenced(x));

  if (names != r_null) {
    const r_ssize size = Rf_xlength(field0);
    validate_names(names, size);
  }

  field0 = set_names_dispatch(field0, names);
  SET_VECTOR_ELT(x, 0, field0);

  UNPROTECT(1);
  return x;
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
