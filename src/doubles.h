#ifndef CLOCK_DOUBLES_H
#define CLOCK_DOUBLES_H

#include "clock.h"

namespace rclock {

class doubles
{
  const cpp11::doubles read_;
  cpp11::writable::doubles write_;
  bool writable_;
  r_ssize size_;

public:
  doubles() noexcept;
  doubles(const cpp11::doubles& x);
  doubles(r_ssize size);

  bool is_na(r_ssize i) const noexcept;
  r_ssize size() const noexcept;

  void assign(double x, r_ssize i);
  void assign_na(r_ssize i);

  double operator[](r_ssize i) const noexcept;

  SEXP sexp() const noexcept;
};

namespace detail {

static const cpp11::doubles empty_doubles = cpp11::doubles{};

} // namespace detail

inline
doubles::doubles() noexcept
  : read_(detail::empty_doubles),
    writable_(false),
    size_(0)
  {}

inline
doubles::doubles(const cpp11::doubles& x)
  : read_(x),
    writable_(false),
    size_(x.size())
  {}

inline
doubles::doubles(r_ssize size)
  : read_(detail::empty_doubles),
    write_(cpp11::writable::doubles(size)),
    writable_(true),
    size_(size)
  {}

inline
bool
doubles::is_na(r_ssize i) const noexcept {
  return std::isnan(this->operator[](i));
}

inline
r_ssize
doubles::size() const noexcept {
  return size_;
}

inline
void
doubles::assign(double x, r_ssize i) {
  if (!writable_) {
    write_ = cpp11::writable::doubles(read_);
    writable_ = true;
  }
  write_[i] = x;
}

inline
void
doubles::assign_na(r_ssize i) {
  return assign(r_dbl_na, i);
}

inline
double
doubles::operator[](r_ssize i) const noexcept {
  return writable_ ? write_[i] : read_[i];
}

inline
SEXP
doubles::sexp() const noexcept {
  return writable_ ? write_ : read_;
}

} // namespace rclock

#endif
