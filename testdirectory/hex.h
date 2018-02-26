/****************************************************************
 * Header file for the 'Hex' class to contain one hex operand.
 *
 * Author/copyright:  Duncan Buell
 * Used with permission and modified by: Katherine Haberlin
 * Date: 4 December 2017
 *
**/

#ifndef HEX_H
#define HEX_H

#include <iostream>
using namespace std;

//#include "../../Utilities/scanner.h"
//#include "../../Utilities/scanline.h"
#include "../../Utilities/utils.h"

#include "globals.h"

class Hex {
  public:
    Hex();
    Hex(string hexoperand, Globals globals);
    virtual ~Hex();

    string GetErrorMessages() const;
    int GetValue() const;
    string GetText() const;
    bool HasAnError() const;
    bool IsNegative() const;
    bool IsNotNull() const;
    bool IsNull() const;
    string ToString() const;

  private:
    bool is_invalid_;
    bool is_negative_;
    bool is_null_;
    int value_;
    string error_messages_;
    string text_;

    Globals globals_;

    void ParseHexOperand();
};

#endif
