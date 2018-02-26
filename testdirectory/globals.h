/****************************************************************
 * Header file for the 'Globals' class for global constants and
 * functions.
 *
 * Author/copyright:  Duncan Buell
 * Used with permission and modified by: Katherine Haberlin
 * Date: 4 December 2017
 *
**/

#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>
#include <bitset>
#include <math.h>
using namespace std;

//#include "../../Utilities/scanner.h"
//#include "../../Utilities/scanline.h"
#include "../../Utilities/utils.h"

class Globals {
  public:
    static const int kMaxMemory = 4096;

    int BitStringToDec(const string thebits) const;
    string DecToBitString(int value, const int how_many_bits) const;

  private:
};
#endif
