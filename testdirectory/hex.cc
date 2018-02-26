#include "hex.h"

/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Class 'Hex' as a container for one hex operand.
 *
 * Author: Duncan A. Buell
 * Used with permission and modified by: Katherine Haberlin
 * Date: 4 December 2017
**/

/******************************************************************************
 * Constructor
**/
Hex::Hex() {
}

/******************************************************************************
 * Constructor
**/
Hex::Hex(string text, Globals globals) {
  text_ = text;
  this->ParseHexOperand();
  globals_ = globals;
  is_null_ = false;
}

/******************************************************************************
 * Destructor
**/
Hex::~Hex() {
}

/******************************************************************************
 * Accessors and Mutators
**/

/******************************************************************************
 * Accessor for 'error_messages_'.
**/
string Hex::GetErrorMessages() const {
  string error_messages = "";
  if (is_invalid_) {
    error_messages += "\n***** ERROR -- HEX " + text_ + " IS INVALID";
  }
  return error_messages;
}

/******************************************************************************
 * Accessor for 'text_'.
**/
string Hex::GetText() const {
  return text_;
}

/******************************************************************************
 * Accessor for 'value_'.
**/
int Hex::GetValue() const {
  return value_;
}

/******************************************************************************
 * Accessor for error flags.
 * The answer to 'has an error' is 'true' iff 'is_invalid_' is true.
**/
bool Hex::HasAnError() const {
  return is_invalid_;
}

/******************************************************************************
 * Accessor for 'is_negative_'.
**/
bool Hex::IsNegative() const {
  return is_negative_;
}

/******************************************************************************
 * Accessor for 'is_null_' in the negative.
**/
bool Hex::IsNotNull() const {
  return !is_null_;
}

/******************************************************************************
 * Accessor for 'is_null_' in the positive.
**/
bool Hex::IsNull() const {
  return is_null_;
}

/******************************************************************************
 * General functions.
**/


/******************************************************************************
 * Function 'ParseHexOperand'.
 * Parses the hext into a decimal value and sets the error flags.
**/
void Hex::ParseHexOperand() {
#ifdef EBUG
#endif
  Utils::log_stream << "enter ParseHexOperand\n";

   

  string hexdigits = "0123456789ABCDEF";

  is_invalid_ = false;

  if (text_.length() != 5) {
    value_ = 0;
//    value_ = globals_.kBadHex;
    is_invalid_ = true;
  } else if (text_ == "     ") {
    is_null_ = true;
  } else {
    char char0 = text_.at(0);
    char char1 = text_.at(1);
    int value1 = hexdigits.find(char1);
    char char2 = text_.at(2);
    int value2 = hexdigits.find(char2);
    char char3 = text_.at(3);
    int value3 = hexdigits.find(char3);
    char char4 = text_.at(4);
    int value4 = hexdigits.find(char4);

    if ((value1 == string::npos) || (value2 == string::npos) ||
        (value3 == string::npos) || (value4 == string::npos)) {
//      value_ = globals_.kBadHex;
      value_ = 0;
      is_invalid_ = true;
    } else {
      value_ = value4;
      value_ += value3 * 16;
      value_ += value2 * 16 * 16;
      value_ += value1 * 16 * 16 * 16;
    }

    is_negative_ = false;
    if (char0 == '-') {
      value_ = 65536 - value_;
      is_negative_ = true;
    } else if (char0 != '+') {
      value_ = 0;
//      value_ = globals_.kBadHex;
      is_invalid_ = true;
    }
  }

  Utils::log_stream << "leave ParseHexOperand" << value_ << endl;
#ifdef EBUG
#endif
}

/******************************************************************************
 * Function 'ToString'.
 * This function formats a 'Hex' for prettyprinting.
 *
 * Returns:
 *   the prettyprint string for printing
**/
string Hex::ToString() const {
#ifdef EBUG
  Utils::log_stream << "enter ToString\n"; 
#endif
  string s = "";

  if (text_ == "nullhexoperand") {
    s += Utils::Format(".....", 5);
  } else {
    s += Utils::Format(text_, 5);
  }

#ifdef EBUG
  Utils::log_stream << "leave ToString\n"; 
#endif
  return s;
}
