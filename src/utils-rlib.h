#ifndef CIVIL_UTILS_RLIB_H
#define CIVIL_UTILS_RLIB_H

#include "r.h"

// -----------------------------------------------------------------------------

extern SEXP r_syms_x;
extern SEXP r_syms_class;
extern SEXP r_syms_names;

// -----------------------------------------------------------------------------

void r_abort(const char* fmt, ...) __attribute__((noreturn));

__attribute__((noreturn)) static inline void never_reached(const char* fn) {
  Rf_errorcall(R_NilValue, "Internal error: Reached the unreachable in `%s()`.", fn);
}

// -----------------------------------------------------------------------------

static inline enum r_type r_typeof(SEXP x) {
  return static_cast<enum r_type>(TYPEOF(x));
}

static inline const char* r_type_as_friendly_c_string(enum r_type type) {
  switch (type) {
  case r_type_null:         return "NULL";
  case r_type_symbol:       return "a symbol";
  case r_type_pairlist:     return "a pairlist node";
  case r_type_closure:      return "a function";
  case r_type_environment:  return "an environment";
  case r_type_promise:      return "an internal promise";
  case r_type_call:         return "a call";
  case r_type_special:      return "a primitive function";
  case r_type_builtin:      return "a primitive function";
  case r_type_string:       return "an internal string";
  case r_type_logical:      return "a logical vector";
  case r_type_integer:      return "an integer vector";
  case r_type_double:       return "a double vector";
  case r_type_complex:      return "a complex vector";
  case r_type_character:    return "a character vector";
  case r_type_dots:         return "an internal dots object";
  case r_type_any:          return "an internal `any` object";
  case r_type_list:         return "a list";
  case r_type_expression:   return "an expression vector";
  case r_type_bytecode:     return "an internal bytecode object";
  case r_type_pointer:      return "a pointer";
  case r_type_weakref:      return "a weak reference";
  case r_type_raw:          return "a raw vector";
  case r_type_s4:           return "an S4 object";

  case r_type_new:          return "an internal `new` object";
  case r_type_free:         return "an internal `free` object";

  case r_type_function:     return "a function";
  }

  never_reached("r_type_as_friendly_c_string");
}

static inline const char* r_sexp_as_friendly_c_string(SEXP x) {
  return r_type_as_friendly_c_string(r_typeof(x));
}

// -----------------------------------------------------------------------------

static inline bool r_is_number(SEXP x) {
  return (TYPEOF(x) == INTSXP) &&
    (Rf_length(x) == 1) &&
    (INTEGER(x)[0] != NA_INTEGER);
}

static inline bool r_is_string(SEXP x) {
  return (TYPEOF(x) == STRSXP) &&
    (Rf_length(x) == 1) &&
    (STRING_ELT(x, 0) != NA_STRING);
}

// -----------------------------------------------------------------------------

static inline bool r_dbl_is_missing(double x) {
  return ISNAN(x);
}

// -----------------------------------------------------------------------------

static inline SEXP r_new_string(const char* x) {
  return Rf_mkCharCE(x, CE_UTF8);
}

static inline SEXP r_new_scalar_character(SEXP x) {
  return Rf_ScalarString(x);
}
static inline SEXP r_new_scalar_character_from_c_string(const char* x) {
  return Rf_mkString(x);
}

static inline SEXP r_new_scalar_integer(int x) {
  return Rf_ScalarInteger(x);
}
static inline SEXP r_new_scalar_logical(int x) {
  return Rf_ScalarLogical(x);
}

static inline SEXP r_new_vector(r_type type, r_ssize size) {
  return Rf_allocVector(type, size);
}
static inline SEXP r_new_double(r_ssize size) {
  return r_new_vector(r_type_double, size);
}
static inline SEXP r_new_integer(r_ssize size) {
  return r_new_vector(r_type_integer, size);
}
static inline SEXP r_new_character(r_ssize size) {
  return r_new_vector(r_type_character, size);
}
static inline SEXP r_new_list(r_ssize size) {
  return r_new_vector(r_type_list, size);
}

// -----------------------------------------------------------------------------

static inline SEXP r_list_get(SEXP x, r_ssize i) {
  return VECTOR_ELT(x, i);
}
static inline int r_int_get(SEXP x, r_ssize i) {
  return INTEGER_ELT(x, i);
}
static inline SEXP r_chr_get(SEXP x, r_ssize i) {
  return STRING_ELT(x, i);
}

// -----------------------------------------------------------------------------

static inline void r_chr_poke(SEXP x, r_ssize i, SEXP value) {
  return SET_STRING_ELT(x, i, value);
}
static inline SEXP r_list_poke(SEXP x, r_ssize i, SEXP value) {
  return SET_VECTOR_ELT(x, i, value);
}

// -----------------------------------------------------------------------------

static inline int* r_int_deref(SEXP x) {
  return INTEGER(x);
}
static inline double* r_dbl_deref(SEXP x) {
  return REAL(x);
}

static inline const int* r_int_deref_const(SEXP x) {
  return (const int*) INTEGER(x);
}
static inline const double* r_dbl_deref_const(SEXP x) {
  return (const double*) REAL(x);
}

// -----------------------------------------------------------------------------

static inline void r_mark_precious(SEXP x) {
  return R_PreserveObject(x);
}
static inline void r_unmark_precious(SEXP x) {
  return R_ReleaseObject(x);
}

static inline bool r_is_shared(SEXP x) {
  return MAYBE_REFERENCED(x);
}
static inline void r_mark_shared(SEXP x) {
  MARK_NOT_MUTABLE(x);
}


static inline SEXP r_new_shared_vector(r_type type, r_ssize size) {
  SEXP out = r_new_vector(type, size);
  r_mark_precious(out);
  r_mark_shared(out);
  return out;
}
static inline SEXP r_new_shared_character_from_c_string(const char* name) {
  SEXP out = r_new_scalar_character_from_c_string(name);
  r_mark_precious(out);
  r_mark_shared(out);
  return out;
}

// -----------------------------------------------------------------------------

static inline r_ssize r_length(SEXP x) {
  return Rf_xlength(x);
}

static inline bool r_is_scalar(SEXP x) {
  return r_length(x) == 1;
}

// -----------------------------------------------------------------------------

static inline bool r_is_null(SEXP x) {
  return x == r_null;
}

// -----------------------------------------------------------------------------

static inline SEXP r_eval(SEXP expr, SEXP env) {
  return Rf_eval(expr, env);
}

// -----------------------------------------------------------------------------

static inline SEXP r_env_poke(SEXP env, SEXP sym, SEXP value) {
  Rf_defineVar(sym, value, env);
  return env;
}

// -----------------------------------------------------------------------------

static inline SEXP r_sym(const char* x) {
  return Rf_install(x);
}

// -----------------------------------------------------------------------------

static inline SEXP r_node_car(SEXP x) { return CAR(x); }
static inline SEXP r_node_cdr(SEXP x) { return CDR(x); }
static inline SEXP r_node_tag(SEXP x) { return TAG(x); }
static inline SEXP r_node_caar(SEXP x) { return CAAR(x); }
static inline SEXP r_node_cadr(SEXP x) { return CADR(x); }
static inline SEXP r_node_cdar(SEXP x) { return CDAR(x); }
static inline SEXP r_node_cddr(SEXP x) { return CDDR(x); }

static inline SEXP r_node_poke_car(SEXP x, SEXP newcar) {
  SETCAR(x, newcar);
  return x;
}
static inline SEXP r_node_poke_cdr(SEXP x, SEXP newcdr) {
  SETCDR(x, newcdr);
  return x;
}
static inline SEXP r_node_poke_tag(SEXP x, SEXP tag) {
  SET_TAG(x, tag);
  return x;
}
static inline SEXP r_node_poke_caar(SEXP x, SEXP newcaar) {
  SETCAR(CAR(x), newcaar);
  return x;
}
static inline SEXP r_node_poke_cadr(SEXP x, SEXP newcar) {
  SETCADR(x, newcar);
  return x;
}
static inline SEXP r_node_poke_cdar(SEXP x, SEXP newcdar) {
  SETCDR(CAR(x), newcdar);
  return x;
}
static inline SEXP r_node_poke_cddr(SEXP x, SEXP newcdr) {
  SETCDR(CDR(x), newcdr);
  return x;
}

// -----------------------------------------------------------------------------

static inline SEXP r_pairlist_find(SEXP node, SEXP tag) {
  while (node != r_null) {
    if (r_node_tag(node) == tag) {
      return node;
    }
    node = r_node_cdr(node);
  }

  return r_null;
}

static inline SEXP r_pairlist_get(SEXP node, SEXP tag) {
  return r_node_car(r_pairlist_find(node, tag));
}

// -----------------------------------------------------------------------------

static inline void r_poke_attribute(SEXP x, SEXP tag, SEXP value) {
  Rf_setAttrib(x, tag, value);
}
static inline void r_poke_class(SEXP x, SEXP value) {
  r_poke_attribute(x, r_syms_class, value);
}
static inline void r_poke_names(SEXP x, SEXP value) {
  r_poke_attribute(x, r_syms_names, value);
}


static inline SEXP r_attrib(SEXP x) {
  return ATTRIB(x);
}

// Unlike Rf_getAttrib(), this never allocates. This also doesn't bump
// refcounts or namedness.
static inline SEXP r_get_attribute(SEXP x, SEXP tag) {
  return r_pairlist_get(r_attrib(x), tag);
}
static inline SEXP r_get_names(SEXP x) {
  return r_get_attribute(x, r_syms_names);
}
static inline SEXP r_get_class(SEXP x) {
  return r_get_attribute(x, r_syms_class);
}

// -----------------------------------------------------------------------------

static inline SEXP r_clone(SEXP x) {
  return Rf_shallow_duplicate(x);
}
static inline SEXP r_maybe_clone(SEXP x) {
  if (r_is_shared(x)) {
    return r_clone(x);
  } else {
    return x;
  }
}

// -----------------------------------------------------------------------------

static inline SEXP r_int_recycle(SEXP x, r_ssize size) {
  r_ssize x_size = r_length(x);

  if (x_size == size) {
    return x;
  }
  if (x_size != 1) {
    Rf_errorcall(r_null, "`x` must be size 1 or %i.", (int) size);
  }

  int val = r_int_get(x, 0);

  SEXP out = PROTECT(r_new_integer(size));
  int* p_out = r_int_deref(out);

  for (r_ssize i = 0; i < size; ++i) {
    p_out[i] = val;
  }

  UNPROTECT(1);
  return out;
}

static inline SEXP r_chr_recycle(SEXP x, r_ssize size) {
  r_ssize x_size = r_length(x);

  if (x_size == size) {
    return x;
  }
  if (x_size != 1) {
    Rf_errorcall(r_null, "`x` must be size 1 or %i.", (int) size);
  }

  SEXP val = r_chr_get(x, 0);

  SEXP out = PROTECT(r_new_character(size));

  for (r_ssize i = 0; i < size; ++i) {
    r_chr_poke(out, i, val);
  }

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

#endif
