#include "pullet16assembler.h"

/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Class 'Assembler' for assembling code.
 *
 * Author: Duncan A. Buell
 * Used with permission and modified by: Katherine Haberlin
 * Date: 4 December 2017
**/

/******************************************************************************
 * Constructor
**/
Assembler::Assembler() {
  globals_ = Globals();

}

/******************************************************************************
 * Destructor
**/
Assembler::~Assembler() {
}

/******************************************************************************
 * Accessors and Mutators
**/

/******************************************************************************
 * General functions.
**/

/******************************************************************************
 * Function 'Assemble'.
 * This top level function assembles the code.
 *
 * Parameters:
 *   in_scanner - the scanner to read for source code
 *   out_stream - the output stream to write to
**/
void Assembler::Assemble(Scanner& in_scanner, string binary_filename, ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter Assemble\n"; 
#endif

  //Create table of valid mnemonics using a set.
  this->ValidMnemonics();

  ////////////////////////////////////////////////////////////////////////////
  // Pass one
  // Produce the symbol table and detect errors in symbols.
  Utils::log_stream << endl << endl << "PASS ONE" << endl;
  out_stream << "PASS ONE\n" << endl;
  cout << "PASS ONE" << endl;
  this->PassOne(in_scanner, out_stream); 

  ////////////////////////////////////////////////////////////////////////////
  // Pass two
  // Generate the machine code.
  Utils::log_stream << endl << endl << "PASS TWO" << endl;
  out_stream << "PASS TWO\n" << endl;
  cout << "PASS TWO" << endl;
  this->PassTwo();

  ////////////////////////////////////////////////////////////////////////////
  // Dump the results.
  this->PrintCodeLines(out_stream);
  this->PrintSymbolTable(out_stream);
  this->PrintMachineCode(binary_filename, out_stream);

#ifdef EBUG
  Utils::log_stream << "leave Assemble\n"; 
#endif
}

/******************************************************************************
 * Function 'GetInvalidMessage'.
 * This creates a "value is invalid" error message.
 *
 * Parameters:
 *   leadingtext - the text of what it is that is invalid
 *   symbol - the symbol that is invalid
**/
string Assembler::GetInvalidMessage(string leadingtext, string symbol) {

  string returnvalue = leadingtext;
  has_an_error_ = true;
  return returnvalue;
}

/******************************************************************************
 * Function 'GetInvalidMessage'.
 * This creates a "value is invalid" error message.
 *
 * Parameters:
 *   leadingtext - the text of what it is that is invalid
 *   hex - the hex operand that is invalid
**/
string Assembler::GetInvalidMessage(string leadingtext, Hex hex) {

  string returnvalue = "";
  has_an_error_ = true;

  return returnvalue;
}

/******************************************************************************
 * Function 'GetUndefinedMessage'.
 * This creates a "symbol is undefined" error message.
 *
 * Parameters:
 *   badtext - the undefined symbol text
**/
string Assembler::GetUndefinedMessage(string badtext) {

  string returnvalue = "";
  has_an_error_ = true;

  return returnvalue;
}

/******************************************************************************
 * Function 'PassOne'.
 * Produce the symbol table and detect multiply defined symbols.
 *
 * CAVEAT: We have deliberately forced symbols and mnemonics to have
 *         blank spaces at the end and thus to be all the same length.
 *         Symbols are three characters, possibly with one or two blank at end.
 *         Mnemonics are three characters, possibly with one blank at end.
 *
 * Parameters:
 *   in_scanner - the input stream from which to read
 *   out-stream - the output stream to which to write
**/
void Assembler::PassOne(Scanner& in_scanner, ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter PassOne\n"; 
#endif

  int linecounter = 0;  
  pc_in_assembler_ = 0;
  has_an_error_ = false;

  //Read one line at a time.
  string line = in_scanner.NextLine();
  while (line.length() > 0) {
       
  line.resize(80);

  //Create instance of Codeline
  CodeLine code_line;
  while (line.substr(0, 1) == "*") {
    code_line.SetCommentsOnly(linecounter, line);
    line = in_scanner.NextLine();
    line.resize(80);
  } 

  //Break the line down into substrings.
  string label = line.substr(0, 3);
  if (label == "   ") {
    label = "nulllabel";
  }
  string mnemonic = line.substr(4, 3);
  if (mnemonic == "   ") {
    mnemonic = "nullmnemonic";
  }
  string addr = line.substr(8, 1);
  string symoperand = line.substr(10, 3);
  if (symoperand == "   ") {
    symoperand = "nullsymoperand";
  }
  string hexoperand = line.substr(14, 5);
  string comments = line.substr(20);
  string code = "nullcode";

  code_line.SetCodeLine(linecounter, pc_in_assembler_, label, mnemonic,
                        addr, symoperand, hexoperand, comments, code);
  codelines_.push_back(code_line);
      
  //Update Symbol Table
  if (label != "nulllabel") {
    this->UpdateSymbolTable(pc_in_assembler_, label);
  }

  //Check for END Statement
  if (mnemonic == "END") {
    found_end_statement_ = true;
  }
    
  //Increment and retrieve next line
  linecounter += 1;
  pc_in_assembler_ += 1;
  line = in_scanner.NextLine();
  }

  //Print Codelines
  this->PrintCodeLines(out_stream);

  maxpc_ = linecounter;

#ifdef EBUG
  Utils::log_stream << "leave PassOne\n"; 
#endif
}

/******************************************************************************
 * Function 'PassTwo'.
 * This function does pass two of the assembly process.
 * It creates the machine code for each codeline.
 * The default value for saving space is '1111111111111111'
**/
void Assembler::PassTwo() {
#ifdef EBUG
  Utils::log_stream << "enter PassTwo\n"; 
#endif

  //Run through codelines
  //If opcode is valid, create the string of machine code
  pc_in_assembler_ = 0;
  bool set_machine_code;

  for (auto it = codelines_.begin(); it != codelines_.end(); ++it) {

    if (has_an_error_  || !found_end_statement_) {
      break;
    }

    set_machine_code = true;
    CodeLine codeline = (*it);

    this->SetNewPC(codeline);

    string mnemonic = codeline.GetMnemonic();
    Hex hex = codeline.GetHexObject();
    int hex_dec = hex.GetValue();
    string code = "";

    string addr = codeline.GetAddr();
    string symoperand = codeline.GetSymOperand();
    string full_address = this->FindAddress(addr, symoperand);    

    if (mnemonic.find("RD") != std::string::npos) {
      mnemonic = "RD ";
    }

    if (mnemonics_.count(mnemonic) != 0) { 
      
      if (mnemonic == "BAN") {
        code += "000" + full_address;
      } else if (mnemonic == "SUB") {
        code += "001" + full_address;
      } else if (mnemonic == "STC") {
        code += "010" + full_address;
      } else if (mnemonic == "AND") {
        code += "011" + full_address;
      } else if (mnemonic == "ADD") {
        code += "100" + full_address;
      } else if (mnemonic == "LD ") {
        code += "101" + full_address;
      } else if (mnemonic == "BR ") {
        code += "110" + full_address;
      } else if(mnemonic == "RD ") {
        code = "1110000000000001";
      } else if (mnemonic == "STP") {
        code = "1110000000000010";
      } else if (mnemonic == "WRT") {
        code = "1110000000000011";
      } else if (mnemonic == "ORG") {
        set_machine_code = false;
        pc_in_assembler_ = hex_dec - 1;
      } else if (mnemonic == "DS ") {
        set_machine_code = false;
        for (int i = 0; i < hex_dec; i ++) {
          machinecode_[pc_in_assembler_ + i] = "1111111111111111";
        }
      } else if (mnemonic == "END") {
        set_machine_code = false;
        break;
      } else if (mnemonic == "HEX") {
        code = globals_.DecToBitString(hex_dec, 16);
      }
    } else {
      this->GetInvalidMessage("INVALID MNEMONIC", mnemonic); 
      cout << "INVALID MNEMONIC" << endl;
    }//if

    if (set_machine_code == true) {
      codeline.SetMachineCode(code);
      machinecode_[pc_in_assembler_] = code;
    }
    
  }//for

#ifdef EBUG
  Utils::log_stream << "leave PassTwo\n"; 
#endif
}

/******************************************************************************
 * Function 'PrintCodeLines'.
 * This function prints the code lines.
**/
void Assembler::PrintCodeLines(ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter PrintCodeLines\n"; 
#endif
  string s = "";

  for (auto iter = codelines_.begin(); iter != codelines_.end(); ++iter) {
    s += (*iter).ToString() + '\n';
  }

  if (!found_end_statement_) {
    s += "\n***** ERROR -- NO 'END' STATEMENT\n";
    has_an_error_ = true;
  }

  cout << s << endl;
  out_stream << s << endl;
  Utils::log_stream << s << endl;

#ifdef EBUG
  Utils::log_stream << "leave PrintCodeLines\n"; 
#endif
}

/******************************************************************************
 * Function 'PrintMachineCode'.
 * This function prints the machine code.
**/
void Assembler::PrintMachineCode(string binary_filename, ofstream& out_stream) {
#ifdef EBUG
#endif
  string s = "MACHINE CODE \n";

  Utils::log_stream << "enter PrintMachineCode" << " "
                    << binary_filename << endl; 

  //Print to standard output, output stream, logstream
  for (auto it = machinecode_.begin(); it != machinecode_.end(); ++it) {
    s += it->second + "\n";
  }

  cout << s << endl;
  out_stream << s << endl;
  Utils::log_stream << s << endl;

  this->WriteBinaryFile(binary_filename);

#ifdef EBUG
  Utils::log_stream << "leave PrintMachineCode" << endl; 
#endif
}

/******************************************************************************
 * Function 'PrintSymbolTable'.
 * This function prints the symbol table.
**/
void Assembler::PrintSymbolTable(ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter PrintSymbolTable\n"; 
#endif
  string s = "\n SYMBOL TABLE\n    SYM LOC FLAGS\n";

  for (int i = 0; i < symbol_vector_.size(); ++i) {
    s += symbol_vector_.at(i) + "\n";
  }

  cout << s << endl;
  out_stream << s << endl;
  Utils::log_stream << s << endl;


#ifdef EBUG
  Utils::log_stream << "leave PrintSymbolTable\n"; 
#endif
}

/******************************************************************************
 * Function 'SetNewPC'.
 * This function gets a new value for the program counter.
 *
 * No return value--this sets the class's PC variable.
 *
 * Parameters:
 *   codeline - the line of code from which to update
**/
void Assembler::SetNewPC(CodeLine codeline) {
#ifdef EBUG
  Utils::log_stream << "enter SetNewPC\n"; 
#endif
  
  pc_in_assembler_ = codeline.GetPC();

#ifdef EBUG
  Utils::log_stream << "leave SetNewPC\n"; 
#endif
}

/******************************************************************************
 * Function 'UpdateSymbolTable'.
 * This function updates the symbol table for a putative symbol.
 * Note that there is a hack here, in that the default value is 0
 * and that would mean we can't store a symbol at location zero.
 * So we add one, and then back that out after the full first pass is done.
 *
 * Parameters:
 *   pc - the program counter
 *   symboltext - the label of a codeline
**/
void Assembler::UpdateSymbolTable(int pc, string symboltext) {
#ifdef EBUG
  Utils::log_stream << "enter UpdateSymbolTable\n"; 
#endif

  string s = "SYM ";

  if (symboltext != "nullsymoperand") {
    s += symboltext + " " + Utils::Format(pc) + " ";
    if (symboltable_.count(symboltext) == 0) {
      symboltable_[symboltext] = pc;
    } else {
      s += this->GetInvalidMessage("SYMBOL ALREADY USED", symboltext);
    }
    symbol_vector_.push_back(s);
  }
#ifdef EBUG
  Utils::log_stream << "leave UpdateSymbolTable\n"; 
#endif
}

/******************************************************************************
 * Function 'ValidMnemonics'.
 * This creates a set of valid mnemonic opcodes.
 *
**/
void Assembler::ValidMnemonics() {

#ifdef EBUG
  Utils::log_stream << "enter ValidMnemonics\n";
#endif

  //Format I
  mnemonics_.insert("BAN");
  mnemonics_.insert("SUB");
  mnemonics_.insert("STC");
  mnemonics_.insert("AND");
  mnemonics_.insert("ADD");
  mnemonics_.insert("LD ");
  mnemonics_.insert("BR ");

  //Format II 
  mnemonics_.insert("RD ");
  mnemonics_.insert("STP");
  mnemonics_.insert("WRT");

  //Pseudo-Op Instructions
  mnemonics_.insert("ORG");
  mnemonics_.insert("END");
  mnemonics_.insert("HEX");
  mnemonics_.insert("DS ");

  //Printing
  Utils::log_stream << "VALID MNEMOICS:\n";
  for (string const& mnemonic : mnemonics_) {
    Utils::log_stream << mnemonic << "\n";
  }
 
#ifdef EBUG
  Utils::log_stream << "leave ValidMnemonics\n";
#endif
  
}

/******************************************************************************
 * Function 'FindAddress'.
 * Find the address of the function
 *
 * Parameters:
 *   addr - for indirect addressing
 *   symoperand - the symbol to be searched for in the symbol table
**/
string Assembler::FindAddress(string addr, string symoperand) {
#ifdef EBUG
  Utils::log_stream << "enter FindAddress\n";
#endif
  
  string ret_string = "0";

  int location = symboltable_[symoperand];

  if (addr == "*") {
    CodeLine indirect = codelines_.at(location);
    location = symboltable_[indirect.GetSymOperand()];
    ret_string = "1";
  }
  CodeLine temp = codelines_.at(location);
  Hex hex = temp.GetHexObject();
  int hex_dec = hex.GetValue();

  ret_string += globals_.DecToBitString(hex_dec, 12);
  
  return ret_string;

#ifdef EBUG
  Utils::log_stream << "leave FindAddress\n";
#endif
}

/******************************************************************************
 * Function 'WriteBinaryFile'.
 * Writes a binary file of machine code
**/

void Assembler::WriteBinaryFile(string binary_filename) {
#ifdef EBUG
  Utils::log_stream << "enter WriteBinaryFile\n";
#endif

  FILE *fp = fopen(binary_filename.c_str(), "w");

  int size = machinecode_.size();

  for (int i = 0; i < size; ++i) {
    string machine_string = machinecode_[i];
    int machine_decimal = globals_.BitStringToDec(machine_string);
    short machine_binary = static_cast<short>(machine_decimal);
    fwrite(&machine_binary, 2, 1, fp);
  }

  fclose(fp);

#ifdef EBUG
  Utils::log_stream << "leave WriteBinaryFile\n";
#endif
}
