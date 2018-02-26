/****************************************************************
 * Header file for the Pullet16 assembler.
 *
 * Author/copyright:  Duncan Buell
 * Used with permission and modified by: Katherine Haberlin
 * Date: 4 December 2017
 *
**/

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <fstream>
using namespace std;

#include "../../Utilities/scanner.h"
#include "../../Utilities/scanline.h"
#include "../../Utilities/utils.h"

#include "globals.h"
#include "codeline.h"
#include "hex.h"
#include "symbol.h"

class Assembler {
  public:
    Assembler();
    virtual ~Assembler();

    void Assemble(Scanner& in_scanner, string binary_filename, ofstream& out_stream);

  private:
    bool found_end_statement_;
    bool has_an_error_;

    const string kDummyCodeA = "1100110011001100";
    const string kDummyCodeB= "111100001111";
    const string kDummyCodeC = "1111000000000000";
    const string kDummyCodeD = "0000000011110000";

    int pc_in_assembler_;
    int maxpc_;
    vector<CodeLine> codelines_;
    map<int, string> machinecode_;
    map<string, int> symboltable_;
    map<string, string> opcodes_;
    set<string> mnemonics_;
    vector<string> symbol_vector_;

    string GetInvalidMessage(string leadingtext, string invalidstring);
    string GetInvalidMessage(string leadingtext, Hex hex);
    string GetUndefinedMessage(string badtext);
    void PassOne(Scanner& in_scanner);
    void PassOne(Scanner& in_scanner, ofstream& out_stream);
    void PassTwo();
    void PrintCodeLines(ofstream& out_stream);
    void PrintMachineCode(string binary_filename, ofstream& out_stream);
    void PrintSymbolTable(ofstream& out_stream);
    void SetNewPC(CodeLine codeline);
    void UpdateSymbolTable(int pc, string symboltext);
    void ValidMnemonics();
    void WriteBinaryFile(string binary_filename);
    string FindAddress(string addr, string symoperand);

    Globals globals_;

};

#endif
