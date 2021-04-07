#ifndef CLOCK_FAILURE_H
#define CLOCK_FAILURE_H

#include "clock.h"

// -----------------------------------------------------------------------------

namespace rclock {

class failures {
private:
    r_ssize n_;
    r_ssize first_;

public:
    CONSTCD11 failures() NOEXCEPT;

    void write(r_ssize i);
    CONSTCD11 bool any_failures() const NOEXCEPT;

    void warn_parse() const;
    void warn_format() const;
};

CONSTCD11
inline
failures::failures() NOEXCEPT
    : n_(0),
      first_(0)
    {}

inline
void
failures::write(r_ssize i) {
    if (n_ == 0) {
        first_ = i;
    }
    ++n_;
}

CONSTCD11
inline
bool
failures::any_failures() const NOEXCEPT {
    return n_ > 0;
}

inline
void
failures::warn_parse() const {
    cpp11::writable::integers n(1);
    cpp11::writable::integers first(1);
    n[0] = (int) n_;
    first[0] = (int) first_ + 1;
    auto r_warn = cpp11::package("clock")["warn_clock_parse_failures"];
    r_warn(n, first);
}

inline
void
failures::warn_format() const {
  cpp11::writable::integers n(1);
  cpp11::writable::integers first(1);
  n[0] = (int) n_;
  first[0] = (int) first_ + 1;
  auto r_warn = cpp11::package("clock")["warn_clock_format_failures"];
  r_warn(n, first);
}

} // namespace rclock

// -----------------------------------------------------------------------------
#endif // CLOCK_FAILURE_H
