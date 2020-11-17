#include "utils.h"

SEXP civil_syms_tzone = NULL;

SEXP civil_strings = NULL;
SEXP civil_strings_posixct = NULL;
SEXP civil_strings_posixt = NULL;
SEXP civil_strings_date = NULL;

SEXP civil_classes_date = NULL;
SEXP civil_classes_posixct = NULL;

// -----------------------------------------------------------------------------

void civil_init_utils() {
  civil_syms_tzone = r_sym("tzone");


  civil_strings = r_new_shared_vector(r_type_character, 3);

  civil_strings_posixct = r_new_string("POSIXct");
  r_chr_poke(civil_strings, 0, civil_strings_posixct);

  civil_strings_posixt = r_new_string("POSIXt");
  r_chr_poke(civil_strings, 1, civil_strings_posixt);

  civil_strings_date = r_new_string("Date");
  r_chr_poke(civil_strings, 2, civil_strings_date);


  civil_classes_date = r_new_shared_vector(r_type_character, 1);
  r_chr_poke(civil_classes_date, 0, civil_strings_date);

  civil_classes_posixct = r_new_shared_vector(r_type_character, 2);
  r_chr_poke(civil_classes_posixct, 0, civil_strings_posixct);
  r_chr_poke(civil_classes_posixct, 1, civil_strings_posixt);
}
