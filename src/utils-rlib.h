#ifndef CIVIL_UTILS_RLIB_H
#define CIVIL_UTILS_RLIB_H

#include "r.h"

// -----------------------------------------------------------------------------

extern sexp r_syms_x;
extern sexp r_syms_class;
extern sexp r_syms_names;

// -----------------------------------------------------------------------------

void r_abort(const char* fmt, ...) __attribute__((noreturn));

__attribute__((noreturn)) static inline void never_reached(const char* fn) {
  Rf_errorcall(R_NilValue, "Internal error: Reached the unreachable in `%s()`.", fn);
}

// -----------------------------------------------------------------------------

static inline enum r_type r_typeof(sexp x) {
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

static inline const char* r_sexp_as_friendly_c_string(sexp x) {
  return r_type_as_friendly_c_string(r_typeof(x));
}

// -----------------------------------------------------------------------------

static inline bool r_is_number(sexp x) {
  return (TYPEOF(x) == INTSXP) &&
    (Rf_length(x) == 1) &&
    (INTEGER(x)[0] != NA_INTEGER);
}

static inline bool r_is_string(sexp x) {
  return (TYPEOF(x) == STRSXP) &&
    (Rf_length(x) == 1) &&
    (STRING_ELT(x, 0) != NA_STRING);
}

// -----------------------------------------------------------------------------

static inline bool r_dbl_is_missing(double x) {
  return ISNAN(x);
}

// -----------------------------------------------------------------------------

static inline sexp r_new_string(const char* x) {
  return Rf_mkCharCE(x, CE_UTF8);
}

static inline sexp r_new_scalar_character(sexp x) {
  return Rf_ScalarString(x);
}
static inline sexp r_new_scalar_character_from_c_string(const char* x) {
  return Rf_mkString(x);
}

static inline sexp r_new_scalar_integer(int x) {
  return Rf_ScalarInteger(x);
}
static inline sexp r_new_scalar_logical(int x) {
  return Rf_ScalarLogical(x);
}

static inline sexp r_new_vector(r_type type, r_ssize size) {
  return Rf_allocVector(type, size);
}
static inline sexp r_new_double(r_ssize size) {
  return r_new_vector(r_type_double, size);
}

// -----------------------------------------------------------------------------

static inline sexp r_list_get(sexp x, r_ssize i) {
  return VECTOR_ELT(x, i);
}
static inline int r_int_get(sexp x, r_ssize i) {
  return INTEGER_ELT(x, i);
}

// -----------------------------------------------------------------------------

static inline void r_chr_poke(sexp x, r_ssize i, sexp value) {
  return SET_STRING_ELT(x, i, value);
}

// -----------------------------------------------------------------------------

static inline double* r_dbl_deref(sexp x) {
  return REAL(x);
}

static inline const int* r_int_deref_const(sexp x) {
  return (const int*) INTEGER(x);
}
static inline const double* r_dbl_deref_const(sexp x) {
  return (const double*) REAL(x);
}

// -----------------------------------------------------------------------------

static inline void r_mark_precious(sexp x) {
  return R_PreserveObject(x);
}
static inline void r_unmark_precious(sexp x) {
  return R_ReleaseObject(x);
}

static inline void r_mark_shared(sexp x) {
  MARK_NOT_MUTABLE(x);
}

static inline sexp r_new_shared_vector(r_type type, r_ssize size) {
  sexp out = r_new_vector(type, size);
  r_mark_precious(out);
  r_mark_shared(out);
  return out;
}
static inline sexp r_new_shared_character_from_c_string(const char* name) {
  SEXP out = r_new_scalar_character_from_c_string(name);
  r_mark_precious(out);
  r_mark_shared(out);
  return out;
}

// -----------------------------------------------------------------------------

static inline r_ssize r_length(sexp x) {
  return Rf_xlength(x);
}

static inline bool r_is_scalar(sexp x) {
  return r_length(x) == 1;
}

// -----------------------------------------------------------------------------

static inline bool r_is_null(sexp x) {
  return x == r_null;
}

// -----------------------------------------------------------------------------

static inline sexp r_eval(sexp expr, sexp env) {
  return Rf_eval(expr, env);
}

// -----------------------------------------------------------------------------

static inline sexp r_env_poke(sexp env, sexp sym, sexp value) {
  Rf_defineVar(sym, value, env);
  return env;
}

// -----------------------------------------------------------------------------

static inline sexp r_sym(const char* x) {
  return Rf_install(x);
}

// -----------------------------------------------------------------------------

static inline sexp r_node_car(sexp x) { return CAR(x); }
static inline sexp r_node_cdr(sexp x) { return CDR(x); }
static inline sexp r_node_tag(sexp x) { return TAG(x); }
static inline sexp r_node_caar(sexp x) { return CAAR(x); }
static inline sexp r_node_cadr(sexp x) { return CADR(x); }
static inline sexp r_node_cdar(sexp x) { return CDAR(x); }
static inline sexp r_node_cddr(sexp x) { return CDDR(x); }

static inline sexp r_node_poke_car(sexp x, sexp newcar) {
  SETCAR(x, newcar);
  return x;
}
static inline sexp r_node_poke_cdr(sexp x, sexp newcdr) {
  SETCDR(x, newcdr);
  return x;
}
static inline sexp r_node_poke_tag(sexp x, sexp tag) {
  SET_TAG(x, tag);
  return x;
}
static inline sexp r_node_poke_caar(sexp x, sexp newcaar) {
  SETCAR(CAR(x), newcaar);
  return x;
}
static inline sexp r_node_poke_cadr(sexp x, sexp newcar) {
  SETCADR(x, newcar);
  return x;
}
static inline sexp r_node_poke_cdar(sexp x, sexp newcdar) {
  SETCDR(CAR(x), newcdar);
  return x;
}
static inline sexp r_node_poke_cddr(sexp x, sexp newcdr) {
  SETCDR(CDR(x), newcdr);
  return x;
}

// -----------------------------------------------------------------------------

static inline sexp r_pairlist_find(sexp node, sexp tag) {
  while (node != r_null) {
    if (r_node_tag(node) == tag) {
      return node;
    }
    node = r_node_cdr(node);
  }

  return r_null;
}

static inline sexp r_pairlist_get(sexp node, sexp tag) {
  return r_node_car(r_pairlist_find(node, tag));
}

// -----------------------------------------------------------------------------

static inline void r_poke_attribute(sexp x, sexp tag, sexp value) {
  Rf_setAttrib(x, tag, value);
}
static inline void r_poke_class(sexp x, sexp value) {
  r_poke_attribute(x, r_syms_class, value);
}
static inline void r_poke_names(sexp x, sexp value) {
  r_poke_attribute(x, r_syms_names, value);
}


static inline sexp r_attrib(sexp x) {
  return ATTRIB(x);
}

// Unlike Rf_getAttrib(), this never allocates. This also doesn't bump
// refcounts or namedness.
static inline sexp r_get_attribute(sexp x, sexp tag) {
  return r_pairlist_get(r_attrib(x), tag);
}
static inline sexp r_get_names(sexp x) {
  return r_get_attribute(x, r_syms_names);
}
static inline sexp r_get_class(sexp x) {
  return r_get_attribute(x, r_syms_class);
}

// -----------------------------------------------------------------------------

#endif
