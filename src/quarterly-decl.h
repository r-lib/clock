#ifndef CLOCK_QUARTERLY_DECL_H
#define CLOCK_QUARTERLY_DECL_H

#include <tzdb/date.h>

namespace quarterly
{

using days = date::days;
using years = date::years;

using quarters = std::chrono::duration
  <int, date::detail::ratio_divide<years::period, std::ratio<4>>>;

} // namespace quarterly

#endif
