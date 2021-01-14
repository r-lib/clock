#ifndef CLOCK_INTEGERS_H
#define CLOCK_INTEGERS_H

#include "clock.h"

namespace rclock {

static const cpp11::integers clock_empty_integers = cpp11::integers{};

class integers
{
  const cpp11::integers& read_;
  cpp11::writable::integers write_;
  bool writable_;

public:
  CONSTCD11 integers() NOEXCEPT;
  CONSTCD11 integers(const cpp11::integers& x) NOEXCEPT;
  integers(r_ssize size);

  bool is_na(r_ssize i) const NOEXCEPT;
  r_ssize size() const NOEXCEPT;

  void assign(int x, r_ssize i);
  void assign_na(r_ssize i);

  int operator[](r_ssize i) const NOEXCEPT;

  SEXP sexp() const NOEXCEPT;

private:
  bool is_writable() const NOEXCEPT;
  void as_writable();
};

CONSTCD11
inline
integers::integers() NOEXCEPT
  : read_(clock_empty_integers),
    writable_(false)
  {}

CONSTCD11
inline
integers::integers(const cpp11::integers& x) NOEXCEPT
  : read_(x),
    writable_(false)
  {}

inline
integers::integers(r_ssize size)
  : read_(clock_empty_integers),
    write_(cpp11::writable::integers(size)),
    writable_(true)
  {}

inline
bool
integers::is_na(r_ssize i) const NOEXCEPT {
  return this->operator[](i) == r_int_na;
}

inline
r_ssize
integers::size() const NOEXCEPT {
  return read_.size();
}

inline
void
integers::assign(int x, r_ssize i) {
  if (!is_writable()) {
    as_writable();
  }
  write_[i] = x;
}

inline
void
integers::assign_na(r_ssize i) {
  return assign(r_int_na, i);
}

inline
int
integers::operator[](r_ssize i) const NOEXCEPT {
  return writable_ ? write_[i] : read_[i];
}

inline
SEXP
integers::sexp() const NOEXCEPT {
  return writable_ ? write_ : read_;
}

inline
bool
integers::is_writable() const NOEXCEPT {
  return writable_;
}

inline
void
integers::as_writable() {
  write_ = cpp11::writable::integers(read_);
  writable_ = true;
}

} // namespace rclock

#endif
