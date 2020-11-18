#ifndef CIVIL_CIVIL_TYPES_H
#define CIVIL_CIVIL_TYPES_H

#include <cpp11/integers.hpp>
#include <cpp11/list_of.hpp>

typedef cpp11::list_of<cpp11::integers> civil_list_of_integers;
typedef cpp11::writable::list_of<cpp11::writable::integers> civil_writable_list_of_integers;

typedef cpp11::integers civil_field;
typedef cpp11::writable::integers civil_writable_field;

typedef cpp11::list_of<civil_field> civil_rcrd;
typedef cpp11::writable::list_of<civil_writable_field> civil_writable_rcrd;

#endif
