#include "symbol.h"

/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Class 'Symbol' as a container for one symbol.
 *
 * Author: Duncan A. Buell
 * Used with permission and modified by: Katherine Haberlin
 * Date: 4 December 2017
**/

/******************************************************************************
 * Constructor
**/
Symbol::Symbol() {
}

/******************************************************************************
 * Constructor
**/
Symbol::Symbol(string text, int programcounter) {
  location_ = programcounter;
  is_multiply_ = false;
  text_ = text;
  is_invalid_ = this->CheckInvalid();
}

/******************************************************************************
 * Destructor
**/
Symbol::~Symbol() {
}

/******************************************************************************
 * Accessors and Mutators
**/

/******************************************************************************
 * Accessor for 'error_messages_'.
 * There is a kluge here for getting the internal newline character.
**/
string Symbol::GetErrorMessages() const {
  bool previouserror = false;
  string error_messages = "";

  if (is_invalid_) {
    error_messages += "***** ERROR -- SYMBOL " + text_ + " IS INVALID";
    previouserror = true;
  }
  if (is_multiply_) {
    if (previouserror) {
      error_messages += "\n";
    }
    error_messages += "***** ERROR -- SYMBOL " + text_ + " IS MULTIPLY DEFINED";
    previouserror = true; // set this just in case we add more cases later
  }

  return error_messages;
}

/******************************************************************************
 * Accessor for the 'location_'.
**/
int Symbol::GetLocation() const {
  return location_;
}

/******************************************************************************
 * Accessor for the existence of errors.
**/
bool Symbol::HasAnError() const {
  return (is_invalid_ || is_multiply_);
}

/******************************************************************************
 * Mutator 'SetMultiply'.
 * Sets the 'is_multiply' value to 'true'.
**/
void Symbol::SetMultiply() {
  is_multiply_ = true;
}

/******************************************************************************
 * General functions.
**/

/******************************************************************************
 * Function 'CheckInvalid'.
 * Returns the boolean to say whether a symbol is invalid.
**/
bool Symbol::CheckInvalid() const {
  bool returnvalue = false; // false means no, not invalid

  return returnvalue;
}

/******************************************************************************
 * Function 'ToString'.
 * This function formats an 'Symbol' for prettyprinting.
 *
 * Returns:
 *   the prettyprint string for printing
**/
string Symbol::ToString() const {
#ifdef EBUG
  Utils::log_stream << "enter ToString\n"; 
#endif
  string s = "";

  if (text_ == "nullsymbol") {
    s += Utils::Format("sss", 3);
  } else {
    s += Utils::Format(text_, 3);
  }

  s += Utils::Format(location_, 4);
  if (is_invalid_) {
    s += " INVALID";
  }
  if (is_multiply_) {
    s += " MULTIPLY";
  }

#ifdef EBUG
  Utils::log_stream << "leave ToString\n"; 
#endif
  return s;
}
