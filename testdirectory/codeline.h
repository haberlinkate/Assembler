/****************************************************************
 * Header file for the 'CodeLine' class to contain one code line.
 *
 * Author/copyright:  Duncan Buell
 * Used with permission and modified by: Katherine Haberlin
 * Date: 4 December 2017
 *
**/

#ifndef CODELINE_H
#define CODELINE_H

#include <iostream>
using namespace std;

//#include "../../Utilities/scanner.h"
//#include "../../Utilities/scanline.h"
#include "../../Utilities/utils.h"

#include "globals.h"
#include "hex.h"

class CodeLine {
  public:
    CodeLine();
    CodeLine(Globals globals);
    virtual ~CodeLine();

    string GetAddr() const;
    string GetCode() const;
    string GetComments() const;
    string GetErrorMessages() const;
    Hex GetHexObject() const;
    string GetLabel() const;
    string GetMnemonic() const;
    int GetPC() const;
    string GetSymOperand() const;

    bool HasLabel() const;
    bool HasSymOperand() const;

    bool IsAllComment() const;
    void SetCodeLine(int linecounter, int pc, string label, string mnemonic,
                     string addr, string symoperand, string hexoperand,
                     string comments, string code);
    void SetCommentsOnly(int linecounter, string line);
    void SetErrorMessages(string messages);
    void SetMachineCode(string code);
    void SetPC(int what);
    string ToString() const;

  private:
    bool is_all_comment_;

    int linecounter_;
    int pc_;

    string addr_;
    string code_;
    string comments_;
    string error_messages_;
    string label_;
    string mnemonic_;
    string symoperand_;
    string hexoperand_; 

    Globals globals_;
    Hex hex_;
};

#endif
