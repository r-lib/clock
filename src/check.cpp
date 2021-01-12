#include "clock.h"
#include "check.h"

template <enum component Component>
void
check_range_impl(const cpp11::integers& x, const char* arg) {
  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    // Should never be `NA`, so don't allow that to pass through
    check_range<Component>(x[i], arg);
  }
}

[[cpp11::register]]
void
check_range(const cpp11::integers& x,
            const cpp11::strings& component,
            const cpp11::strings& arg) {
  std::string x_arg_string = arg[0];
  const char* x_arg = x_arg_string.c_str();

  switch (parse_component(component)) {
  case component::year: return check_range_impl<component::year>(x, x_arg);
  case component::month: return check_range_impl<component::month>(x, x_arg);
  case component::day: return check_range_impl<component::day>(x, x_arg);
  case component::hour: return check_range_impl<component::hour>(x, x_arg);
  case component::minute: return check_range_impl<component::minute>(x, x_arg);
  case component::second: return check_range_impl<component::second>(x, x_arg);
  case component::millisecond: return check_range_impl<component::millisecond>(x, x_arg);
  case component::microsecond: return check_range_impl<component::microsecond>(x, x_arg);
  case component::nanosecond: return check_range_impl<component::nanosecond>(x, x_arg);
  case component::quarternum: return check_range_impl<component::quarternum>(x, x_arg);
  case component::quarterday: return check_range_impl<component::quarterday>(x, x_arg);
  case component::weeknum: return check_range_impl<component::weeknum>(x, x_arg);
  case component::weekday: return check_range_impl<component::weekday>(x, x_arg);
  case component::weekday_index: return check_range_impl<component::weekday_index>(x, x_arg);
  }
}
