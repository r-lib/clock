#ifndef CLOCK_INTEGERS_H
#define CLOCK_INTEGERS_H

#include "clock.h"

namespace rclock {

class integers
{
  const cpp11::integers read_;
  cpp11::writable::integers write_;
  bool writable_;
  r_ssize size_;

public:
  integers() noexcept;
  integers(const cpp11::integers& x);
  integers(r_ssize size);

  bool is_na(r_ssize i) const noexcept;
  r_ssize size() const noexcept;

  void assign(int x, r_ssize i);
  void assign_na(r_ssize i);

  int operator[](r_ssize i) const noexcept;

  SEXP sexp() const noexcept;
};

namespace detail {

static const cpp11::integers empty_integers = cpp11::integers{};

} // namespace detail

inline
integers::integers() noexcept
  : read_(detail::empty_integers),
    writable_(false),
    size_(0)
  {}

inline
integers::integers(const cpp11::integers& x)
  : read_(x),
    writable_(false),
    size_(x.size())
  {}

inline
integers::integers(r_ssize size)
  : read_(detail::empty_integers),
    write_(cpp11::writable::integers(size)),
    writable_(true),
    size_(size)
  {}

inline
bool
integers::is_na(r_ssize i) const noexcept {
  return this->operator[](i) == r_int_na;
}

inline
r_ssize
integers::size() const noexcept {
  return size_;
}

inline
void
integers::assign(int x, r_ssize i) {
  if (!writable_) {
    write_ = cpp11::writable::integers(read_);
    writable_ = true;
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
integers::operator[](r_ssize i) const noexcept {
  return writable_ ? write_[i] : read_[i];
}

inline
SEXP
integers::sexp() const noexcept {
  return writable_ ? write_ : read_;
}

} // namespace rclock

#endif
