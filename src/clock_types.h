#ifndef CLOCK_CLOCK_TYPES_H
#define CLOCK_CLOCK_TYPES_H

#include <cpp11/integers.hpp>
#include <cpp11/list_of.hpp>

typedef cpp11::list_of<cpp11::integers> clock_list_of_integers;
typedef cpp11::writable::list_of<cpp11::writable::integers> clock_writable_list_of_integers;

typedef cpp11::integers clock_field;
typedef cpp11::writable::integers clock_writable_field;

typedef cpp11::list_of<clock_field> clock_rcrd;
typedef cpp11::writable::list_of<clock_writable_field> clock_writable_rcrd;

#endif
