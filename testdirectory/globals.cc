#include "globals.h"

/****************************************************************
 * Code file for the 'Globals' class for global constants and
 * functions.
 *
 * Author/copyright:  Duncan Buell
 * Used with permission and modified by: Katherine Haberlin
 * Date: 4 December 2017
 *
**/

/******************************************************************************
 * Function 'BitStringToDec'.
 * Convert a bit string to a decimal value.
**/
int Globals::BitStringToDec(const string thebits) const {
#ifdef EBUG
  Utils::log_stream << "enter BitStringToDec\n"; 
#endif

  //int stoivalue = std::stoi(thebits, nullptr, 2);
  
  int stoivalue = 0;
  int size = thebits.length();

  for (int i = 0; i < size; ++i) {
    if (thebits.substr(i, 1) == "1") {
      stoivalue += pow(2, (size - i - 1));
    }
  }

  return stoivalue;
}

/******************************************************************************
 * Function 'DecToBitString'.
 * This function converts a decimal 'int' to a string of 0s and 1s.
 *
 * Parameters:
 *   what - the value to convert
 *   how_many_bits - the length of the result
**/
string Globals::DecToBitString(int value, const int how_many_bits) const {
#ifdef EBUG
  Utils::log_stream << "enter DecToBitString\n";
#endif

  string bitsetvalue = "";
  if (how_many_bits == 12) {
    bitsetvalue = std::bitset<12>(value).to_string();
  }
  else if (how_many_bits == 16) {
    bitsetvalue = std::bitset<16>(value).to_string();
  } else {
    Utils::log_stream << "ERROR DECTOBITSTRING " << value << " " << how_many_bits << endl;
    exit(0);
  }

#ifdef EBUG
  Utils::log_stream << "leave DecToBitString\n";
#endif

  return bitsetvalue;
}
