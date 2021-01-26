#ifndef CLOCK_LOCALE_H
#define CLOCK_LOCALE_H

#include "clock.h"
#include <locale>

// -----------------------------------------------------------------------------

enum class decimal_mark {
  period,
  comma
};

class clock_numpunct : public std::numpunct<char>
{
private:
  const enum decimal_mark mark_;

public:
  explicit clock_numpunct(const decimal_mark& mark,
                          std::size_t refs = 0)
    : std::numpunct<char>(refs),
      mark_(mark)
    {}

  using typename std::numpunct<char>::char_type;

protected:
  virtual char_type do_decimal_point() const {
    return mark_ == decimal_mark::comma ? ',' : '.';
  }
};

// -----------------------------------------------------------------------------

#endif
