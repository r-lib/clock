#ifndef CIVIL_LOCALE_H
#define CIVIL_LOCALE_H

#include "civil.h"
#include "utils.h"
#include <locale>

static inline std::locale civil_load_locale(const std::string& locale) {
  try {
    return std::locale{locale};
  }
  catch (const std::runtime_error& error) {
    civil_abort("Failed to load locale.");
  }
}

#endif
